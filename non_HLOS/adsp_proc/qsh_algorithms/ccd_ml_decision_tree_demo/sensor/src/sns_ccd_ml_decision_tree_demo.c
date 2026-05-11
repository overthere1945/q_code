/**============================================================================
  @file sns_ccd_ml_decision_tree_demo.c

  @brief Example dummy AR algo integrated with ccd_mldt

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "sns_ccd_ml_decision_tree_demo_sensor.h"
#include "sns_register.h"
#include "sns_sensor_instance.h"
#include "sns_sensor.h"

extern const sns_sensor_instance_api
    sns_ccd_ml_decision_tree_demo_sensor_inst_api;
extern const sns_sensor_api sns_ccd_ml_decision_tree_demo_sensor_api;

sns_rc
sns_ccd_ml_decision_tree_demo_register(sns_register_cb const *register_api)
{
  register_api->init_sensor(sizeof(sns_ccd_ml_decision_tree_demo_sensor_state),
                            &sns_ccd_ml_decision_tree_demo_sensor_api,
                            &sns_ccd_ml_decision_tree_demo_sensor_inst_api);

  return SNS_RC_SUCCESS;
}