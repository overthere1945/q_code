#pragma once
/*==============================================================================
  @file sns_ai_oem1_algo_math.h

  AI_OEM1 math functions

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

/*=============================================================================
  Macro Definitions
  ============================================================================*/
#define SNS_AI_OEM1_MAX( a, b ) \
  ( { __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; } )

#define SNS_AI_OEM1_MIN( a, b ) \
  ( { __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; } )

#ifndef ARR_SIZE
#define ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

/*=============================================================================
  Public Function Declaration
  ============================================================================*/

