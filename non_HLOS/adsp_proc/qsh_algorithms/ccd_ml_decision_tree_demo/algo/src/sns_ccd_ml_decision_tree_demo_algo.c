/**============================================================================
  @file sns_ccd_ml_decision_tree_demo_algo.c

  @brief Example dummy AR algo integrated with ccd_mldt

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "sns_ccd_ml_decision_tree_demo_algo.h"
#include "sns_ccd_ml_decision_tree_demo_algo_interface.h"
#include "sns_ccd_ml_decision_tree_demo_sensor_instance.h"
#include "sns_mem_util.h"
#include "sns_printf.h"
#include "sns_types.h"

#include "sns_ml_demo.pb.h"

#define MAX_GAP_NS  1000000000ULL //1 Seconds
#define DURATION_NS 3000000000ULL //3 Seconds

typedef struct {
    sns_ar_motion_state state;
    const char *model;
} StateModelMapping;

static const StateModelMapping state_model_mappings[] =
{
  {SNS_AR_STATIONARY, "StatExit_Detector"},
  {SNS_AR_WALK, "WalkExit_Detector"},
  {SNS_AR_RUN, "RunExit_Detector"},
  {SNS_AR_MV, "VehExit_Detector"}
};

size_t sns_ccd_ml_decision_tree_demo_algo_get_state_size(void)
{
    return sizeof(sns_ccd_ml_decision_tree_demo_algo_state);
}


/**
 * @brief Fake AR algorithm data processor
 *
 * This function simulates the processing of 3 seconds of data and cycles through
 * predefined AR states. It serves as a placeholder for a real algorithm that can
 * detect actual states and request a meaningful model for CCD to load.
 */
bool sns_ccd_ml_decision_tree_demo_algo_process_sample(
    void *state, sample_type type, const float *data, sns_time ts, 
    sns_algo_output_struct *output_struct)
{
  UNUSED_VAR(data);
  UNUSED_VAR(type);
  sns_ccd_ml_decision_tree_demo_algo_state *algo_state =
      (sns_ccd_ml_decision_tree_demo_algo_state*)state;
  bool state_detected = false;

  sns_time delta = ts - algo_state->prev_sample_ts;

  // If large gap between samples or first ever sample, treat this data as the new start time
  // for 3 second duration calculation
  if (algo_state->start_time == 0 || delta >= sns_convert_ns_to_ticks(MAX_GAP_NS))//TODO: add some error?
  {
      algo_state->start_time = ts;
  }

  sns_time elapsed_time = ts - algo_state->start_time;

  //Fake process 3 seconds of data
  if (elapsed_time >= sns_convert_ns_to_ticks(DURATION_NS)) {
      //Cycle through AR states
      algo_state->state_idx = (algo_state->state_idx + 1) % ARR_SIZE(state_model_mappings);
      algo_state->start_time = ts;
      state_detected = true;
  }

  output_struct->state = state_model_mappings[algo_state->state_idx].state;
  sns_strlcpy(output_struct->model_to_load, state_model_mappings[algo_state->state_idx].model, MAX_MODEL_NAME_LEN);
  output_struct->timestamp = sns_get_system_time();

  algo_state->prev_sample_ts = ts;
  return state_detected;
}

/**
 * @brief Fake process CCD te gesture event.
 * TE state can be used to support in algorithm processing
 * or to help determine which model to load.
 */
void sns_ccd_ml_decision_tree_demo_algo_process_te_state(
    void *state, const sns_ccd_te_event *data, sns_time ts)
{
  sns_ccd_ml_decision_tree_demo_algo_state *algo_state =
      (sns_ccd_ml_decision_tree_demo_algo_state*)state;

  algo_state->te_state = *data;
  algo_state->te_event_ts = ts;
}

void sns_ccd_ml_decision_tree_demo_algo_init(
    void* state)
{
  sns_ccd_ml_decision_tree_demo_algo_state *algo_state =
      (sns_ccd_ml_decision_tree_demo_algo_state*)state;
  algo_state->prev_sample_ts = 0;
  algo_state->start_time = 0;
  algo_state->state_idx = 0;
}