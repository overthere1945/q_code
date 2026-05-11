#pragma once
/** ============================================================================
 * @file
 *
 * @brief Helper functions and macros for submitting debug messages
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * @note
 * IMPORTANT :
 * All the macros below (except the following 4) and functions defined
 * in this file are internal to the framework and irrelevant for
 * Algorithm Developers and Diver Vendors.
 * Only the 4 user MACROs described below are important for DEBUG F3 Messages.
 * 1) SNS_PRINTF :
 *    - This MACRO should be used in Sensor Context and DEBUG Messages will
 *    be generated only in Big Image. However, (by default) ERROR and FATAL
 *    levels will also be available in Micro Image.
 * 2) SNS_INST_PRINTF :
 *    - This MACRO should be used in Instance Context and DEBUG Messages will
 *    be generated only in Big Image. However, (by default) ERROR and FATAL
 *    levels will also be available in Micro Image.
 * 3) SNS_UPRINTF :
 *    - This MACRO can be used in Sensor Context and DEBUG Messages will be
 *    generated in both Big as well as Micro Image.
 * 4) SNS_INST_UPRINTF :
 *    - This MACRO can be used in Instance Context and DEBUG Messages will be
 *    generated in both Big as well as Micro Image.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdarg.h>

#include "sns_diag_service.h"
#include "sns_msg_defs.h"
#include "sns_sensor.h"
#include "sns_sensor_instance.h"
#include "sns_types.h"

/*=============================================================================
  Macro Definitions : Framework
  ===========================================================================*/

#ifndef SNS_DIAG_SSID
/**
 * @brief SNS_DIAG_SSID should be defined in the scons file to pick a particular
 * PRINTF_ID for this module. If SNS_DIAG_SSID has not been defined, put the
 * messages into the default SNS_PRINTF_ID_SENSOR_EXT.
 *
 * @note Defined here if note already defined.
 *
 */
#define SNS_DIAG_SSID SNS_PRINTF_ID_SENSOR_EXT
#endif

#ifndef SNS_DIAG_ERROR_SSID
/**
 * @brief SNS_DIAG_ERROR_SSID is defined as SNS_DIAG_SSID here
 * if not ready defined.
 *
 */
#define SNS_DIAG_ERROR_SSID SNS_DIAG_SSID
#endif

/**
 * @brief Macros to help count variable number of arguments, max_args=16.
 *
 */
#define SELECT_NARG(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,     \
                    _13, _14, _15, N, ...)                                     \
  N
#define NARGS(...)                                                             \
  SELECT_NARG(_0, ##__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3,  \
              2, 1, 0)

#if defined(SNS_PRINTF_REL_MODE)

/**
 * @brief Section name for Big Image.
 *
 */
#define SECTION_BIG_IMAGE __attribute__((section(".rodata1")))

/**
 * @brief Big Image section has string in island memory.
 *
 */
#define BIG_IMAGE_STRING_IN_ISLAND false

/**
 * @brief Section name for Micro Image.
 *
 */
#define SECTION_MICRO_IMAGE __attribute__((section(".rodata1")))

/**
 * @brief Micro Image section has string in island memory.
 *
 */
#define MICRO_IMAGE_STRING_IN_ISLAND false

#elif defined(SNS_PRINTF_DEV_MODE)

/**
 * @brief Section name for Big Image.
 *
 */
#define SECTION_BIG_IMAGE            __attribute__((section(".rodata.sns")))

/**
 * @brief Big Image section has string in island memory.
 *
 */
#define BIG_IMAGE_STRING_IN_ISLAND   true

/**
 * @brief Section name for Micro Image.
 *
 */
#define SECTION_MICRO_IMAGE          __attribute__((section(".rodata.sns")))

/**
 * @brief Micro Image section has string in island memory.
 *
 */
#define MICRO_IMAGE_STRING_IN_ISLAND true

#else

/**
 * @brief Section name for Big Image.
 *
 */
#define SECTION_BIG_IMAGE            __attribute__((section(".rodata1")))

/**
 * @brief Big Image section has string in island memory.
 *
 */
#define BIG_IMAGE_STRING_IN_ISLAND   false

/**
 * @brief Section name for Micro Image.
 *
 */
#define SECTION_MICRO_IMAGE          __attribute__((section(".rodata.sns")))

/**
 * @brief Micro Image section has string in island memory.
 *
 */
#define MICRO_IMAGE_STRING_IN_ISLAND true
#endif

