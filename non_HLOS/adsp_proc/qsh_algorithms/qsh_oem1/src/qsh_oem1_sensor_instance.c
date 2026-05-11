/*=============================================================================
  @file qsh_oem1_sensor_instance.c

  The qsh_oem1 virtual Sensor Instance implementation

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "pb_decode.h"
#include "pb_encode.h"
#include "qsh_oem1_sensor_instance.h"
#include "qurt_island.h"
#include "sns_event_service.h"
#include "sns_mem_util.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_rc.h"
#include "sns_sensor_instance.h"
#include "sns_service_manager.h"
#include "sns_stream_service.h"
#include "sns_time.h"
#include "uSleep_mode_trans.h"

sns_rc qsh_oem1_island_exit(void)
{
  if(qurt_island_get_status())
  {
    uSleep_exit();
  }
  return SNS_RC_SUCCESS;
}

/** Send on-change request to AMD sensor */
static void qsh_oem1_send_amd_request(sns_sensor_instance *const this)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  sns_request request = (sns_request){.message_id = SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG,
                                      .request_len = 0,
                                      .request = NULL};
  inst_state->amd_stream->api->send_request(inst_state->amd_stream, &request);
  SNS_INST_PRINTF(MED, this, "AMD: request sent [msgid=%d] to enable AMD",
                  SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG);
}

/** Send open command to qsh_wifi */
static void qsh_oem1_send_qsh_wifi_open_request(sns_sensor_instance *const this)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  qsh_wifi_cmd_open wifi_svc_open_cmd = qsh_wifi_cmd_open_init_default;
  wifi_svc_open_cmd.get_capability = true;
  wifi_svc_open_cmd.get_version = true;

  uint8_t buffer[20];
  size_t encoded_len =
      pb_encode_request(buffer, sizeof(buffer), &wifi_svc_open_cmd, qsh_wifi_cmd_open_fields, NULL);
  if(encoded_len > 0)
  {
    sns_request request = (sns_request){.message_id = QSH_WIFI_MSGID_QSH_WIFI_CMD_OPEN,
                                        .request_len = encoded_len,
                                        .request = buffer};

    inst_state->wifi_svc_stream->api->send_request(inst_state->wifi_svc_stream, &request);
    SNS_INST_PRINTF(MED, this, "WiFi: request sent [msgid=%d] to open qsh_wifi driver",
                    QSH_WIFI_MSGID_QSH_WIFI_CMD_OPEN);
  }
}

/** Send open command to qsh_location */
static void qsh_oem1_send_qsh_location_open_request(sns_sensor_instance *const this)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  qsh_location_open location_open = qsh_location_open_init_default;
  location_open.version = QSH_LOCATION_VERSION;

  uint8_t buffer[20];
  size_t encoded_len =
      pb_encode_request(buffer, sizeof(buffer), &location_open, qsh_location_open_fields, NULL);
  if(encoded_len > 0)
  {
    sns_request request = (sns_request){.message_id = QSH_LOCATION_MSGID_QSH_LOCATION_OPEN,
                                        .request_len = encoded_len,
                                        .request = buffer};

    inst_state->location_stream->api->send_request(inst_state->location_stream, &request);
    SNS_INST_PRINTF(MED, this, "Location: request sent [msgid=%d] to open qsh_location driver",
                    QSH_LOCATION_MSGID_QSH_LOCATION_OPEN);
  }
}

/** Decode wifi accesss point information from the scan results protobuf stream */
static bool qsh_oem1_decode_ap_list(pb_istream_t *pbstream, const pb_field_t *field, void **arg)
{
  UNUSED_VAR(field);
  sns_sensor_instance *this = *arg;
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  qsh_wifi_bss_info bss_info = qsh_wifi_bss_info_init_default;

  if(pb_decode_noinit(pbstream, qsh_wifi_bss_info_fields, &bss_info) &&
     inst_state->wifi_data.ap_count < MAX_WIFI_AP_LIST_COUNT)
  {
    qsh_oem1_wifi_info *ap = &(inst_state->wifi_data.ap_list[inst_state->wifi_data.ap_count++]);
    ap->rssi = bss_info.rssi;
    sns_memscpy(&ap->bssid, sizeof(ap->bssid), &bss_info.bssid, sizeof(bss_info.bssid));
    SNS_INST_PRINTF(LOW, this, "WiFi: [%02d] bssid=" PRI_MAC_FMT ", rssi=%d",
                    inst_state->wifi_data.ap_count, PRI_MAC_ARG(ap->bssid.bytes), ap->rssi);
  }
  return true;
}

