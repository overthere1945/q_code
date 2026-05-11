#pragma once
/** ============================================================================
 * @file
 *
 * @brief Manages Diagnostic Services for Sensors and Sensor Instances.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stddef.h>
#include <stdint.h>

#include "sns_rc.h"
#include "sns_service.h"

#if !defined(__FILENAME__)
/**
 * @brief Find the base filename using __FILE__ macro.
 * Fall back to generic run-time computation for other compilers.
 *
 */
#include <string.h>
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

/*==============================================================================
  Forward Declarations
  ============================================================================*/

struct sns_sensor_instance;
struct sns_sensor;
struct sns_sensor_uid;
struct sns_msg_const_type;

/*==============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief The Diagnostics Manager. Will be obtained from
 * sns_service_manager::get_service.
 *
 */
typedef struct sns_diag_service
{
  sns_service service;                    /*!< Service information. */
  const struct sns_diag_service_api *api; /*!< Public api provided by the
                                           * framework to be used by the sensor.
                                           */
} sns_diag_service;

/**
 * @brief Types of sensor state logs.
 *
 */
typedef enum sns_diag_sensor_state_log
{
  SNS_DIAG_SENSOR_STATE_LOG_INTERNAL = 0,  /*!< Internal sensor state logs.  */
  SNS_DIAG_SENSOR_STATE_LOG_RAW = 1,       /*!< Raw state logs for drivers  */
  SNS_DIAG_SENSOR_STATE_LOG_INTERRUPT = 2, /*!< Sensor interrupt state logs. */
} sns_diag_sensor_state_log;

/**
 * @brief Priority levels for debug messages.
 *
 */
typedef enum sns_message_priority
{
  SNS_LOW = 0,   /*!< Low Priority levels for debug messages. */
  SNS_MED = 1,   /*!< Medium priority levels for debug messages. */
  SNS_HIGH = 2,  /*!< High priority levels for debug messages. */
  SNS_ERROR = 3, /*!< Error debug messages. */
  SNS_FATAL = 4, /*!< Fatal error debug messages. */
} sns_message_priority;

#if defined(SNS_USES_QSHRINK4)
#define SNS_LOW   0
#define SNS_MED   1
#define SNS_HIGH  2
#define SNS_ERROR 3
#define SNS_FATAL 4
#endif

/**
 * @brief Callback Function to PB Encode Log.
 *
 * @param[in] log               Pointer to log packet information. Data stored
 *                              here could be either the unencoded log packet
 *                              message or any suitable data from which the
 *                              encoded log packet can be generated.
 * @param[in]  log_size         Size of the log packet information.
 * @param[in]  encoded_log_size Maximum permitted encoded size of the log.
 * @param[out] encoded_log      Location where encoded log should be generated.
 * @param[out] bytes_written    Actual bytes written as part of encode.
 *
 * @return sns_rc.
 *
 */
typedef sns_rc (*sns_diag_encode_log_cb)(void *log, size_t log_size,
                                         size_t encoded_log_size,
                                         void *encoded_log,
                                         size_t *bytes_written);
/**
 * @brief Diag Service APIs.
 *
 */
typedef struct sns_diag_service_api
{
  uint32_t struct_len; /*!< Length of sns_diag_service_api structure. */

  /**
   * @brief Get max supported log packet size.
   *
   * @param[in] service Diag Service reference.
   *
   * @return
   *  - uint32_t  Maximum size of a log packet supported by the Framework
   *              (bytes).
   *
   */
  uint32_t (*get_max_log_size)(sns_diag_service const *service);

  /**
   * @brief Allocate memory for a log packet
   * Memory allocated by this function is used to store log packet information
   * and is returned as part of sns_diag_encode_log_cb for encoding. Only a
   * single outstanding allocation for log packet memory is allowed per log
   * type as defined in sns_diag_sensor_state_log.
   *
   * @param[in] service     Diag Service.
   * @param[in] instance    Instance pointer of the sensor.
   * @param[in] sensor_uid  UID of sensor for which log packet is being created.
   * @param[in] log_size    Requested size in bytes to store log packet
   *                        information.
   * @param[in] log_type    Type of log packet.
   *
   * @return
   *  - void* Allocated memory pointer of a log packet for an instance;
   *  - NULL  if out of memory or logging disabled for this sensor.
   *
   */
  void *(*alloc_log)(sns_diag_service *service,
                     struct sns_sensor_instance *instance,
                     struct sns_sensor_uid const *sensor_uid, uint32_t log_size,
                     sns_diag_sensor_state_log log_type);

  /**
   * @brief Submit a log packet.
   *
   * @param[in] service           This diag instance.
   * @param[in] instance          Instance pointer of the sensor submitting the
   *                              log.
   * @param[in] sensor_uid        UID of the sensor submitting the log.
   * @param[in] log_size          Size of the log packet information in bytes.
   * @param[in] log               Pointer to the log packet information.
   * @param[in] log_type          Type of log.
   * @param[in] encoded_log_size  Size of the encoded log in bytes.
   * @param[in] encode_log_cb     Function used to encode log.
   *
   * @return
   *  - SNS_RC_NOT_AVAILABLE  If the log packet is currently disabled.
   *  - SNS_RC_INVALID_TYPE   If the suid is not recognized.
   *  - SNS_RC_SUCCESS log    Packet was successfully submitted.
   *
   */
  sns_rc (*submit_log)(sns_diag_service *service,
                       struct sns_sensor_instance *instance,
                       struct sns_sensor_uid const *sensor_uid,
                       uint32_t log_size, void *log,
                       sns_diag_sensor_state_log log_type,
                       uint32_t encoded_log_size,
                       sns_diag_encode_log_cb encode_log_cb);

  /**
   * @brief printf() style function for printing a debug message from sensor.
   * Limited printf formatting support (only 32-bit int argument supported).
   *
   * @param[in] service     This diag service instance.
   * @param[in] sensor      Pointer to the sensor printing this message.
   * @param[in] msgs_struct Diag-defined static const msg_v2/v4_const_type for
   *                        the message.
   * @param[in] nargs       Number of arguments included in "...".
   *
   * @return
   *  - None.
   *
   */
  void (*sensor_printf_v2)(sns_diag_service *service, struct sns_sensor *sensor,
                           struct sns_msg_const_type const *msg_struct,
                           uint32_t nargs, ...);

  /**
   * @brief printf()-like function for printing a debug message from sensor
   * instance.
   * limited printf formatting support (only 32-bit int argument supported).
   *
   * @param[in] service This diag service instance.
   * @param[in] sensor  Instance pointer to the sensor instance printing this
   *                    message.
   * @param[in] diag    Defined static const msg_v2/v4_const_type for the
   * message.
   * @param[in] number  Number of arguments included in "...".
   *
   * @return
   *  - None.
   *
   */
  void (*sensor_inst_printf_v2)(sns_diag_service *service,
                                struct sns_sensor_instance *instance,
                                struct sns_msg_const_type const *msg_struct,
                                uint32_t nargs, ...);
} sns_diag_service_api;
