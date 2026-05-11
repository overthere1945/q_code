#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Abort the program is the expression evaluates to false.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/
#include <stdbool.h>
#include "err.h"
#include "sns_printf.h"
#include "sns_time.h"

#if defined(SSC_TARGET_HEXAGON) || defined(SSC_TARGET_ASPEN_SWM)
/**
 * @brief Abort the program if the expression evaluates to true.
 *
 * @param[in] value The boolean condition that must evaluate to false in order
 *                  to trigger an error.
 */
#define SNS_ASSERT(value)                                                      \
  if(!(value))                                                                 \
  ERR_FATAL(#value, 0, 0, 0)

#define SNS_ERR_FATAL(msg, arg1, arg2, arg3) ERR_FATAL(msg, arg1, arg2, arg3)

#elif defined SSC_TARGET_X86

#define SNS_ASSERT(value)                                                      \
  do                                                                           \
  {                                                                            \
    if(!(value))                                                               \
      abort();                                                                 \
  } while(false)

#define SNS_ERR_FATAL(msg, arg1, arg2, arg3)                                   \
  do                                                                           \
  {                                                                            \
    SNS_PRINTF(FATAL, sns_fw_printf, msg, arg1, arg2, arg3);                   \
    abort();                                                                   \
  } while(false)

#endif

/**
 * @brief Abort the program if the expression evaluates to true.
 *
 * @note This macro should be used for debugging purposes only. It will print
 * a message on stderr and then call abort().
 *
 * @param[in] value The boolean condition that must evaluate to false in order
 *                  to trigger an error.
 * @param[in] msg   A printf-style format string describing what went wrong.
 *
 */
#define SNS_VERBOSE_ASSERT(value, msg, ...)                                    \
  if(!(value))                                                                 \
  {                                                                            \
    SNS_PRINTF(FATAL, sns_fw_printf, msg, ##__VA_ARGS__);                      \
    sns_busy_wait(sns_convert_ns_to_ticks(50000000ULL));                       \
    SNS_ASSERT(value);                                                         \
  }                                                                            \
  (void)0
