/*=============================================================================
  @file qsh_oem1_sensor_island.c

  The qsh_oem1 virtual Sensor implementation

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_oem1_sensor.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

/* See sns_sensor::get_sensor_uid */
static sns_sensor_uid const *qsh_oem1_get_sensor_uid(sns_sensor const *this)
{
  UNUSED_VAR(this);
  static const sns_sensor_uid qsh_oem1_suid = QSH_OEM1_SUID;
  return &qsh_oem1_suid;
}

/*===========================================================================
  Public Data Definitions
  ===========================================================================*/

sns_sensor_api qsh_oem1_api = {
    .struct_len = sizeof(sns_sensor_api),
    .init = &qsh_oem1_init,
    .deinit = &qsh_oem1_deinit,
    .get_sensor_uid = &qsh_oem1_get_sensor_uid,
    .set_client_request = &qsh_oem1_set_client_request,
    .notify_event = &qsh_oem1_notify_event,
};
