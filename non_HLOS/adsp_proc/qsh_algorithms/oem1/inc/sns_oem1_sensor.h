#pragma once
/*=============================================================================
  @file oem1_sensor.h

  The OEM1 virtual Sensor

  Copyright (c) 2017-2019,2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_sensor_uid.h"
#include "sns_sensor.h"
#include "sns_data_stream.h"
#include "sns_resampler.pb.h"
#include "sns_sensor_util.h"
#include "sns_diag_service.h"
#include "sns_suid_util.h"

// By default OEM1 requests accel data through resampler
// To skip resampler and use direct accel data, please enable below flag
//#define OEM1_SUPPORT_DIRECT_SENSOR_REQUEST

// By default OEM1 output is event type
// To change it to streaming type - please disable below flag
// The output event is generated when the accel Z data value is changed from
// positive to negative or vice versa (refer to sns_oem1_inst_notify_event())
#define OEM1_SUPPORT_EVENT_TYPE

// Enable this flag to make use of values in registry file (sns_oem1.json)
// This can be customized to add more i/p params
#define OEM1_SUPPORT_REGISTRY

// Minumum sample rate for oem1
#define OEM1_MINUMUM_SAMPLE_RATE 1.0
/*=============================================================================
  Type Definitions
  ===========================================================================*/
#define OEM1_SUID                                                                                  \
  {                                                                                                \
    .sensor_uid = {                                                                                \
      0xee,                                                                                        \
      0xf4,                                                                                        \
      0x35,                                                                                        \
      0xf8,                                                                                        \
      0x28,                                                                                        \
      0x75,                                                                                        \
      0x16,                                                                                        \
      0xa7,                                                                                        \
      0xa1,                                                                                        \
      0x96,                                                                                        \
      0x4b,                                                                                        \
      0x50,                                                                                        \
      0x7b,                                                                                        \
      0xf7,                                                                                        \
      0xe3,                                                                                        \
      0xe0                                                                                         \
    }                                                                                              \
  }

typedef struct
{
  size_t encoded_data_event_len;
  float sample_rate;
  float down_value;
} sns_oem1_config;

typedef enum sns_oem1_dependent_sensors
{
  SNS_ACCEL = 0,
#ifdef OEM1_SUPPORT_REGISTRY
  SNS_REGISTRY,
#endif // OEM1_SUPPORT_REGISTRY
#ifndef OEM1_SUPPORT_DIRECT_SENSOR_REQUEST
  SNS_RESAMPLER,
#endif // OEM1_SUPPORT_DIRECT_SENSOR_REQUEST
  SNS_OEM1_NUM_OF_DEP_SENSORS
} sns_oem1_dependent_sensors;

typedef struct sns_oem1_sensor_state
{
  // Points to our SUID
  sns_sensor_uid const *suid;

  // Requests to the registry sensor
  sns_data_stream *registry_stream;

  SNS_SUID_LOOKUP_DATA(SNS_OEM1_NUM_OF_DEP_SENSORS) suid_lookup_data;

  bool first_pass;
  sns_oem1_config config;
  sns_diag_service *diag_service;
} sns_oem1_sensor_state;

// These are defined in sns_oem1_sensor.c
//  and referred in vtable in sns_oem1_sensor_island.c
sns_rc sns_oem1_init(sns_sensor *const this);
sns_rc sns_oem1_deinit(sns_sensor *const this);
sns_sensor_instance *sns_oem1_set_client_request(sns_sensor *const this,
                                                 struct sns_request const *exist_request,
                                                 struct sns_request const *new_request,
                                                 bool remove);

sns_rc sns_oem1_notify_event(sns_sensor *const this);
