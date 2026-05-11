/*=============================================================================
  @file qsh_oem1.c

  qsh_oem1 (template algorithm) implementation

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_sensor.h"
#include "sns_register.h"
#include "qsh_oem1_sensor.h"
#include "sns_sensor_instance.h"

/*=============================================================================
  External Variable Declarations
  ===========================================================================*/

extern sns_sensor_instance_api qsh_oem1_sensor_instance_api;
extern sns_sensor_api qsh_oem1_api;

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

sns_rc qsh_oem1_register(sns_register_cb const *register_api)
{
  register_api->init_sensor(sizeof(qsh_oem1_sensor_state), &qsh_oem1_api,
                            &qsh_oem1_sensor_instance_api);

  return SNS_RC_SUCCESS;
}
