/**============================================================================
  @file sns_tppe.c

  @brief TPPE (template algorithm) implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_register.h"
#include "sns_sensor_instance.h"
#include "sns_sensor.h"
#include "sns_tppe_sensor.h"

/*=============================================================================
  External Variable Declarations
  ===========================================================================*/

extern const sns_sensor_instance_api sns_tppe_sensor_instance_api;
extern const sns_sensor_api sns_tppe_api;

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

sns_rc sns_tppe_register(sns_register_cb const *register_api)
{
  register_api->init_sensor(sizeof(sns_tppe_sensor_state), &sns_tppe_api,
                            &sns_tppe_sensor_instance_api);

  return SNS_RC_SUCCESS;
}
