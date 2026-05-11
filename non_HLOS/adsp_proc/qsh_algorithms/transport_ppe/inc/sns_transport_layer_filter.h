#pragma once
/*=============================================================================
  @file sns_tppe_transport_layer_filter.h

  Transport layer filter

  Copyright (c) 2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Includes
  ===========================================================================*/

#include "sns_rc.h"
#include "sns_sensor_event.h"
#include "sns_transport_layer_util.h"
#include <stddef.h>
#include <stdint.h>

/*=============================================================================
  Public Function Declarations
  ===========================================================================*/

void sns_transport_filter(sns_transport_layer_api *api, sns_transport_decode_buffer *buffer,
                          sns_all_filters *triggers, sns_tppe_event_trigger *event_trigger);
