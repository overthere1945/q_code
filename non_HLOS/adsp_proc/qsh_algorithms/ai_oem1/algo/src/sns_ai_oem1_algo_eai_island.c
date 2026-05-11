/*=============================================================================
  @file sns_ai_oem1_algo_eai_island.c

  AI_OEM1 algo EAI APIs

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

EAI_RESULT sns_ai_oem1_eai_model_execute(
    sns_ai_oem1_algo_eai_context_t * context )
{

  EAI_RESULT eai_ret = EAI_SUCCESS;

  // Execute eAI
  eai_ret = eai_execute( context->eai_handle,
                         &( context->io_batch ),
                         1 );

  return eai_ret;

}

/* ---------------------------------------------------------------------------*/
