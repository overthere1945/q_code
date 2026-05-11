#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Timer for Sensors.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/
/*
********************************************************************************
                               Includes
********************************************************************************
*/
#include <stdbool.h>
#include <stdint.h>
#include "sns_osa_attr.h"
#include "sns_rc.h"
#include "sns_time.h"

/*
********************************************************************************
                               Typedefs
********************************************************************************
*/
typedef struct sns_timer sns_timer; /*!< OS dependent timer */

/**
 * @brief Union of attributes that can be specified during creation of a timer.
 *
 */
typedef union
{
  char __size[__SIZEOF_ATTR_TIMER]; /*!< Size of this structure. */
  long int __alignment;
} sns_timer_attr;

typedef void *sns_timer_cb_func_arg;

/**
 * @brief Timer callback function signature.
 *
 */
typedef void (*sns_timer_cb_func)(sns_timer_cb_func_arg);

/*
********************************************************************************
                               Functions
********************************************************************************
*/
/**
 * @brief Initializes the given timer attribute structure with defaults.
 *
 * @param[inout] attrib        The attribute structure.
 *
 * @return
 *   - SNS_RC_SUCCESS:            Attribute structure initialized.
 *   - SNS_RC_OUTSIDE_RANGE:      Input parameter is invalid.
 *
 */
sns_rc sns_timer_attr_init(sns_timer_attr *attrib);

/**
 * @brief Specifies whether the timer should be periodic or one-shot.
 *
 * @param[inout] attrib       The attribute structure.
 * @param[in]  is_periodic    Whether the timer should be periodic or one-shot.
 *
 * @return
 *   - SNS_RC_SUCCESS:            Attribute set.
 *   - SNS_RC_OUTSIDE_RANGE:      One or more input parameters are invalid.
 *
 */
sns_rc sns_timer_attr_set_periodic(sns_timer_attr *attrib, bool is_periodic);

/**
 * @brief Specifies whether the timer should be deferrable or not.
 *
 * @param[inout] attrib        The attribute structure.
 * @param[in]  is_deferrable   Whether the timer should be deferrable or not.
 *
 * @return
 * - SNS_RC_SUCCESS:            Attribute set.
 * - SNS_RC_OUTSIDE_RANGE:      One or more input parameters are invalid.
 *
 */
sns_rc sns_timer_attr_set_deferrable(sns_timer_attr *attrib,
                                     bool is_deferrable);

/**
 * @brief Specifies the memory type where the timer will be located.
 *
 * @param[inout] attrib        The attribute structure.
 * @param[in]  mem_type        The memory type.
 *
 * @return
 *   - SNS_RC_SUCCESS:            Attribute set.
 *   - SNS_RC_OUTSIDE_RANGE:      One or more input parameters are invalid.
 *
 */
sns_rc sns_timer_attr_set_memory_partition(sns_timer_attr *attrib,
                                           sns_osa_mem_type mem_type);

/**
 * @brief Creates a new timer.
 *
 * @param[in] timer_cb         Function called when timer expires.
 * @param[in] cb_func_arg      Input parameter to timer_cb.
 * @param[in] attrib           The initialized timer attribute structure.
 * @param[out] timer           Destination for the newly created timer.
 *
 * @return
 *   - SNS_RC_SUCCESS:              New timer successfully created.
 *   - SNS_RC_RESOURCE_UNAVAIL:     No memory.
 *   - SNS_RC_OUTSIDE_RANGE:        One or more input parameters are invalid.
 *
 */
sns_rc sns_timer_create(sns_timer_cb_func timer_cb,
                        sns_timer_cb_func_arg cb_func_arg,
                        const sns_timer_attr *attrib, sns_timer **timer);

/**
 * @brief Deletes the given timer.
 *
 * @param[in] timer            The timer.
 *
 * @return
 *    - SNS_RC_SUCCESS:              Timer deleted.
 *    - SNS_RC_OUTSIDE_RANGE:        Given timer not found.
 *
 * @note
 * Depending on implementation the timer callback function may or may not be
 * called after the timer is deleted.
 */
sns_rc sns_timer_delete(sns_timer *timer);

/**
 * @brief Starts or restart the given timer to expire after the given duration.
 *
 * @param[in] timer          The timer.
 * @param[in] duration_ticks Number of time ticks before timer should expire.
 *
 * @return
 *    - SNS_RC_SUCCESS:              Timer started.
 *    - SNS_RC_OUTSIDE_RANGE:        One or more input parameters are invalid.
 *
 */
sns_rc sns_timer_start_relative(sns_timer *timer, sns_time duration_ticks);

/**
 * @brief Starts or restart the given timer to expire at the given time tick.
 *
 * @param[in] timer            The timer.
 * @param[in] expiry           The time tick when timer should expire.
 *
 * @return
 *   - SNS_RC_SUCCESS:              Timer started.
 *   - SNS_RC_OUTSIDE_RANGE:        One or more input parameters are invalid.
 *
 */
sns_rc sns_timer_start_absolute(sns_timer *timer, sns_time expiry);

/**
 * @brief Stops the given timer and return remaining time if necessary.
 *
 * @param[in] timer           The timer.
 * @param[out] time_left      If not NULL, destination for remaining time ticks.
 *
 * @return
 * - SNS_RC_SUCCESS:              Timer stopped.
 * - SNS_RC_OUTSIDE_RANGE:        One or more input parameters are invalid.
 *
 */
sns_rc sns_timer_stop(sns_timer *timer, sns_time *time_left);

/**
 * @brief Gets the amount of remaining time before the given timer expires.
 *
 * @param[in] timer             The timer.
 * @param[out] time_left        Destination for remaining time ticks.
 *
 * @return
 * - SNS_RC_SUCCESS:              Remaining time retrieved.
 * - SNS_RC_OUTSIDE_RANGE:        One or more input parameters are invalid.
 *
 */
sns_rc sns_timer_get_duration(sns_timer *timer, sns_time *time_left);

/**
 * @brief Sets timer speedup factor
 *
 * @param[in] speedup_factor   Timer speed up factor.
 *
 * @return
 *    - SNS_RC_SUCCESS:        Speedup factor update success.
 *    - SNS_RC_FAILURE:        Speedup factor update failed.
 */
sns_rc sns_timer_set_speedup_factor(uint32_t speedup_factor);

/**
 * @brief Initializes timer and should be called once.
 *
 * @return
 *    - SNS_RC_SUCCESS:        Timer init success.
 *    - SNS_RC_FAILURE:        Timer init failed.
 *
 */
sns_rc sns_timer_init(void);
