#pragma once
/**============================================================================
  @file sns_ccd_ml_decision_tree_demo_algo.h

  @brief Example dummy AR algo integrated with ccd_mldt

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "sns_time.h"
#include "sns_ccd_gesture.pb.h"

#include <stdbool.h>

typedef struct sns_ccd_ml_decision_tree_demo_algo_state
{
  uint8_t state_idx;
  sns_time start_time;
  sns_time prev_sample_ts;
  sns_time te_event_ts;
  sns_ccd_te_event te_state;
} sns_ccd_ml_decision_tree_demo_algo_state;