#ifdef SNS_USES_QSHRINK4_CORE_MPSS_3_10
#define SNS_PRINTF_MSG_CONST xx_msg_v4_const
#else
#define SNS_PRINTF_MSG_CONST xx_msg_const
#endif

#ifndef SNS_PD_NAME
#define SNS_PD_NAME ""
#endif

/**
 * @brief Creates a constant string which is placed in micro image, for use by
 * diag.
 *
 */
#define SNS_MSG_V2_CONST_MICRO(xx_ss_id, xx_ss_mask, xx_fmt)                   \
  static const char SNS_xx_img_str[] SECTION_MICRO_IMAGE =                     \
      msg_file ":" SNS_PD_NAME xx_fmt;                                         \
  static const sns_msg_const_type xx_msg_const SECTION_MICRO_IMAGE = {         \
      .desc = {.line = __LINE__,                                               \
               .ss_id = (xx_ss_id),                                            \
               .ss_mask = (xx_ss_mask)},                                       \
      .msg = (char *)&SNS_xx_img_str}

/**
 * @brief Creates a constant string which is placed in big image, for use by
 * diag.
 *
 */
#define SNS_MSG_V2_CONST_BIG(xx_ss_id, xx_ss_mask, xx_fmt)                     \
  static const char SNS_xx_img_str[] SECTION_BIG_IMAGE =                       \
      msg_file ":" SNS_PD_NAME xx_fmt;                                         \
  static const sns_msg_const_type xx_msg_const SECTION_BIG_IMAGE = {           \
      .desc = {.line = __LINE__,                                               \
               .ss_id = (xx_ss_id),                                            \
               .ss_mask = (xx_ss_mask)},                                       \
      .msg = (char *)&SNS_xx_img_str}

/**
 * @brief Creates a constant string which is placed in micro image, for use by
 * diag.
 *
 */
#define SNS_MSG_V4_CONST_MICRO(xx_ss_id, xx_ss_mask, xx_fmt)                   \
  static const char SNS_xx_img_str[] QSR_4_0_ATTR =                            \
      MSG_STR(xx_ss_id) ":" MSG_STR(__LINE__) ":" msg_file                     \
                                              ":" SNS_PD_NAME xx_fmt;          \
  static const msg_v4_const_type SNS_PRINTF_MSG_CONST                          \
      __attribute__((section("QSR_4_0_MSG.fmt.rodata." MSG_STR(                \
                         xx_ss_id) "." MSG_STR(xx_ss_mask) "." SECT_EXT),      \
                     used)) = {{xx_ss_mask, (char *)&SNS_xx_img_str}}

/**
 * @brief Creates a constant string which is placed in big image, for use by
 * diag.
 *
 */
#define SNS_MSG_V4_CONST_BIG(xx_ss_id, xx_ss_mask, xx_fmt)                     \
  static const char SNS_xx_img_str[] QSR_4_0_ATTR =                            \
      MSG_STR(xx_ss_id) ":" MSG_STR(__LINE__) ":" msg_file                     \
                                              ":" SNS_PD_NAME xx_fmt;          \
  static const msg_v4_const_type SNS_PRINTF_MSG_CONST                          \
      __attribute__((section("QSR_4_0_MSG.fmt.rodata." MSG_STR(                \
                         xx_ss_id) "." MSG_STR(xx_ss_mask) "." SECT_EXT),      \
                     used)) = {{xx_ss_mask, (char *)&SNS_xx_img_str}}

/**
 * @brief Choose V2 and V4 debug messages based on availability.
 *
 */
#ifdef SNS_USES_QSHRINK4
#define SNS_MSG_CONST_BIG   SNS_MSG_V4_CONST_BIG
#define SNS_MSG_CONST_MICRO SNS_MSG_V4_CONST_MICRO
#else
#define SNS_MSG_CONST_BIG   SNS_MSG_V2_CONST_BIG
#define SNS_MSG_CONST_MICRO SNS_MSG_V2_CONST_MICRO
#endif /* SNS_USES_QSHRINK4 */

#define SNS_MSG_CONST_FATAL(xx_ss_mask, xx_fmt)                                \
  SNS_MSG_CONST_MICRO(SNS_DIAG_ERROR_SSID, xx_ss_mask, xx_fmt)
#define SNS_MSG_CONST_FATAL_IN_ISLAND MICRO_IMAGE_STRING_IN_ISLAND

