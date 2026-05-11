#pragma once
/** ============================================================================
 * @file
 *
 * @brief The APIs to vote for the clocks
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

#define SNS_CLOCK_CLIENT_NAME_LEN 32

/*==============================================================================
  Typedefs
  ============================================================================*/

/**
 *  @brief Opaque handle to the clock utility interface
 *
 */
typedef struct sns_clock_handle sns_clock_handle;

/**
 *  @brief Supported master port IDs
 *
 */
typedef enum sns_clock_id
{
  SNS_CLOCK_ID_NONE = 0,
  SNS_CLOCK_ID_PRIMARY_CPU = 1,   /*!<  Primary CPU clock ID  */
  SNS_CLOCK_ID_SECONDARY_CPU = 2, /*!<  Secondary CPU clock ID  */
  SNS_CLOCK_ID_CCD = 3,           /*!<  CCD clock ID  */
  SNS_CLOCK_ID_PRAM = 4           /*!<  PRAM clock ID  */

} sns_clock_id;

/**
 *  @brief Supported Clock API types
 *
 */
typedef enum sns_clock_api_type
{
  SNS_CLOCK_API_TYPE_NONE = 0,
  SNS_CLOCK_API_TYPE_SYNC = 1,   /*!<  Synchronous API  */
  SNS_CLOCK_API_TYPE_ASYNC = 2   /*!<  Asynchronous API   */  

} sns_clock_api_type;

/**
 *  @brief Client registration parameters structure
 *
 */
typedef struct sns_clock_register_param
{
  char client_name[SNS_CLOCK_CLIENT_NAME_LEN]; /*!<  Client API */
  sns_clock_id clock_id;                       /*!<  Clock ID  */

} sns_clock_register_param;

/**
 *  @brief Client request parameters structure
 *
 */
typedef struct sns_clock_req_param
{
  sns_clock_api_type api_type;                /*!<  API type  */
  uint32_t clock_hz;                          /*!<  Clock value in Hz  */

} sns_clock_req_param;

/*=============================================================================
  Public function declaration
  ============================================================================*/

/**
 * @brief The API to initialize Sensor Clock utility
 * @note: This API should be called only once after boot up by the framework.
 *
 * @par Parameters
 *      None.
 *
 * @return
 * - SNS_RC_SUCCESS        Success
 * - SNS_RC_FAILED         Failed
 *
 */

sns_rc sns_clock_init(void);

/* ---------------------------------------------------------------------------*/

/**
 * @brief The API to Register client for the clock voting.
 *
 * @param[in]     reg_param     Address of the client registration params struct
 * @param[in,out] handle        Address of the client handle
 *
 * @return
 * - SNS_RC_SUCCESS             Success.
 * - SNS_RC_INVALID_VALUE       Invalid handle passed.
 * - SNS_RC_FAILED              Handle registration failed.
 *
 */

sns_rc sns_clock_register(sns_clock_register_param *reg_param,
                          sns_clock_handle **handle);

/* ---------------------------------------------------------------------------*/
/**
 * @brief The API to de-register client handle
 *
 * @param[in] handle            Address of the registered client handle
 *
 * @return
 * - SNS_RC_SUCCESS             Success. Handle will be set to NULL.
 * - SNS_RC_INVALID_VALUE       Invalid handle passed.
 * - SNS_RC_FAILED              Client de-registration failed.
 *
 */
sns_rc sns_clock_deregister(sns_clock_handle **handle);

/* ---------------------------------------------------------------------------*/
/**
 * @brief The API to vote for the clock
 *
 * @param[in] handle            Registered client handle
 * @param[in] req_param         Address of clock request parameters
 * struct
 *
 * @return
 * - SNS_RC_SUCCESS             Request is processed successfully.
 * - SNS_RC_INVALID_VALUE       Invalid handle passed.
 * - SNS_RC_FAILED              Request failed.
 *
 */
sns_rc sns_clock_request(sns_clock_handle *handle,
                         sns_clock_req_param *req_param);

/* ---------------------------------------------------------------------------*/