/** Encode the list of access points to protobuf stream*/
static bool qsh_oem1_encode_ap_list(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
  UNUSED_VAR(field);
  sns_sensor_instance *this = *arg;
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  for(uint32_t index = 0; index < inst_state->wifi_data.ap_count; index++)
  {
    qsh_oem1_wifi_info ap;
    sns_memscpy(&ap, sizeof(ap), &inst_state->wifi_data.ap_list[index], sizeof(ap));

    if(!pb_encode_tag_for_field(stream, field))
    {
      SNS_INST_PRINTF(ERROR, this, "pb_encode_tag_for_field failed for qsh_oem1_wifi_info!");
      return false;
    }
    if(!pb_encode_submessage(stream, qsh_oem1_wifi_info_fields, &ap))
    {
      SNS_INST_PRINTF(ERROR, this, "pb_encode_submessage failed for qsh_oem1_wifi_info!");
      return false;
    }
  }
  return true;
}

/** Publish wifi scan results to the client */
static void qsh_oem1_publish_event(sns_sensor_instance *const this)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  qsh_oem1_event oem1_event = qsh_oem1_event_init_default;
  oem1_event.amd_event_ts = inst_state->amd_event_ts;
  oem1_event.amd_state = inst_state->amd_state.state;
  oem1_event.wifi_scan_ts = inst_state->wifi_data.wifi_scan_ts;
  oem1_event.wifi_access_point_count = inst_state->inst_config.wifi_svc_result_count;
  oem1_event.wifi_access_point_list.funcs.encode = &qsh_oem1_encode_ap_list;
  oem1_event.wifi_access_point_list.arg = this;

  // Update location fix if available
  if(inst_state->location_data.fix_utc_ts)
  {
    oem1_event.location_fix_utc_ts = inst_state->location_data.fix_utc_ts;
    oem1_event.location_latitude = inst_state->location_data.latitude;
    oem1_event.location_longitude = inst_state->location_data.longitude;
  }

  size_t encoded_event_len = 0;
  pb_get_encoded_size(&encoded_event_len, qsh_oem1_event_fields, &oem1_event);
  sns_sensor_event *event_out = inst_state->event_service->api->alloc_event(
      inst_state->event_service, this, encoded_event_len);

  pb_ostream_t stream = pb_ostream_from_buffer((uint8 *)event_out->event, event_out->event_len);

  if(!pb_encode(&stream, qsh_oem1_event_fields, (void *)&oem1_event))
  {
    SNS_INST_PRINTF(ERROR, this, "pb_encode failed for qsh_oem1_event!");
  }
  else
  {
    event_out->message_id = QSH_OEM1_MSGID_QSH_OEM1_EVENT;
    event_out->event_len = encoded_event_len;
    event_out->timestamp = inst_state->amd_event_ts;

    sns_rc rc = inst_state->event_service->api->publish_event(inst_state->event_service, this,
                                                              event_out, inst_state->suid);
    if(rc != 0)
    {
      SNS_INST_PRINTF(ERROR, this, "Error in publishing qsh_oem1_event [msgid=%d] to client, rc=%d",
                      QSH_OEM1_MSGID_QSH_OEM1_EVENT, rc);
    }
    else
    {
      SNS_INST_PRINTF(MED, this, "Publishing qsh_oem1_event [msgid=%d] to client was successful!",
                      QSH_OEM1_MSGID_QSH_OEM1_EVENT);
    }
  }
}

/* Send wifi scan request to qsh wifi sensor driver */
sns_rc qsh_oem1_send_wifi_scan_request(sns_sensor_instance *const this)
{
  sns_rc rc = SNS_RC_FAILED;
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  // Scan for all access points in 2.4Ghz and 5Ghz band
  qsh_wifi_cmd_scan_req wifi_svc_scan_req = qsh_wifi_cmd_scan_req_init_default;
  wifi_svc_scan_req.scan_type = QSH_WIFI_SCAN_TYPE_ACTIVE_AND_PASSIVE_DFS;
  wifi_svc_scan_req.band = QSH_WIFI_BAND_MASK_2P4_GHZ | QSH_WIFI_BAND_MASK_5_GHZ;

  uint8_t buffer[200];
  size_t encoded_len = pb_encode_request(buffer, sizeof(buffer), &wifi_svc_scan_req,
                                         qsh_wifi_cmd_scan_req_fields, NULL);
  if(encoded_len > 0)
  {
    sns_request request = (sns_request){.message_id = QSH_WIFI_MSGID_QSH_WIFI_CMD_SCAN_REQ,
                                        .request_len = encoded_len,
                                        .request = buffer};

    inst_state->wifi_data.ap_count = 0;
    sns_memset(inst_state->wifi_data.ap_list, 0, sizeof(inst_state->wifi_data.ap_list));
    rc = inst_state->wifi_svc_stream->api->send_request(inst_state->wifi_svc_stream, &request);
  }

  return rc;
}