#define SNS_MSG_CONST_ERROR(xx_ss_mask, xx_fmt)                                \
  SNS_MSG_CONST_MICRO(SNS_DIAG_ERROR_SSID, xx_ss_mask, xx_fmt)
#define SNS_MSG_CONST_ERROR_IN_ISLAND MICRO_IMAGE_STRING_IN_ISLAND

#define SNS_MSG_CONST_HIGH(xx_ss_mask, xx_fmt)                                 \
  SNS_MSG_CONST_BIG(SNS_DIAG_SSID, xx_ss_mask, xx_fmt)
#define SNS_MSG_CONST_HIGH_IN_ISLAND BIG_IMAGE_STRING_IN_ISLAND

#define SNS_MSG_CONST_MED(xx_ss_mask, xx_fmt)                                  \
  SNS_MSG_CONST_BIG(SNS_DIAG_SSID, xx_ss_mask, xx_fmt)
#define SNS_MSG_CONST_MED_IN_ISLAND BIG_IMAGE_STRING_IN_ISLAND

#define SNS_MSG_CONST_LOW(xx_ss_mask, xx_fmt)                                  \
  SNS_MSG_CONST_BIG(SNS_DIAG_SSID, xx_ss_mask, xx_fmt)
#define SNS_MSG_CONST_LOW_IN_ISLAND BIG_IMAGE_STRING_IN_ISLAND

#define SNS_UMSG_CONST_FATAL(xx_ss_mask, xx_fmt)                               \
  SNS_MSG_CONST_MICRO(SNS_DIAG_ERROR_SSID, xx_ss_mask, xx_fmt)
#define SNS_UMSG_CONST_FATAL_IN_ISLAND MICRO_IMAGE_STRING_IN_ISLAND

#define SNS_UMSG_CONST_ERROR(xx_ss_mask, xx_fmt)                               \
  SNS_MSG_CONST_MICRO(SNS_DIAG_ERROR_SSID, xx_ss_mask, xx_fmt)
#define SNS_UMSG_CONST_ERROR_IN_ISLAND MICRO_IMAGE_STRING_IN_ISLAND

#define SNS_UMSG_CONST_HIGH(xx_ss_mask, xx_fmt)                                \
  SNS_MSG_CONST_MICRO(SNS_DIAG_SSID, xx_ss_mask, xx_fmt)
#define SNS_UMSG_CONST_HIGH_IN_ISLAND MICRO_IMAGE_STRING_IN_ISLAND

#define SNS_UMSG_CONST_MED(xx_ss_mask, xx_fmt)                                 \
  SNS_MSG_CONST_MICRO(SNS_DIAG_SSID, xx_ss_mask, xx_fmt)
#define SNS_UMSG_CONST_MED_IN_ISLAND MICRO_IMAGE_STRING_IN_ISLAND

#define SNS_UMSG_CONST_LOW(xx_ss_mask, xx_fmt)                                 \
  SNS_MSG_CONST_MICRO(SNS_DIAG_SSID, xx_ss_mask, xx_fmt)
#define SNS_UMSG_CONST_LOW_IN_ISLAND MICRO_IMAGE_STRING_IN_ISLAND
/*=============================================================================
  Macro Definitions : User
  ===========================================================================*/

/**
 * @brief Print a log message from a Sensor.  Only supports integral arguments.
 *
 * @param[in] prio   LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] sensor sns_sensor.
 * @param[in] fmt    Args printf style format string.
 *                    and arguments. %s and %f are not supported.
 *
 */
