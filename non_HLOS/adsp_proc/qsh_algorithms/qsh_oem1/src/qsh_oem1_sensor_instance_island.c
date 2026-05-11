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
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_rc.h"
#include "sns_request.h"
#include "sns_sensor_instance.h"
#include "sns_time.h"

/*============================================================================
  Preprocessor Definitions and Constants
  ===========================================================================*/

/*============================================================================
  Static functions
  ===========================================================================*/

/* Process AMD events */
static void qsh_oem1_process_amd_event(sns_sensor_instance *const this, pb_istream_t *pbstream)
{
  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  sns_amd_event amd_data = sns_amd_event_init_default;
  if(!pb_decode_noinit(pbstream, sns_amd_event_fields, &amd_data))
  {
    SNS_INST_PRINTF(ERROR, this, "AMD: error in decoding amd data event");
  }

  // When the device is stationary,
  else if(SNS_AMD_EVENT_TYPE_STATIONARY == amd_data.state &&
          inst_state->amd_state.state != amd_data.state)
  {
    SNS_INST_PRINTF(HIGH, this, "AMD: received AMD stationary event [msgid=%d]",
                    SNS_AMD_MSGID_SNS_AMD_EVENT);
    inst_state->amd_state.state = amd_data.state;
    inst_state->amd_event_ts = sns_get_system_time();

    // Send the Location update request to stop the tracking session if it's running.
    if((inst_state->inst_config.location_capabilities & QSH_LOCATION_CAPABILITIES_POSITION) &&
       inst_state->inst_config.location_session_running)
    {
      qsh_oem1_island_exit();
      if(qsh_oem1_send_location_update_request(this, false) != SNS_RC_SUCCESS)
      {
        SNS_INST_PRINTF(ERROR, this,
                        "Location: error sending update request to stop position tracking");
      }
      else
      {
        SNS_INST_PRINTF(
            MED, this,
            "Location: request sent [msgid=%d] to stop position tracking on AMD stationary",
            QSH_LOCATION_MSGID_QSH_LOCATION_UPDATE);
      }
    }
    // Send WiFi scan request to scan for access points when the device is stationary
    if(inst_state->inst_config.wifi_svc_capabilities & QSH_WIFI_CAP_SCAN_REQUEST_BASED)
    {
      qsh_oem1_island_exit();
      if(qsh_oem1_send_wifi_scan_request(this) != SNS_RC_SUCCESS)
      {
        SNS_INST_PRINTF(ERROR, this, "WiFi: error sending scan request!");
      }
      else
      {
        SNS_INST_PRINTF(
            MED, this, "WiFi: request sent [msgid=%d] to scan wifi access points on AMD stationary",
            QSH_WIFI_MSGID_QSH_WIFI_CMD_SCAN_REQ);
      }
    }
  }

  // when the device is in motion,
  else if(SNS_AMD_EVENT_TYPE_MOTION == amd_data.state &&
          inst_state->amd_state.state != amd_data.state)
  {
    SNS_INST_PRINTF(HIGH, this, "AMD: received AMD motion event [msgid=%d]",
                    SNS_AMD_MSGID_SNS_AMD_EVENT);
    inst_state->amd_state.state = amd_data.state;
    inst_state->amd_event_ts = sns_get_system_time();

    // Send the Location update request to start the tracking session if it's not running.
    if((inst_state->inst_config.location_capabilities & QSH_LOCATION_CAPABILITIES_POSITION) &&
       !inst_state->inst_config.location_session_running)
    {
      qsh_oem1_island_exit();
      if(qsh_oem1_send_location_update_request(this, true) != SNS_RC_SUCCESS)
      {
        SNS_INST_PRINTF(ERROR, this,
                        "Location: error sending update request to start position tracking");
      }
      else
      {
        SNS_INST_PRINTF(
            MED, this, "Location: request sent [msgid=%d] to start position tracking on AMD motion",
            QSH_LOCATION_MSGID_QSH_LOCATION_UPDATE);
      }
    }
  }
}

/**
 * Process incoming events to the qsh_oem1 Sensor Instance.
 */
