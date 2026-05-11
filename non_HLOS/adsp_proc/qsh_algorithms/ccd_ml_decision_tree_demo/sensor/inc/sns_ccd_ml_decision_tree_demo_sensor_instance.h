#pragma once
/**============================================================================
  @file sns_ccd_ml_decision_tree_demo_sensor_instance.h

  @brief Example dummy AR algo integrated with ccd_mldt

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_data_stream.h"
#include "sns_sensor_uid.h"
#include "sns_time.h"

#include "sns_ml_demo.pb.h"
#include "sns_ccd_gesture.pb.h"


#define MAX_NUM_MODELS 4
#define MAX_MODEL_NAME_LEN 64

typedef enum sns_ar_algo_mode 
{
  WAITING_STATE_EXIT_EVENT,
  PROCESSING_SAMPLES
} sns_ar_algo_mode;

typedef struct sns_ccd_ml_decision_tree_demo_sensor_inst_state
{
  bool is_configured;
  sns_ccd_te_event te_state;
  uint32_t te_state1;
  sns_ar_algo_mode state;
  sns_ar_motion_state curr_ar_state;
  sns_data_stream *ccd_mldt_stream;
  sns_data_stream *accel_stream;
  sns_data_stream *gyro_stream;
  sns_time prev_event_ts;
  sns_sensor_uid accel_uid;
  sns_sensor_uid gyro_uid;
  char model_list[MAX_NUM_MODELS][MAX_MODEL_NAME_LEN];
} sns_ccd_ml_decision_tree_demo_sensor_inst_state;