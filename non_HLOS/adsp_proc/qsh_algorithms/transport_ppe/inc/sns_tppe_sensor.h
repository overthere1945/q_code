#pragma once
/*=============================================================================
  @file sns_tppe_sensor.h

  The TPPE virtual Sensor

  Copyright (c) 2022-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "qsh_invoke.h"
#include "sns_data_stream.h"
#include "sns_sensor_uid.h"
#include "sns_sensor_util.h"
#include "sns_sensor.h"
#include "sns_suid_util.h"
#include "sns_tppe_request_handler.h"

/*=============================================================================
  Macros
  ===========================================================================*/

// uncomment this for debug prints
#define SNS_TPPE_DEBUG_PRINT

#ifdef SNS_TPPE_DEBUG_PRINT
#define SNS_TPPE_PRINTF(prio, sensor, ...)                                                         \
  do                                                                                               \
  {                                                                                                \
    SNS_UPRINTF(prio, sensor, __VA_ARGS__);                                                        \
  } while(0)

#define SNS_TPPE_INST_PRINTF(prio, inst, ...)                                                      \
  do                                                                                               \
  {                                                                                                \
    SNS_INST_UPRINTF(prio, inst, __VA_ARGS__);                                                     \
  } while(0)
#else
#define SNS_TPPE_PRINTF(prio, sensor, ...)    UNUSED_VAR(sensor);
#define SNS_TPPE_INST_PRINTF(prio, inst, ...) UNUSED_VAR(inst);
#endif

#define SNS_TPPE_SUID                                                                              \
  {                                                                                                \
    .sensor_uid = {                                                                                \
      0XC1,                                                                                        \
      0X7D,                                                                                        \
      0X53,                                                                                        \
      0X1F,                                                                                        \
      0X4C,                                                                                        \
      0XEF,                                                                                        \
      0X2A,                                                                                        \
      0XE3,                                                                                        \
      0X0C,                                                                                        \
      0X8C,                                                                                        \
      0XEB,                                                                                        \
      0X45,                                                                                        \
      0X72,                                                                                        \
      0X54,                                                                                        \
      0XD3,                                                                                        \
      0X9C                                                                                         \
    }                                                                                              \
  }

#define SNS_TPPE_MAX_DATA_TYPE_LEN (64)
#define SNS_TPPE_MAX_API_NAME_LEN  (64)
#define SNS_TPPE_MAX_MSG_ID_LEN    (10)
#define SNS_TPPE_MAX_TRIGGERS_SIZE (4096)

/*=============================================================================
  Type Definitions
  ===========================================================================*/

typedef struct
{
  uint8_t msg_id_cnt;
  uint32_t batch_period;
  uint32_t crc_cache_max_len;
  char type[SNS_TPPE_MAX_DATA_TYPE_LEN];
  char api[SNS_TPPE_MAX_API_NAME_LEN];
  uint32_t msg_ids[SNS_TPPE_MAX_MSG_ID_LEN];
} sns_tppe_registry_config;

typedef struct sns_tppe_sensor_state
{
  sns_data_stream *registry_stream;
  sns_tppe_registry_config reg_config;
  SNS_SUID_LOOKUP_DATA(1) tppe_suid_lookup_data;
  SNS_SUID_LOOKUP_DATA(1) tppe_batch_suid_lookup_data;
  SNS_SUID_LOOKUP_DATA(1) transport_suid_lookup_data;
  size_t raw_adv_data_max_len;
} sns_tppe_sensor_state;

/*=============================================================================
  Public Function Declarations
  ===========================================================================*/

sns_rc sns_tppe_init(sns_sensor *const this);

sns_rc sns_tppe_deinit(sns_sensor *const this);

sns_sensor_instance *sns_tppe_set_client_request(sns_sensor *const this,
                                                 struct sns_request const *exist_request,
                                                 struct sns_request const *new_request,
                                                 bool remove);

sns_rc sns_tppe_notify_event(sns_sensor *const this);
