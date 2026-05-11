#pragma once
/*==============================================================================
  @file sns_ai_oem1_algo.h

  AI_OEM1 Sensor Algorithm Interface

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <complex.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sns_ai_oem1_algo_eai_model.h"
#include "sns_ai_oem1_algo_input.h"
#include "sns_ai_oem1_algo_interface.h"
#include "sns_ai_oem1_algo_math.h"
#include "sns_ai_oem1_algo_model.h"
#include "sns_math_util.h"
#include "msg.h"

/*=============================================================================
  Type Definitions
  ============================================================================*/

typedef struct sns_ai_oem1_algo_state_type
{
  sns_ai_oem1_algo_model_state_type model_state;

  sns_ai_oem1_device_state curr_state;       // current device orientation state
  sns_ai_oem1_device_state past_state;       // previous device orientation state
  
} sns_ai_oem1_algo_state_type;

/*=============================================================================
  Public Function Declaration
  ============================================================================*/

// Feature extraction code
void sns_ai_oem1_algo_preprocess_cbuffer( sns_ai_oem1_algo_input* input,
                                          sns_ai_oem1_algo_fe_state* state );

void sns_ai_oem1_algo_feature_extraction( sns_ai_oem1_algo_fe_state* state );

// AIMET quant code
void sns_ai_oem1_aimet_quantization_apply( void* output,
                                           void* input,
                                           size_t length,
                                           size_t pad,
                                           sns_ai_oem1_eai_model_quant_info_t quant_info );

void sns_ai_oem1_aimet_quantization_reverse( void* output,
                                             void* input,
                                             size_t length,
                                             sns_ai_oem1_eai_model_quant_info_t quant_info );

// EAI model code
void sns_ai_oem1_eai_model_load( sns_ai_oem1_algo_eai_context_t* eai_context,
                                 const char* eai_model,
                                 size_t eai_model_size );

bool sns_ai_oem1_eai_model_init( sns_ai_oem1_algo_eai_context_t* context );

bool sns_ai_oem1_eai_model_setbuffers( sns_ai_oem1_algo_eai_context_t* context,
                                       void* restrict scratch_buf,
                                       void* restrict persistent_buf );

EAI_RESULT sns_ai_oem1_eai_model_execute( sns_ai_oem1_algo_eai_context_t* context );
