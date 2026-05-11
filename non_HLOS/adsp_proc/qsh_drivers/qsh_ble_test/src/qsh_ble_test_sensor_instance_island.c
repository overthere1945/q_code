/*=============================================================================
  @file qsh_ble_test_sensor_instance_island.c

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/
/*=============================================================================
  Includes
  ===========================================================================*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "qurt_thread.h"

#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_rc.h"
#include "sns_request.h"
#include "sns_sensor.h"
#include "sns_sensor_instance.h"
#include "sns_sensor_util.h"
#include "sns_sensor_uid.h"
#include "sns_service_manager.h"
#include "sns_stream_service.h"

#include "sns_std.pb.h"
#include "sns_std_sensor.pb.h"

#include "qsh_ble.pb.h"
#include "qsh_ble_test.pb.h"
#include "qsh_ble_test_sensor.h"
#include "qsh_ble_test_sensor_instance.h"

typedef enum
{
  HCI_CMD_PKT       = 0x01,
  HCI_ACL_PKT       = 0x02,
  HCI_EVT_PKT       = 0x04,
} hci_packet_type_t;

#define MAX_BT_HDR_SIZE 4
#define BT_EVT_HDR_SIZE 2
#define BT_EVT_HDR_LEN_OFFSET 1

#define BLE_EVENT_CODE 0x3E
#define BLE_ADV_REPORT_EVENT_SUBCODE 0x02

// data types
#define DATA_TYPE_LOCAL_NAME_SHORT 0x08
#define DATA_TYPE_LOCAL_NAME_COMPLETE 0x09

static void
qsh_ble_test_print_ble_scan_record(
    sns_sensor_instance *const this, const unsigned char *scan_record, unsigned int len)
{
  int curr_pos = 0;

  // parse the data field, just extracting device name as an example.
  char device_name[32] = {0};
  while (curr_pos < len)
  {
    uint8_t length = scan_record[curr_pos++];
    if (length == 0) {
      break;
    }

    int data_length = length - 1;
    uint8_t field_type = scan_record[curr_pos++];

    switch (field_type)
    {
      case DATA_TYPE_LOCAL_NAME_SHORT:
      case DATA_TYPE_LOCAL_NAME_COMPLETE:
        sns_memscpy(device_name, sizeof(device_name) - 1,
            scan_record + curr_pos, data_length);
        SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_print_ble_scan_record, device_name=%c%c%c%c%c%c%c%c",
            device_name[0], device_name[1], device_name[2], device_name[3], device_name[4],
            device_name[5], device_name[6], device_name[7]);
        break;
      default:
        break;
    }
    curr_pos += data_length;
  }
}

//7.7.65.2 LE Advertising Report event
static void
qsh_ble_test_print_ble_adv_event(
    sns_sensor_instance *const this, const unsigned char *raw_event, unsigned int len)
{
  int curr_pos = 0;
  if (len < BT_EVT_HDR_SIZE + 1) {
    SNS_INST_UPRINTF(ERROR, this, "qsh_ble_test_print_ble_adv_event, len is invalid");
    return;
  }

  uint8_t packet_type = raw_event[curr_pos++];
  if (packet_type != HCI_EVT_PKT)
  {
    SNS_INST_UPRINTF(ERROR, this, "qsh_ble_test_print_ble_adv_event, not an event");
    return;
  }

  uint8_t event_code = raw_event[curr_pos++];
  int pram_len = raw_event[curr_pos++];
  if (len != pram_len + BT_EVT_HDR_SIZE + 1)
  {
    SNS_INST_UPRINTF(ERROR, this, "qsh_ble_test_print_ble_adv_event, len is wrong");
    return;
  }

  uint8_t sub_code = 0;
  if (event_code == BLE_EVENT_CODE)
    sub_code = raw_event[curr_pos++];

  if (event_code != BLE_EVENT_CODE || sub_code != BLE_ADV_REPORT_EVENT_SUBCODE)
  {
    SNS_INST_UPRINTF(ERROR, this, "qsh_ble_test_print_ble_adv_event, not an adv event");
    return;
  }
  uint8_t num_reports = raw_event[curr_pos++];

  while (num_reports--)
  {
    if (curr_pos >= len)
    {
      SNS_INST_UPRINTF(ERROR, this, "qsh_ble_test_print_ble_adv_event, invalid adv event");
      return;
    }

    // skip event_type field
    curr_pos++;
    // skip address_type field
    curr_pos++;
    // skip address field
    curr_pos += 6;

    uint8_t data_len = raw_event[curr_pos++];
    qsh_ble_test_print_ble_scan_record(this, raw_event + curr_pos, data_len);
    curr_pos += data_len;

    if (curr_pos > len - 1)
    {
      SNS_INST_UPRINTF(ERROR, this, "qsh_ble_test_print_ble_adv_event, invalid data_len=%d", data_len);
      return;
    }
    signed char rssi = raw_event[curr_pos++];

    SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_print_ble_adv_event, island=%d, data_len=%d, rssi=%d",
        qurt_island_get_status(), data_len, rssi);
  }
}

static void
qsh_ble_test_process_ble_adv_event(
    sns_sensor_instance *const this, pb_istream_t* pbstream)
{
  qsh_ble_adv_event adv_event = qsh_ble_adv_event_init_default;
  pb_buffer_arg buffer_arg = { .buf = NULL, .buf_len = 0 };
  adv_event.raw_event.funcs.decode = pb_decode_string_cb;
  adv_event.raw_event.arg = &buffer_arg;

  if (!pb_decode(pbstream, qsh_ble_adv_event_fields, &adv_event))
  {
    SNS_INST_UPRINTF(ERROR, this, "qsh_ble_test_process_ble_adv_event, failed to decode");
    return;
  }

  qsh_ble_test_print_ble_adv_event(this, buffer_arg.buf, buffer_arg.buf_len);
}

static void
qsh_ble_test_activate_ble_sensor(sns_sensor_instance *const this)
{
  qsh_ble_test_inst_state *inst_state = (qsh_ble_test_inst_state*)this->state->state;

  SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_activate_ble_sensor");

  sns_request sensor_req = (sns_request){
    .message_id = SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG,
    .request_len = 0, .request = NULL };

  inst_state->ble_stream->api->send_request(inst_state->ble_stream, &sensor_req);
}

static bool
qsh_ble_test_inst_handle_test_config(
    sns_sensor_instance *const this, sns_request const* client_request)
{
  qsh_ble_test_inst_state *inst_state = (qsh_ble_test_inst_state*)this->state->state;

  qsh_ble_test_config config = qsh_ble_test_config_init_default;

  if (!pb_decode_request(client_request, qsh_ble_test_config_fields, NULL, &config)) {
    SNS_INST_UPRINTF(ERROR, this, "handle_test_config, failed to decode request");
    return false;
  }

  SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_inst_handle_test_config, requests=0x%x", config.requests);
  inst_state->test_reqs = config.requests;

  if (config.requests & QSH_BLE_TEST_REQ_ROUTING_SCAN)
  {
    qsh_ble_test_activate_ble_sensor(this);
    inst_state->ble_test_started_timestamp = sns_get_system_time();
  }

  return true;
}

static void
qsh_ble_test_deliver_result_event(
    sns_sensor_instance *const this, uint32_t test_reqs)
{
  qsh_ble_test_inst_state *inst_state = (qsh_ble_test_inst_state*)this->state->state;

  SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_deliver_result_event, test_reqs=0x%x",
      test_reqs);

  qsh_ble_test_result_event result_event = qsh_ble_test_result_event_init_default;

  if (test_reqs & QSH_BLE_TEST_REQ_ROUTING_SCAN)
  {
    result_event.has_routing_scan_result = true;

    sns_std_error error = inst_state->ble_stream != NULL ? SNS_STD_ERROR_NO_ERROR : SNS_STD_ERROR_FAILED;
    result_event.routing_scan_result.error = error;
    result_event.routing_scan_result.has_num_of_adv_event_received = true;
    result_event.routing_scan_result.num_of_adv_event_received = inst_state->num_of_ble_adv_event_received;

    result_event.routing_scan_result.has_time_spent_s = true;
    uint64_t elapsed_ticks = sns_get_system_time() - inst_state->ble_test_started_timestamp;
    uint32_t elapsed_time_s = (uint32_t)((elapsed_ticks / 1000 * sns_get_time_tick_resolution()) / 1000 / 1000);
    result_event.routing_scan_result.time_spent_s = elapsed_time_s;

    SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_deliver_result_event, num=%d, time_spent_s=%d",
        result_event.routing_scan_result.num_of_adv_event_received, elapsed_time_s);
  }

  size_t encoded_len = 0;
  pb_get_encoded_size(&encoded_len, qsh_ble_test_result_event_fields, &result_event);

  sns_sensor_event *event_out = inst_state->event_service->api->alloc_event(inst_state->event_service,
      this, encoded_len);

  if (event_out == NULL)
  {
    SNS_INST_UPRINTF(ERROR, this, "qsh_ble_test_deliver_result_event, failed to alloc event");
    return;
  }

  pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *)event_out->event, event_out->event_len);
  pb_encode(&stream, qsh_ble_test_result_event_fields, &result_event);

  event_out->timestamp = sns_get_system_time();
  event_out->message_id = QSH_BLE_TEST_MSGID_QSH_BLE_TEST_RESULT_EVENT;
  inst_state->event_service->api->publish_event(inst_state->event_service, this, event_out, NULL);
}

static void
qsh_ble_test_inst_handle_get_result(
    sns_sensor_instance *const this, sns_request const* client_request)
{
  SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_inst_handle_get_result");

  qsh_ble_test_get_result get_result = qsh_ble_test_get_result_init_default;

  if (!pb_decode_request(client_request, qsh_ble_test_get_result_fields, NULL, &get_result)) {
    SNS_INST_UPRINTF(ERROR, this, "handle_get_result, failed to decode request");
    return;
  }

  qsh_ble_test_deliver_result_event(this, get_result.requests);
}

sns_rc
qsh_ble_test_inst_set_client_config(
    sns_sensor_instance *const this, sns_request const* client_request)
{
  sns_rc rc = SNS_RC_SUCCESS;
  qsh_ble_test_inst_state *state = (qsh_ble_test_inst_state*)this->state->state;

  SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_inst_set_client_config, msg_id=%d", client_request->message_id);
  if (client_request->message_id == QSH_BLE_TEST_MSGID_QSH_BLE_TEST_CONFIG)
  {
    qsh_ble_test_inst_handle_test_config(this, client_request);
  }
  else if (client_request->message_id == QSH_BLE_TEST_MSGID_QSH_BLE_TEST_GET_RESULT)
  {
    qsh_ble_test_inst_handle_get_result(this, client_request);
  }
  else if (client_request->message_id == SNS_STD_MSGID_SNS_STD_FLUSH_REQ)
  {
    SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_inst_set_client_config, flush req");
    sns_sensor_util_send_flush_event(state->suid, this);
  }
  else
  {
    SNS_INST_UPRINTF(ERROR, this, "qsh_ble_test_inst_set_client_config, Unsupported message ID %d",
         client_request->message_id);
    rc = SNS_RC_NOT_SUPPORTED;
  }

  return rc;
}

sns_rc
qsh_ble_test_inst_init(sns_sensor_instance *this,
    sns_sensor_state const *state)
{
  sns_rc rc = SNS_RC_SUCCESS;
  qsh_ble_test_inst_state *inst_state =
      (qsh_ble_test_inst_state *)this->state->state;
  qsh_ble_test_sensor_state *sensor_state = (qsh_ble_test_sensor_state*)state->state;
  memset(inst_state, 0, sizeof(qsh_ble_test_inst_state));

  inst_state->suid = sensor_state->suid;

  sns_service_manager *service_mgr = this->cb->get_service_manager(this);

  inst_state->event_service = (sns_event_service*)
      service_mgr->get_service(service_mgr, SNS_EVENT_SERVICE);

  sns_stream_service *stream_service = (sns_stream_service*)
      service_mgr->get_service(service_mgr, SNS_STREAM_SERVICE);

  sns_suid_lookup_get(&sensor_state->suid_lookup_data, "ble", &inst_state->ble_suid);

  stream_service->api->create_sensor_instance_stream(
      stream_service,
      this,
      inst_state->ble_suid,
      &inst_state->ble_stream);

  SNS_INST_UPRINTF(HIGH, this, "inst_state->ble_stream = %p", inst_state->ble_stream);

  SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_inst_init done");
  return rc;
}

sns_rc
qsh_ble_test_inst_deinit(sns_sensor_instance *const this)
{
  qsh_ble_test_inst_state *inst_state = (qsh_ble_test_inst_state*)this->state->state;

  if (inst_state->ble_stream != NULL)
    sns_sensor_util_remove_sensor_instance_stream(this, &inst_state->ble_stream);

  SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_inst_deinit done");
  return SNS_RC_SUCCESS;
}

sns_rc
qsh_ble_test_inst_notify_event(sns_sensor_instance *const this)
{
  SNS_INST_UPRINTF(HIGH, this, "qsh_ble_test_inst_notify_event");

  qsh_ble_test_inst_state *inst_state = (qsh_ble_test_inst_state*)this->state->state;
  sns_sensor_event *ble_event_in = NULL;

  // qsh_ble events
  if (NULL != inst_state->ble_stream)
  {
    for (ble_event_in = inst_state->ble_stream->api->peek_input(inst_state->ble_stream);
        ble_event_in != NULL;
        ble_event_in = inst_state->ble_stream->api->get_next_input(inst_state->ble_stream))
    {
      pb_istream_t pbstream =
          pb_istream_from_buffer((pb_byte_t*)ble_event_in->event, ble_event_in->event_len);

      if (QSH_BLE_MSGID_QSH_BLE_ADV_EVENT == ble_event_in->message_id)
      {
        inst_state->num_of_ble_adv_event_received++;
        qsh_ble_test_process_ble_adv_event(this, &pbstream);
      }
      else if (SNS_STD_MSGID_SNS_STD_ERROR_EVENT == ble_event_in->message_id)
      {
        SNS_INST_UPRINTF(ERROR, this, "inst_notify_event, BLE: received SNS_STD_ERROR_EVENT");
        sns_sensor_util_remove_sensor_instance_stream(this, &inst_state->ble_stream);
        qsh_ble_test_deliver_result_event(this, QSH_BLE_TEST_REQ_ROUTING_SCAN);
      }
    }
  }

  return SNS_RC_SUCCESS;
}

const sns_sensor_instance_api qsh_ble_test_sensor_instance_api =
{
  .struct_len = sizeof(sns_sensor_instance_api),
  .init = &qsh_ble_test_inst_init,
  .deinit = &qsh_ble_test_inst_deinit,
  .set_client_config = &qsh_ble_test_inst_set_client_config,
  .notify_event = &qsh_ble_test_inst_notify_event
};

