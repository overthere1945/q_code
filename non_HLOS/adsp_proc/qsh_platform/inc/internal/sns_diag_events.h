#pragma once
/** ===========================================================================
  @file sns_diag_events.h

  @brief Abstraction layer for the Diag events APIs.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_osa_thread_signal.h"

/**
 * @brief Types of diag events to register.
 *
 */
typedef enum
{
  SNS_DIAG_LOG_MASK_CHANGE_NOTIFY, /* Log Mask change notification event */
  SNS_DIAG_MSG_MASK_CHANGE_NOTIFY, /* Message Mask change notification event */
} sns_diag_events_type;

/*=============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * @brief Function to register for Diag events change notification.
 *
 * @param[in] mask          For which change notification will be received.
 * @param[in] thread_signal Signal object to which signal_mask will be set.
 * @param[in] signal_mask   Signal mask that will be set to signal object
 *                          thread_signal upon mask change.
 *
 * @return handle
 */
void *sns_diag_events_register(sns_diag_events_type mask,
                               sns_osa_thread_signal *const thread_signal,
                               uint32_t const signal_mask);

/**
 * @brief Function to un-register from diag events change notification.
 *
 * @param[in] handle        Pointer to handle from register function.
 * @param[in] thread_signal Signal object to which signal_mask will be set.
 * @param[in] signal_mask   Signal mask that will be set to signal object
 *                          thread_signal upon mask change.
 *
 * @return
 *  None
 */
void sns_diag_events_unregister(void *handle,
                                sns_osa_thread_signal *const thread_signal,
                                uint32_t const signal_mask);
