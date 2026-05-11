#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Internal definitions for spi_port_api and spi_async_port_api.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include "sns_async_com_port_int.h"
#include "sns_com_port_types.h"
#include "sns_sync_com_port_service.h"

/*=============================================================================
  Type Definitions
  ============================================================================*/

/*=============================================================================
  Public function declaration for ( spi_port_api functions )
  ============================================================================*/

/**
 * @brief The API to initialize a instance for spi communication.
 *
 * @param[in] port_handle handle to sns_sync_com_port_handle.
 *
 * @return
 * - SNS_RC_SUCCESS:        Initialization was successful.
 * - SNS_RC_NOT_SUPPORTED:  Not Supported on platform.
 * - SNS_RC_FAIL:           Otherwise.
 *
 */
sns_rc sns_open_spi(sns_sync_com_port_handle *port_handle);

/**
 * @brief The API to deinitialize a instance for spi communication.
 *
 * @param[in] port_handle handle to sns_sync_com_port_handle.
 *
 * @return
 * - SNS_RC_SUCCESS:        Deinitialization was successful.
 * - SNS_RC_NOT_SUPPORTED:  Not Supported on platform.
 * - SNS_RC_FAIL:           Otherwise.
 *
 */
sns_rc sns_close_spi(sns_sync_com_port_handle *port_handle);

/**
 * @brief The API to handle the enabling and disabling of resources required for
 * spi communication.
 *
 * @param[in] port_handle  handle to sns_sync_com_port_handle.
 * @param[in] power_bus_on boolean for toggling state of bus power.
 *
 * @return
 * - SNS_RC_SUCCESS:        Operation was successful.
 * - SNS_RC_NOT_SUPPORTED:  Not Supported on platform.
 * - SNS_RC_FAIL:           Otherwise.
 *
 */
sns_rc sns_pwr_update_spi(sns_sync_com_port_handle *port_handle,
                          bool power_bus_on);

/**
 * @brief The API to make a vectored read / write call over spi.
 *
 * @param[in] port_handle     Handle to sns_sync_com_port_handle.
 * @param[in] vectors         (Array) Metadata about the transaction taking
 *                            place on bus.
 * @param[in] num_vectors     Number of vectored read and or writes for the
 *                            operation.
 * @param[in] save_write_time Boolean that specifies if write time should be
 *                            recored.
 * @param[out] xfer_bytes     Total bytes actually read / written.
 *
 * @return
 * - SNS_RC_SUCCESS:        Operation was successful.
 * - SNS_RC_NOT_SUPPORTED:  Not Supported on platform.
 * - SNS_RC_FAIL:           Otherwise.
 *
 */
sns_rc sns_vectored_rw_spi(sns_sync_com_port_handle *port_handle,
                           sns_port_vector *vectors, int32_t num_vectors,
                           bool save_write_time, uint32_t *xfer_bytes);

/**
 * @brief The API to make a vectored read / write call over spi.
 *
 * @param[in] port_handle     Handle to sns_sync_com_port_handle.
 * @param[in] com_port_ex     Stores com port configuration (extended).
 * @param[in] vectors         (Array) Metadata about the transaction taking
 *                            place on bus.
 * @param[in] num_vectors     Number of vectored read and or writes for the
 *                            operation.
 * @param[in] save_write_time Boolean that specifies if write time should be
 *                            recored.
 * @param[out] xfer_bytes     Total bytes actually read / written.
 * @param[in]  async          Specify if async.
 *
 * @return
 * - SNS_RC_SUCCESS:        Operation was successful.
 * - SNS_RC_NOT_SUPPORTED:  Not Supported on platform.
 * - SNS_RC_FAIL:           Otherwise.
 *
 */
sns_rc sns_vectored_rw_spi_ex(sns_sync_com_port_handle *port_handle,
                              sns_com_port_config_ex *com_port_ex,
                              sns_port_vector *vectors, int32_t num_vectors,
                              bool save_write_time, uint32_t *xfer_bytes,
                              bool async);

/**
 * @brief The API to make a simple write transfer over spi.
 *
 * @param[in] port_handle     Handle to sns_sync_com_port_handle.
 * @param[in] is_write        Boolean that specifies if this is a write
 *                            operation.
 * @param[in] save_write_time Boolean that specifies if write time should be
 *                            recored.
 * @param[in] buffer          Buffer for data to be written or read in.
 * @param[in] bytes           Size of buffer.
 * @param[out] xfer_bytes     Total bytes actually read / written.
 *
 * @return
 * - SNS_RC_SUCCESS:        Write was successful.
 * - SNS_RC_NOT_SUPPORTED:  Not Supported on platform.
 * - SNS_RC_FAIL:           Otherwise.
 *
 */
sns_rc sns_simple_txfr_port_spi(sns_sync_com_port_handle *port_handle,
                                bool is_write, bool save_write_time,
                                uint8_t *buffer, uint32_t bytes,
                                uint32_t *xfer_bytes);

/**
 * @brief The API to get the time taken for the last spi write.
 *
 * @param[in] port_handle  Handle to sns_sync_com_port_handle.
 * @param[out] write_time  Contains time taken for transaction.
 *
 * @return
 * - SNS_RC_SUCCESS:        Getting time was successful.
 * - SNS_RC_NOT_SUPPORTED:  Not Supported on platform.
 * - SNS_RC_FAIL:           Otherwise.
 *
 */
sns_rc sns_get_write_time_spi(sns_sync_com_port_handle *port_handle,
                              sns_time *write_time);

/*=============================================================================
  Public function declaration for ( spi_async_port_api functions )
  ============================================================================*/

/**
 * @brief The API to register a callback for spi_async_port_api.
 *
 * @param[in] port_handle  Handle to sns_sync_com_port_handle.
 * @param[in] callback     Function callback assignment.
 * @param[in] args         Arguments for callback.
 *
 * @return
 * - SNS_RC_SUCCESS:        Operation was successful.
 * - SNS_RC_NOT_SUPPORTED:  Not Supported on platform.
 * - SNS_RC_FAIL:           Otherwise.
 *
 */
sns_rc sns_ascp_register_spi_callback(void *port_handle,
                                      sns_ascp_callback callback, void *args);

/**
 * @brief The API to make a vectored read / write call over spi asynchronously.
 *
 * @param[in] port_handle      Handle to sns_sync_com_port_handle.
 * @param[in] num_vectors      Number of vectored read and or writes for the
 *                             operation.
 * @param[in] vectors          (Array) Metadata about the transaction taking
 *                             place on bus.
 * @param[in] save_write_time  Boolean that specifies if write time should be
 *                             recored.
 *
 * @return
 * - SNS_RC_SUCCESS:        Operation was successful.
 * - SNS_RC_NOT_SUPPORTED:  Not Supported on platform.
 * - SNS_RC_FAIL:           Otherwise.
 *
 */
sns_rc sns_async_vectored_rw_spi(void *port_handle, uint8_t num_vectors,
                                 sns_port_vector vectors[],
                                 bool save_write_time);