/*=============================================================================
  @file sns_ai_oem1_algo_deinit.c

  AI_OEM1 algorithm - state deinitalization APIs

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

bool sns_ai_oem1_algo_deinit( sns_ai_oem1_algo_handle* algo_state )
{

  bool rc = true;

  sns_ai_oem1_algo_state_type* state = (sns_ai_oem1_algo_state_type*)algo_state;

  // Deinit EAI
  if( state->model_state.eai_context.eai_handle != 0 )
  {
    if( EAI_SUCCESS != eai_deinit( state->model_state.eai_context.eai_handle ) )
    {
      rc = false;
    }
  }
  return rc;

}

