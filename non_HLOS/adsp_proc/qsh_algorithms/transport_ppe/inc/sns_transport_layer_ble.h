#pragma once
/*=============================================================================
  @file sns_tppe_transport_layer_ble.h

  Transport layer utility for BLE

  Copyright (c) 2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Includes
  ===========================================================================*/

#include "sns_memory_service.h"
#include "sns_rc.h"
#include "sns_sensor_event.h"
#include "sns_tppe_messages.h"
#include <stddef.h>
#include <stdint.h>

/*=============================================================================
  Macros
  ===========================================================================*/

#define SNS_TRANSPORT_LAYER_BLE_PROTO_NAME "qsh_ble_ext.proto"

/*=============================================================================
  Type Definitions
  ===========================================================================*/

sns_rc sns_transport_layer_ble_decode_event(sns_sensor_instance *instance, sns_sensor_event *event,
                                            sns_transport_decode_buffer *buffer,
                                            sns_memory_service *memory_service);
