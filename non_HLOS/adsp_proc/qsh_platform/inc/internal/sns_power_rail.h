#pragma once
/** ============================================================================
 * @file
 *
 * @brief The APIs to vote for the sensor power rail
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its
 * subsidiaries. All rights reserved. Confidential and Proprietary -
 * Qualcomm Technologies, Inc.
 *
 *  ============================================================================
 */

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdint.h>
#include "sns_rc.h"

/*==============================================================================
  Macros
  ============================================================================*/

#define SNS_POWER_RAIL_CLIENT_NAME_LEN 32

/*==============================================================================
  Typedefs
  ============================================================================*/

/**
 *  Opaque handle to the sensors power rail utility interface
 *
 */
typedef struct sns_power_rail_handle sns_power_rail_handle;

/**
 * Supported sensors power rail modes
 *
 */
typedef enum sns_power_rail_mode
{
  SNS_POWER_RAIL_MODE_OFF = 0,
  SNS_POWER_RAIL_MODE_LPM = 1,
  SNS_POWER_RAIL_MODE_NPM = 2

} sns_power_rail_mode;

/**
 *  Client registration configuration structure
 *
 */
typedef struct sns_power_rail_config
{
  char rail_name[SNS_POWER_RAIL_CLIENT_NAME_LEN];

} sns_power_rail_config;

/**
 *  Client request configuration structure
 *
 */
typedef struct sns_power_rail_vote
{
  sns_power_rail_mode mode;

} sns_power_rail_vote;

typedef struct sns_rail_info
{
    char name[SNS_POWER_RAIL_CLIENT_NAME_LEN];
    uint32_t mV;
} sns_rail_info;

/*=============================================================================
  Public function declaration
  ============================================================================*/

/**
 * @brief The API to initialize Sensor power rail utility
 * @note: This API should be called only once after boot up by the framework.
 *
 * @par configurations
 *      None.
 *
 *
 * @retval SNS_RC_SUCCESS        Success
 * @retval SNS_RC_FAILED         Failed
 */

sns_rc sns_power_rail_init(void);

/* ---------------------------------------------------------------------------*/

/**
 * @brief The API to Register client for the sensors power rail voting.
 *
 * @param[in,out] handle        Address of the client handle
 * @param[in]     reg_config   Address of the client registration config struct
 *
 * @ret
 * @retval SNS_RC_SUCCESS        Success.
 * @retval SNS_RC_INVALID_VALUE  Invalid handle passed
 * @retval SNS_RC_FAILED         Handle registration failed
 */

sns_rc sns_power_rail_register(sns_power_rail_handle **handle,
                               sns_power_rail_config *reg_config);

/* ---------------------------------------------------------------------------*/
/**
 * @brief The API to de-register client handle
 *
 * @param[in] handle             Address of the registered client handle
 *
 * @retval SNS_RC_SUCCESS        Success. Handle will be set to NULL
 * @retval SNS_RC_INVALID_VALUE  Invalid handle passed
 * @retval SNS_RC_FAILED         Handle de-registration failed
 *
 */
sns_rc sns_power_rail_deregister(sns_power_rail_handle **handle);

/* ---------------------------------------------------------------------------*/
/**
 * @brief The API to vote for the sensors power rail
 *
 * @param[in] handle             Registered client handle
 * @param[in] req_config         Address of sensors power rail request
 *                               parameters struct
 *
 * @retval SNS_RC_SUCCESS        Request is processed successfully
 * @retval SNS_RC_INVALID_VALUE  Invalid handle passed
 * @retval SNS_RC_FAILED         Operation failed
 *
 */
sns_rc sns_power_rail_request(sns_power_rail_handle *handle,
                              sns_power_rail_vote *req_config);

/* ---------------------------------------------------------------------------*/
