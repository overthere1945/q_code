#pragma once
/** ============================================================================
 * @file
 *
 * @brief Internal Printf support.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/
/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_diag_service.h"
#include "sns_printf.h"
#include "sns_types.h"

/*=============================================================================
  Dependencies
  ===========================================================================*/

extern sns_sensor *sns_fw_printf;

#ifndef SNS_PRINTF_DISABLED
extern void sns_diag_sensor_sprintf(uint16_t ssid, /*!< DIAG Subsystem ID */
                                    const sns_sensor *sensor,
                                    sns_message_priority prio, const char *file,
                                    uint32_t line, const char *format, ...);

#else
static inline void sns_diag_sensor_sprintf(uint16_t ssid,
                                           const sns_sensor *sensor,
                                           sns_message_priority prio,
                                           const char *file, uint32_t line,
                                           const char *format, ...)
{
  UNUSED_VAR(ssid);
  UNUSED_VAR(sensor);
  UNUSED_VAR(prio);
  UNUSED_VAR(file);
  UNUSED_VAR(line);
  UNUSED_VAR(format);
}
#endif /* SNS_PRINTF_DISABLED */

#ifndef SNS_PRINTF_DISABLED
extern void sns_diag_sensor_vsprintf(uint16_t ssid, /*!< DIAG Subsystem ID */
                                     const sns_sensor *sensor,
                                     sns_message_priority prio,
                                     const char *file, uint32_t line,
                                     const char *format, va_list args);

#else
static inline void sns_diag_sensor_vsprintf(uint16_t ssid,
                                            const sns_sensor *sensor,
                                            sns_message_priority prio,
                                            const char *file, uint32_t line,
                                            const char *format, va_list args)
{
  UNUSED_VAR(ssid);
  UNUSED_VAR(sensor);
  UNUSED_VAR(prio);
  UNUSED_VAR(file);
  UNUSED_VAR(line);
  UNUSED_VAR(format);
  UNUSED_VAR(args);
}
#endif /* SNS_PRINTF_DISABLED */

/*=============================================================================
  Macro Definitions
  ===========================================================================*/

#define SNS_DIAG_PTR "%x"

/**
 * @brief Size of the Sensors diag F3 trace buffer. This buffer will be used to
 * store F3 messages on the Heap and can be extracted from the RAM dump in case
 * of a crash.
 *
 */
#define SNS_F3_TRACE_SIZE 16 * 1024

/**
 * @brief Print a log message from a Sensor.
 * Full printf support; less efficient than SNS_PRINTF.
 *
 * @param[in] prio     LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] sensor   sns_sensor.
 * @param[in] fmt      Args printf style format string and arguments.
 *
 */
#define SNS_SPRINTF(prio, sensor, fmt, ...)                                    \
  do                                                                           \
  {                                                                            \
    sns_diag_sensor_sprintf(SNS_DIAG_SSID, sensor, SNS_##prio, __FILENAME__,   \
                            __LINE__, SNS_PD_NAME fmt, ##__VA_ARGS__);         \
  } while(0)

/**
 * @brief Print a log message from a Sensor.
 * Full printf support; less efficient than SNS_PRINTF.
 *
 * @param[in] prio     LOW, MED, HIGH, ERROR, FATAL.
 * @param[in] sensor   sns_sensor.
 * @param[in] fmt      va_list printf style format string and arguments.
 *
 */
#define SNS_VSPRINTF(prio, sensor, fmt, args)                                  \
  do                                                                           \
  {                                                                            \
    sns_diag_sensor_vsprintf(SNS_DIAG_SSID, sensor, SNS_##prio, __FILENAME__,  \
                             __LINE__, SNS_PD_NAME fmt, args);                 \
  } while(0)
