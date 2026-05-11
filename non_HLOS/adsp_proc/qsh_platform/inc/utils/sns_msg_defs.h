#pragma once

/** ============================================================================
 * @file sns_msg_def.h
 *
 * @brief Definitions for print messages
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/**----------------------------------------------------------------------------
  Include Files
  ---------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/**----------------------------------------------------------------------------
  Macros and Constants
  ---------------------------------------------------------------------------*/
#define SNS_PRINTF_ID_LEGACY     53
#define SNS_PRINTF_ID_FRAMEWORK  122
#define SNS_PRINTF_ID_PLATFORM   123
#define SNS_PRINTF_ID_SENSOR_INT 124
#define SNS_PRINTF_ID_SENSOR_EXT 125

#if defined(MY_GCC_FILE)
#define msg_file MY_GCC_FILE
#else
#define msg_file __FILE__
#endif

/**----------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
typedef struct sns_msg_desc_type
{
  uint16_t line;    /*!< Line number in source file */
  uint16_t ss_id;   /*!< Subsystem ID               */
  uint32_t ss_mask; /*!< Subsystem Mask             */
} sns_msg_desc_type;

typedef struct sns_msg_const_type
{
  sns_msg_desc_type desc;
  const char *msg;
} sns_msg_const_type;
