/*=============================================================================
  @file sns_tppe_sensor_instance.c

  The tppe Sensor Instance implementation

  Copyright (c) 2022-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_batch.pb.h"
#include "sns_memory_service.h"
#include "sns_rc.h"
#include "sns_sensor_instance.h"
#include "sns_std.pb.h"
#include "sns_time.h"
#include "sns_tppe_crc_cache.h"
#include "sns_tppe_debug_info.h"
#include "sns_tppe_dup_detection.h"
#include "sns_tppe_sensor_instance.h"
#include "sns_tppe_struct_extender.h"
#include "sns_transport_layer_filter.h"
#include <stdint.h>

/*=============================================================================
  Static Function Definitions
==============================================================================*/

static size_t encode_batch_config_message(uint8_t *buffer, size_t buffer_size,
                                          sns_sensor_instance *const this,
                                          sns_std_request *tppe_std_request)
{
  size_t encoded_len = 0;
  sns_std_request std_request = sns_std_request_init_default;
  sns_batch_config batch_config = sns_batch_config_init_default;
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;

  // setup suid for sensor to be batched
  sns_memscpy(&batch_config.suid, sizeof(sns_std_suid), &inst_state->tl_suid,
              sizeof(sns_sensor_uid));

  // setup the batch message ids
  batch_config.batch_msg_ids_count = inst_state->reg_config.msg_id_cnt;
  for(int i = 0; i < batch_config.batch_msg_ids_count; i++)
  {
    batch_config.batch_msg_ids[i] = inst_state->reg_config.msg_ids[i];
  }

  // setup the default batch period from registry
  std_request.has_batching = true;
  std_request.batching.batch_period = inst_state->reg_config.batch_period;

  // overwrite the batch period if provided by the client
  if(tppe_std_request->has_batching)
  {
    std_request.batching.batch_period = tppe_std_request->batching.batch_period;
  }

  SNS_TPPE_INST_PRINTF(MED, this, "Batch period : %u forwarded to batch sensor",
                       std_request.batching.batch_period);

  encoded_len =
      pb_encode_request(buffer, buffer_size, &batch_config, sns_batch_config_fields, &std_request);

  return encoded_len;
}

static sns_rc send_batch_config_req(sns_sensor_instance *const this,
                                    sns_std_request *tppe_std_request)
{
  sns_rc rc = SNS_RC_SUCCESS;
  uint8_t buffer[200];
  size_t encoded_len;
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;

  if(NULL == inst_state->batch_tl_stream)
  {
    sns_service_manager *service_mgr = this->cb->get_service_manager(this);
    sns_stream_service *stream_svc =
        (sns_stream_service *)service_mgr->get_service(service_mgr, SNS_STREAM_SERVICE);
    rc = stream_svc->api->create_sensor_instance_stream(stream_svc, this, inst_state->batch_suid,
                                                        &inst_state->batch_tl_stream);
  }

  if(NULL != inst_state->batch_tl_stream)
  {
    encoded_len = encode_batch_config_message(buffer, sizeof(buffer), this, tppe_std_request);

    sns_request batch_config_request = (sns_request){.message_id = SNS_BATCH_MSGID_SNS_BATCH_CONFIG,
                                                     .request_len = encoded_len,
                                                     .request = buffer};

    rc = inst_state->batch_tl_stream->api->send_request(inst_state->batch_tl_stream,
                                                        &batch_config_request);
  }

  return rc;
}
/**
Go through all the triggers
if dup period < batch period, relace it with batch period.
**/
static void sns_tppe_init_dp_start_time(sns_sensor_instance *const this)
{
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;
  sns_all_filters *all_filters = &inst_state->all_filters;
  sns_single_trigger *triggers = all_filters->wakeup_triggers;

  for(int i = 0; i < all_filters->trigger_count; i++)
  {
    triggers[i].dup_detect_period_start_ts = sns_get_system_time();
  }
}
/*=============================================================================
  Public Function Definitions
==============================================================================*/

/* See sns_sensor_instance_api::init */
sns_rc sns_tppe_inst_init(sns_sensor_instance *this, sns_sensor_state const *state)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;
  sns_tppe_sensor_state *sensor_state = (sns_tppe_sensor_state *)state->state;
  sns_transport_layer_params params = {.proto_name = sensor_state->reg_config.api};

  inst_state->tl_api = sns_transport_get_transport_layer_api(&params);
  if(NULL == inst_state->tl_api)
  {
    rc = SNS_RC_FAILED;
  }

  inst_state->reg_config = sensor_state->reg_config;
  sns_suid_lookup_get(&sensor_state->transport_suid_lookup_data, sensor_state->reg_config.type,
                      &inst_state->tl_suid);
  sns_suid_lookup_get(&sensor_state->tppe_batch_suid_lookup_data, "batch", &inst_state->batch_suid);
  return rc;
}

