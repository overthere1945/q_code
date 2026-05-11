/**============================================================================
  @file sns_ccd_ml_decision_tree_demo_sensor_instance.c

  @brief Example dummy AR algo integrated with ccd_mldt

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "sns_ccd_ml_decision_tree_demo_algo_interface.h"
#include "sns_ccd_ml_decision_tree_demo_sensor_instance.h"
#include "sns_ccd_ml_decision_tree_demo_sensor.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_request.h"
#include "sns_service_manager.h"
#include "sns_sensor_instance.h"
#include "sns_sensor_util.h"
#include "sns_stream_service.h"

#include "sns_ccd_gesture.pb.h"
#include "sns_std.pb.h"
#include "sns_std_sensor.pb.h"
#include "sns_types.h"


#define DEMO_ALGO_FLUSH_PERIOD_US 2000000
#define DEMO_ALGO_BATCH_PERIOD_US 1000000
#define DEMO_ALGO_SR 100
#define NUM_STREAMS 2

static void* get_ptr_to_algo_state(sns_sensor_instance *const this)
{
  void *ptr = NULL;
  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
    (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;
  
  ptr = (void *)(inst_state + 1);
  return ptr;
}

static void publish_ar_event(sns_sensor_instance *const this, 
                             sns_ar_motion_state state, sns_time ts)
{

  sns_ar_event ar_event;
  ar_event.states[0] = state;
  ar_event.states_count = 1;
  pb_send_event(this, sns_ar_event_fields, &ar_event,  ts, 
            SNS_ACTIVITY_RECOGNITION_MSGID_SNS_AR_EVENT, NULL);
  SNS_INST_PRINTF(HIGH, this, "sending AR event: %d", ar_event.states[0]);
}

static sns_rc request_ccd_model_list(sns_data_stream *mldt_stream)
{
  sns_rc rc = SNS_RC_FAILED;
  if(mldt_stream != NULL)
  {
    sns_request req =
    {
      .message_id = SNS_CCD_GESTURE_MSGID_SNS_CCD_MODEL_LIST_REQ,
      .request = NULL,
      .request_len = 0,
    };
    mldt_stream->api->send_request(mldt_stream, &req);
    rc = SNS_RC_SUCCESS;
  }
  return rc;
}

static sns_rc request_ccd_model(sns_sensor_instance *const this,
              sns_data_stream *mldt_stream, char *model_name)
{
  sns_rc rc = SNS_RC_FAILED;
  if(mldt_stream != NULL)
  {
    uint8_t buffer[128];
    pb_buffer_arg data;
    sns_request request;
    sns_ccd_model_config_req model_req;

    data = (pb_buffer_arg){
        .buf = model_name,
        .buf_len = strlen(model_name) + 1,
    };
    request.message_id = SNS_CCD_GESTURE_MSGID_SNS_CCD_MODEL_CONFIG_REQ;
    request.request = buffer;
    model_req.model_name.arg = &data;
    model_req.model_name.funcs.encode = pb_encode_string_cb;

    request.request_len = pb_encode_request(buffer, sizeof(buffer), &model_req, sns_ccd_model_config_req_fields, NULL);

    if(request.request_len > 0)
    {
      mldt_stream->api->send_request(mldt_stream, &request);
      rc = SNS_RC_SUCCESS;
    }
    else
    {
      rc = SNS_RC_FAILED;
      SNS_INST_PRINTF(ERROR, this, "Failed to encode request");
    }
  }
  return rc;
}

static sns_rc send_on_change_request(sns_sensor_instance *const this,
                                      sns_data_stream *stream)
{
  UNUSED_VAR(this);
  sns_request req;
  req = (sns_request){
      .message_id = SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG,
      .request = NULL,
      .request_len = 0,
  };
  stream->api->send_request(stream, &req);
  return SNS_RC_SUCCESS;
}


static sns_rc send_std_request(sns_sensor_instance *const this,
                                      sns_data_stream *stream, bool flush_only)
{
  UNUSED_VAR(this);
  sns_rc rc = SNS_RC_SUCCESS;
  sns_std_sensor_config std_cfg = sns_std_sensor_config_init_default;
  sns_std_request std_req = sns_std_request_init_default;
  sns_request req;
  uint8_t encoded_msg[50];

  req = (sns_request){
      .message_id = SNS_STD_SENSOR_MSGID_SNS_STD_SENSOR_CONFIG,
      .request = encoded_msg,
  };

  std_req = (sns_std_request){
      .has_batching = flush_only,
      .batching.batch_period = DEMO_ALGO_BATCH_PERIOD_US,
      .batching.has_flush_period = true,
      .batching.flush_period = DEMO_ALGO_FLUSH_PERIOD_US,
      .batching.has_flush_only = true,
      .batching.flush_only = flush_only,
      .payload = {{NULL}, NULL},
  };

  std_cfg = (sns_std_sensor_config){
      .sample_rate = DEMO_ALGO_SR,
  };

  req.request_len =
      pb_encode_request(encoded_msg, sizeof(encoded_msg), &std_cfg,
                        sns_std_sensor_config_fields, &std_req);

  if(req.request_len > 0)
  {
    stream->api->send_request(stream, &req);
    SNS_INST_PRINTF(HIGH, this, "Sent flush only config");
  }
  return rc;
}

static void send_flush_req(sns_sensor_instance *const this,
                           sns_data_stream *stream)
{
  UNUSED_VAR(this);
  sns_request req;
  req = (sns_request){
      .message_id = SNS_STD_MSGID_SNS_STD_FLUSH_REQ,
      .request = NULL,
      .request_len = 0,
  };
  stream->api->send_request(stream, &req);
}

static sns_rc
create_streams(sns_sensor_instance *const this,
               sns_ccd_ml_decision_tree_demo_sensor_state *sensor_state)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
      (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;

  sns_sensor_uid ccd_mldt_uid;
  sns_sensor_uid accel_uid;
  sns_sensor_uid gyro_uid;

  sns_suid_lookup_get(&sensor_state->lookup_data, "accel", &accel_uid);

  sns_suid_lookup_get(&sensor_state->lookup_data, "gyro", &gyro_uid);

  sns_suid_lookup_get(&sensor_state->lookup_data, "ccd_mldt", &ccd_mldt_uid);

  sns_service_manager *service_mgr = this->cb->get_service_manager(this);
  sns_stream_service *stream_svc =
      (sns_stream_service *)service_mgr->get_service(service_mgr,
                                                     SNS_STREAM_SERVICE);

  rc = stream_svc->api->create_sensor_instance_stream(
      stream_svc, this, accel_uid, &inst_state->accel_stream);

  rc = stream_svc->api->create_sensor_instance_stream(
      stream_svc, this, gyro_uid, &inst_state->gyro_stream);

  rc = stream_svc->api->create_sensor_instance_stream(
      stream_svc, this, ccd_mldt_uid, &inst_state->ccd_mldt_stream);

  return rc;
}

static void update_state_and_req_next_model(sns_sensor_instance *const this, sns_algo_output_struct *algo_output)
{
  SNS_INST_PRINTF(HIGH, this, "update_state_and_req_next_model");
  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
      (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;
  if(algo_output->state != inst_state->curr_ar_state)
  {
    publish_ar_event(this, algo_output->state, algo_output->timestamp);
    request_ccd_model(this, inst_state->ccd_mldt_stream, algo_output->model_to_load);
    inst_state->prev_event_ts = algo_output->timestamp;
    inst_state->curr_ar_state = algo_output->state;
  }
}

static sns_rc handle_stream_event(sns_sensor_instance *const this, sns_sensor_event *event, sample_type type)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
      (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;

  bool state_detected = false;
  float data[3];
  size_t arr_idx = 0;
  sns_algo_output_struct algo_output;

  if(inst_state->state == PROCESSING_SAMPLES)
  {
    if(event->message_id == SNS_STD_SENSOR_MSGID_SNS_STD_SENSOR_EVENT)
    {
      sns_std_sensor_event std_event = sns_std_sensor_event_init_default;

      if(!pb_decode_std_sensor_event(
              this, (const char *)event->event, event->event_len, data,
              3, &arr_idx,
              &std_event))
      {
        SNS_INST_PRINTF(ERROR, this, "Failed to decode std sensor event");
      }
      else
      {
        state_detected = sns_ccd_ml_decision_tree_demo_algo_process_sample(get_ptr_to_algo_state(this),
                        type, data, event->timestamp, &algo_output);
        if(state_detected)
        {
          SNS_INST_PRINTF(HIGH, this, "State changed. Back to Flush only. Waiting mldt");
          update_state_and_req_next_model(this, &algo_output);
          send_std_request(this, inst_state->accel_stream, true);
          send_std_request(this, inst_state->gyro_stream, true);
          inst_state->state = WAITING_STATE_EXIT_EVENT;
        }
      }
    }
    else if(event->message_id == SNS_STD_MSGID_SNS_STD_FLUSH_EVENT)
    {
      if(type == ACCEL)
      {
        SNS_INST_PRINTF(HIGH, this, "Received accel flush event");
        send_std_request(this, inst_state->accel_stream, false);
      }
      else
      {
        SNS_INST_PRINTF(HIGH, this, "Received gyro flush event");
        send_std_request(this, inst_state->gyro_stream, false);
      }
    }
  }
  return rc;
}

static sns_rc process_stream_events(sns_sensor_event *event, int stream_idx,
                                   void *userdata)
{

  sns_rc rc = SNS_RC_SUCCESS;
  sns_sensor_instance *this = (sns_sensor_instance *)userdata;
  switch(stream_idx)
  {
    case ACCEL:
      rc = handle_stream_event(this, event, ACCEL);
      break;
    case GYRO:
      rc = handle_stream_event(this, event, GYRO);
      break;
    default:
      SNS_INST_PRINTF(ERROR, this, "Unexpected stream_idx: %d", stream_idx);
  }
  return rc;
}

static sns_rc handle_accel_gyro_events(sns_sensor_instance *const this)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_data_stream *streams[NUM_STREAMS];
  sns_timesync_event_info info[NUM_STREAMS];
  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
      (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;
  
  streams[ACCEL] = inst_state->accel_stream;
  streams[GYRO] = inst_state->gyro_stream;
  rc = sns_process_streams_in_timesync(streams, info, NUM_STREAMS, 
                                       process_stream_events, this);
  return rc;
}

static sns_rc handle_model_list_event(sns_sensor_instance *const this, sns_sensor_event *event)
{
  SNS_INST_PRINTF(HIGH, this, "Received model list event");
  sns_rc rc = SNS_RC_SUCCESS;
  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
      (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;

  char *model_ptrs[MAX_NUM_MODELS];
  for(int i = 0; i < MAX_NUM_MODELS; i++)
  {
    model_ptrs[i] = inst_state->model_list[i];
  }

  uint8_t idx = 0;
  pb_string_arr_arg arg;

  arg.buf_arr = (uint8_t **)model_ptrs;
  arg.arr_len = MAX_NUM_MODELS;
  arg.arr_index = &idx;
  arg.max_str_len = MAX_MODEL_NAME_LEN;

  pb_istream_t stream = pb_istream_from_buffer((void *)event->event,
                                                event->event_len);

  sns_ccd_model_list_event dec_list = sns_ccd_model_list_event_init_default;
  dec_list.model_names.funcs.decode = pb_decode_string_arr_cb;
  dec_list.model_names.arg = &arg;

  if(!pb_decode(&stream, sns_ccd_model_list_event_fields, &dec_list))
  {
    SNS_INST_PRINTF(ERROR, this, "Error decoding model list event");
  }
  else
  {
    if(idx != MAX_NUM_MODELS)
    {
      SNS_INST_PRINTF(ERROR, this, "Missing models. Model cnt: %d", idx);
    }
  }
  return rc;
}


static void handle_gesture_event(sns_sensor_instance *const this, sns_sensor_event *event)
{
  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
      (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;
  sns_ccd_gesture_event te_event;

  pb_istream_t stream =
      pb_istream_from_buffer((pb_byte_t *)event->event, event->event_len);
  if(!pb_decode_noinit(&stream, sns_ccd_gesture_event_fields, &te_event))
  {
    SNS_INST_PRINTF(ERROR, this, "mldt event decode failed!");
  }
  else
  {
    SNS_INST_PRINTF(HIGH, this, "te_state1: %u", te_event.event.te_event.state1);
    sns_ccd_ml_decision_tree_demo_algo_process_te_state(
          get_ptr_to_algo_state(this), &te_event.event.te_event, event->timestamp);
    if(inst_state->state == WAITING_STATE_EXIT_EVENT)
    {
      SNS_INST_PRINTF(HIGH, this, "Received gesture vent - sending flush req");
      send_flush_req(this, inst_state->accel_stream);
      send_flush_req(this, inst_state->gyro_stream);
      inst_state->state = PROCESSING_SAMPLES;
    }
  }
}

static sns_rc handle_model_config_event(sns_sensor_instance *const this, sns_sensor_event *event)
{
  sns_rc rc = SNS_RC_SUCCESS;
  SNS_INST_PRINTF(HIGH, this, "Received model config event");

  pb_istream_t stream = pb_istream_from_buffer((void *)event->event,
                                          event->event_len);
  pb_buffer_arg arg;

  sns_ccd_model_config_event model_event = sns_ccd_model_config_event_init_default;
  model_event.model_name.arg = &arg;
  model_event.model_name.funcs.decode = pb_decode_string_cb;

  if(!pb_decode_noinit(&stream, sns_ccd_model_config_event_fields, &model_event))
  {
    SNS_INST_PRINTF(ERROR, this, "Error decoding model config event");
    rc = SNS_RC_FAILED;
  }
  else
  {
   if((sns_rc)model_event.status != SNS_RC_SUCCESS)
   {
    SNS_INST_PRINTF(ERROR, this, "Model load failure");
    rc = SNS_RC_FAILED;
   }
   else
   {
    SNS_INST_PRINTF(HIGH, this, "Model loaded. Status: %d", model_event.status);
   }
  }
  return rc;
}


static sns_rc handle_ccd_events(sns_sensor_instance *const this)
{
  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
      (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;
  sns_rc rc = SNS_RC_SUCCESS;

  if(inst_state->ccd_mldt_stream != NULL)
  {
    sns_sensor_event *event = inst_state->ccd_mldt_stream->api->peek_input(inst_state->ccd_mldt_stream);
    while(event != NULL)
    {
      if(event->message_id == SNS_CCD_GESTURE_MSGID_SNS_CCD_MODEL_CONFIG_EVENT)
      {
        rc = handle_model_config_event(this, event);
      }
      else if(event->message_id == SNS_CCD_GESTURE_MSGID_SNS_CCD_MODEL_LIST_EVENT)
      {
        rc = handle_model_list_event(this, event);
      }
      else if(event->message_id == SNS_CCD_GESTURE_MSGID_SNS_CCD_GESTURE_EVENT)
      {
        handle_gesture_event(this, event);
      }
      event = inst_state->ccd_mldt_stream->api->get_next_input(inst_state->ccd_mldt_stream);
    }
  }
  return rc;
}


static sns_rc sns_ccd_ccd_ml_decision_tree_demo_sensor_inst_notify_event(
    sns_sensor_instance *const this)
{
  sns_rc rc = SNS_RC_SUCCESS;
  rc = handle_ccd_events(this);
  rc = handle_accel_gyro_events(this);
  return rc;
}

static sns_rc sns_ccd_ccd_ml_decision_tree_demo_sensor_inst_set_client_config(
    sns_sensor_instance *const this, sns_request const *client_req)

{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
      (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;

  if(!inst_state->is_configured)
  {
    switch(client_req->message_id)
    {
      case SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG:
      {
        sns_time ts = sns_get_system_time();
        inst_state->state = WAITING_STATE_EXIT_EVENT;
        send_std_request(this, inst_state->accel_stream, true);
        send_std_request(this, inst_state->gyro_stream, true);
        send_on_change_request(this, inst_state->ccd_mldt_stream);
        publish_ar_event(this, inst_state->curr_ar_state, ts);
        inst_state->prev_event_ts = ts;
        break;
      }
    }
  }
  else
  {
    if(client_req->message_id == SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG)
    {
      if(inst_state->prev_event_ts != 0)
      {
        publish_ar_event(this, inst_state->curr_ar_state, inst_state->prev_event_ts);
      }
    }
  }
  return rc;
}

static sns_rc sns_ccd_ccd_ml_decision_tree_demo_sensor_inst_init(
    sns_sensor_instance *this, sns_sensor_state const *state)

{
  SNS_INST_PRINTF(HIGH, this, "init");
  sns_ccd_ml_decision_tree_demo_sensor_state *sensor_state =
      (sns_ccd_ml_decision_tree_demo_sensor_state *)state->state;

  sns_ccd_ml_decision_tree_demo_sensor_inst_state *inst_state =
      (sns_ccd_ml_decision_tree_demo_sensor_inst_state *)this->state->state;

  inst_state->curr_ar_state = SNS_AR_STATIONARY;
  create_streams(this, sensor_state);
  request_ccd_model_list(inst_state->ccd_mldt_stream);
  sns_ccd_ml_decision_tree_demo_algo_init(get_ptr_to_algo_state(this));
  return SNS_RC_SUCCESS;
}

static sns_rc sns_ccd_ccd_ml_decision_tree_demo_sensor_inst_deinit(
    sns_sensor_instance *const this)
{
  SNS_INST_PRINTF(HIGH, this, "deinit");
  return SNS_RC_SUCCESS;
}

const sns_sensor_instance_api sns_ccd_ml_decision_tree_demo_sensor_inst_api = {
    .struct_len = sizeof(sns_sensor_instance_api),
    .init = &sns_ccd_ccd_ml_decision_tree_demo_sensor_inst_init,
    .deinit = &sns_ccd_ccd_ml_decision_tree_demo_sensor_inst_deinit,
    .set_client_config =
        &sns_ccd_ccd_ml_decision_tree_demo_sensor_inst_set_client_config,
    .notify_event =
        &sns_ccd_ccd_ml_decision_tree_demo_sensor_inst_notify_event};