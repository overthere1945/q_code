#pragma once
/*=============================================================================
  @file qsh_audio_speaker_diarization_sensor_instance.h

  SDZ Sensor instance

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_sensor_instance.h"
#include "qsh_audio_speaker_diarization_sensor.h"
#include "qsh_audio_speaker_diarization.pb.h"

#include "asps_api.h"
#include "asps_sdz_usecase_api.h"
#include "sdz_api.h"

/*============================================================================
  Type Declarations
  ===========================================================================*/


//acs sensor state structure
typedef struct sns_sdz_inst_state
{
  //handle of event service to publish sdz events or error events.
  sns_event_service *event_service;

  //sdz enable/disable.
  uint32_t sdz_enable;
} qsh_audio_speaker_diarization_inst_state;

void qsh_audio_speaker_diarization_inst_publish_event(sns_sensor_instance *const this, event_id_sdz_output_event_t *event_ptr);

sns_rc qsh_audio_speaker_diarization_inst_init(sns_sensor_instance *this, sns_sensor_state const *state_ptr);
sns_rc qsh_audio_speaker_diarization_inst_deinit(sns_sensor_instance *const this);
sns_rc qsh_audio_speaker_diarization_inst_set_client_config(sns_sensor_instance *const this, sns_request const *client_request_ptr);
