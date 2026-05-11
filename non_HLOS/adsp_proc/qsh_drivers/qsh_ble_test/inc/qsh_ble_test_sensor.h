#pragma once

/*=============================================================================
  @file qsh_ble_test_sensor.h

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include <stdbool.h>

#include "sns_sensor.h"
#include "sns_sensor_uid.h"
#include "sns_std_type.pb.h"
#include "sns_suid_util.h"

// QSH BLE test sensor version: major.minor.revision(0x####.##.##)
#define QSH_BLE_TEST_SENSOR_VERSION 0x00010000U

#define QSH_BLE_TEST_SUID \
  {  \
    .sensor_uid =  \
    {  \
      0x17, 0x63, 0x8f, 0xf2, 0x2c, 0x44, 0x81, 0xb7,  \
      0xe3, 0x4b, 0x2e, 0xbe, 0xc1, 0xfa, 0x28, 0x3c  \
    }  \
  }

typedef struct qsh_ble_test_sensor_state {
  // Points to our SUID
  const sns_sensor_uid *suid;

  // ble
  SNS_SUID_LOOKUP_DATA(1) suid_lookup_data;
} qsh_ble_test_sensor_state;
