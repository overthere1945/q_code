#pragma once
/** ============================================================================
 * @file
 *
 * @brief Utility for sns types.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*******************************************************************************
                               Includes
*******************************************************************************/
#include <stdlib.h>

/*******************************************************************************
                               Constants/Macros
*******************************************************************************/

#ifndef ARR_SIZE
/**
 * @brief Returns number of elements in an array.
 *
 */
#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

#ifndef UNUSED_VAR
/**
 * @brief Used to intentionally indicate that a variable is unused. Can be used
 * to suppress compiler warnings for unused variables.
 *
 */
#define UNUSED_VAR(var) ((void)(var));
#endif

#ifndef ALIGN_8
/**
 * @brief Rounds off the size to align to the next 8 byte address.
 *
 */
#define ALIGN_8(size) ((((size_t)(size)) + 0x07) & 0xFFFFFFF8)
#endif

#ifndef SSC_TARGET_ASPEN_SWM
#ifndef likely
/**
 * @brief Provides branch prediction information to
 * compiler so that compiler can optimize the code. Use
 * likely if a condtional is True 90% of the time.
 * Otherwise code may run slower.
 *
 * @example
 * ...
 * if(likely(i > 0))
 * {
 *   // This is a likely condition and branch prediction has been optimized.
 * }
 * ...
 *
 */
#define likely(x) __builtin_expect((long)(x), 1)
#endif

#ifndef unlikely
/**
 * @brief Provides branch prediction information to
 * compiler so that compiler can optimize the code. Use
 * unlikely if a condtional is False 90% of the time.
 * Otherwise code may run slower.
 *
 * @example
 * ...
 * if(unlikely(i > 0))
 * {
 *   // This is a unlikely condition and branch prediction has been optimized.
 * }
 * ...
 *
 */
#define unlikely(x) __builtin_expect((long)(x), 0)
#endif
#endif /* SSC_TARGET_ASPEN_SWM */
