#pragma once
/** ============================================================================
 * @file
 *
 * @brief The APIs to set system sleep attributes
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 *  ==========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdint.h>
#include "sns_rc.h"

/*==============================================================================
  Macros
  ============================================================================*/

#define SNS_SLEEP_CLIENT_NAME_LEN 32

/*==============================================================================
  Typedefs
  ============================================================================*/

/**
 *  @brief Opaque handle to the Sleep utility interface
 *
 */
typedef struct sns_sleep_handle sns_sleep_handle;

/**
 *  @brief The supported low power modes in island
 *
 */
typedef enum sns_sleep_mode
{
  SNS_SLEEP_MODE_UNSUPPORTED = 0, /*!< Unsupported sleep mode */
  SNS_SLEEP_MODE_ENABLE = 1,      /*!< Enable Low power sleep modes */
  SNS_SLEEP_MODE_DISABLE = 2,     /*!< Disable Low power sleep modes */

} sns_sleep_mode;

/**
 *  @brief The structure for island low mode attribute information
 *
 */
typedef struct sns_island_lpm_info_t
{
  sns_sleep_mode island_lpm; /*!< Enable/Disable Island Low power sleep modes */

} sns_island_lpm_info_t;

/**
 *  @brief The structure for TCM power collapse attribute information
 *
 */
typedef struct sns_tcm_lpm_info_t
{
  sns_sleep_mode tcm_pc;   /*!< Enable/Disable TCM Power collapse */

} sns_tcm_lpm_info_t;

/**
 *  @brief The structure for system wakeup interval attribute information
 *
 */
typedef struct sns_wakeup_interval_info_t
{
  int32_t wakeup_interval_us;  /*!< System wakeup interval in us */

} sns_wakeup_interval_info_t;

typedef enum sns_sleep_attr
{
  SNS_SLEEP_ATTR_UNSUPPORTED = 0, 
  /*!<  Island LPM: refer sns_island_lpm_info_t */
  SNS_SLEEP_ATTR_ISLAND_LPM = 1,
  /*!<  TCM power collapse mode: refer sns_tcm_lpm_info_t  */
  SNS_SLEEP_ATTR_TCM_LPM = 2,
  /*!<  System wakeup interval: refer sns_wakeup_interval_info_t  */
  SNS_SLEEP_ATTR_WAKUP_INTERVAL = 3,
  SNS_SLEEP_ATTR_MAX,

} sns_sleep_attr;

/**
 *  @brief Client register parameters structure
 *
 */
typedef struct sns_sleep_register_param
{
  char client_name[SNS_SLEEP_CLIENT_NAME_LEN]; /*!<  Client name  */
  sns_sleep_attr attr;

} sns_sleep_register_param;

/**
 *  @brief Client request parameters structure
 *
 */
typedef struct sns_sleep_request_param
{
  void *attr_info;                   /*!<  Sleep Attribute info struct  */

} sns_sleep_request_param;

/*=============================================================================
  Public function declaration
  ============================================================================*/

/**
 * @brief The API to Register client for setting the sleep attribute.
 *
 * @param[in]     reg_param   Address of the client registration structure.
 * @param[in,out] handle      Address of the client handle.
 *
 * @return
 * - SNS_RC_SUCCESS           Client registration is successful.
 * - SNS_RC_INVALID_VALUE:    Invalid registration parameters passed.
 * - SNS_RC_FAILED:           Client registration failed.
 *
 */

sns_rc sns_sleep_register(sns_sleep_register_param *reg_param,
                          sns_sleep_handle **handle);

/* ---------------------------------------------------------------------------*/
/**
 * @brief The API to de-register client handle.
 *
 * @param[in] handle             Address of the registered client handle.
 *
 * @return
 * - SNS_RC_SUCCESS:             Client de-registration is successful.
 * - SNS_RC_INVALID_VALUE:       Invalid handle passed.
 * - SNS_RC_FAILED:              Client de-registration failed.
 *
 */
sns_rc sns_sleep_deregister(sns_sleep_handle **handle);

/* ---------------------------------------------------------------------------*/
/**
 * @brief The API to set sleep attributes value.
 *
 * @param[in] handle             Registered client handle.
 * @param[in] req_param          Address request parameter structure
 *
 * @return
 * - SNS_RC_SUCCESS:             Request is processed successfully.
 * - SNS_RC_INVALID_VALUE:       Invalid request parameters passed.
 * - SNS_RC_FAILED:              Request failed.
 *
 */
sns_rc sns_sleep_request(sns_sleep_handle *handle,
                         sns_sleep_request_param *req_param);

/* ---------------------------------------------------------------------------*/
