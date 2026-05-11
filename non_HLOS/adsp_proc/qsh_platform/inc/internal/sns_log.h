#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Debug service wrapper for sensors framework.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/
#include <stdbool.h>

#if defined(SSC_TARGET_X86)
#include <stdio.h>
typedef enum
{
  SNS_SIM_LOG_DISABLE_ALL, // Disable all log packets
  SNS_SIM_LOG_DEFAULT,     // Enable default log packets
  SNS_SIM_LOG_ENABLE_ALL   // Enable all log packets
} sns_logging_level_e;

extern sns_logging_level_e sns_logging_level;
extern FILE *fp_dlf;
#endif

/**
 * @brief Macro to packed tightly together members without any padding bytes, 
 * which can reduce the overall size of the structure.
 */
#if defined __GNUC__
#define SNS_PACK(x) x __attribute__((__packed__))
#elif defined __GNUG__
#define SNS_PACK(x) x __attribute__((__packed__))
#elif defined __arm
#define SNS_PACK(x) __packed x
#elif defined _WIN32
#define SNS_PACK(x) x /*!< Microsoft uses #pragma pack() prologue/epilogue */
#else
#define SNS_PACK(x) x __attribute__((__packed__))
#endif

/**
 * @brief Logging header type 
 *
 */
typedef SNS_PACK(struct)
{
  uint16_t len; /*!< Specifies the length, in bytes of the
                 *    entry, including this header.
                 */

  uint16_t code; /*!< Specifies the log code for the entry as
                  *   enumerated above. Note: This is
                  *   specified as word to guarantee size.
                  */

  uint32_t ts[2]; /*!< The system timestamp for the log entry. The
                   *    upper 48 bits represent elapsed time since
                   *    6 Jan 1980 00:00:00 in 1.25 ms units. The
                   *    low order 16 bits represent elapsed time
                   *    since the last 1.25 ms tick in 1/32 chip
                   *    units (this 16 bit counter wraps at the
                   *    value 49152).
                   */
}
sns_log_hdr_type;

typedef uint16_t sns_log_code_type;

/**
 * @brief This function returns whether a particular code is enabled for
 * logging.
 *
 * @param[in] code   Specifies the code.
 *
 * @return
 *  -True:     If log mask is enabled.
 *  -False:    If log mask is disabled.
 *
 */
bool sns_log_status(sns_log_code_type code);

/**
 * @brief This function sets the length field in the given log record.
 *
 * @note ''ptr' must not point to a log entry that was allocated by log_alloc().
 * Use with caution.  It is possible to corrupt a log record using this
 * command.  It is intended for use only with accumulated log records, not
 * buffers returned by log_alloc().
 *
 * @param[in] ptr    Pointer points to the log whose length field is being set.
 * @param length     Length to be set in the log header.
 *
 * @return
 *  - None.
 *
 */
void sns_log_set_length(void *ptr, unsigned int length);

/**
 * @brief This function sets the log code.
 *
 * @param[in] ptr   Pointer points to the log whose log code field is being set.
 * @param[in] code  Specifies the code.
 *
 * @return
 * - None.
 *
 */
void sns_log_set_code(void *ptr, sns_log_code_type code);

/**
 * @brief This function captures the system time and stores it in the given
 * log record.
 *
 * @param[in] ptr  Pointer points to the log whose timestamp field is being set.
 *
 * @return
 *   - None.
 *
 */
void sns_log_set_timestamp(void *ptr);

/**
 * @brief This function is called to log an accumulated log entry. If logging is
 * enabled for the entry by the external device, then the entry is copied
 * into the diag allocation manager and commited immediately.
 *
 * @note Header contents must be assigned by the caller prior to calling this
 * function.  Therefore, sns_log_set code(), sns_log_set_length(), and
 * sns_log_set_timestamp() must be used prior to this call.
 *
 * @param[in] ptr       Pointer points to the log which is to be submitted.
 *
 * @return
 *  - True:  If log is submitted successfully into diag buffers.
 *  - False: If there is no space left in the buffers.
 *
 */
bool sns_log_submit(void *ptr);