/* See sns_sensor_instance_api::deinit */
sns_rc sns_tppe_inst_deinit(sns_sensor_instance *const this)
{
  sns_service_manager *manager = this->cb->get_service_manager(this);
  sns_memory_service *memory_service =
      (sns_memory_service *)manager->get_service(manager, SNS_MEMORY_SERVICE);
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;
  sns_sensor_util_remove_sensor_instance_stream(this, &inst_state->batch_tl_stream);
  if(inst_state->tppe_config_memory_block)
  {
    memory_service->api->free(this, inst_state->tppe_config_memory_block);
  }
  sns_tcc_deinit(&inst_state->crc_cache);
  return SNS_RC_SUCCESS;
}

sns_rc sns_tppe_inst_set_client_config(sns_sensor_instance *const this,
                                       sns_request const *client_request)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_sensor_uid empty_suid = (sns_sensor_uid){.sensor_uid = {0}};
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;
  sns_service_manager *service_mgr = this->cb->get_service_manager(this);
  sns_memory_service *memory_service =
      (sns_memory_service *)service_mgr->get_service(service_mgr, SNS_MEMORY_SERVICE);
  if(client_request->message_id == SNS_TRANSPORT_PPE_MSGID_SNS_TRANSPORT_PPE_CONFIG)
  {
    bool decode_status = false;
    sns_std_request std_request = sns_std_request_init_default;
    decode_status = decode_tppe_config(client_request, &std_request, &inst_state->all_filters);

    SNS_TPPE_INST_PRINTF(MED, this,
                         "tppe decode status : %d, prop_filter_count %d, "
                         "service df count %d, manuf df count %d",
                         decode_status, inst_state->all_filters.prop_filter_count,
                         inst_state->all_filters.service_df_count,
                         inst_state->all_filters.manuf_df_count);

    if(decode_status)
    {
      print_all_triggers(this);
      sns_tppe_init_dp_start_time(this);
      rc = sns_tcc_init(&inst_state->crc_cache, inst_state->all_filters.trigger_count,
                        inst_state->crc_cache_max_len, memory_service, this);
      if(SNS_RC_SUCCESS == rc)
      {
        if(!sns_sensor_uid_compare(&inst_state->batch_suid, &empty_suid))
        {
          SNS_TPPE_INST_PRINTF(MED, this, "Send batch config request", client_request->message_id);
          rc = send_batch_config_req(this, &std_request);
        }
        else if(NULL == inst_state->tl_stream)
        {

          sns_stream_service *stream_svc =
              (sns_stream_service *)service_mgr->get_service(service_mgr, SNS_STREAM_SERVICE);
          rc = stream_svc->api->create_sensor_instance_stream(stream_svc, this, inst_state->tl_suid,
                                                              &inst_state->tl_stream);
        }
      }
      else
      {
        SNS_TPPE_INST_PRINTF(ERROR, this, "Initialize CRC cache: FAILED",
                             client_request->message_id);
      }
    }
    else
    {
      rc = SNS_RC_FAILED;
    }
  }
  else if(client_request->message_id == SNS_STD_MSGID_SNS_STD_DEBUG_REQ)
  {
    handle_debug_dump_request(this);
  }
  else
  {
    // forward requests to batch sensor
    if(NULL != inst_state->batch_tl_stream)
    {
      SNS_TPPE_INST_PRINTF(MED, this, "Forwarding req msg_id : %d to batch sensor",
                           client_request->message_id);
      rc = inst_state->batch_tl_stream->api->send_request(inst_state->batch_tl_stream,
                                                          (sns_request *)client_request);
    }
    // forward requests to ble sensor
    else if(NULL != inst_state->tl_stream)
    {
      SNS_TPPE_INST_PRINTF(MED, this, "Forwarding req msg_id : %d to ble sensor",
                           client_request->message_id);
      rc = inst_state->tl_stream->api->send_request(inst_state->tl_stream,
                                                    (sns_request *)client_request);
    }
  }
  return rc;
}
