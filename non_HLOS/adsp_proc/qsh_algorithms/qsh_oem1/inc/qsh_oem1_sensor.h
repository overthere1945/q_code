#pragma once
/*=============================================================================
  @file qsh_oem1_sensor.h

  The qsh_oem1 virtual Sensor

  Copyright (c) 2021-2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_data_stream.h"
#include "sns_sensor.h"
#include "sns_sensor_uid.h"
#include "sns_sensor_util.h"
#include "sns_suid_util.h"

// QSH location driver vesion number
#define QSH_LOCATION_VERSION 0x01000000

/*=============================================================================
  Type Definitions
  ===========================================================================*/
#define QSH_OEM1_SUID                                                                              \
  {                                                                                                \
    .sensor_uid = {                                                                                \
      0xc1,                                                                                        \
      0x68,                                                                                        \
      0x36,                                                                                        \
      0x47,                                                                                        \
      0xb4,                                                                                        \
      0xe4,                                                                                        \
      0x47,                                                                                        \
      0x77,                                                                                        \
      0x86,                                                                                        \
      0xc8,                                                                                        \
      0xf1,                                                                                        \
      0x9d,                                                                                        \
      0x2f,                                                                                        \
      0xde,                                                                                        \
      0x74,                                                                                        \
      0x32                                                                                         \
    }                                                                                              \
  }

/** QSH_OEM1 Sensor state with SUIDs of self and dependent sensors */
typedef struct qsh_oem1_sensor_state
{
  // Points to our SUID
  sns_sensor_uid const *suid;

  // AMD, Wifi, Location
  SNS_SUID_LOOKUP_DATA(3) suid_lookup_data;

} qsh_oem1_sensor_state;

// These are defined in qsh_oem1_sensor.c
// and referred in vtable in qsh_oem1_sensor_island.c
sns_rc qsh_oem1_init(sns_sensor *const this);
sns_rc qsh_oem1_deinit(sns_sensor *const this);
sns_sensor_instance *qsh_oem1_set_client_request(sns_sensor *const this,
                                                 struct sns_request const *exist_request,
                                                 struct sns_request const *new_request,
                                                 bool remove);

sns_rc qsh_oem1_notify_event(sns_sensor *const this);
