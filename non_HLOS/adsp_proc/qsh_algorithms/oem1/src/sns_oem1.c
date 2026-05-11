/**============================================================================
  @file sns_oem1.c

  @brief OEM1 (template algorithm) implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_sensor.h"
#include "sns_register.h"
#include "sns_oem1_sensor.h"
#include "sns_sensor_instance.h"

/*=============================================================================
  External Variable Declarations
  ===========================================================================*/

extern const sns_sensor_instance_api sns_oem1_sensor_instance_api;
extern const sns_sensor_api sns_oem1_api;

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

sns_rc sns_oem1_register(sns_register_cb const *register_api)
{
  register_api->init_sensor(sizeof(sns_oem1_sensor_state), &sns_oem1_api,
                            &sns_oem1_sensor_instance_api);

  return SNS_RC_SUCCESS;
}
