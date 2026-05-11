#pragma once
/** ============================================================================
 *  @file
 *
 *  @brief OS Abstraction layer for interrupt service routine for Sensors.
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

#include "sns_rc.h"

/*=============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * @brief OS dependent opaque structure for ISR
 *
 */
typedef struct sns_osa_isr sns_osa_isr;

/**
 * @brief Argument type for ISR function
 *
 */
typedef void *sns_osa_isr_func_arg;
/**
 * @brief Signature of ISR function
 *
 */
typedef void (*sns_osa_isr_func)(sns_osa_isr_func_arg, int);

/*=============================================================================
  Public Functions
  ===========================================================================*/

/**
 *  @brief  Registers ISR for the given interrupt number.
 *
 *  @param [in] int_num            Interrupt number.
 *  @param [in] thread_name        Name of IST
 *  @param [in] heap               Which heap to allocate thread stack from
 *  @param [in] isr                The ISR to register.
 *  @param [in] isr_arg            First argument passed to the ISR.
 *
 *  @return
 *   - SNS_RC_SUCCESS:              Successfully registered the ISR for the
 *                                  interrupt.
 *   - SNS_RC_INVALID_VALUE:        Invalid thread name
 *   - SNS_RC_INVALID_STATE:        Interrupt is already registered.
 *   - SNS_RC_NOT_AVAILABLE:        Interrupt not configured.
 *   - SNS_RC_NOT_SUPPORTED:        This feature is disabled.
 *
 *  @note New thread may be created if not found
 */
sns_rc sns_osa_isr_register(int int_num, char const *thread_name,
                            sns_mem_heap_id heap, sns_osa_isr_func isr,
                            sns_osa_isr_func_arg isr_arg)
    __attribute__((nonnull(2, 4)));

/**
 *
 *  @brief  Deregisters the given interrupt number.
 *
 *  @param [in] int_num             Interrupt number.
 *  @param [in] thread_name         Name of IST given in sns_osa_isr_register()
 *
 *  @return
 *   - SNS_RC_SUCCESS:              Successfully deregistered the interrupt.
 *   - SNS_RC_INVALID_STATE:        Interrupt is not registered.
 *   - SNS_RC_NOT_AVAILABLE:        IST is processing an interrupt.
 *
 */
sns_rc sns_osa_isr_deregister(int int_num, char const *thread_name)
    __attribute__((nonnull(2)));
