#pragma once

/** ============================================================================
 * @file
 *
 * @brief Definitions for System time.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * Inc. All Rights Reserved. Confidential and Proprietary - Qualcomm
 * Technologies, Inc.
 *
 * =============================================================================
 */

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdint.h>
#include "sns_rc.h"

/*==============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief Time in ticks.
 *
 */
typedef uint64_t sns_time;

/*==============================================================================
  Function Declarations
  ============================================================================*/

/**
 * @brief Gets the current system time tick.
 *
 * @return
 *  - sns_time  Current system time tick.
 *
 */
sns_time sns_get_system_time(void);

/**
 * @brief Gets number of nanoseconds in one time tick.
 *
 * @return
 *  - uint64_t Number of nanoseconds.
 *
 */
uint64_t sns_get_time_tick_resolution(void);

/**
 * @brief Gets number of picoseconds in one time tick.
 *
 * @return
 *  - uint64_t Number of picoseconds.
 *
 */
uint64_t sns_get_time_tick_resolution_in_ps(void);

/**
 * @brief Returns the number of ticks equivalent to the give number of
 * nanoseconds.
 *
 * @param [in] time_ns  Time in nanoseconds.
 *
 * @return
 *  - sns_time number of ticks.
 *
 */
sns_time sns_convert_ns_to_ticks(uint64_t time_ns);

/**
 * @brief Returns the microseconds equivalent for the provided ticks.
 *
 * @note A very large tick value can cause an overflow. The limit before an
 * overflow is ~11 days in ticks.
 *
 * @param [in] ticks  Number of ticks.
 *
 * @return
 *  - sns_time microsecond equivalent of ticks.
 *
 */
 uint64_t sns_convert_ticks_to_usec(sns_time ticks);

/**
 * @brief Returns the seconds equivalent for the provided ticks.
 *
 * @param [in] ticks  Number of ticks.
 *
 * @return
 *  - sns_time second equivalent of ticks.
 *
 */
 uint64_t sns_convert_ticks_to_sec(sns_time ticks);

#ifndef SNS_RESTRICT_BUSY_WAIT
/**
 * @brief Waits for the given number of time ticks.
 *
 * @param [in] time_ticks    Wait duration.
 *
 * @note This function should only be called from uImage Mode capable threads.
 * It can be called while in uImage Mode as well as Normal Mode from uImage
 * Threads.
 *
 * @return
 *  - SNS_RC_SUCCESS   Success.
 *  - Any other value  Failure.
 */
sns_rc sns_busy_wait(sns_time time_ticks);

#endif /* SNS_RESTRICT_BUSY_WAIT */

/**
 * @brief Waits for the given number of time ticks.
 *
 * @param [in] time_ticks    Wait duration.
 *
 * @note This function should only be called from Normal Mode threads.
 *
 * @return
 *  - SNS_RC_SUCCESS   Success.
 *  - Any other value  Failure.
 *
 */
sns_rc sns_busy_wait_normal_mode(sns_time time_ticks);

/**
 * @brief Rewind the simulation start time in epoch time domain,
 *        such that the present epoch time converts to the,
 *        external_absolute_time passed in as input
 *
 * @param[in] xternal_absolute_time In ticks
 *
 * @return
 *  - SNS_RC_SUCCESS   Return only SNS_RC_SUCCESS on completetion.
 *
 */
sns_rc sns_timer_set_absolute_start_time(sns_time xternal_absolute_time);
