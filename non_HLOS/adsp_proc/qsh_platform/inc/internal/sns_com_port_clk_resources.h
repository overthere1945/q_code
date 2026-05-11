#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Manages bus clock resources.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/
/**
*******************************************************************************
                               Includes
*******************************************************************************
*/
#include <stdbool.h>
#include <stdint.h>

#include "sns_com_port_types.h"
#include "sns_rc.h"
/**
*******************************************************************************
                                  Public Functions
********************************************************************************
*/

/**
 * @brief Enables clock resources to operate the I2C bus at the requested speed.
 * This must be called prior to any bus operations (like opening a bus).
 * If the clock resources have to change, this may exit island mode.
 * This function is reference counted per bus_instance -- release_clk_resources
 * must be called an equal number of times for this clock resources
 * to be released.
 *
 * @param[in] qup_type            SSC QUP or TOP level QUP.
 * @param[in] qup_instance        QUP instance number.
 * @param[in] bus_type            I2C or I3C.
 * @param[in] bus_instance        As defined by the core APIs.
 * @param[in] new_bus_speed_KHz   The bus speed for future bus operations.
 * @param[out] prev_bus_speed_KHz The bus speed previously requested (or 0).
 *
 * @return
 *  - SNS_RC_FAILED:   Internal failure.
 *  - SNS_RC_SUCCESS:  Action succeeded.
 *
 */
sns_rc sns_i2c_setup_clk_resources(sns_qup_type qup_type, uint8_t qup_instance,
                                   sns_bus_type bus_type, uint8_t bus_instance,
                                   int32_t new_bus_speed_KHz,
                                   int32_t *prev_bus_speed_KHz);

/**
 *  Enables clock resources to operate the SPI bus at the requested speed.
 *
 *  This must be called prior to any bus operations (like opening a bus).
 *  If the clock resources have to change, this may exit island mode.
 *  This function is reference counted per bus_instance -- release_clk_resources
 *  must be called an equal number of times for this clock resources
 *  to be released.
 *
 *  @param[in] qup_type            SSC QUP or TOP level QUP.
 *  @param[in] qup_instance        qup instance no.
 *  @param[in] bus_instance        As defined by the core APIs.
 *  @param[in] new_bus_speed_KHz   The bus speed for future bus operations.
 *  @param[out] prev_bus_speed_KHz The bus speed previously requested (or 0).
 *
 *  @return
 *  - SNS_RC_FAILED:  Internal failure.
 *  - SNS_RC_SUCCESS: Action succeeded.
 */
sns_rc sns_spi_setup_clk_resources(sns_qup_type qup_type, uint8_t qup_instance,
                                   uint8_t bus_instance,
                                   int32_t new_bus_speed_KHz,
                                   int32_t *prev_bus_speed_KHz);
								   
/**
 * @brief Releases I2C bus clock resources.
 * This will decrement the reference counter for clock resources. When the
 * counter reaches 0, it will vote to turn off bus clocks.
 * Note that releasing the clk resources will not "go back" to a previous
 * bus speed.
 *
 * @param[in] qup_type          SSC QUP or TOP level QUP.
 * @param[in] qup_instance      QUP instance number.
 * @param[in] bus_instance      As defined by the core APIs.
 *
 * @return
 *   - SNS_RC_FAILED:    Internal failure.
 *   - SNS_RC_SUCCESS:   Action succeeded.
 *
 */
sns_rc sns_i2c_release_clk_resources(sns_qup_type qup_type,
                                     uint8_t qup_instance,
                                     uint8_t bus_instance);
/**
 *  Releases SPI bus clock resources.
 *
 *  This will decrement the reference counter for clock resources. When the
 *  counter reaches 0, it will vote to turn off bus clocks.
 *  Note that releasing the clk resources will not "go back" to a previous
 *  bus speed.
 *
 *  @param[in] qup_type         SSC QUP or TOP level QUP.
 *  @param[in] qup_instance     qup instance no.
 *  @param[in] bus_instance     As defined by the core APIs.
 *
 *  @return
 *  - SNS_RC_FAILED:   Internal failure.
 *  - SNS_RC_SUCCESS:  Action succeeded.
 */
sns_rc sns_spi_release_clk_resources(sns_qup_type qup_type,
                                     uint8_t qup_instance,
                                     uint8_t bus_instance);