#pragma once
/** =============================================================================
 * @file sns_island_service.h
 *
 * @brief Provides a synchronous API to request island mode exit.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * =============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "sns_rc.h"
#include "sns_service.h"

struct sns_sensor;
struct sns_sensor_instance;

/*==============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief The island service. Can be obtained from
 * sns_service_managere::getService.
 *
 */
typedef struct sns_island_service
{
  sns_service service;                /*!< Service information. */
  struct sns_island_service_api *api; /*!< Public API provided by the Service to
                                       *   be used by the Sensor.
                                       */
} sns_island_service;

/**
 * @brief API made available to Sensors.
 *
 */
typedef struct sns_island_service_api
{
  uint32_t struct_len; /*!< Length of sns_island_service_api structure. */

  /**
   * @brief Request exit from island mode available to a Sensor.
   *
   * @note: Sensors typically use this service API to request island mode
   * exit from an island mode function before calling a normal mode function.
   * The normal mode function should be called only if this API returns
   * SNS_RC_SUCCESS.
   * Typical uses are:
   *  1. Physical sensor self-test:
   *     - handling timer/interrupt events for self-test execution.
   *  2. Handling registry sensor events.
   *
   * @param[in] service   Island service reference.
   * @param[in] sensor    Sensor reference that is requesting island exit.
   *                      NULL if an instance is requesting island exit.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*sensor_island_exit)(sns_island_service *service,
                               struct sns_sensor const *sensor);

  /**
   * @brief Request exit from island mode available to a Sensor Instance.
   *
   * @note Sensor Instances typically use this service API to request
   * island mode exit from an island mode function before calling a normal
   * mode function. The normal mode function should be called only if this API
   * returns SNS_RC_SUCCESS.
   * Typical uses are:
   *  1. Physical sensor self-test:
   *     - handling timer/interrupt events for self-test execution.
   *  2. Handling registry sensor events.
   *
   * @param[in] service   Island service reference.
   * @param[in] instance  Sensor instance reference that is requesting island
   *                      exit. NULL if a sensor is requesting island exit.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*sensor_instance_island_exit)(
      sns_island_service *service, struct sns_sensor_instance const *instance);

  /**
   * @brief Generates a log packet of the sns_island_trace_log type and commits
   * it.
   *
   * @note This API must be used for debug only. Typical use of this API is to
   * confirm if a line of code is executing in island mode.
   * All calls to this debug API should be disabled by default and only enabled
   * using registry config or compile switches when necessary for debug.
   *
   * @param[in] service           Island service reference.
   * @param[in] user_defined_id   The value to be added to the cookie field in
   *                              the log packet.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*island_log_trace)(sns_island_service *service,
                             uint64_t user_defined_id);

} sns_island_service_api;
