#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Asynchronous Communication Port (ASCP) API to access COM
 * buses. This is an internal header file used by ASCP Sensor
 * and bus specific implementations.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include "sns_com_port_types.h"
#include "sns_rc.h"
#include "sns_sync_com_port_service.h"
#include "sns_time.h"

/**
 * @brief Com port version struct.
 *
 */
typedef struct sns_async_com_port_version
{
  uint16_t major; /*!< Major revision number */
  uint16_t minor; /*!< Minor revision number */
} sns_async_com_port_version;

/**
 * @brief Callback that will be called when an asynchronous read/write
 * operation is completed.
 *
 * @note Elements in vectors[] array are not of same size
 *
 * @param[in] port_handle      Port handle for the bus.
 * @param[in] num_vectors      Number of Vectors.
 * @param[in] vectors          Vector for read/write operation.
 * @param[in] xfer_bytes       Total bytes actually read/written.
 * @param[in] rw_err           Error code for the transaction.
 * @param[in] user_args        User defined parameter.
 *
 * @return
 *     - None.
 *
 */

typedef void (*sns_ascp_callback)(void *port_handle, uint8_t num_vectors,
                                  sns_port_vector vectors[],
                                  uint32_t xfer_bytes, sns_rc rw_err,
                                  void *user_args);

/**
 * @brief Structure containing all APIs provided by the Asynchronous COM port.
 *
 */
typedef struct sns_ascp_port_api
{

  size_t struct_len;

  /**
   * @brief The ASCP is a subclass of the SCP.The SCP API provides the utilities
   * to open and close a port and control bus power rails. The ASCP API
   * adds the abilities to register for a callback and provides the asynchronous
   * register read operation only.
   *
   */
  sns_sync_com_port_service_api *sync_com_port;

  /**
   * @brief Get the version of the Synchronous COM Port API.
   *
   * @param[out] version         Version of this API.
   *
   * @return
   * - SNS_RC_INVALID_VALUE:  Version parameter is NULL.
   * - SNS_RC_SUCCESS:        Action succeeded.
   *
   */
  sns_rc (*sns_ascp_get_version)(sns_sync_com_port_version *version);

  /**
   * @brief Register for a callback to be notified of a completed asynchronous
   * register read operation.
   *
   * @param[in] port_handle         Port handle for the bus.
   * @param[in] callback            Callback to be registered
   * @param[in] args                Callback function parameters
   *
   * @return
   *  - SNS_RC_SUCCESS: Action succeeded.
   *
   */

  sns_rc (*sns_ascp_register_callback)(void *port_handle,
                                       sns_ascp_callback callback, void *args);

  /**
   * @brief Read register asynchronusly.
   * This is a vectored API. It can be used for to read  a single register
   * @note This operation is valid only on a port that has been
   * successfully opened using sns_scp_open().
   *
   * @param[in] port_handle     Port handle for the bus.
   * @param[in] num_vectors     Number of vectors
   * @param[in] vectors         A single register read
   *                            operations.
   * @param[in] save_write_time True if time of the bus write
   *                            transaction must be saved. If
   *                            there are multiple write vectors
   *                            then this will only save the time
   *                            of the last write vector.
   *
   * @return
   *  - SNS_RC_FAILED:  Unable to transfer over bus.
   *  - SNS_RC_SUCCESS: Action succeeded
   *
   */

  sns_rc (*sns_ascp_register_rw)(void *port_handle, uint8_t num_vectors,
                                 sns_port_vector vectors[],
                                 bool save_write_time);

  /**
   * @brief Gets the timestamp for most recent write operation. This API must
   * be called only when save_write_time is 'true' for the most recent write
   * operation on the bus. It is typically used for S4S mode of operation to
   * get the start time when the ST command is sent to the physical sensor. For
   * I2C and I3C, the timestamp is when the START condition occurs for the
   * write transfer. For SPI, the timestamp is an instance during the write
   * transfer. This operation is valid only on a port that has been successfully
   * opened using sns_ascp_open().
   * @note
   * The returned write_time is in units of System Ticks * 32.
   * This value should be divided by 32 when comparing to the
   * current system time.
   *
   * @param[in] port_handle    Port handle for the bus.
   * @param[out] write_time    System timestamp in (ticks * 32).
   *
   * @return
   *  - SNS_RC_FAILED:   Unable to get write time.
   *  - SNS_RC_SUCCESS:  Action succeeded.
   *
   */

  sns_rc (*sns_ascp_get_write_time)(void *port_handle, sns_time *write_time);

} sns_ascp_port_api;
