#pragma once
/*=============================================================================
  @file sns_tppe_debug_dump_util.h

  Contains function definitions required for duplicate detection and filtering

  Copyright (c) 2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/
#include "sns_data_stream.h"
#include "sns_rc.h"
#include "sns_sensor_instance.h"
/*============================================================================
  MACROS
===========================================================================*/
// selecting a reasonable size for an event
#define SNS_DDU_MAX_DUMP_STR_LEN 300

/**
 * This utility will provide sensors an ability to send debug dump response.
 * This utility will only support debug_string message in sns_std_debug_resp
 * This utility will not support binary_data field. Sensors that wish to use
 * this field, will need to implement their own method.
 *
 * Usage:
 *
 * Sensors will call methods in this order, init-->dump-->deinit. The util will
 * take care of sending events.
 *
 * 1. The sensors will initialize the util using sns_ddu_init
 * 2. The sensors can dump multiple strings using sns_ddu_dump_str method
 * 3. The utility will automatically send out partial debug response events as
 *    and when max supported size is reached
 * 4. The sensor should call deinit after completion. If there is any unsent
 *    data , ddu will send that data with partial field set to false. Since
 *    this is the only way to send a final event, sensors are forced to call
 *    ddu_deinit method.
 *
 **/
typedef struct sns_debug_dump_util
{
  const sns_sensor_instance *instance;
  char *buffer;
  size_t max_buffer_len;
  size_t cur_index;
  size_t available_size;
} sns_debug_dump_util;

/**
 * @name sns_ddu_init
 * @param thiz: the current debug dump structure
 * @param instance: the instance which is using this util
 * @return sns_rc success/failed
 * @note thiz cannot be NULL
 **/
sns_rc sns_ddu_init(sns_debug_dump_util *thiz, sns_sensor_instance *const instance);

/**
 * @name sns_ddu_dump_str
 * @param thiz: the current debug dump structure
 * @param format: format string to be dumped
 * @param ... : variable arguments matching format string
 * @return sns_rc,
 *  returns not supported if the size of the string to be printed is bigger
 *  than system supported packet size
 *  returns failed, if called without initializing properly
 **/

sns_rc sns_ddu_dump_str(sns_debug_dump_util *thiz, const char *const format, ...);
/**
 * @name sns_ddu_deinit
 * @param thiz: the current debug dump structure
 * @return void
 **/
void sns_ddu_deinit(sns_debug_dump_util *thiz);