#pragma once
/*=============================================================================
  @file sns_tppe_debug_info.h

  Contains function definitions required for duplicate detection and filtering

  Copyright (c) 2022-23 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "sns_tppe_sensor_instance.h"

void print_all_triggers(sns_sensor_instance *const this);
void handle_debug_dump_request(sns_sensor_instance *const this);