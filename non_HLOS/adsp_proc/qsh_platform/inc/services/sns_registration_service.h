#pragma once
/** =============================================================================
 * @file
 *
 * @brief Registration service provides functionality to sensors to register new
 * sensors within the same library.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stddef.h>
#include <stdint.h>

#include "sns_rc.h"
#include "sns_service.h"

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief sns_registration_service.
 *
 */
typedef struct sns_registration_service
{
  sns_service service;                      /*!< Service information. */
  struct sns_registration_service_api *api; /*!< Public api provided by the 
                                                 framework to be used by the 
                                                 sensor. */
} sns_registration_service;

/**
 * @brief API made available to Sensors.
 *
 */
typedef struct sns_registration_service_api
{
  uint32_t struct_len;  /*!< Length of sns_registration_service_api structure. */

  /**
   * @brief Allocate and initialize a Sensor in the same library of requesting 
   * sensor.
   *
   * @param[in] sensor        Sensor reference that is requesting creation of
   *                          new sensor. This values should never be NULL.
   * @param[in] state_len     Size to be allocated for sns_sensor::state.
   * @param[in] sensor_api    Sensor API implemented by the Sensor developer.
   * @param[in] instance_api  Sensor Instance API; by the Sensor developer.
   *
   * @return
   *  - SNS_RC_POLICY         State_len too large.
   *  - SNS_RC_NOT_AVAILABLE  Sensor UID is already in-use.
   *  - SNS_RC_FAILED         Sensor initialization failed.
   *  - SNS_RC_SUCCESS        Success.
   *
   */
  sns_rc (*sns_sensor_create)(
      const sns_sensor *sensor, uint32_t state_len,
      struct sns_sensor_api const *sensor_api,
      struct sns_sensor_instance_api const *instance_api);

} sns_registration_service_api;
