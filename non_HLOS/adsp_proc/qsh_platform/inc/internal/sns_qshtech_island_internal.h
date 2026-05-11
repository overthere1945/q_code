#pragma once
/** ============================================================================
 * @file
 *
 * @brief Qshtech island vote internal APIs.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/
/*==============================================================================
  Include Files
  ============================================================================*/

#include "sns_rc.h"

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
 * @brief The API to initialize qshtech island vote utility. This API should be
 *        called only once at the bootup.
 *
 * @return
 *   - SNS_RC_SUCCESS: No error occurred; success.
 *   - SNS_RC_FAILED:  Internal error occurred.
 *
 */
sns_rc sns_qshtech_island_init(void);
