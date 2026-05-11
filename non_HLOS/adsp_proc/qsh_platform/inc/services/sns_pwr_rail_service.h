#pragma once
/** =============================================================================
 * @file
 *
 * @brief Power rail management API.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * =============================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include "sns_rc.h"
#include "sns_service.h"
#include "sns_time.h"

#define RAIL_NUMBER_MAX           2
#define RAIL_NAME_STRING_SIZE_MAX 30

/**
 * @brief Defining initial gpio number of local rail.
 *
 */
#define INVALID_GPIONUM -1

/**
 * @brief Maximum possible size of gpio number.
 *
 */
#define GPIO_NUMBER_STRING_SIZE_MAX 4

/**
 * @brief Define a dummy rail for OEMs that use external rails for their
 * sensors. Dummy rail config is required so island mode block/unblock logic
 * still works for those OEMs. Turning the dummy rail on at init also ensures
 * that GDSC is voted on during startup.
 *
 */
#define DUMMY_SENSOR_VDD "/pmic/client/dummy_vdd"
#define SNS_PWR_RAIL_VDD "/see/rail/eLDO"

/**
 * @brief Defines prefix of local eLDO rail that is compared with the
 * prefix of the local rail name before declaring rail as valid.
 *
 */
#define SNS_LOCAL_ELDO "/sns/local/eLDO/"

/**
 * @brief Defines prefix of remote eLDO rail that is compared with the
 * prefix of the remote rail name before declaring rail as valid.
 *
 */
#define SNS_REMOTE_ELDO "/sns/remote/eLDO/"

/**  Forward Declarations. */
struct sns_pwr_rail_service_api;
struct sns_sensor;

/**
 * @brief Power rail states provided by this service.
 *
 */
typedef enum
{
  SNS_RAIL_OFF,     /*!< OFF state.
                     *   Must be used when there are no active Sensors 
                     *   Instances. 
                     */
  SNS_RAIL_ON_LPM,  /*!< Low Power Mode.
                     *   Must be used as an ON state when sensor power 
                     *   requirement
                     *   is < 10 mA. This spec is based on chipset's pmic 
                     *   document.
                     */
  SNS_RAIL_ON_NPM   /*!< Normal Power Mode.
                     *   Must be used as an ON state when sensor power requirement
                     *   is >= 10 mA. This spec is based on chipset's pmic 
                     *   document. 
                     */
} sns_power_rail_state;

/**
 * @brief Rail name definition.
 *
 */
typedef struct sns_rail_name
{
  char name[RAIL_NAME_STRING_SIZE_MAX];
} sns_rail_name;

/**
 * @brief Rail configuration definition.
 *
 */
typedef struct
{
  sns_power_rail_state rail_vote;       /*!< Overall vote for all sensor rails.
                                         *   At any time, all rails for a sensor 
                                         *   can have a single state from 
                                         *   sns_power_rail_state.
                                         */
  uint8_t num_of_rails;                 /*!< Number of rails in sns_rail_name array. */
  sns_rail_name rails[RAIL_NUMBER_MAX]; /*!< Array of rail names.
                                         *   These are power rails connected to 
                                         *   the sensor HW. 
                                         */
} sns_rail_config;

/**
 * @brief Sensors Power Rail Service type definitions.
 *
 */
typedef struct sns_pwr_rail_service
{
  sns_service service;                  /*!< Service information. */
  struct sns_pwr_rail_service_api *api; /*!< Public API provided by the Service to
                                         *   be used by the Sensor.
                                         */
} sns_pwr_rail_service;

/**
 * @brief Sensors Power Rail Service APIs.
 *
 */
typedef struct sns_pwr_rail_service_api
{
  size_t struct_len;  /*!< Length of sns_pwr_rail_service_api structure. */

  /**
   * @brief Register power rails for a physical sensor.
   * All physical sensors typically have one or more power rails
   * that supply power to the sensor hardware. Power rail
   * configuration for all sensors shall be defined in the
   * non-volatile registry. After the sensor driver fetches rail
   * information from the registry, it shall register all rails
   * using this API. Rail registration must happen only once for
   * any rail.
   *
   *  @param[in] service               Power rail service reference.
   *  @param[in] rail_config           Rail config being registered.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*sns_register_power_rails)(sns_pwr_rail_service *service,
                                     sns_rail_config const *rail_config);

  /**
   * @brief Vote for a power rail status change.
   * Each physical sensor driver uses this API to turn it's
   * sensors power rails ON when there is an active client
   * request. It is also the driver's responsibility to vote for
   * power rails OFF when all clients requests are disabled (i.e.
   * there are no Sensors Instances present).
   *
   * @param[in] service            Power rail service reference.
   * @param[in] sensor             Sensor requesting rail update.
   * @param[in] rails_config       Rails and their status change
   *                               vote.
   * @param[out] rail_on_timestamp Timestamp in ticks when the
   *                               rail was turned ON. When the
   *                               rails are being turned OFF, the
   *                               client can choose not to get
   *                               rail_on_timestamp information.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*sns_vote_power_rail_update)(sns_pwr_rail_service *service,
                                       struct sns_sensor const *sensor,
                                       sns_rail_config const *rails_config,
                                       sns_time *rail_on_timestamp);

} sns_pwr_rail_service_api;
