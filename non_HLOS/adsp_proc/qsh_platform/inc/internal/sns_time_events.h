#pragma once
/** ===========================================================================
  @file sns_time_events.h

  @brief Abstraction layer for the Timer events APIs.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_osa_thread_signal.h"

/*=============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * @brief Function to register for Timer events change notification.
 *
 * @param[in] rcevt_name    Name of rcevt.
 * @param[in] thread_signal Signal object to which signal_mask will be set.
 * @param[in] signal_mask   Signal mask that will be set to signal object
 *                          thread_signal upon mask change.
 *
 * @return handle
 */
void *sns_time_events_register(char *const rcevt_name,
                               sns_osa_thread_signal *const thread_signal,
                               uint32_t const signal_mask);

/**
 * @brief Function to un-register from time events change notification.
 *
 * @param[in] handle        Pointer to handle from register function.
 * @param[in] thread_signal Signal object to which signal_mask will be set.
 * @param[in] signal_mask   Signal mask that will be set to signal object
 *                          thread_signal upon mask change.
 *
 * @return
 *  None
 */
void sns_time_events_unregister(void *handle,
                                sns_osa_thread_signal *const thread_signal,
                                uint32_t const signal_mask);
