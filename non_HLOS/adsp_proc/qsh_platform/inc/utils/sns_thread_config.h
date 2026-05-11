#pragma once
/** ============================================================================
 * @file
 *
 * @brief This file has the API declaration of sensor thread configuration.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdint.h>
#include "sns_osa_attr.h"

/**
 * @brief SNS_THREAD_ATTR_NAME_MAXLEN
 * set to 14 to avoid thread name truncation
 * when name is procesed by qurt.
 * sns_osa_thread_attr_set_name will copy the name
 * into a buffer of size (QURT_THREAD_ATTR_NAME_MAXLEN 16).
 * The buffer will postfix the qurt_process_id to the name
 * and submit the name to qurt.
 * This will leave us w/ a minimum of 2 characters of buffer space
 * for the qurt_process_id to be appended (e.g (sns_thread_id).
 *
 */
#define SNS_THREAD_ATTR_NAME_MAXLEN 14

/*=============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief Structure holding config information for created threads.
 *
 */
typedef struct sns_thread_config
{
  char name[SNS_THREAD_ATTR_NAME_MAXLEN]; /*!< Thread name. */
  uint16_t thread_stack;                  /*!< Thread stack in bytes. */
  uint16_t kernel_stack;                  /*!< Island thread kernel stack in
                                           *   bytes.
                                           */

  uint8_t priority;          /*!< Thread priority. */
  sns_osa_mem_type mem_type; /*!< Thread memory allocated from this
                              *   memory type.
                              */

  uint8_t watchdog_timeout_sec; /*!< watchdog will bite if remains
                                 *   active for more than this
                                 *   many seconds.
                                 */
} sns_thread_config;

/*=============================================================================
  Public function declaration
  ============================================================================*/

/**
 * @brief The API to query sensor thread configuration from the sensor thread
 * table.
 *
 * @param[in] thread_name Pointer to the thread name.
 * ( max size of SNS_THREAD_ATTR_NAME_MAXLEN ).
 *
 * @return
 * - thread_config: If thread is found in sensor thread table
 * - NULL:          Otherwise.
 *
 */
sns_thread_config *sns_osa_get_thread_config(char *thread_name);

/* ---------------------------------------------------------------------------*/
