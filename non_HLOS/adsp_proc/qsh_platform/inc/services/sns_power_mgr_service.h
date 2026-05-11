#pragma once
/** =============================================================================
 * @file
 *
 * @brief Service for sensor power management.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdint.h>
#include "sns_rc.h"
#include "sns_service.h"

/*=============================================================================
  Macro constants
  ===========================================================================*/

/** ======================== Version History ===================================
 *  @brief Version  comments
 *  1      Initial version with mcps voting api support.
 *
 */
#define SNS_POWER_MGR_SERVICE_VERSION (1)

/*=============================================================================
  Type Definitions
  ===========================================================================*/

typedef void *sns_power_mgr_handle;

/**
 * @brief The Sensor Power Manager.
 * Will be obtained from sns_service_manager::get_service.
 *
 */
typedef struct sns_power_mgr_service
{
  sns_service service;                         /*!< Service information. */
  const struct sns_power_mgr_service_api *api; /*!< Public api provided by the
                                                *   framework to be used by the
                                                *   sensor.
                                                */
} sns_power_mgr_service;

/**
 * @brief The Sensor Power Manager APIs.
 *
 */
typedef struct sns_power_mgr_service_api
{
  uint32_t struct_len; /*!< Length of sns_power_mgr_service_api structure. */

  /**
   * @brief Register sensor client with sensor power manager service
   *
   * @note  This API is not supported in island.
   *
   * @param[in]    client_name    Ptr to the client name. The max size is 32
   *                              char.
   * @param[inout] handle         Handle returned by power manager service
   *
   * @return
   *  - SNS_RC_INVALID_VALUE Input parameters are invalid.
   *  - SNS_RC_FAILED        Client registration failed.
   *  - SNS_RC_SUCCESS       Action succeeded.
   *
   */
  sns_rc (*sns_register_client)(char *client_name,
                                sns_power_mgr_handle **handle);

  /**
   * @brief De-register sensor client with sensor power manager service
   *
   * @note This API is not supported in island.
   *
   *  @param[inout] handle   Reference to the handle.
   *
   *  @return
   *  - SNS_RC_INVALID_VALUE  Passed in handle is invalid.
   *  - SNS_RC_SUCCESS        Handle is freed.
   *
   */
  sns_rc (*sns_deregister_client)(sns_power_mgr_handle **handle);

  /**
   * @brief
   * Vote for MCPS( Million cycles per sec ) requirement
   * Note: This API is not supported in island.
   *
   * @param[inout]  handle    Handle returned by power manager service.
   * @param[in]     mcps      Requested mcps.
   *
   * @return
   *  - SNS_RC_INVALID_VALUE  Input parameters are invalid.
   *  - SNS_RC_SUCCESS        Action succeeded.
   *
   */
  sns_rc (*sns_update_mcps_vote)(sns_power_mgr_handle *handle, int16_t mcps);

} sns_power_mgr_service_api;