/* Send location update request to qsh location sensor driver */
sns_rc qsh_oem1_send_location_update_request(sns_sensor_instance *const this, bool start)
{
  sns_rc rc = SNS_RC_FAILED;
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  // Send position request with 2 seconds interval
  qsh_location_update location_update_req = qsh_location_update_init_default;
  location_update_req.start = start;
  location_update_req.location_request = QSH_LOCATION_POSITION_REQUEST;
  location_update_req.interval = LOCATION_UPDATE_INTERVAL_MS;

  uint8_t buffer[50];
  size_t encoded_len = pb_encode_request(buffer, sizeof(buffer), &location_update_req,
                                         qsh_location_update_fields, NULL);
  if(encoded_len > 0)
  {
    sns_request request = (sns_request){.message_id = QSH_LOCATION_MSGID_QSH_LOCATION_UPDATE,
                                        .request_len = encoded_len,
                                        .request = buffer};

    inst_state->inst_config.location_session_running = start;
    rc = inst_state->location_stream->api->send_request(inst_state->location_stream, &request);
  }

  return rc;
}

/* Process Wifi event scanning results */
void qsh_oem1_process_wifi_evt_scan_result(sns_sensor_instance *const this, pb_istream_t *pbstream)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  qsh_wifi_evt_scan_results scan_result = qsh_wifi_evt_scan_results_init_default;
  scan_result.ap_list.funcs.decode = &qsh_oem1_decode_ap_list;
  scan_result.ap_list.arg = this;

  if(pb_decode_noinit(pbstream, qsh_wifi_evt_scan_results_fields, &scan_result))
  {
    if(scan_result.status == QSH_WIFI_ERROR_NONE)
    {
      inst_state->wifi_data.wifi_scan_ts = scan_result.ref_time;
      SNS_INST_PRINTF(HIGH, this, "WiFi: received scan result event [msgid=%d], result_count=%d",
                      QSH_WIFI_MSGID_QSH_WIFI_EVT_SCAN_RESULTS, (int)scan_result.result_count);

      // Wifi scan results maybe split into multiple events.
      // If this is the last event in the series and the total number of access points is
      // different then the previous scan results, publish the result to the client.
      if(scan_result.last_in_series &&
         scan_result.result_total != inst_state->inst_config.wifi_svc_result_count)
      {
        inst_state->inst_config.wifi_svc_result_count = scan_result.result_total;
        qsh_oem1_publish_event(this);
      }
    }
    else
    {
      SNS_INST_PRINTF(ERROR, this, "WiFi: received NACK [%d] for [msgid=%d]", scan_result.status,
                      QSH_WIFI_MSGID_QSH_WIFI_EVT_SCAN_RESULTS);
    }
  }
  else
  {
    SNS_INST_PRINTF(ERROR, this, "WiFi: error decoding scan result event [msgid=%d]",
                    QSH_WIFI_MSGID_QSH_WIFI_EVT_SCAN_RESULTS);
  }
}

/* Process Wifi command ACK */
void qsh_oem1_process_wifi_cmd_ack(sns_sensor_instance *const this, pb_istream_t *pbstream)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  qsh_wifi_cmd_ack cmd_ack = qsh_wifi_cmd_ack_init_default;
  if(pb_decode_noinit(pbstream, qsh_wifi_cmd_ack_fields, &cmd_ack))
  {
    if(cmd_ack.status == QSH_WIFI_ERROR_NONE)
    {
      SNS_INST_PRINTF(HIGH, this, "WiFi: recevied ACK for [msgid=%d]", cmd_ack.command);
      if(cmd_ack.command == QSH_WIFI_MSGID_QSH_WIFI_CMD_OPEN)
      {
        inst_state->inst_config.wifi_svc_version = cmd_ack.version;
        inst_state->inst_config.wifi_svc_capabilities = cmd_ack.capabilities;
        SNS_INST_PRINTF(MED, this, "WiFi: version=0x%x, capabilities=0x%x",
                        (int)inst_state->inst_config.wifi_svc_version,
                        (int)inst_state->inst_config.wifi_svc_capabilities);
      }
    }
    else
    {
      // Send error event to the client
      sns_std_error_event error_event;
      error_event.error = SNS_STD_ERROR_FAILED;
      if(!pb_send_event(this, sns_std_error_event_fields, &error_event, sns_get_system_time(),
                        SNS_STD_MSGID_SNS_STD_ERROR_EVENT, inst_state->suid))
      {
        SNS_INST_PRINTF(ERROR, this, "Failed to send error event!");
      }

      SNS_INST_PRINTF(ERROR, this, "WiFi: received NACK [%d] for [msgid=%d]", cmd_ack.status,
                      cmd_ack.command);
    }
  }
}

