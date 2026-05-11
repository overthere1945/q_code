#pragma once
/*==============================================================================
  @file sns_ai_oem1_algo_input.h

  AI_OEM1 Sensor Algorithm Input Interface

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include "sns_cbuffer.h"

/*=============================================================================
  Static Data Definitions
  ============================================================================*/

#define SNS_AI_OEM1_NUM_INPUT_AXIS    (3)

#define SNS_AI_OEM1_CIRC_BUF_SIZE   (100)
#define SNS_AI_OEM1_BATCH_SAMPLES    (50)

// AI_OEM1 algo is triggered when circular buffer count = 50
// When a new AI_OEM1 instance is initiated,
// Reset circular buffer count to, 50 - 100 = -50..
// So it waits 100 new samples to trigger AI_OEM1 algo.
#define SNS_AI_OEM1_RESET_BUF_CNT (-50)

/*=============================================================================
  Type Definitions
  ============================================================================*/

// Handle to circular buffer with required size/#axis/type
typedef struct sns_ai_oem1_algo_cbuf
{

  SNS_CBUFFER(SNS_AI_OEM1_CIRC_BUF_SIZE, SNS_AI_OEM1_NUM_INPUT_AXIS, float);

} sns_ai_oem1_algo_cbuf;

// Structure to store the "accel" buffer
typedef struct sns_ai_oem1_algo_input
{

  sns_ai_oem1_algo_cbuf      accel_buffer;

} sns_ai_oem1_algo_input;

/*!---------------------------------------------------------------------------*/

