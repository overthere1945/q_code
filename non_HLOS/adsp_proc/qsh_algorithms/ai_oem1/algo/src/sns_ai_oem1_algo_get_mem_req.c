/*=============================================================================
  @file sns_ai_oem1_algo_get_mem_req.c

  AI_OEM1 algorithm - get memory requirement

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

size_t sns_ai_oem1_algo_get_mem_req(sns_ai_oem1_mem_type mem_type )
{

  size_t mem_req = 0;

  switch( mem_type )
  {
    case SNS_AI_OEM1_MEM_TYPE_ALGO_STATE_SIZE:
    {
      mem_req = sizeof( sns_ai_oem1_algo_state_type );
      break;
    }
    case SNS_AI_OEM1_MEM_TYPE_MODEL_ALIGNMENT:
    {
      eai_get_property( NULL, EAI_PROP_MODEL_ALIGNMENT, &mem_req );
      break;
    }
    case SNS_AI_OEM1_MEM_TYPE_MODEL_SIZE:
    {
      mem_req = (size_t)SNS_AI_OEM1_EAI_MODEL_MEMSIZE;
      break;
    }
    default:
    {
      mem_req = 0;
      break;
    }
  }
  return mem_req;
  
}

/* ---------------------------------------------------------------------------*/

size_t sns_ai_oem1_algo_get_model_mem_req(
    sns_ai_oem1_algo_handle* algo_state,
    sns_ai_oem1_model_mem_type mem_type )
{

  sns_ai_oem1_algo_state_type* state = (sns_ai_oem1_algo_state_type*)algo_state;

  size_t size = 0;

  switch( mem_type )
  {
    case SNS_AI_OEM1_MODEL_MEM_TYPE_SCRATCH_SIZE:
    {
      size = (size_t)state->model_state.eai_context.scratch_buffer.memory_size;
      break;
    }
    case SNS_AI_OEM1_MODEL_MEM_TYPE_PERSISTENT_SIZE:
    {
      size = (size_t)state->model_state.eai_context.persistent_buffer.memory_size;
      break;
    }
    default:
    {
      size = 0;
      break;
    }
  }
  return size;
  
}

/* ---------------------------------------------------------------------------*/
