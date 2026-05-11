/*=============================================================================
  @file sns_ai_oem1_algo_feature_extraction_island.c

  AI_OEM1 algorithm - feature extraction APIs

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

void sns_ai_oem1_algo_feature_extraction(
    sns_ai_oem1_algo_fe_state * state )
{

  // feature extraction block,
  // Inputs are the three channels (x, y, z) of output of preprocessing block,
  // Output is the feature vector

  uint32_t k;
  float x_mean = 0;
  float y_mean = 0;
  float z_mean = 0;

  for( k = 0; k < state->post_preproc_sample_count; k++){

    x_mean += state->preprocess_output[k][0];
    y_mean += state->preprocess_output[k][1];
    z_mean += state->preprocess_output[k][2];

  }

  x_mean /= state->post_preproc_sample_count;
  y_mean /= state->post_preproc_sample_count;
  z_mean /= state->post_preproc_sample_count;

  state->feature_vector[0] = x_mean;
  state->feature_vector[1] = y_mean;
  state->feature_vector[2] = z_mean;

}