/* Process location command ACK */
void qsh_oem1_process_location_ack(sns_sensor_instance *const this, pb_istream_t *pbstream)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  qsh_location_ack ack = qsh_location_ack_init_default;
  if(pb_decode_noinit(pbstream, qsh_location_ack_fields, &ack))
  {
    if(ack.command == QSH_LOCATION_MSGID_QSH_LOCATION_OPEN)
    {
      if(ack.error)
      {
        SNS_INST_PRINTF(HIGH, this, "Location: received ACK for [msgid=%d]", ack.command);
      }
      else // the location driver sends a NACK if it's already opened.
      {
        SNS_INST_PRINTF(HIGH, this, "Location: received NACK for [msgid=%d]", ack.command);
      }
      inst_state->inst_config.location_version = ack.version;
      inst_state->inst_config.location_capabilities = ack.capabilities;
      SNS_INST_PRINTF(MED, this, "Location: version=0x%x, capabilities=0x%x",
                      (int)inst_state->inst_config.location_version,
                      (int)inst_state->inst_config.location_capabilities);
    }
    else if(ack.command == QSH_LOCATION_MSGID_QSH_LOCATION_UPDATE && !ack.error)
    {
      inst_state->inst_config.location_session_running = false;
      if(ack.error)
      {
        SNS_INST_PRINTF(HIGH, this, "Location: received ACK for [msgid=%d]", ack.command);
      }
      else
      {
        SNS_INST_PRINTF(ERROR, this, "Location: received NACK for [msgid=%d]", ack.command);
      }
    }
  }
}

/* Process location position events */
void qsh_oem1_process_location_position_event(sns_sensor_instance *const this,
                                              pb_istream_t *pbstream)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  qsh_location_position_event position_event = qsh_location_position_event_init_default;
  if(pb_decode_noinit(pbstream, qsh_location_position_event_fields, &position_event))
  {
    inst_state->location_data.fix_utc_ts = position_event.timestamp;
    inst_state->location_data.latitude = position_event.has_latitude ? position_event.latitude : 0;
    inst_state->location_data.longitude =
        position_event.has_longitude ? position_event.longitude : 0;

    SNS_INST_PRINTF(LOW, this, "Location: received position event [msgid=%d] LAT=%d, LON=%d",
                    QSH_LOCATION_MSGID_QSH_LOCATION_POSITION_EVENT, position_event.latitude,
                    position_event.longitude);

    // Send WiFi scan request to scan for access points
    if(inst_state->inst_config.wifi_svc_capabilities & QSH_WIFI_CAP_SCAN_REQUEST_BASED)
    {
      qsh_oem1_island_exit();
      if(qsh_oem1_send_wifi_scan_request(this) != SNS_RC_SUCCESS)
      {
        SNS_INST_PRINTF(ERROR, this, "WiFi: error sending scan request!");
      }
      else
      {
        SNS_INST_PRINTF(MED, this, "WiFi: request sent [msgid=%d] to scan wifi access points",
                        QSH_WIFI_MSGID_QSH_WIFI_CMD_SCAN_REQ);
      }
    }
  }
  else
  {
    SNS_INST_PRINTF(ERROR, this, "Location: error decoding position event [msgid=%d]",
                    QSH_LOCATION_MSGID_QSH_LOCATION_POSITION_EVENT);
  }
}

/* Process qsh_location_meas_and_clk_event */
void qsh_oem1_process_location_meas_and_clk_event(sns_sensor_instance *const this,
                                                  pb_istream_t *pbstream)
{
  qsh_location_meas_and_clk_event meas_and_clk_event = qsh_location_meas_and_clk_event_init_default;
  if(pb_decode_noinit(pbstream, qsh_location_meas_and_clk_event_fields, &meas_and_clk_event))
  {
    SNS_INST_PRINTF(LOW, this, "Location: received clock event [msgid=%d] clock-time=%u (ms)",
                    QSH_LOCATION_MSGID_QSH_LOCATION_MEAS_AND_CLK_EVENT,
                    (uint32_t)(meas_and_clk_event.clock.time / 1000000));
  }
  else
  {
    SNS_INST_PRINTF(ERROR, this, "Location: error decoding clock event [msgid=%d]",
                    QSH_LOCATION_MSGID_QSH_LOCATION_MEAS_AND_CLK_EVENT);
  }
}

