#pragma once
/** ============================================================================
 * @file
 *
 * @brief Qshtech island vote util.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 =============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdbool.h>

#include "sns_rc.h"
#include "sns_sensor.h"

/*=============================================================================
  Static Data Definitions
  ============================================================================*/

/*=============================================================================
  Type Definitions
  ============================================================================*/

/*=============================================================================
  Public function declaration
  ============================================================================*/

/**
 * @brief Qshtech Island memory pool vote callback function.
 * This API votes for qshtech memory pool if atleast one qshtech island
 * sensor is active.
 * This API removes qshtech memory pool vote if there are no qshtech
 * island sensors active.
 *
 * @note This API should be passed by the sensor as callback function
 * during register_island_memory_vote_cb(). refer sns_memory_service.h.
 *
 * @param [in] sensor Sensor pointer.
 * @param [in] enable
 *              - True: add vote for qshtech island memory pool.
 *              - false: remove vote for qshtech island memory pool.
 *
 * @return
 *  - SNS_RC_SUCCESS: No error occurred; success.
 *  - SNS_RC_FAILED: Internal error occurred.
 *  - SNS_RC_NOT_SUPPORTED: This API is not supported or is not implemented.
 *
 */
sns_rc sns_qshtech_island_memory_vote(sns_sensor *const sensor, bool enable);
