/*=============================================================================
  @file sns_ai_oem1_algo_run_island.c

  AI_OEM1 algorithm - execution APIs

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*=============================================================================
  Include Files
  ============================================================================*/

#include "sns_ai_oem1_algo.h"

/*=============================================================================
  Static Function Declaration
  ============================================================================*/

/*=============================================================================
  Public Function Definition
  ============================================================================*/

int16_t sns_ai_oem1_algo_run(
    sns_ai_oem1_algo_handle * algo_handle, 
    sns_ai_oem1_algo_input * input, 
    sns_ai_oem1_device_state * output, 
    bool * is_orientation_changed )
{

  // Variable definitions
  sns_ai_oem1_algo_state_type* algo_state = (sns_ai_oem1_algo_state_type*) algo_handle;
  EAI_RESULT model_ret;

  // process
  algo_state->past_state = algo_state->curr_state;

  model_ret = EAI_FAIL;

  sns_ai_oem1_algo_preprocess_cbuffer( input, &( algo_state->model_state.fe_state ) );

  sns_ai_oem1_algo_feature_extraction( &( algo_state->model_state.fe_state ) );

  // input quantizing
  sns_ai_oem1_aimet_quantization_apply(
      (void*)algo_state->model_state.eai_context.io_batch.inputs[0].addr,
      (void*)algo_state->model_state.fe_state.feature_vector,
      AI_OEM1_FEATURE_VECTOR_SIZE,
      0,
      algo_state->model_state.input_quantization );
  
  // execute model
  model_ret = sns_ai_oem1_eai_model_execute( &( algo_state->model_state.eai_context ) );
 
  if (model_ret != EAI_SUCCESS)
  {
    algo_state->curr_state = SNS_AI_OEM1_DEVICE_STATE_UNKNOWN;   
  }
  else
  {
    
    /* --------------------------------------------------------------------------- * 
     *                                                                             *
     * NOTE:                                                                       *
     *                                                                             *
     * If ML model's o/p layer has more than 1 node, reverse quantization would    *
     * return the floating point value corresponding to the o/p of each node..     *
     * These values can be interpreted based on the activation function used for   *
     * output layer neurons, while training the ML model.                          *
     *                                                                             *
     * --------------------------------------------------------------------------- */

    // output reverse quantization
    sns_ai_oem1_aimet_quantization_reverse(
        (void*)algo_state->model_state.ai_oem1_output,
        (void*)algo_state->model_state.eai_context.io_batch.outputs[0].addr,
        SNS_AI_OEM1_EAI_MODEL_LENOUTPUT,
        algo_state->model_state.output_quantization );
        
        

    /* --------------------------------------------------------------------------- *
     *                                                                             *
     * NOTE:                                                                       *
     *                                                                             *
     * AI_OEM1 eAI ML model has a single neuron in the output layer, with Sigmoid  *
     * Activation function.. => 0.0 <= out <= 1.0, rounding off the float to get   *
     * the integer typecast for device state.                                      *
     *                                                                             *
     * --------------------------------------------------------------------------- */

    algo_state->curr_state = (sns_ai_oem1_device_state)roundf(algo_state->model_state.ai_oem1_output[0]);
	
  }

  // orientation change detection
  if( algo_state->past_state == algo_state->curr_state )
  {
    algo_state->model_state.is_orientation_changed = false;
  }
  else
  {
    algo_state->model_state.is_orientation_changed = true;
  }

  // Finalize
  *output = algo_state->curr_state;
  *is_orientation_changed = algo_state->model_state.is_orientation_changed;

  return (int16_t) model_ret;
}