/* See sns_sensor_instance_api::init */
sns_rc qsh_oem1_inst_init(sns_sensor_instance *this, sns_sensor_state const *state)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;
  qsh_oem1_sensor_state *sensor_state = (qsh_oem1_sensor_state *)state->state;
  sns_service_manager *service_mgr = this->cb->get_service_manager(this);

  sns_stream_service *stream_service =
      (sns_stream_service *)service_mgr->get_service(service_mgr, SNS_STREAM_SERVICE);
  inst_state->event_service =
      (sns_event_service *)service_mgr->get_service(service_mgr, SNS_EVENT_SERVICE);
  inst_state->suid = sensor_state->suid;

  sns_sensor_uid lookup_suid;
  sns_suid_lookup_get(&sensor_state->suid_lookup_data, "wifi_svc", &lookup_suid);
  stream_service->api->create_sensor_instance_stream(stream_service, this, lookup_suid,
                                                     &inst_state->wifi_svc_stream);

  sns_suid_lookup_get(&sensor_state->suid_lookup_data, "location", &lookup_suid);
  stream_service->api->create_sensor_instance_stream(stream_service, this, lookup_suid,
                                                     &inst_state->location_stream);

  sns_suid_lookup_get(&sensor_state->suid_lookup_data, "amd", &lookup_suid);
  stream_service->api->create_sensor_instance_stream(stream_service, this, lookup_suid,
                                                     &inst_state->amd_stream);

  inst_state->inst_config.wifi_svc_version = 0;
  inst_state->inst_config.wifi_svc_result_count = 0;
  inst_state->inst_config.wifi_svc_capabilities = QSH_WIFI_CAP_NONE;
  inst_state->inst_config.location_capabilities = 0;

  inst_state->location_data.fix_utc_ts = 0;
  inst_state->location_data.latitude = 0;
  inst_state->location_data.longitude = 0;

  inst_state->amd_state.state = SNS_AMD_EVENT_TYPE_UNKNOWN;

  if(NULL == inst_state->amd_stream || NULL == inst_state->location_stream ||
     NULL == inst_state->wifi_svc_stream)
  {
    return SNS_RC_FAILED;
  }
  return SNS_RC_SUCCESS;
}

/* See sns_sensor_instance_api::deinit */
sns_rc qsh_oem1_inst_deinit(sns_sensor_instance *const this)
{
  SNS_INST_PRINTF(MED, this, "qsh_oem1_inst_deinit");

  // Stop the Location tracking session if it's running
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;
  if(inst_state->inst_config.location_session_running)
  {
    qsh_oem1_send_location_update_request(this, false);
  }

  // Send a close request to the qsh_location driver
  sns_request request = (sns_request){
      .message_id = QSH_LOCATION_MSGID_QSH_LOCATION_CLOSE, .request_len = 0, .request = NULL};
  if(NULL != inst_state->location_stream)
  {
    inst_state->location_stream->api->send_request(inst_state->location_stream, &request);
  }
  // Remove all instance streams
  sns_sensor_util_remove_sensor_instance_stream(this, &inst_state->wifi_svc_stream);
  sns_sensor_util_remove_sensor_instance_stream(this, &inst_state->location_stream);
  sns_sensor_util_remove_sensor_instance_stream(this, &inst_state->amd_stream);

  return SNS_RC_SUCCESS;
}

/* See sns_sensor_instance_api::set_client_config */
sns_rc qsh_oem1_inst_set_client_config(sns_sensor_instance *const this,
                                       sns_request const *client_request)
{
  sns_rc rc = SNS_RC_SUCCESS;
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  // Only on-change request is supported
  if(SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG == client_request->message_id)
  {
    SNS_INST_PRINTF(LOW, this,
                    "qsh_oem1_inst_set_client_config: received on-change request [msgid=%u]",
                    SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG);

    // Send sensor config request to enable AMD
    qsh_oem1_send_amd_request(this);

    // Send open request to the qsh_wifi driver
    qsh_oem1_send_qsh_wifi_open_request(this);

    // Send open request to the qsh_location driver
    qsh_oem1_send_qsh_location_open_request(this);
  }

  else if(client_request->message_id == SNS_STD_MSGID_SNS_STD_FLUSH_REQ)
  {
    sns_sensor_util_send_flush_event(inst_state->suid, this);
  }

  else
  {
    rc = SNS_RC_NOT_SUPPORTED;
    SNS_INST_PRINTF(ERROR, this, "qsh_oem1_inst_set_client_config: Unsupported request [msgid=%u]",
                    client_request->message_id);
  }

  return rc;
}
