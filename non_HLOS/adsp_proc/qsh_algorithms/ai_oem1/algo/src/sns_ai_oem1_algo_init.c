/*=============================================================================
  @file sns_ai_oem1_algo_init.c

  AI_OEM1 algorithm - state initalization APIs

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*=============================================================================
  Include Files
  ============================================================================*/

#include "sns_ai_oem1_algo.h"

/*=============================================================================
  Public Function Definition
  ============================================================================*/

bool sns_ai_oem1_algo_model_init(
    sns_ai_oem1_algo_handle * algo_handle, 
    void * restrict model_buf, 
    sns_ai_oem1_mem_location model_loc )
{

  bool ret = true;

  sns_ai_oem1_algo_state_type* state = (sns_ai_oem1_algo_state_type*)algo_handle;

  // EAI: Load model to context
  state->model_state.eai_context.buffer_memtype = (EAI_MEM_TYPE)model_loc;

  state->model_state.eai_context.model_buffer.addr = model_buf;

  state->model_state.eai_context.eai_init_flags = EAI_INIT_FLAGS_LPI;
  
  // Initialize client performance config
  state->model_state.eai_context.perf_config = (eai_client_perf_config_t) EAI_CLIENT_PERF_CONFIG_INIT;
  state->model_state.eai_context.perf_config.priority = EAI_CLIENT_PRIORITY_MEDIUM;
  
  // Load eAI model to context's model buffer
  sns_ai_oem1_eai_model_load( &( state->model_state.eai_context ),
                              (char*)sns_ai_oem1_eai_model,
                              SNS_AI_OEM1_EAI_MODEL_MEMSIZE );
  
  // Initialize the eai runtime instance and get the handle to the same 
  ret &= sns_ai_oem1_eai_model_init( &( state->model_state.eai_context ) );

  // Finalize
  return ret;
  
}

/* ---------------------------------------------------------------------------*/

bool sns_ai_oem1_algo_state_init(
    sns_ai_oem1_algo_handle * algo_handle, 
    void * restrict scratch_buf, 
    void * restrict persistent_buf )
{

  bool ret = true;

  sns_ai_oem1_algo_state_type* state = (sns_ai_oem1_algo_state_type*)algo_handle;
  
  // Initialize model_state, and associated attributes : states/quantization information
  state->model_state.is_orientation_changed = false;
  
  state->curr_state = SNS_AI_OEM1_DEVICE_STATE_UNKNOWN;
  state->past_state = SNS_AI_OEM1_DEVICE_STATE_UNKNOWN;

  // Store the i/p & o/p quantization parameters as model_state attributes
  sns_memscpy( &state->model_state.input_quantization,
               sizeof( state->model_state.input_quantization ),
               &sns_ai_oem1_eai_model_input_quantization,
               sizeof( sns_ai_oem1_eai_model_input_quantization ) );

  sns_memscpy( &state->model_state.output_quantization,
               sizeof( state->model_state.output_quantization ),
               &sns_ai_oem1_eai_model_output_quantization,
               sizeof( sns_ai_oem1_eai_model_output_quantization ) );

  // Set associated scratch & persistent buffers -> apply config to eAI runtime instance 
  ret &= sns_ai_oem1_eai_model_setbuffers( &( state->model_state.eai_context ),
                                           scratch_buf,
                                           persistent_buf );

  // Finalize
  return ret;
  
}

/* ---------------------------------------------------------------------------*/
