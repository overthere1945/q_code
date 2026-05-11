#pragma once
/** ============================================================================
 * @file
 *
 * @brief An abstraction layer for obtaining information about the underlying
 *        hardware
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "sns_rc.h"
#include "sns_types.h"

/*============================================================================
  Abstracted Structures
  ============================================================================*/
typedef enum sns_hw_info_type
{
  SNS_HW_INFO_TYPE_PLATFORM_INFO,    /*!< Get platform information. */
  SNS_HW_INFO_TYPE_PLATFORM_TYPE,    /*!< Get platform type as text. */
  SNS_HW_INFO_TYPE_PLATFORM_SLT_KEY, /*!< Get platform System Level Test Key. */
  SNS_HW_INFO_TYPE_CHIP_ID,          /*!< Get platform chipID. */
  SNS_HW_INFO_TYPE_CHIP_FAMILY_ID,   /*!< Get platform chipfamilyID. */
} sns_hw_info_type;

/**
 * @brief Structure containing platform info.
 *
 */
typedef struct sns_hw_platform_info
{
  uint32_t platform;
  uint32_t version;
  uint32_t subtype;
} sns_hw_platform_info;

/**
 * @brief Structure containing platform type info.
 *
 */
typedef struct sns_hw_platform_type
{
  uint32_t platform;
  char *text;
} sns_hw_platform_type;

/**
 * @brief Structure containing the platform slt key.
 *
 */
typedef struct sns_hw_platform_slt_key
{
  uint32_t *key;
} sns_hw_platform_slt_key;

/**
 * @brief Structure containing the chip id.
 *
 */
typedef struct sns_hw_chip_id
{
  uint32_t id;
} sns_hw_chip_id;

/**
 * @brief Structure containing the chipfamily id.
 *
 */
typedef struct sns_hw_chip_family_id
{
  uint32_t id;
} sns_hw_chip_family_id;

/*============================================================================
  Abstracted Functions
  ============================================================================*/
/**
 * @brief Handles the retrieval of hardware information based on target
 * platform.
 *
 * @param[in]  info_type Determines the type of retrieval.
 * @param[out] info      Output strcture expected by the retrieval type.
 *
 * @return sns_rc 0 == Success, any other value == Failure.
 *
 */
sns_rc sns_get_hw_info(sns_hw_info_type info_type, void *info);
