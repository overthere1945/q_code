#pragma once
/** ======================================================================================
  @file sns_profile.h

  @brief Latency profile utility header

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

======================================================================================
**/

/**
*****************************************************************************************
                               Includes
*****************************************************************************************
*/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "sns_time.h"
/**
*****************************************************************************************
                               Structures
*****************************************************************************************
*/

typedef enum sns_profile_action
{
  PROFILE_START = 1,
  PROFILE_STOP = 2,
} sns_profile_action;

typedef struct _profiling
{
  uint64_t min_ticks;   // Minimum ticks taken for profiling
  uint64_t max_ticks;   // maximum ticks taken for profiling
  uint64_t avg_ticks;   // Running Average ticks
  uint64_t iterations;  // Number of iteartions
  uint64_t start_ticks; // Start ticks for profiling
} sns_profile_t;

void sns_latency_profile_reset(sns_profile_t *profiled_data);
void sns_latency_profile_log(sns_profile_t *profiled_data);
void sns_latency_profile(sns_profile_action action,
                         sns_profile_t *profiled_data);

void sns_profile_timeline(sns_profile_t *timeline_data, sns_time timestamp);
void sns_profile_timeline_log(sns_profile_t *timeline_data,
                              uint32_t identifier);
