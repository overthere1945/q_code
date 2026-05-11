#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Print utility for diag messages.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/
/*=============================================================================
  Include Files
  ===========================================================================*/
#include <stdarg.h>
#include <stdint.h>
#include "sns_diag_service.h"

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_sensor;
struct sns_fw_sensor_instance;

/*=============================================================================
  Macros and Constants
  ===========================================================================*/

/**
 * @brief Define the Max string length.
 *
 */
#define SNS_PRINT_MESSAGE_MAX_LEN 256

/*=============================================================================
  Public functions
  ===========================================================================*/
/**
 * @brief Print the buffer to diag.
 *
 * @note Please don't use this function directly. Public API is provided by
 * diag service using the sensor_printf() and sensor_inst_printf() functions in
 * sns_diag_service.h.
 *
 * @param[in] ssid          Diag subsystem ID.
 * @param[in] prio          Message priority.
 * @param[in] file          File name string.
 * @param[in] line          File line number.
 * @param[in] buf           Buffer to print.
 *
 * @return
 *  - None.
 *
 */
void sns_diag_print_buf(uint32_t ssid, sns_message_priority prio,
                        const char *file, uint32_t line, const char *buf);

/**
 * @brief Function prototype for printing a va_list to diag.
 *
 * @note Please don't use this function directly. Public API is
 * provided by using the SNS_PRINTF() and
 * SNS_INST_PRINTF() functions in sns_printf.h.
 *
 * @param[in] sensor       Sensor Pointer (either this or instance may be NULL).
 * @param[in] instance     Instance Pointer (either this or sensor may be NULL).
 * @param[in] msg_struct   Diag defined debug structure.
 * @param[in] nargs        Number of arguments.
 * @param[in] args         va_list of arguments.
 *
 * @return
 *  - None.
 *
 */
typedef void (*sns_diag_vprintf_fptr)(
    struct sns_sensor *sensor, struct sns_sensor_instance *instance,
    struct sns_msg_const_type const *msg_struct, uint32_t nargs, va_list args);

/**
 * @brief This function sends all messages to diag.
 *
 * @param[in] sensor       Sensor Pointer (either this or instance may be NULL).
 * @param[in] instance     Instance Pointer (either this or sensor may be NULL).
 * @param[in] msg_struct   Diag defined debug structure.
 * @param[in] nargs        Number of arguments.
 * @param[in] args         va_list of arguments.
 *
 * @return
 *  - None.
 *
 */
void sns_diag_vprintf_v2(struct sns_sensor *sensor,
                         struct sns_sensor_instance *instance,
                         struct sns_msg_const_type const *msg_struct,
                         uint32_t nargs, va_list args);

/**
 * @brief This function sends only error and fatal messages to diag.
 *
 * @param[in] sensor       Sensor Pointer (either this or instance may be NULL).
 * @param[in] instance     Instance Pointer (either this or sensor may be NULL).
 * @param[in] msg_struct   Diag defined debug structure.
 * @param[in] nargs        Number of arguments.
 * @param[in] args         va_list of arguments.
 *
 * @return
 *  - None.
 *
 */
void sns_diag_vprintf_v2_err_fatal_only(
    struct sns_sensor *sensor, struct sns_sensor_instance *instance,
    struct sns_msg_const_type const *msg_struct, uint32_t nargs, va_list args);
