#pragma once
/** ============================================================================
 * @file
 *
 * @brief The APIs to vote for the bus bandwidth.
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

#define SNS_BUS_BW_CLIENT_NAME_LEN 32

/*==============================================================================
  Typedefs
  ============================================================================*/

/**
 * @brief Opaque handle to the bus_bandwidth utility interface.
 *
 */
typedef struct sns_bus_bw_handle sns_bus_bw_handle;

/**
 * @brief Supported master port IDs.
 *
 */
typedef enum sns_bus_bw_master_port_id
{
  SNS_BUS_BW_MASTER_PORT_ID_NONE = 0,
  SNS_BUS_BW_MASTER_PORT_ID_PRIMARY = 1,  /*!< Primary core Master port id */ 
  SNS_BUS_BW_MASTER_PORT_ID_SECONDARY = 2 /*!< Secondary core Master port id */

} sns_bus_bw_master_port_id;

/**
 * @brief Supported slave port IDs.
 *
 */
typedef enum sns_bus_bw_slave_port_id
{
  SNS_BUS_BW_SLAVE_PORT_ID_NONE = 0,
  SNS_BUS_BW_SLAVE_PORT_ID_DDR = 1,  /*!< Slave port id for DDR Memory */
  SNS_BUS_BW_SLAVE_PORT_ID_SRAM = 2, /*!< Slave port id for SRAM Memory */ 
  SNS_BUS_BW_SLAVE_PORT_ID_TCM = 3   /*!< Slave port id for TCM Memory */ 

} sns_bus_bw_slave_port_id;

/**
 * @brief Client registration parameters structure.
 *
 */
typedef struct sns_bus_bw_register_param
{
  char client_name[SNS_BUS_BW_CLIENT_NAME_LEN]; /*!< Client name */
  sns_bus_bw_master_port_id master_port;        /*!< Bus master port */
  sns_bus_bw_slave_port_id slave_port;          /*!< Bus slave port */

} sns_bus_bw_register_param;

/**
 *  @brief Client request parameters structure
 *
 */
typedef struct sns_bus_bw_req_param
{
  uint64_t bw;        /*!< Bandwidth in bytes per sec. */
  uint16_t usage_pct; /*!< Bandwidth usage in percentage. */

} sns_bus_bw_req_param;

/*=============================================================================
  Public function declaration
  ============================================================================*/

/**
 * @brief The API to Register client for the bus bandwidth voting.
 *
 * @param[in]     reg_param   Address of the client registration params struct.
 * @param[in,out] handle      Address of the client handle.
 *
 * @return
 * - SNS_RC_SUCCESS:          Client registration is successful.
 * - SNS_RC_INVALID_VALUE:    Invalid registration parameters passed.
 * - SNS_RC_FAILED:           Client registration failed.
 *
 */

sns_rc sns_bus_bw_register(sns_bus_bw_register_param *reg_param,
                           sns_bus_bw_handle **handle);

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
sns_rc sns_bus_bw_deregister(sns_bus_bw_handle **handle);

/* ---------------------------------------------------------------------------*/
/**
 * @brief The API to vote for the bus bandwidth.
 *
 * @param[in] handle             Registered client handle.
 * @param[in] req_param          Address of bandwidth request parameters struct.
 *
 * @return
 * - SNS_RC_SUCCESS:             Request is processed successfully.
 * - SNS_RC_INVALID_VALUE:       Invalid request parameters passed.
 * - SNS_RC_FAILED:              Request failed.
 *
 */
sns_rc sns_bus_bw_request(sns_bus_bw_handle *handle,
                          sns_bus_bw_req_param *req_param);

/* ---------------------------------------------------------------------------*/
