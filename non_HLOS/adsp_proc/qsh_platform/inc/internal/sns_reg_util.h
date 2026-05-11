#pragma once
/** ============================================================================
 * @file
 *
 * @brief A simple utility to parse incoming registry data.  Intended for use by
 * Sensors; is a replacement for/wrapper around decoding registry events
 * manually by the Sensor directly.
 *
 * The Sensor developer should create a registry data stream normally.
 * Within notify_event, sns_reg_event_handle should be called each time.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/
/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "sns_rc.h"
#include "sns_types.h"

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_data_stream;

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief  Type of registry data to be parsed; positive numbers indicate
 * the string buffer length.
 *
 */
typedef enum
{
  SNS_REG_TYPE_INT = -1, /*!< Registry item is an integer */
  SNS_REG_TYPE_FLT = -2, /*!< Registry item is a float */
} sns_reg_type;

/**
 * @brief Registry item table entry
 *
 */
typedef struct sns_reg_util_table_item
{
  char const *name;  /*!< Name of registry item */
  sns_reg_type type; /*!< Item type; Positive values refer to
                      *    string length of data
                      */
  void *data;        /*!< Where to copy the registry item value */
} sns_reg_util_table_item;

/**
 * @brief Registry Group
 *
 */
typedef struct sns_reg_util_table
{
  char const *name;               /*!< Full name of this registry group;
                                   *    If empty, will match any registry group
                                   */
  uint32_t items_len;             /*!< Length of items array */
  sns_reg_util_table_item *items; /*!< One entry per registry item
                                   *    for processing
                                   */
  bool group_recv;                /*!< True once this registry group has
                                   *    been received and parsed
                                   */
} sns_reg_util_table;

/*=============================================================================
  Function Declarations
  ===========================================================================*/

/**
 * @brief Handle all pending events on the registry data stream.
 * Must be called within the Sensor's notify_event function.
 *
 * @param[in] stream     Open registry data stream.
 * @param[in] tables     Client-provided tables to store decoded registry items.
 * @param[in] tables_len Length of the tables array.
 *
 * @return
 *  - SNS_RC_SUCCESS:        One or more registry groups were successfully
 *                           parsed
 *  - SNS_RC_NOT_AVAILABLE:  Registry stream inactive or no pending events
 *  - SNS_RC_INVALID_STATE:  Received error event from registry;
 *                           Implies that registry data will be unavailable.
 *
 */
sns_rc sns_reg_util_handle(struct sns_data_stream *stream,
                           sns_reg_util_table *tables, uint32_t tables_len);
