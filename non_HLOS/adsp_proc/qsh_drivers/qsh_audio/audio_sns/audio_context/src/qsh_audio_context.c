/**============================================================================
  @file qsh_audio_context.c

  @brief Audio Context Sensor implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_register.h"
#include "sns_sensor.h"
#include "sns_sensor_instance.h"

#include "qsh_audio_context_sensor.h"

/*=============================================================================
  External Variable Declarations
  ===========================================================================*/

// apis for audio context proxy sensor
extern const sns_sensor_instance_api qsh_audio_context_sensor_instance_api;
extern const sns_sensor_api          qsh_audio_context_api;


/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

sns_rc qsh_audio_context_register(sns_register_cb const *register_api)
{

   // initializing the audio context proxy sensor.
   register_api->init_sensor(sizeof(qsh_audio_context_sensor_state),
                             &qsh_audio_context_api,
                             &qsh_audio_context_sensor_instance_api);

   return SNS_RC_SUCCESS;
}
