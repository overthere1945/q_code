#pragma once
/** =============================================================================
 * @file
 *
 * @brief Provides an interface for calling QSH functions from external
 * (non QSH) threads. In public api 'QSH_INCLUDES'.
 *
 * @note: it is not safe to access QSH functions or access the QSH Sensor (or
 * Sensor Instance) pointers outside of QSH-managed threads & functions.
 * Use the functionality herein to access other QSH functions & sensor
 * state from external threads & callbacks.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 *  ==========================================================================*/
#include "sns_sensor.h"
#include "sns_rc.h"

/**
 * @brief Opaque handle to the QSH invoke interface.
 *
 * @note: this should not be a member of a sensor or intance state structure
 * For island operation, this variable shall be allocated in island.
 *
 */
typedef struct qsh_invoke_handle qsh_invoke_handle;

/**
 * @brief Function prototype for a QSH function to be called from the external
 * thread.
 *
 * @param[in] sensor    Pointer to sensor which registered the handle.
 * @param[in] user_arg  User defined arg that is provided when qsh_invoke is
 *                      called.
 *
 * @return
 * - Result code, as defined by the user.
 *
 */
typedef sns_rc (*qsh_invoke_cb)(sns_sensor *const sensor, void *user_arg);

/**
 * @brief Creates an qsh_invoke_handle for later use.
 *
 * @note  Must be called from within a QSH regisetered function.
 *
 * @param[in]  sensor      Pointer to sensor structure.
 * @param[out] handle      Address of pointer to handle.
 *
 * @return
 *    - SNS_RC_SUCCESS: success. handle will point to allocated handle.
 *    - SNS_RC_FAILED:  Internal error. handle will be NULL.
 *    - SNS_RC_POLICY:  Sensor does not have permissions to allocate an invoke
 *                      interface handle.
 *
 */
sns_rc qsh_invoke_register(sns_sensor *sensor, qsh_invoke_handle **handle);

/**
 * @brief Frees the qsh_invoke_handle.
 *
 * @note  Must be called from within a QSH regisetered function.
 *
 * @param[inout] handle Address of pointer to registered handle.
 *
 * @return
 *    - SNS_RC_SUCCESS: Succes. Handle will be set to NULL.
 *    - SNS_RC_FAILED: Internal error.
 *    - SNS_RC_INVALID_VALUE: Invalid handle pointer.
 *
 */
sns_rc qsh_invoke_deregister(qsh_invoke_handle **handle);

/**
 * @brief Invokes a sensor function.
 * All QSH libraries are single threaded. The QSH framework will guarantee that
 * only one QSH registered function pointer is called at a time.
 * If the provided function pointer is not in island, QSH will exit island
 * (and block island reentry) until the function returns.
 *
 * @note To avoid deadlock, please do not call this function with a
 *       locked mutex which may also be locked by a QSH-registered function.
 *
 * @param[in]  handle    Pointer to handle allocated by qsh_invoke_register.
 * @param[in]  cb        Sensor function to invoke.
 * @param[out] cb_rc     Return code from the invoked function.
 * @param[in]  user_arg  Argument to be passed to the invoked function.
 *
 * @return
 *  - SNS_RC_SUCCESS:       The callback was invoked. Check cb_rc for the
 *                          result of the callback.
 *  - SNS_RC_FAILED:        internal error.
 *  - SNS_RC_INVALID_STATE: The sensor associated with the handle is
 *                          not valid.
 *  - SNS_RC_INVALID_VALUE: The handle is not registered.
 *
 */
sns_rc qsh_invoke(qsh_invoke_handle *handle, qsh_invoke_cb cb, sns_rc *cb_rc,
                  void *user_arg);
