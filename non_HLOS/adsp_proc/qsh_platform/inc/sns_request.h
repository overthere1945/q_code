#pragma once
/** ============================================================================
 * @file
 *
 * @brief Client-generated configuration request.  Buffer contains a published,
 * backward-compatible structure.  Requests may have a version number,
 * but Sensors must appropriately handle older or newer versions.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <inttypes.h>

/*==============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief Structure for a Client-generated configuration request.
 *
 */
typedef struct sns_request
{

  uint32_t message_id;  /*!< Message ID of the sns_std_request::payload. */

  uint32_t request_len; /*!< Length of the request payload. */

  void *request;        /*!< Request payload; is an encoded message of type 
                         *   sns_std_request. 
                         */
} sns_request;
