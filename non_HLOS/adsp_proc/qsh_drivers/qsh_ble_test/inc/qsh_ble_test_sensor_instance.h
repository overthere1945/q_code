#pragma once

/*=============================================================================
  @file qsh_ble_test_sensor_instance.h

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include <stdbool.h>
#include <stdint.h>

#include "sns_data_stream.h"
#include "sns_event_service.h"
#include "sns_sensor.h"
#include "sns_sensor_event.h"
#include "sns_sensor_instance.h"
#include "sns_stream_service.h"
#include "sns_sensor_uid.h"
#include "sns_time.h"

typedef struct qsh_ble_test_inst_state
{
  const sns_sensor_uid *suid;

  sns_event_service *event_service;
  sns_data_stream *ble_stream;
  sns_sensor_uid ble_suid;

  uint32_t test_reqs;

  sns_time ble_test_started_timestamp;
  uint32_t num_of_ble_adv_event_received;
} qsh_ble_test_inst_state;