#ifndef SNS_PRINTF_DISABLED
#define SNS_PRINTF(prio, sensor, fmt, ...)                                     \
  do                                                                           \
  {                                                                            \
    SNS_MSG_CONST_##prio(1 << (SNS_##prio), fmt);                              \
    sns_diag_sensor_printf_v3(                                                 \
        NULL, (sns_sensor *)(sensor), SNS_MSG_CONST_##prio##_IN_ISLAND,        \
        (struct sns_msg_const_type const *)(&SNS_PRINTF_MSG_CONST),            \
        NARGS(__VA_ARGS__), ##__VA_ARGS__);                                    \
  } while(0)
#else
#define SNS_PRINTF(prio, sensor, fmt, ...)                                     \
  do                                                                           \
  {                                                                            \
    sns_diag_sensor_printf_v2(NULL, (sns_sensor *)(sensor), NULL,              \
                              NARGS(__VA_ARGS__), ##__VA_ARGS__);              \
  } while(0)
#endif

/**
 * @brief Print a log message from a Sensor.  Only supports integral arguments.
 *
 * @param[in] prio   LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] sensor sns_sensor.
 * @param[in] fmt    Args printf style format string.
 *                   and arguments. %s and %f are not supported.
 *
 */
#ifndef SNS_PRINTF_DISABLED
#define SNS_VPRINTF(prio, sensor, fmt, nargs, args)                            \
  do                                                                           \
  {                                                                            \
    SNS_MSG_CONST_##prio(1 << (SNS_##prio), fmt);                              \
    sns_diag_sensor_vprintf(                                                   \
        NULL, (sns_sensor *)(sensor), SNS_MSG_CONST_##prio##_IN_ISLAND,        \
        (struct sns_msg_const_type const *)(&SNS_PRINTF_MSG_CONST), nargs,     \
        args);                                                                 \
  } while(0)
#else
#define SNS_VPRINTF(prio, sensor, fmt, nargs, args)                            \
  do                                                                           \
  {                                                                            \
    UNUSED_VAR(prio);                                                          \
    UNUSED_VAR(sensor);                                                        \
    UNUSED_VAR(fmt);                                                           \
    UNUSED_VAR(nargs);                                                         \
    UNUSED_VAR(args);                                                          \
  } while(0)
#endif

/**
 * @brief Print a log message from a Sensor Instance.  Only supports integral
 * arguments.
 *
 * @param[in] prio   LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] inst   Sensor instance.
 * @param[in] fmt    Args printf style format string.
 *                   and arguments. %s and %f are not supported.
 *
 */
#ifndef SNS_PRINTF_DISABLED
#define SNS_INST_PRINTF(prio, inst, fmt, ...)                                  \
  do                                                                           \
  {                                                                            \
    SNS_MSG_CONST_##prio(1 << (SNS_##prio), fmt);                              \
    sns_diag_sensor_inst_printf_v3(                                            \
        NULL, (sns_sensor_instance *)(inst), SNS_MSG_CONST_##prio##_IN_ISLAND, \
        (struct sns_msg_const_type const *)(&SNS_PRINTF_MSG_CONST),            \
        NARGS(__VA_ARGS__), ##__VA_ARGS__);                                    \
  } while(0)
#else
#define SNS_INST_PRINTF(prio, inst, fmt, ...)                                  \
  do                                                                           \
  {                                                                            \
    sns_diag_sensor_inst_printf_v2(NULL, (sns_sensor_instance *)(inst), NULL,  \
                                   NARGS(__VA_ARGS__), ##__VA_ARGS__);         \
  } while(0)
#endif

#ifndef SNS_PRINTF_DISABLED

/**
 * @brief Print a log message from a Sensor Instance.  Only supports integral
 * arguments.
 *
 * @param[in] prio   LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] inst   Sensor instance.
 * @param[in] fmt    Args printf style format string.
 *                   and arguments. %s and %f are not supported.
 * @param[in] nargs  Number of arguments.
 * @param[in] args   Arguments list.
 *
 */
#define SNS_INST_VPRINTF(prio, inst, fmt, nargs, args)                         \
  do                                                                           \
  {                                                                            \
    SNS_MSG_CONST_##prio(1 << (SNS_##prio), fmt);                              \
    sns_diag_sensor_inst_vprintf(                                              \
        NULL, (sns_sensor_instance *)(inst), SNS_MSG_CONST_##prio##_IN_ISLAND, \
        (struct sns_msg_const_type const *)(&SNS_PRINTF_MSG_CONST), nargs,     \
        args);                                                                 \
  } while(0)
#else

/**
 * @brief Print a log message from a Sensor Instance.  Only supports integral
 * arguments.
 *
 * @param[in] prio   LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] sensor sns_sensor.
 * @param[in] fmt    Args printf style format string.
 *                   and arguments. %s and %f are not supported.
 *
 */
#define SNS_INST_VPRINTF(prio, inst, fmt, nargs, args)                         \
  do                                                                           \
  {                                                                            \
    UNUSED_VAR(prio);                                                          \
    UNUSED_VAR(inst);                                                          \
    UNUSED_VAR(fmt);                                                           \
    UNUSED_VAR(nargs);                                                         \
    UNUSED_VAR(args);                                                          \
  } while(0)
#endif

#ifndef SNS_PRINTF_DISABLED

/**
 * @brief Print a Micro Image log message from a Sensor.  Only supports integral
 * arguments.
 *
 * @param[in] prio   LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] sensor sns_sensor.
 * @param[in] fmt    Args printf style format string.
 *                   and arguments. %s and %f are not supported.
 *
 */
#define SNS_UPRINTF(prio, sensor, fmt, ...)                                    \
  do                                                                           \
  {                                                                            \
    SNS_UMSG_CONST_##prio(1 << (SNS_##prio), fmt);                             \
    sns_diag_sensor_printf_v3(                                                 \
        NULL, (sns_sensor *)(sensor), SNS_UMSG_CONST_##prio##_IN_ISLAND,       \
        (struct sns_msg_const_type const *)(&SNS_PRINTF_MSG_CONST),            \
        NARGS(__VA_ARGS__), ##__VA_ARGS__);                                    \
  } while(0)
#else

/**
 * @brief Print a Micro Image log message from a Sensor.  Only supports integral
 * arguments.
 *
 * @param[in] prio   LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] sensor sns_sensor.
 * @param[in] fmt    Args printf style format string.
 *                   and arguments. %s and %f are not supported.
 *
 */
#define SNS_UPRINTF(prio, sensor, fmt, ...)                                    \
  do                                                                           \
  {                                                                            \
    sns_diag_sensor_printf_v2(NULL, (sns_sensor *)(sensor), NULL,              \
                              NARGS(__VA_ARGS__), ##__VA_ARGS__);              \
  } while(0)
#endif

#ifndef SNS_PRINTF_DISABLED

/**
 * @brief Print a Micro Image log message from a Sensor Instance.  Only supports
 * integral arguments.
 *
 * @param[in] prio   LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] inst   Sensor instance.
 * @param[in] fmt    Args printf style format string.
 *                   and arguments. %s and %f are not supported.
 *
 */
#define SNS_INST_UPRINTF(prio, inst, fmt, ...)                                 \
  do                                                                           \
  {                                                                            \
    SNS_UMSG_CONST_##prio(1 << (SNS_##prio), fmt);                             \
    sns_diag_sensor_inst_printf_v3(                                            \
        NULL, (sns_sensor_instance *)(inst),                                   \
        SNS_UMSG_CONST_##prio##_IN_ISLAND,                                     \
        (struct sns_msg_const_type const *)(&SNS_PRINTF_MSG_CONST),            \
        NARGS(__VA_ARGS__), ##__VA_ARGS__);                                    \
  } while(0)
#else

/**
 * @brief Print a Micro Image log message from a Sensor Instance.  Only supports
 * integral arguments.
 *
 *
 * @param[in] prio   LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] inst   Sensor instance.
 * @param[in] fmt    Args printf style format string.
 *                   and arguments. %s and %f are not supported.
 *
 */
#define SNS_INST_UPRINTF(prio, inst, fmt, ...)                                 \
  do                                                                           \
  {                                                                            \
    sns_diag_sensor_inst_printf_v2(NULL, (sns_sensor_instance *)(inst), NULL,  \
                                   NARGS(__VA_ARGS__), ##__VA_ARGS__);         \
  } while(0)
#endif

/*=============================================================================
  Function Prototypes : Framework
  ===========================================================================*/

#ifndef SNS_PRINTF_DISABLED
/**
 * @brief Please use SNS_PRINTF/SNS_UPRINTF and do not call this function
 * directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
extern void sns_diag_sensor_printf_v2(sns_diag_service *, sns_sensor *,
                                      struct sns_msg_const_type const *,
                                      uint32_t, ...);

/**
 * @brief Please use SNS_INST_PRINTF/SNS_INST_UPRINTF and do not call this
 * function directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
extern void sns_diag_sensor_inst_printf_v2(sns_diag_service *,
                                           sns_sensor_instance *,
                                           struct sns_msg_const_type const *,
                                           uint32_t, ...);

/**
 * @brief Please use SNS_PRINTF/SNS_UPRINTF and do not call this function
 * directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
extern void sns_diag_sensor_printf_v3(sns_diag_service *, sns_sensor *, bool,
                                      struct sns_msg_const_type const *,
                                      uint32_t, ...);

/**
 * @brief Please use SNS_INST_PRINTF/SNS_INST_UPRINTF and do not call this
 * function directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
extern void sns_diag_sensor_inst_printf_v3(sns_diag_service *,
                                           sns_sensor_instance *, bool,
                                           struct sns_msg_const_type const *,
                                           uint32_t, ...);
/**
 * @brief Please use SNS_VPRINTF and do not call this function directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
extern void sns_diag_sensor_vprintf(sns_diag_service *, sns_sensor *, bool,
                                    struct sns_msg_const_type const *, uint32_t,
                                    va_list);

/**
 * @brief Please use SNS_INST_VPRINTF and do not call this function directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
extern void sns_diag_sensor_inst_vprintf(sns_diag_service *,
                                         sns_sensor_instance *, bool,
                                         struct sns_msg_const_type const *,
                                         uint32_t, va_list);

#else

/**
 * @brief Please use SNS_PRINTF/SNS_UPRINTF and do not call this function
 * directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
static inline void
sns_diag_sensor_printf_v2(sns_diag_service *service, sns_sensor *sensor,
                          struct sns_msg_const_type const *const_ptr,
                          uint32_t nargs, ...)
{
  UNUSED_VAR(service);
  UNUSED_VAR(sensor);
  UNUSED_VAR(nargs);
  UNUSED_VAR(const_ptr);
}

/**
 * @brief Please use SNS_INST_PRINTF/SNS_INST_UPRINTF and do not call this
 * function directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
static inline void sns_diag_sensor_inst_printf_v2(
    sns_diag_service *service, sns_sensor_instance *instance,
    struct sns_msg_const_type const *const_ptr, uint32_t nargs, ...)
{
  UNUSED_VAR(service);
  UNUSED_VAR(instance);
  UNUSED_VAR(nargs);
  UNUSED_VAR(const_ptr);
}

/**
 * @brief Please use SNS_PRINTF/SNS_UPRINTF and do not call this function
 * directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
static inline void sns_diag_sensor_printf_v3(
    sns_diag_service *service, sns_sensor *sensor, bool is_island_const_ptr,
    struct sns_msg_const_type const *const_ptr, uint32_t nargs, ...)
{
  UNUSED_VAR(service);
  UNUSED_VAR(sensor);
  UNUSED_VAR(nargs);
  UNUSED_VAR(is_island_const_ptr);
  UNUSED_VAR(const_ptr);
}

/**
 * @brief Please use SNS_INST_PRINTF/SNS_INST_UPRINTF and do not call this
 * function directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 *
 */
static inline void sns_diag_sensor_inst_printf_v3(
    sns_diag_service *service, sns_sensor_instance *instance,
    bool is_island_const_ptr, struct sns_msg_const_type const *const_ptr,
    uint32_t nargs, ...)
{
  UNUSED_VAR(service);
  UNUSED_VAR(instance);
  UNUSED_VAR(nargs);
  UNUSED_VAR(is_island_const_ptr);
  UNUSED_VAR(const_ptr);
}

/**
 * @brief Please use SNS_VPRINTF and do not call this function directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 * @param[in] va_list   VA_List arguments.
 *
 */
static inline void sns_diag_sensor_vprintf(
    sns_diag_service *service, sns_sensor *sensor, bool is_island_const_ptr,
    struct sns_msg_const_type const *const_ptr, uint32_t nargs, va_list args)
{
  UNUSED_VAR(service);
  UNUSED_VAR(sensor);
  UNUSED_VAR(nargs);
  UNUSED_VAR(is_island_const_ptr);
  UNUSED_VAR(const_ptr);
  UNUSED_VAR(args);
}

/**
 * @brief Please use SNS_INST_VPRINTF and do not call this function directly.
 *
 * @param[in] service   Diag service pointer.
 * @param[in] sensor    Sensor service pointer.
 * @param[in] instance  Instance pointer.
 * @param[in] const_ptr Message type pointer.
 * @param[in] nargs     Print arguments.
 * @param[in] va_list   VA_List arguments.
 *
 */
static inline void sns_diag_sensor_inst_vprintf(
    sns_diag_service *service, sns_sensor_instance *instance,
    bool is_island_const_ptr, struct sns_msg_const_type const *const_ptr,
    uint32_t nargs, va_list args)
{
  UNUSED_VAR(service);
  UNUSED_VAR(instance);
  UNUSED_VAR(nargs);
  UNUSED_VAR(is_island_const_ptr);
  UNUSED_VAR(const_ptr);
  UNUSED_VAR(args);
}
#endif /* SNS_PRINTF_DISABLED */