static sns_rc qsh_oem1_inst_notify_event(sns_sensor_instance *const this)
{
  sns_sensor_event *amd_event_in = NULL;
  sns_sensor_event *wifi_svc_event_in = NULL;
  sns_sensor_event *location_event_in = NULL;

  qsh_oem1_inst_state *inst_state = (qsh_oem1_inst_state *)this->state->state;

  // AMD events
  if(NULL != inst_state->amd_stream)
  {
    amd_event_in = inst_state->amd_stream->api->peek_input(inst_state->amd_stream);
    while(NULL != amd_event_in)
    {
      pb_istream_t pbstream =
          pb_istream_from_buffer((pb_byte_t *)amd_event_in->event, amd_event_in->event_len);

      if(SNS_AMD_MSGID_SNS_AMD_EVENT == amd_event_in->message_id)
      {
        qsh_oem1_process_amd_event(this, &pbstream);
      }
      else if(SNS_STD_MSGID_SNS_STD_ERROR_EVENT == amd_event_in->message_id)
      {
        SNS_INST_PRINTF(ERROR, this, "AMD: received SNS_STD_ERROR_EVENT [msgid=%d]",
                        SNS_STD_MSGID_SNS_STD_ERROR_EVENT);
      }

      amd_event_in = inst_state->amd_stream->api->get_next_input(inst_state->amd_stream);
    }
  }

  // qsh_wifi events
  if(NULL != inst_state->wifi_svc_stream)
  {
    wifi_svc_event_in = inst_state->wifi_svc_stream->api->peek_input(inst_state->wifi_svc_stream);
    while(NULL != wifi_svc_event_in)
    {
      pb_istream_t pbstream = pb_istream_from_buffer((pb_byte_t *)wifi_svc_event_in->event,
                                                     wifi_svc_event_in->event_len);

      if(QSH_WIFI_MSGID_QSH_WIFI_CMD_ACK == wifi_svc_event_in->message_id)
      {
        qsh_oem1_process_wifi_cmd_ack(this, &pbstream);
      }
      else if(QSH_WIFI_MSGID_QSH_WIFI_EVT_SCAN_RESULTS == wifi_svc_event_in->message_id)
      {
        qsh_oem1_process_wifi_evt_scan_result(this, &pbstream);
      }
      else if(SNS_STD_MSGID_SNS_STD_ERROR_EVENT == wifi_svc_event_in->message_id)
      {
        SNS_INST_PRINTF(ERROR, this, "WiFi: received SNS_STD_ERROR_EVENT [msgid=%d]",
                        SNS_STD_MSGID_SNS_STD_ERROR_EVENT);
      }

      wifi_svc_event_in =
          inst_state->wifi_svc_stream->api->get_next_input(inst_state->wifi_svc_stream);
    }
  }

  // qsh_location events
  if(NULL != inst_state->location_stream)
  {
    location_event_in = inst_state->location_stream->api->peek_input(inst_state->location_stream);
    while(NULL != location_event_in)
    {
      pb_istream_t pbstream = pb_istream_from_buffer((pb_byte_t *)location_event_in->event,
                                                     location_event_in->event_len);

      if(QSH_LOCATION_MSGID_QSH_LOCATION_ACK == location_event_in->message_id)
      {
        qsh_oem1_process_location_ack(this, &pbstream);
      }
      else if(QSH_LOCATION_MSGID_QSH_LOCATION_POSITION_EVENT == location_event_in->message_id)
      {
        qsh_oem1_process_location_position_event(this, &pbstream);
      }
      else if(QSH_LOCATION_MSGID_QSH_LOCATION_MEAS_AND_CLK_EVENT == location_event_in->message_id)
      {
        qsh_oem1_process_location_meas_and_clk_event(this, &pbstream);
      }
      else if(SNS_STD_MSGID_SNS_STD_ERROR_EVENT == location_event_in->message_id)
      {
        SNS_INST_PRINTF(ERROR, this, "Location: received SNS_STD_ERROR_EVENT [msgid=%d]",
                        SNS_STD_MSGID_SNS_STD_ERROR_EVENT);
      }

      location_event_in =
          inst_state->location_stream->api->get_next_input(inst_state->location_stream);
    }
  }

  return SNS_RC_SUCCESS;
}

/*===========================================================================
  Public Data Definitions
  ===========================================================================*/

sns_sensor_instance_api qsh_oem1_sensor_instance_api = {
    .struct_len = sizeof(sns_sensor_instance_api),
    .init = &qsh_oem1_inst_init,
    .deinit = &qsh_oem1_inst_deinit,
    .set_client_config = &qsh_oem1_inst_set_client_config,
    .notify_event = &qsh_oem1_inst_notify_event};
