#pragma once
/** ============================================================================
 * @file
 *
 * @brief Provides a synchronous API to read/write from/to output GPIO
 * pins.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "sns_gpio_types.h"
#include "sns_rc.h"
#include "sns_service.h"

/*==============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief The GPIO service. Can be obtained from
 * sns_service_manager::get_service.
 *
 */
typedef struct sns_gpio_service
{
  sns_service service;                    /*!< Service information. */
  const struct sns_gpio_service_api *api; /*!< Public API provided by the
                                           * Service to be used by the Sensor.
                                           */
} sns_gpio_service;

/**
 * @brief API made available to Sensors.
 *
 */
typedef struct sns_gpio_service_api
{
  uint32_t struct_len; /*!< Length of sns_gpio_service_api structure. */

  /**
   * @brief Read current state of a general purpose input pin.
   *
   * @param[in]   gpio          GPIO pin to read.
   * @param[in]   is_chip_pin   True if GPIO is chip level; false otherwise
   * @param[out]  state         Current gpio state.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*read_gpio)(uint32_t gpio, bool is_chip_pin, sns_gpio_state *state);

  /**
   * @brief Write a value to a general purpose output pin.
   *
   * @param[in] gpio            GPIO pin to write to.
   * @param[in] is_chip_pin     True if GPIO is chip level; false otherwise
   * @param[in] drive_strength  Drive strength config.
   * @param[in] pull            Pull type config.
   * @param[in] state           Output state to set for the gpio.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*write_gpio)(uint32_t gpio, bool is_chip_pin,
                       sns_gpio_drive_strength drive_strength,
                       sns_gpio_pull_type pull, sns_gpio_state state);

} sns_gpio_service_api;
