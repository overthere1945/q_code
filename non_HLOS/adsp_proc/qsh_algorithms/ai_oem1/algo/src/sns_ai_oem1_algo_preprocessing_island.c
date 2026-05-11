/*=============================================================================
  @file sns_ai_oem1_algo_preprocessing_island.c

  AI_OEM1 algorithm - preprocessing APIs

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

void sns_ai_oem1_algo_preprocess_cbuffer(
    sns_ai_oem1_algo_input * input, 
    sns_ai_oem1_algo_fe_state * state )
{

  uint32_t k;
  uint32_t head_idx;
  uint32_t sample_count = 0;
  bool consider_frame = true;
  float* accel_frame = NULL;
  state->is_preproc_vector_full = false;
  state->post_preproc_sample_count = 0;

  for( k = 0; k < AI_OEM1_PREPROC_MAX_SAMPLE_SIZE && sample_count < AI_OEM1_PREPROC_SAMPLE_SIZE ; k++ )
  {

    consider_frame = true;
    head_idx = AI_OEM1_PREPROC_MAX_SAMPLE_SIZE - k - 1;
    accel_frame = SNS_CBUFFER_GET_HEAD( input->accel_buffer, (int)head_idx );
    
    if((accel_frame[0] <= SNS_AI_OEM1_ACCEL_XAXIS_MIN && accel_frame[0] >= SNS_AI_OEM1_ACCEL_XAXIS_MAX) || 
       (accel_frame[1] <= SNS_AI_OEM1_ACCEL_YAXIS_MIN && accel_frame[1] >= SNS_AI_OEM1_ACCEL_YAXIS_MAX) ||
       (accel_frame[2] <= SNS_AI_OEM1_ACCEL_ZAXIS_MIN && accel_frame[2] >= SNS_AI_OEM1_ACCEL_ZAXIS_MAX) )
    {
      consider_frame = false;
    }

    if(true == consider_frame)
    {
      state->preprocess_output[sample_count][0] = accel_frame[0];
      state->preprocess_output[sample_count][1] = accel_frame[1];
      state->preprocess_output[sample_count][2] = accel_frame[2];
      sample_count += 1;
    }

  }

  if(AI_OEM1_PREPROC_SAMPLE_SIZE == sample_count)
  {
    state->is_preproc_vector_full = true;
  }
  
  state->post_preproc_sample_count = sample_count;

}

