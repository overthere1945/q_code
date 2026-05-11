#pragma once
/** ============================================================================
 * @file
 *
 * @brief
 * Provides internal signal service that can spawn an internal thread to handle
 * asynchronous events
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  INCLUDES
  ============================================================================*/

#include <stdint.h>
#include "sns_osa_thread.h"
#include "sns_rc.h"
#include "sns_sensor.h"

/*============================================================================
  MACROS
  ============================================================================*/

#define __SIZEOF_ATTR_SIG 64

/*============================================================================
  TYPEDEFS
  ============================================================================*/

/**
 *  @brief Opaque handle to the Signal utility interface.
 *
 */
typedef struct sns_signal_handle sns_signal_handle;

/**
 * @brief Signal Handle cb function.
 *
 * @param sensor   Sensor object.
 * @param handle   Handle returned from sns_signal_register().
 * @param user_arg User data.
 *
 */
typedef void (*sns_handle_signal_cb)(sns_sensor *sensor,
                                     sns_signal_handle *handle, void *user_arg);
/**
 * @brief Signal Attribute structure client supposed to pass
 * during signal registration.
 *
 */
typedef union
{
  char __size[__SIZEOF_ATTR_SIG];
  long int __alignment;

} sns_signal_attr;

/*==============================================================================
  FUNCTION DECLARATION
  ============================================================================*/

/**
 * @brief  The API to initialize Signal utility state.
 * This API should be called only in the big image.
 *
 * @return
 * - SNS_RC_SUCCESS:  Signal registered initialized successfully.
 * - !SNS_RC_SUCCESS: An error occurred.
 *
 */
sns_rc sns_signal_util_init(void);

/**
 * @brief  The API to initialize signal attributes to default values.
 * This API should be called only in big image.
 *
 * @param [inout] signal_attr  Pointer to Signal Attribute structure.
 *
 * @return
 *     - SNS_RC_SUCCESS:  Signal Attributes initialized to defaults
 *                        successfully.
 *     - !SNS_RC_SUCCESS: An error occurred.
 *
 */
sns_rc sns_signal_attr_init(sns_signal_attr *signal_attr);

/**
 * @brief  The API to set the signal attributes.
 * This API should be called only in big image.
 *
 * @param [inout] signal_attr Pointer to Signal Attributes passed by the client.
 * @param [in]   sensor       Client Sensor pointer.
 * @param [in]   cb           Callback function to be called when signal is set.
 * @param [in]   user_arg     Pointer to user argument.
 *
 * @return
 *     - SNS_RC_SUCCESS:    Signal attributes set successfully.
 *     - !SNS_RC_SUCCESS:   An error occurred.
 *
 */
sns_rc sns_signal_attr_set(sns_signal_attr *signal_attr, sns_sensor *sensor,
                           sns_handle_signal_cb cb, void *user_arg);

/**
 * @brief  The API to register for the signal callback function
 * This API should be called only in big image
 *
 * @param [in] signal_attr Initalized signal attribute structure pointer.
 * @param [out] handle     Address of pointer to handle.
 *                         Client should set the signal using this handle.
 *
 * @return
 *    - SNS_RC_SUCCESS:   Signal registered successfully.
 *    - !SNS_RC_SUCCESS:  An error occurred.
 *
 */
sns_rc sns_signal_register(sns_signal_attr *signal_attr,
                           sns_signal_handle **handle);

/**
 * @brief  The API to unregister signal.
 * This API should be called only in big image.
 *
 * @param [in] handle     Address of pointer to registered handle.
 *
 * @return
 *   - SNS_RC_SUCCESS:   Signal unregistered successfully.
 *   - !SNS_RC_SUCCESS:  An error occurred.
 *
 */
sns_rc sns_signal_unregister(sns_signal_handle **handle);

/**
 * @brief  The API to set registered signal. When the signal is set, signal
 * utility will call the registered cb function.
 *
 * @param [in] handle       Registered handle pointer.
 *
 * @return
 * - SNS_RC_SUCCESS:  Signal set successfully.
 * - !SNS_RC_SUCCESS: An error occurred.
 *
 */
sns_rc sns_signal_set(sns_signal_handle *handle);

/**
 * @brief  The API to get the registered signal mask.
 *
 * @param [in]  handle      Registered handle pointer.
 * @param [out] signal_mask Buffer to store signal_mask.
 *
 * @return
 * - SNS_RC_SUCCESS:  Signal mask get successfully.
 * - !SNS_RC_SUCCESS: An error occurred.
 *
 */
sns_rc sns_signal_get_mask(sns_signal_handle *handle, uint32_t *signal_mask);

/**
 * @brief  The API to get the pointer to the registered thread signal.
 *
 * @param [in]  handle        Registered handle pointer.
 * @param [out] thread_signal Buffer to store pointer to thread_signal.
 *
 * @return
 * - SNS_RC_SUCCESS:  Thread signal pointer get successfully.
 * - !SNS_RC_SUCCESS: An error occurred.
 *
 */
sns_rc sns_signal_get_thread_signal(sns_signal_handle *handle,
                                    sns_osa_thread_signal **thread_signal);

/** --------------------------------------------------------------------------*/
