#pragma once
/*=============================================================================
  @file qsh_audio_speaker_diarization_sensor.h

  SDZ Sensor

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_event_sensor.h"
#include "asps_api.h"
#include "asps_sdz_usecase_api.h"

#define SDZ_SUID \
{  \
  .sensor_uid =  \
  {  \
     0x03, 0xde, 0x34, 0x19, 0xe7, 0xe7, 0x9b, 0x83, \
     0x27, 0xa1, 0x44, 0x06, 0x85, 0x65, 0x2e, 0xe3  \
  }  \
}

#ifndef MIN
#define MIN(a, b)  (((a)<(b))?(a):(b))
#endif

#define IGNORE_RSP_TOKEN (0x11001001) //some random number for command token.

/*=============================================================================
  Type Definitions
  ===========================================================================*/
//audio context sensor state structure.
typedef struct
{
  //base class state structure.
  qsh_audio_event_sensor_state audio_event_state;

  //client handle of uQsocket client interface
  void *uqci_client_handle;

  //0: sdz is disabled (remove usecase)
  //1: sdz is enabled (start usecase)
  uint32_t sdz_enable;

  //module_iid of the sdz module in ADSP
  uint32_t module_iid;

} qsh_audio_speaker_diarization_sensor_state;

// sensor callback function to receive the messages from adsp.
void qsh_audio_speaker_diarization_uqci_receive_msg(void *cb_ctx_ptr, uqsocket_msg_header_t *msg_header);

// function to send usecase info event registration to asps-adsp.
void qsh_audio_speaker_diarization_uqci_register_usecase_info_event(qsh_audio_speaker_diarization_sensor_state *sensor_state_ptr, bool is_register);

// function to handle the received event for usecase info from the asps-adsp.
void qsh_audio_speaker_diarization_receive_usecase_info(sns_sensor *const this, asps_sdz_usecase_register_ack_payload_t *usecase_info_ptr);

// sensor callback function to send the event registration to the module.
sns_rc qsh_audio_speaker_diarization_req_send(sns_sensor *const this);

// sensor public api
sns_rc qsh_audio_speaker_diarization_init(sns_sensor *const this);
sns_rc qsh_audio_speaker_diarization_deinit(sns_sensor *const this);
sns_rc  qsh_audio_speaker_diarization_notify_event(sns_sensor *const this);
sns_sensor_instance * qsh_audio_speaker_diarization_set_client_request(sns_sensor *const this,
                                                       sns_request const *curr_req,
                                                       sns_request const *new_req,
                                                       bool               remove);
sns_sensor_uid const *qsh_audio_speaker_diarization_get_sensor_uid(sns_sensor const *this);