#pragma once
/*==============================================================================
  @file sns_ai_oem1_algo_model.h

  AI_OEM1 model parameters

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include "sns_ai_oem1_algo.h"

/*=============================================================================
  Static Data Definitions
  ============================================================================*/

#define AI_OEM1_FEATURE_VECTOR_SIZE       ( 3 )
#define AI_OEM1_PREPROC_SAMPLE_SIZE       ( 30 )
#define AI_OEM1_PREPROC_MAX_SAMPLE_SIZE   ( 50 )

#define SNS_AI_OEM1_ACCEL_XAXIS_MIN       ( -14.78 )
#define SNS_AI_OEM1_ACCEL_XAXIS_MAX       (  31.13 )

#define SNS_AI_OEM1_ACCEL_YAXIS_MIN       ( -11.89 )
#define SNS_AI_OEM1_ACCEL_YAXIS_MAX       (  24.34 )

#define SNS_AI_OEM1_ACCEL_ZAXIS_MIN       ( -19.14 )
#define SNS_AI_OEM1_ACCEL_ZAXIS_MAX       (  24.61 )

/*=============================================================================
  Type Definitions
  ============================================================================*/

typedef struct sns_ai_oem1_algo_fe_state
{

  bool  is_preproc_vector_full;
  float feature_vector[AI_OEM1_FEATURE_VECTOR_SIZE];                                 // features for classifier
  float preprocess_output[AI_OEM1_PREPROC_SAMPLE_SIZE][AI_OEM1_FEATURE_VECTOR_SIZE]; // output samples from pre-processing
  uint32_t post_preproc_sample_count;

} sns_ai_oem1_algo_fe_state;

typedef struct sns_ai_oem1_algo_model_state_type
{

  bool is_orientation_changed;
  float ai_oem1_output[SNS_AI_OEM1_EAI_MODEL_LENOUTPUT];  
  sns_ai_oem1_algo_fe_state fe_state;
  sns_ai_oem1_algo_eai_context_t eai_context;
  sns_ai_oem1_eai_model_quant_info_t input_quantization;
  sns_ai_oem1_eai_model_quant_info_t output_quantization;
  
} sns_ai_oem1_algo_model_state_type;

