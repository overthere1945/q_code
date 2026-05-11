#pragma once
/** =============================================================================
 * @file
 *
 * @brief The APIs to vote for the latency.
 * @note These APIs are not supported island.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 *  ===========================================================================
 */

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdint.h>
#include "sns_rc.h"

/*==============================================================================
  Type Definitions
  ============================================================================*/

typedef struct sns_latency_handle sns_latency_handle;

/*==============================================================================
  Public Function definitions
  ============================================================================*/

/**
 * @brief The API to Register client for latency voting.
 * @note This API should be called only in big image.
 *
 * @param[in]  client_name      Name of client registering latency handle.
 *                              The max length is 32 characters.
 *
 * @return
 *   - sns_latency_handle: Non NULL when registration is successful.
 *   - NULL:               When registration fails.
 *
 */
sns_latency_handle *sns_latency_register(char *client_name);

/* ---------------------------------------------------------------------------*/
/**
 * @brief The API to set latency in microseconds.
 * @note  This api should be called only in big image.
 *
 * @param[in] handle             Registered client handle.
 * @param[in] latency_us         Latency in microseconds. range
 *                               (1 - UINT32_MAX).
 *
 * @return
 *   - SNS_RC_SUCCESS:        Latency vote is set successfully.
 *   - SNS_RC_INVALID_VALUE:  Invalid handle or latency passed.
 *
 */
sns_rc sns_latency_set(sns_latency_handle *handle, uint32_t latency_us);

/* ---------------------------------------------------------------------------*/
/**
 * @brief The API to deregister the latency handle
 * @note  This function should be called only in big image.
 *
 * @param[inout] handle         Client handle to be deregistered.
 *
 * @return
 *   - SNS_RC_SUCCESS        Handle deregistered successfully.
 *   - SNS_RC_INVALID_VALUE  Invalid handle passed.
 *
 */

sns_rc sns_latency_deregister(sns_latency_handle *handle);

/* ---------------------------------------------------------------------------*/
