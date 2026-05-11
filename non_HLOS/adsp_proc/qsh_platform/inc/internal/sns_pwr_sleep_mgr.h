#pragma once
/** ============================================================================
 * @file
 *
 * @brief Sensors Power Sleep Manager.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
=============================================================================**/

/*******************************************************************************
                               Includes
*******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "sns_rc.h"
#include "sns_sensor_event.h"
#include "sns_sensor_instance.h"

/*******************************************************************************
                                  Constants & Macros
*******************************************************************************/

/*******************************************************************************
                                  Type Definitions
*******************************************************************************/

/*******************************************************************************
                                  Public Functions
*******************************************************************************/

/**
 * @brief Initialize the sensors power sleep manager. Create handles to the
 * sleep driver NPA nodes.
 *
 * @return
 *   -  SNS_RC_SUCCESS:  Operation successful.
 *
 */
sns_rc sns_pwr_sleep_mgr_init(void);

/**
 * @brief Check for physical sensor config events. For physical sensor config
 * events add instance to the power sleep manager instance list.
 *
 * @param[in] sensor_uid           Sensor uid.
 * @param[in] instance             Sensor instance.
 * @param[in] event                Sensor event.
 * @param[in] is_physical_sensor   True if sensor supports sns_std_sensor.proto
 *                                 False otherwise.
 *
 * @return
 *  - SNS_RC_SUCCESS:  Operation successful.
 *  - SNS_RC_FAILED:   Event decode failed.
 *
 */
sns_rc sns_pwr_sleep_mgr_check_config_event(sns_sensor_uid const *sensor_uid,
                                            sns_sensor_instance *instance,
                                            sns_sensor_event const *event,
                                            bool is_physical_sensor);
/**
 * @brief Remove configuration entries from the power sleep manager instance
 * list.Remove the instance wakeup rate from the aggregated Q6 wakeup rate.
 * Update the sleep driver with the new Q6 wakeup rate.
 *
 * @param[in] sensor_uid            Sensor uid.
 * @param[in] instance              The instance address of the sensor.
 *
 * @return
 *  - SNS_RC_SUCCESS:  Operation successful.
 *  - SNS_RC_FAILED:   Remove failed as SUID did not match.
 *
 */
sns_rc sns_pwr_sleep_mgr_remove_config(sns_sensor_uid const *sensor_uid,
                                       sns_sensor_instance *instance);

/**
 * @brief Set/reset island mode blocked flag in power sleep manager.
 *
 * @param[in] is_island_blocked  True if island is blocked, False otherwise.
 *
 * @return
 *  - None.
 *
 */
void sns_pwr_sleep_mgr_set_island_mode_blocked(bool is_island_blocked);
