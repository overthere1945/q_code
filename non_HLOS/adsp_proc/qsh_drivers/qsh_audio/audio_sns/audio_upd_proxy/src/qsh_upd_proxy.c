/**============================================================================
  @file qsh_upd_proxy.c

  @brief Ultrasound Proximity implementation

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

#include "qsh_upd_proxy_sensor.h"

/*=============================================================================
  External Variable Declarations
  ===========================================================================*/

// apis for upd proxy sensor
extern const sns_sensor_instance_api qsh_upd_proxy_sensor_instance_api;
extern const sns_sensor_api          qsh_upd_proxy_api;


/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

sns_rc qsh_upd_proxy_register(sns_register_cb const *register_api)
{

   // initializing the UPD proxy sensor.
   register_api->init_sensor(sizeof(qsh_upd_proxy_sensor_state),
                             &qsh_upd_proxy_api,
                             &qsh_upd_proxy_sensor_instance_api);

   return SNS_RC_SUCCESS;
}
