#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Entry point for all Sensors.  Each loaded Sensors library must
 * implement this function.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================
 */

/*==============================================================================
  Include Files
  ============================================================================*/
#include "sns_rc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_sensor_uid;
struct sns_sensor_api;
struct sns_sensor_instance_api;

/*==============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief Callback functions into the Framework, available only within
 * sns_register_sensors.
 *
 */
typedef struct sns_register_cb
{
  uint32_t struct_len; /*!< Length of sns_register_cb structure. */

  /**
   * @brief Allocate and initialize a Sensor.  Allocate state_len bytes for
   * private use of the Sensor, which will be initialized in sns_sensor::init().
   *
   * @param[in] state_len Size to be allocated for sns_sensor::state.
   * @param[in] sensor_api Sensor API implemented by the Sensor developer.
   * @param[in] instance_api Sensor Instance API; by the Sensor developer.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*init_sensor)(uint32_t state_len,
                        struct sns_sensor_api const *sensor_api,
                        struct sns_sensor_instance_api const *instance_api);

  // PEND: Add query for Framework version.
} sns_register_cb;

/**
 * @brief Call init_sensor for all provided Sensor types. This function will
 * only be called once per loaded library.
 *
 * @param[in] register_api API to register the sensor.
 *
 * @return
 *  - SNS_RC_SUCCESS   Success.
 *  - Any other value  Failure.
 */
typedef sns_rc (*sns_register_sensors)(sns_register_cb const *register_api);

/**
 * @brief Supported Memory segments for QSH codebase.
 *
 */
typedef enum
{
  SNS_MEM_SEG_UNKNOWN = 0, /*!< Invalid value. */
  SNS_MEM_SEG_QSH     = 1, /*!< Memory segment for QSH framework. */
  SNS_MEM_SEG_SSC     = 2, /*!< Memory segment for sensor algorithms. */
  SNS_MEM_SEG_QSHTECH = 3, /*!< Memory segment for QSH algorithms. */
  SNS_MEM_SEG_CAMERA  = 4, /*!< Memory segment for camera. */
  SNS_MEM_SEG_VA      = 5  /*!< Memory segment for audio. */
} sns_mem_segment;

/**
 * @brief Sensor registration entry.  Used in sns_static_sensors.c by SEE
 * initialization. Subject to change: DO NOT USE DIRECTLY.
 *
 */
typedef struct sns_register_entry
{
  sns_register_sensors func;    /*!< Function to register the sensor. */
  uint32_t cnt;                 /*!< Number of registered sensors. */
  bool is_islandLib;            /*!< Flag indicates if library is compiled for 
                                 *   island. 
                                 */
  bool is_multithreaded;        /*!< Flag indicates if library supports 
                                 *   multi-threaded execution. 
                                 */
  sns_mem_segment mem_segment;  /*!< Type of memory segment. */
} sns_register_entry;
