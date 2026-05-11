#pragma once
/**============================================================================
  @file sns_ccd_ml_decision_tree_demo_algo.h

  @brief Interface to fake AR algorithm

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "sns_ccd_ml_decision_tree_demo_sensor_instance.h"
#include "sns_time.h"

#include "sns_ccd_gesture.pb.h"
#include "sns_ml_demo.pb.h"

typedef enum sample_type
{
  ACCEL,
  GYRO
} sample_type;

/**
 * @brief Struct to hold the output variables from the algorithm.
 */
 typedef struct {
  sns_ar_motion_state state; /**< The AR motion state. */
  char model_to_load[MAX_MODEL_NAME_LEN];       /**< Model to load */
  sns_time timestamp;        /**< Timestamp of the output. */
} sns_algo_output_struct;

/**
 * @brief Process a sample a data
 *
 * @param[in]  algo_state     Pointer to internal algo state
 * @param[in]  type           Accel or Gyro
 * @param[in]  data           Data array
 * @param[in]  ts             timestamp of data
 * @param[out] sns_algo_output_struct   Output data. See sns_algo_output_struct.
 *                                      Ignore if return value of function is false.
 *
 * @return
 *  - True  New state detected
 *  - False Otherwise
 */
bool sns_ccd_ml_decision_tree_demo_algo_process_sample(
    void *state,
    sample_type type, const float *data, sns_time ts,
    sns_algo_output_struct *output_struct);

/**
 * @brief Process a CCD Gesture TE event
 *
 * @param[in]  algo_state     Pointer to internal algo state
 * @param[in]  data           te state data
 * @param[in]  ts             timestamp of data   
 *
 * @return
 *  - None
 */
void sns_ccd_ml_decision_tree_demo_algo_process_te_state(
    void *state, const sns_ccd_te_event *data, sns_time ts);

/**
 * @brief Initialize the algo
 *
 * @param[in]  algo_state     Pointer to internal algo state
 *
 * @return
 *  - None
 */
void sns_ccd_ml_decision_tree_demo_algo_init(
    void* algo_state);

/**
 * @brief Get memory requirement for algo state
 *
 * @return
 *  - algo state in bytes
 */
size_t sns_ccd_ml_decision_tree_demo_algo_get_state_size(void);