#pragma once

/** ============================================================================
 *  @file
 *
 *  @brief OS Abstraction layer for thread signaling mechanism.
 *
 *  @copyright Copyright (c) Qualcomm Technologies, Inc. and / or its
 *  subsidiaries. All Rights Reserved. Confidential and Proprietary - Qualcomm
 *  Technologies, Inc.
 *  ===========================================================================
 */
/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdint.h>
#include "sns_osa_attr.h"
#include "sns_rc.h"

/*=============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * @brief OS dependent signaling mechanism.
 *
 */
typedef union
{
  char __size[__SIZEOF_SIGNAL]; /*!< Signal size. */
  long int __alignment;
} sns_osa_thread_signal __attribute__((aligned(8)));

/**
 * @brief 32-bit signal.
 *
 */
typedef uint32_t sns_osa_thread_sigs;

/*=============================================================================
  Public Functions
  ===========================================================================*/

/**
 * @brief  Initializes the given signal object.
 *
 * @param [out] sig      The signal object to be initialized.
 *
 * @return
 *      - None.
 *
 */
void sns_osa_thread_signal_init(sns_osa_thread_signal *sig)
    __attribute__((nonnull));

/**
 * @brief  Deinit the given signal object.
 *
 * @param [in] sig             The signal object to deinitialize.
 *
 * @return
 *    - None.
 *
 */
void sns_osa_thread_signal_deinit(sns_osa_thread_signal *sig)
    __attribute__((nonnull));

/**
 * @brief  Wait on the given signal object.
 *
 * @note
 *  Suspends the current thread until any one of the specified signals is set.
 *
 * @param [in] sig            The signal object on which to wait.
 * @param [in] mask           The mask of signal bits on which the signal is to
 *                            wait.
 * @param [out] curr_signals  Destination for bitmask of current signal values,
 *                            if provided.
 *
 * @return
 *    - None.
 *
 */
void sns_osa_thread_signal_wait(sns_osa_thread_signal *sig,
                                sns_osa_thread_sigs mask,
                                sns_osa_thread_sigs *curr_signals)
    __attribute__((nonnull(1)));

/**
 * @brief  Waits on the signal object up to.
 *
 * @note
 *  Suspends the current thread until any one of the specified signals is set,
 *  or timed out.
 *
 * @param [in] sig             The signal object on which to wait.
 * @param [in] duration        The wait duration.
 * @param [in] mask            The mask of signal bits on which the signal is to
 *                             wait.
 * @param [out] curr_signals   Destination for bitmask of current signal values,
 *                             if provided.
 *
 * @return
 *    - None.
 *
 */
void sns_osa_thread_signal_wait_timed(sns_osa_thread_signal *sig,
                                      uint64_t duration,
                                      sns_osa_thread_sigs mask,
                                      sns_osa_thread_sigs *curr_signals)
    __attribute__((nonnull(1)));

/**
 * @brief  Sets signals on the signal object.
 *
 * @param [in] sig             The signal object.
 * @param [in] mask            The mask of signal bits on which the signal is to
 *                             set.
 * @param [out] old_signals    Destination for bitmask of the old signal values,
 *                             if provided.
 *
 * @return
 *    - None.
 *
 */
void sns_osa_thread_signal_set(sns_osa_thread_signal *sig,
                               sns_osa_thread_sigs mask,
                               sns_osa_thread_sigs *old_signals)
    __attribute__((nonnull(1)));

/**
 * @brief  Gets signal values of the signal object.
 *
 * @param [in] sig              The signal object.
 * @param [out] curr_signals    Destination for bitmask of the current signal.
 *                              values.
 *
 * @return
 * - None.
 *
 */
void sns_osa_thread_signal_get(sns_osa_thread_signal *sig,
                               sns_osa_thread_sigs *curr_signals)
    __attribute__((nonnull));

/**
 * @brief  Clears the signal object.
 *
 * @param [in] sig             The signal object.
 * @param [in] mask            The mask of signal bits to clear.
 * @param [out] old_signals    Destination for bitmask of the old signal values,
 *                             if provided.
 *
 * @return
 * - None.
 *
 */
void sns_osa_thread_signal_clear(sns_osa_thread_signal *sig,
                                 sns_osa_thread_sigs mask,
                                 sns_osa_thread_sigs *old_signals)
    __attribute__((nonnull(1)));
