#pragma once
/** ===========================================================================
 * @file
 *
 * @brief This file contains the definitions of functions that are used to
 *        calculate thread utilization.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_osa_thread.h"
#include "sns_time.h"

/*=============================================================================
  Constant Data Definitions
  ============================================================================*/

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief Mode to control if we need to stop or start computation.
 *
 */
typedef enum
{
  STOP_COMPUTE = 0,
  START_COMPUTE
} sns_thread_util_mode;

/**
 * @brief Structure containing information about a particular
 * thread utilization.
 *
 */
typedef struct sns_thread_utilization
{
  sns_time total_time;        /*!< Total (active + idle) time. */
  sns_time active_time;       /*!< Active time: used for thread utilization
                               *   calculation.
                               */
  sns_time start_time;        /*!< Start time. */
  sns_time prev_start_time;   /*!< Previous start time. */
  sns_time start_active_time; /*!< start active time. */
  uint32_t thread_utilization_percent; /*!< Thread utilization in percent.*/
  bool is_thread_active; /*!< Flag indicating if thread is currently running.*/
  bool is_thread_monitored; /*!< Flag indicating if thread is being monitored.*/
} sns_thread_utilization;

/*=============================================================================
  Public Function Declarations
  ===========================================================================*/

/**
 * @brief The API to initialize Sensor thread utilization utility
 *
 * @par Parameters
 *      None.
 *
 * @ret
 * @retval SNS_RC_SUCCESS        Success
 * @retval SNS_RC_FAILED         Failed
 *
 */

sns_rc sns_thread_utilization_init(void);

/* ---------------------------------------------------------------------------*/

/**
 * @brief Compute per-thread utilization; used to determine appropriate
 * frequency voting.
 *
 * @param[in] thread  Pointer to OSA thread object.
 * @param[in] mode    Stop or start computing thread utilization.
 *
 * @return
 *   - None.
 *
 */

void sns_thread_utilization_compute(sns_osa_thread *thread,
                                    sns_thread_util_mode mode);

/* ---------------------------------------------------------------------------*/
/**
 * @brief Update flag to indicate whether current thread should be monitored by
 * other threads.
 *
 * @param[in] thread      Pointer to OSA thread object.
 * @param[in] value       Value to set is_thread_monitored flag.
 *
 * @return
 *   - None.
 *
 */

void sns_thread_utilization_set_monitor_flag(sns_osa_thread *thread,
                                             bool value);

/* ---------------------------------------------------------------------------*/

/**
 * @brief The API to enable / disable thread utilization monitor
 *
 * @param[in] enable      True: Enable thread utilization monitor
 *                        False: Disable thread utilization monitor
 *
 * @return
 *   - None.
 *
 */
void sns_thread_utilization_enable(bool enable);

/* ---------------------------------------------------------------------------*/
