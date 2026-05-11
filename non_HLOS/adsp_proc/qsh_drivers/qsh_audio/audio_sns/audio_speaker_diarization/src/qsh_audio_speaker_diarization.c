/*=============================================================================
  @file qsh_audio_speaker_diarization.c

  Speaker Diarization implementation

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_register.h"
#include "sns_sensor.h"
#include "sns_sensor_instance.h"

#include "qsh_audio_speaker_diarization_sensor.h"

/*=============================================================================
  External Variable Declarations
  ===========================================================================*/

// apis for speaker diarization sensor
extern const sns_sensor_instance_api qsh_audio_speaker_diarization_sensor_instance_api;
extern const sns_sensor_api          qsh_audio_speaker_diarization_api;

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

sns_rc qsh_audio_speaker_diarization_register(sns_register_cb const *register_api)
{

   // initializing the speaker diarization sensor.
   register_api->init_sensor(sizeof(qsh_audio_speaker_diarization_sensor_state),
                             &qsh_audio_speaker_diarization_api,
                             &qsh_audio_speaker_diarization_sensor_instance_api);

   return SNS_RC_SUCCESS;
}
