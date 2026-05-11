/*=============================================================================
  @file sns_ai_oem1.c

  AI_OEM1 (template algorithm) implementation

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_ai_oem1_sensor.h"
#include "sns_ai_oem1_sensor_instance.h"
#include "sns_register.h"
#include "sns_sensor.h"

/*=============================================================================
  External Variable Declarations
  ===========================================================================*/

extern sns_sensor_api 		   sns_ai_oem1_sensor_api;
extern sns_sensor_instance_api sns_ai_oem1_instance_api;

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

sns_rc sns_ai_oem1_register(sns_register_cb const *register_api)
{
  register_api->init_sensor(sizeof(sns_ai_oem1_sensor_state),
                            &sns_ai_oem1_sensor_api,
                            &sns_ai_oem1_instance_api);

  return SNS_RC_SUCCESS;
}


