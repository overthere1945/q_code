#pragma once
/** ===========================================================================
 * @file
 *
 * @brief The APIs to vote for the Sensor core power.
 * Sensor core power controls the power to the pram, ccd, scc & QUP hw blocks.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/
/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdint.h>
#include "sns_rc.h"

/*=============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief Supported Sensor core power modes.
 *
 */
typedef enum sns_core_power_mode
{
  SNS_CORE_POWER_DISABLE = 0, /*!< Disable Sensor core power. */
  SNS_CORE_POWER_ENABLE = 1   /*!< Enable Sensor core power. */

} sns_core_power_mode;

/*=============================================================================
  Public function declaration
  ============================================================================*/

/**
 * @brief The API to initialize Sensor core power utility.
 * @note This function should be called only by the framework once at the
 * bootup.
 *
 * @return
 * - SNS_RC_SUCCESS:  Initialization is successful.
 * - SNS_RC_FAIL:     Otherwise.
 *
 */
sns_rc sns_core_power_init(void);

/**
 * @brief The API to vote for the Sensor core power.
 *
 * @param[in] mode    SNS_CORE_POWER_DISABLE / SNS_CORE_POWER_ENABLE.
 *
 * @return
 *  - SNS_RC_SUCCESS:  Vote is applied successfully.
 *  - SNS_RC_FAIL:     Otherwise.
 *
 */
sns_rc sns_core_power_vote(sns_core_power_mode mode);
/* ---------------------------------------------------------------------------*/
