#pragma once
/** ============================================================================
 * @file
 *
 * @brief Definitions and utilities used in conjunction with file system.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/* The path where config file is present
 * */
#if defined(SSC_TARGET_HEXAGON)
#define SNS_FS_CONFIG_DIR "/vendor/etc/sensors/"
#elif defined(SSC_TARGET_X86)
#define SNS_FS_CONFIG_DIR "registry/config/"
#elif defined(SSC_TARGET_ASPEN_SWM)
#define SNS_FS_CONFIG_DIR "/vendor/etc/sensors/"
#endif
