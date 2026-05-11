#pragma once
/**============================================================================
  @file sns_ccd_ml_decision_tree_demo_sensor.h

  @brief Example dummy AR algo integrated with ccd_mldt

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_suid_util.h"

#define CCD_ML_DECISION_TREE_DEMO_SUID                                         \
  {                                                                            \
    .sensor_uid = {                                                            \
      0X22,                                                                    \
      0X4e,                                                                    \
      0X5c,                                                                    \
      0X37,                                                                    \
      0X2e,                                                                    \
      0X21,                                                                    \
      0X4b,                                                                    \
      0X67,                                                                    \
      0X8a,                                                                    \
      0X6e,                                                                    \
      0Xb9,                                                                    \
      0X06,                                                                    \
      0X64,                                                                    \
      0Xc1,                                                                    \
      0Xd3,                                                                    \
      0X18                                                                     \
    }                                                                          \
  }

typedef struct sns_ccd_ml_decision_tree_demo_sensor_state
{
  SNS_SUID_LOOKUP_DATA(3) lookup_data;
} sns_ccd_ml_decision_tree_demo_sensor_state;