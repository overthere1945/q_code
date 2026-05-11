#pragma once
/** ============================================================================
 * @file
 *
 * @brief GPIO utility APIs
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Includes
  ===========================================================================*/

#include <stdint.h>
#include <stdbool.h>
#include "sns_interrupt.pb.h"
#include "sns_gpio_types.h"
#include "sns_rc.h"

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief GPIO read configuration
 *
 */
typedef struct sns_gpio_read_config
{
  uint32_t gpio;
  bool is_chip_pin;
} sns_gpio_read_config;

/**
 * @brief GPIO write configuration
 *
 */
typedef struct sns_gpio_write_config
{
  uint32_t gpio;
  bool is_chip_pin;
  sns_gpio_drive_strength drive_strength;
  sns_gpio_pull_type pull;
  sns_gpio_state state;
} sns_gpio_write_config;

/**
 * @brief GPIO interrupt configuration
 * TODO : Move this to pal/interrupt
 *
 */
typedef struct sns_gpio_interrupt_config
{
  sns_interrupt_req const *req;
  bool enable;
  bool set_inactive_cfg;
} sns_gpio_interrupt_config;

/*=============================================================================
  Public Function Declarations
  ============================================================================*/

/**
 * @brief The API to initialize GPIO utility
 * @note: This API should be called only once after boot up by the framework.
 *
 * @par Parameters
 *      None.
 *
 * @return
 * - SNS_RC_SUCCESS        GPIO utility init successful
 * - SNS_RC_FAILED         GPIO utility init failed
 */
sns_rc sns_gpio_init(void);

/**
 * @brief The API to read current state of a general purpose input pin
 *
 * @param[in]   config     Address of the GPIO read configuration
 * @param[out]  state      Current state of the GPIO pin.
 *
 * @return
 * - SNS_RC_SUCCESS        GPIO read successful
 * - SNS_RC_FAILED         GPIO read failed
 */
sns_rc sns_gpio_read(sns_gpio_read_config *config, sns_gpio_state *state);

/**
 * @brief The API to write a value to a general purpose output pin
 *
 * @param[in]   config     Address of the GPIO write configuration
 *
 * @return
 * - SNS_RC_SUCCESS        GPIO write successful
 * - SNS_RC_FAILED         GPIO write failed
 */
sns_rc sns_gpio_write(sns_gpio_write_config *config);

/**
 * @brief The API to configure GPIO for interrupt operation
 * TODO : Move this to pal/interrupt
 *
 * @param[in]   config     Address of the GPIO interrupt configuration
 *
 * @return
 * - SNS_RC_SUCCESS        GPIO interrupt config successful
 * - SNS_RC_FAILED         GPIO interrupt config failed
 */
sns_rc sns_gpio_configure_interrupt(sns_gpio_interrupt_config *config);
