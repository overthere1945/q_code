/**============================================================================
  @file qsh_audio_context_sensor_instance_island.c

  @brief The Audio Context Sensor Instance implementation, island code

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qurt_sclk.h"
#include "qurt_timer.h"
#include "qsh_audio_context_sensor_instance.h"
#include "pb_decode.h"
#include "pb_encode.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

static bool pb_encode_context_list(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
   param_id_asps_get_supported_context_ids_t *context_list_ptr = (param_id_asps_get_supported_context_ids_t *)*arg;
   int                                        i;
   for (i = 0; i < context_list_ptr->num_supported_contexts; i++)
   {
      if (!pb_encode_tag_for_field(stream, field))
         return false;

      if (!pb_encode_varint(stream, context_list_ptr->supported_context_ids[i]))
         return false;
   }
   return true;
}

static bool pb_encode_context_event(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
   pb_buffer_arg *arr   = (pb_buffer_arg *)*arg;
   qsh_audio_context_event *c_arr = (qsh_audio_context_event *)arr->buf;

   for (int i = 0; i < arr->buf_len; i++)
   {
      if (!pb_encode_tag_for_field(stream, field))
      {
         return false;
      }
      if (!pb_encode_submessage(stream, qsh_audio_context_event_fields, &c_arr[i]))
      {
         return false;
      }
   }

   return true;
}

// publish list of supported contexts
void qsh_audio_context_inst_publish_supported_context(sns_sensor_instance *const this,
                                                      param_id_asps_get_supported_context_ids_t *context_list_ptr)
{
   qsh_audio_context_inst_state *state_ptr = (qsh_audio_context_inst_state *)this->state->state;

   qsh_audio_context_list context_list = qsh_audio_context_list_init_default;

   context_list.context_id.funcs.encode = &pb_encode_context_list;
   context_list.context_id.arg          = context_list_ptr;

   size_t encoded_len = 0;

   pb_get_encoded_size(&encoded_len, qsh_audio_context_list_fields, &context_list);

   sns_sensor_event *event_out_ptr =
      state_ptr->event_service->api->alloc_event(state_ptr->event_service, this, encoded_len);
   if (event_out_ptr != NULL)
   {
      pb_ostream_t ostream = pb_ostream_from_buffer((pb_byte_t *)event_out_ptr->event, event_out_ptr->event_len);

      if (pb_encode(&ostream, qsh_audio_context_list_fields, &context_list))
      {
         event_out_ptr->timestamp  = sns_get_system_time();
         event_out_ptr->message_id = QSH_AUDIO_CONTEXT_MSGID_QSH_AUDIO_CONTEXT_LIST;
         event_out_ptr->event_len  = ostream.bytes_written;
         state_ptr->event_service->api->publish_event(state_ptr->event_service, this, event_out_ptr, NULL);
      }
   }
}

// publish received events
void qsh_audio_context_inst_publish_event(sns_sensor_instance *const this,
                                          uint32_t       num_context,
                                          qsh_audio_context_event *event_buf_ptr,
                                          sns_time       timestamp)
{
   qsh_audio_context_inst_state *state_ptr = (qsh_audio_context_inst_state *)this->state->state;
   qsh_audio_context_events      event     = qsh_audio_context_events_init_default;
   pb_buffer_arg                 data      = (pb_buffer_arg){ .buf = event_buf_ptr, .buf_len = num_context };

   event.events.funcs.encode = &pb_encode_context_event;
   event.events.arg          = &data;

   size_t encoded_len = 0;

   // todo: this function does the full encoding to get the size, we can use an estimate to avoid double encoding.
   pb_get_encoded_size(&encoded_len, qsh_audio_context_events_fields, &event);

   sns_sensor_event *event_out_ptr =
      state_ptr->event_service->api->alloc_event(state_ptr->event_service, this, encoded_len);
   if (event_out_ptr != NULL)
   {
      pb_ostream_t ostream = pb_ostream_from_buffer((pb_byte_t *)event_out_ptr->event, event_out_ptr->event_len);

      if (pb_encode(&ostream, qsh_audio_context_events_fields, &event))
      {
         event_out_ptr->timestamp  = QURT_TIMER_TIMETICK_FROM_US(timestamp);
         event_out_ptr->message_id = QSH_AUDIO_CONTEXT_MSGID_QSH_AUDIO_CONTEXT_EVENTS;
         event_out_ptr->event_len  = ostream.bytes_written;
         state_ptr->event_service->api->publish_event(state_ptr->event_service, this, event_out_ptr, NULL);
      }
   }
}

/*===========================================================================
  Public API Definitions
  ===========================================================================*/
/**
 * Process incoming events to the Sensor Instance.
 */
static sns_rc qsh_audio_context_inst_notify_event(sns_sensor_instance *const this)
{
   UNUSED_VAR(this);
   sns_rc rc = SNS_RC_SUCCESS;
   return rc;
}

const sns_sensor_instance_api qsh_audio_context_sensor_instance_api =
{
    .struct_len         = sizeof(sns_sensor_instance_api),
    .init               = &qsh_audio_context_inst_init,
    .deinit             = &qsh_audio_context_inst_deinit,
    .set_client_config  = &qsh_audio_context_inst_set_client_config,
    .notify_event       = &qsh_audio_context_inst_notify_event
};

