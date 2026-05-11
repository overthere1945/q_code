/**============================================================================
  @file sns_tppe_sensor_instance_island.c

  @brief The tppe Sensor Instance implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_batch.pb.h"
#include "sns_memory_service.h"
#include "sns_tppe_crc_cache.h"
#include "sns_tppe_debug_info.h"
#include "sns_tppe_dup_detection.h"
#include "sns_tppe_sensor_instance.h"
#include "sns_tppe_struct_extender.h"
#include "sns_transport_layer_filter.h"

/*=============================================================================
  Static Function Definitions
==============================================================================*/

static bool is_filter_msg_id(uint32_t *filter_msg_ids, uint8_t filter_msg_id_count, uint32_t msg_id)
{
  bool rv = false;
  for(int i = 0; i < filter_msg_id_count; i++)
  {
    if(msg_id == filter_msg_ids[i])
    {
      rv = true;
      break;
    }
  }
  return rv;
}

static sns_rc forward_event(sns_sensor_instance *const this, sns_sensor_event *event)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_service_manager *manager = this->cb->get_service_manager(this);
  sns_event_service *event_service =
      (sns_event_service *)manager->get_service(manager, SNS_EVENT_SERVICE);
  sns_sensor_event *send_event =
      event_service->api->alloc_event(event_service, this, event->event_len);
  if(NULL != send_event)
  {
    send_event->event_len = event->event_len;
    send_event->message_id = event->message_id;
    send_event->timestamp = event->timestamp;
    sns_memscpy(send_event->event, send_event->event_len, event->event, event->event_len);
    rc = event_service->api->publish_event(event_service, this, send_event, NULL);
    SNS_TPPE_INST_PRINTF(MED, this, "Forwarded non-filtered event msg id : %d", event->message_id);
  }
  return rc;
}

static bool send_tppe_event(sns_sensor_instance *const this, sns_sensor_event *tl_event,
                            uint32_t *triggers, size_t trigger_count)
{
  bool rv = false;
  sns_transport_ppe_event tppe_event = sns_transport_ppe_event_init_default;

  pb_uint_32_arr_arg uint32_arr_arg = {.arr = triggers, .arr_len = trigger_count};

  pb_buffer_arg event_buff =
      (pb_buffer_arg){.buf = tl_event->event, .buf_len = tl_event->event_len};

  tppe_event.event_msg_id = tl_event->message_id;
  tppe_event.trigger_ids.funcs.encode = pb_encode_uint_32_arr_cb;
  tppe_event.trigger_ids.arg = &uint32_arr_arg;
  tppe_event.event.funcs.encode = pb_encode_string_cb;
  tppe_event.event.arg = &event_buff;

  rv = pb_send_event(this, sns_transport_ppe_event_fields, &tppe_event, tl_event->timestamp,
                     SNS_TRANSPORT_PPE_MSGID_SNS_TRANSPORT_PPE_EVENT, NULL);
  SNS_TPPE_INST_PRINTF(MED, this, "Filter satisfied, send tppe event");
  return rv;
}

static sns_rc handle_tl_event(sns_sensor_instance *const this)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;
  sns_service_manager *manager = this->cb->get_service_manager(this);
  sns_memory_service *memory_service =
      (sns_memory_service *)manager->get_service(manager, SNS_MEMORY_SERVICE);
  sns_sensor_event *event = NULL;
  sns_data_stream *tppe_data_stream = NULL;

  tppe_data_stream =
      (NULL != inst_state->batch_tl_stream) ? inst_state->batch_tl_stream : inst_state->tl_stream;

  if(NULL != tppe_data_stream)
  {
    event = tppe_data_stream->api->peek_input(tppe_data_stream);
    while(NULL != event)
    {
      sns_rc rc = SNS_RC_SUCCESS;
      if(is_filter_msg_id(inst_state->reg_config.msg_ids, inst_state->reg_config.msg_id_cnt,
                          event->message_id))
      {
        if(0 == inst_state->all_filters.trigger_count)
        {
          send_tppe_event(this, event, NULL, 0);
          SNS_TPPE_INST_PRINTF(MED, this, "No triggers populated, send tppe event");
        }
        else
        {
          rc = inst_state->tl_api->decode_event(this, event, &inst_state->decode_buffer,
                                                memory_service);
          if(SNS_RC_SUCCESS == rc)
          {
            inst_state->event_triggers.event_ts = event->timestamp;
            sns_transport_filter(inst_state->tl_api, &inst_state->decode_buffer,
                                 &inst_state->all_filters, &inst_state->event_triggers);
            if(0 != inst_state->event_triggers.trigger_id_count)
            {
              if(!is_duplicate_event(this))
              {
                send_tppe_event(this, event, inst_state->event_triggers.trigger_ids,
                                inst_state->event_triggers.trigger_id_count);
              }
              else
              {
                SNS_TPPE_INST_PRINTF(MED, this, "Dropping Duplicate event");
              }
            }
            else
            {
              SNS_TPPE_INST_PRINTF(MED, this, "Filter NOT satisfied, dropping event");
            }
          }
          else
          {
            SNS_TPPE_INST_PRINTF(MED, this, "Decode Transport event FAILED");
          }
        }
      }
      else
      {
        if(event->message_id != SNS_BATCH_MSGID_SNS_BATCH_COMPLETE_EVENT)
        {
          rc = forward_event(this, event);
        }
      }

      sns_transport_layer_free_data_items(this, &inst_state->decode_buffer, memory_service);

      event = tppe_data_stream->api->get_next_input(tppe_data_stream);
    }
  }
  return rc;
}

static sns_rc sns_tppe_inst_notify_event(sns_sensor_instance *const this)
{
  sns_rc rc = SNS_RC_SUCCESS;
  rc = handle_tl_event(this);
  return rc;
}

void *sns_tppe_memory_service_malloc(sns_memory_service *memory_service,
                                     sns_sensor_instance *const this, size_t size)
{
  void *mem_ptr = NULL;
#ifdef SNS_ISLAND_INCLUDE_TPPE
  mem_ptr = memory_service->api->malloc_island(this, size);
#else
  mem_ptr = memory_service->api->malloc(this, size);
#endif // SNS_ISLAND_INCLUDE_TPPE
  return mem_ptr;
}

/*=============================================================================
  Sensor Instance API
==============================================================================*/

const sns_sensor_instance_api sns_tppe_sensor_instance_api = {
    .struct_len = sizeof(sns_sensor_instance_api),
    .init = &sns_tppe_inst_init,
    .deinit = &sns_tppe_inst_deinit,
    .set_client_config = &sns_tppe_inst_set_client_config,
    .notify_event = &sns_tppe_inst_notify_event};
