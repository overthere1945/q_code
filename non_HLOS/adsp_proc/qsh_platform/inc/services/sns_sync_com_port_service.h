#pragma once
/** =============================================================================
 * @file
 *
 * @brief Manages sync com port service.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdint.h>
#include <stdlib.h>
#include "sns_com_port_types.h"
#include "sns_rc.h"
#include "sns_sensor_uid.h"
#include "sns_service.h"
#include "sns_time.h"

/*=============================================================================
  Macros
  ===========================================================================*/

#define COM_PORT_ENABLE_DEBUG

#ifdef COM_PORT_ENABLE_DEBUG
/**
 * @brief Printf() like function for logging purposes.
 *
 */
#define COM_PORT_PRINTF(prio, sensor, ...)                                     \
  do                                                                           \
  {                                                                            \
    SNS_PRINTF(prio, sensor, __VA_ARGS__);                                     \
  } while(0)
#else
#define COM_PORT_PRINTF(prio, sensor, ...)
#endif

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_sensor;
struct sns_sensor_instance;

/*=============================================================================
  Type Definitions
  ===========================================================================*/

typedef void *sns_sync_com_port_handle;

/**
 * @brief The Sync Com Port Service Manager.  Will be obtained from
 * sns_service_manager::get_service.
 *
 */
typedef struct sns_sync_com_port_service
{
  sns_service service;                             /*!< Service information. */
  struct sns_sync_com_port_service_api const *api; /*!< Public api provided by
                                                    * the framework to be used
                                                    * by the sensor.
                                                    */
} sns_sync_com_port_service;

/**
 * @brief Com port version struct.
 *
 */
typedef struct sns_sync_com_port_version
{
  uint16_t major; /*!< Major version number. */
  uint16_t minor; /*!< Minor version number. */
} sns_sync_com_port_version;

/**
 * @brief The Sync Com Port Service APIs.
 *
 */
typedef struct sns_sync_com_port_service_api
{

  size_t struct_len; /*!< Length of sns_sync_com_port_service_api structure. */

  /**
   * @brief Registers a bus COM port with the Synchronous COM Port (SCP)
   * utility.
   * Each physical sensor shall register with the SCP before any
   * SCP API can be used. Note that this does not open a COM port.
   *
   *  @param[in] com_config    COM port config for the bus.
   *  @param[out] port_handle  Port handle for the bus.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  Input parameters are invalid.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_register_com_port)(sns_com_port_config const *com_config,
                                      sns_sync_com_port_handle **port_handle);

  /**
   * @brief Deregisters a bus COM port with the Synchronous COM Port (SCP)
   * utility and sets the port handle to NULL.
   * This will power off and close the com port, if powered on
   * or open.
   *
   *  @param[in] port_handle   Reference to the port handle.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  Passed in port handle is invalid.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_deregister_com_port)(sns_sync_com_port_handle **port_handle);

  /**
   * @brief Get the version of the Synchronous COM Port API.
   *
   * @param[out] version      Version of this API.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  version parameter is NULL.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_get_version)(sns_sync_com_port_version *version);

  /**
   * @brief Opens a new COM port with bus configuration com_config.
   * The bus is powered ON after this function returns
   * successfully. Each physical sensor driver that needs access
   * to a communication bus (I2C/SPI/I3C/UART) shall use this API
   * typically in initialization routine. The physical sensor
   * driver must save port handle for future use of the bus.
   *
   *  @param[in] port_handle   Port handle for the bus.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  requested bus and/or port handle are invalid.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_open)(sns_sync_com_port_handle *port_handle);

  /**
   * @brief Closes and powers off the COM port.
   * The physical sensor driver would typically close the port when
   * it's being un-installed or unloaded. This operation is valid
   * only on a port that has been successfully opened using
   * sns_scp_open(). Once a bus is closed, it shall not be
   * accessed by the driver.
   *
   *  @param[in] port_handle   Port handle for the bus.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  passed in port handle is invalid.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_close)(sns_sync_com_port_handle *port_handle);

  /**
   * @brief Updates bus power status to ON or OFF.
   * Typically, a physical sensor driver shall turn bus power ON
   * before initiating a series of COM transfers. It shall turn
   * bus power off after to save power. A COM port cannot be used
   * unless it is in power ON state. It is required for all device
   * drivers to turn the bus OFF when not in use.
   * This operation is valid only on a port that has been
   * successfully opened using sns_scp_open().
   *
   *  @param[in] port_handle    Port handle for the bus.
   *  @param[in] power_bus_on   true to power ON.
   *                            false to power OFF.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  passed in port handle is invalid.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_update_bus_power)(sns_sync_com_port_handle *port_handle,
                                     bool power_bus_on);

  /**
   * @brief Read and/or write multiple registers.
   * This is a vectored API. It can be used for:
   * 1) Read single/multiple registers in the same
   *    transfer.
   * 2) Write to single/multiple register in same transfer.
   * 3) Write to and read from single/multiple registers in same
   *    transfer.
   * This operation is valid only on a port that has been
   * successfully opened using sns_scp_open().
   *
   *  @param[in] port_handle      Port handle for the bus.
   *  @param[in] vectors          An array of register read/write operations.
   *  @param[in] num_vectors      Number of elements in vectors array.
   *  @param[in] save_write_time  True if time of the bus write transaction must
   *                              be saved. If there are multiple write vectors
   *                              then this will only save the time of the last
   *                              write vector.
   *  @param[out] xfer_bytes      Total number of bytes read & written for all
   *                              registers in this vectored transfer. May be
   *                              NULL.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  passed in port handle is invalid.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_register_rw)(sns_sync_com_port_handle *port_handle,
                                sns_port_vector *vectors, int32_t num_vectors,
                                bool save_write_time, uint32_t *xfer_bytes);

  /**
   * @brief Read and/or write multiple registers.
   * This is a vectored API. It can be used for:
   * 1) Read single/multiple registers in the same
   *    transfer.
   * 2) Write to single/multiple register in same transfer.
   * 3) Write to and read from single/multiple registers in same
   *    transfer.
   * 4) This is the updated API which also handles the advanced COM Port
   *    configuration parameters in sns_com_port_config_ex.
   * This operation is valid only on a port that has been
   * successfully opened using sns_scp_open().
   *
   *  @param[in] port_handle      Port handle for the bus.
   *  @param[in] com_port_ex      Structure with Extended COM Port Parameters.
   *  @param[in] vectors          An array of register read/write operations.
   *  @param[in] num_vectors      Number of elements in vectors array.
   *  @param[in] save_write_time  True if time of the bus write transaction must
   *                              be saved. If there are multiple write vectors
   *                              then this will only save the time of the last
   *                              write vector.
   *  @param[out] xfer_bytes      Total number of bytes read & written for all
   *                              registers in this vectored transfer. May be
   *                              NULL.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  passed in port handle is invalid.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_register_rw_ex)(sns_sync_com_port_handle *port_handle,
                                   sns_com_port_config_ex *com_port_ex,
                                   sns_port_vector *vectors,
                                   int32_t num_vectors, bool save_write_time,
                                   uint32_t *xfer_bytes);

  /**
   * @brief Read/write operation directly to the slave.
   * This operation is valid only on a port that has been
   * successfully opened using sns_scp_open().
   *
   *  @param[in]  port_handle     Port handle
   *  @param[in]  is_write        True for write operation else
   *                              false.
   *
   *  @param[in]  save_write_time True if time of the bus write
   *                              transaction must be saved.
   *
   *  @param[out] buffer          Buffer for read/write operation.
   *  @param[in]  bytes           Number of bytes to read/write
   *  @param[out] xfer_bytes      Total bytes actually read/written.
   *
   *  @note Cause for specific error returns:
   *  - SNS_RC_INVALID_VALUE - passed in port handle is invalid.
   *  - SNS_RC_NOT_AVAILABLE - action not supported
   *
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  passed in port handle is invalid.
   *  - SNS_RC_NOT_AVAILABLE  action not supported.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_simple_rw)(sns_sync_com_port_handle *port_handle,
                              bool is_write, bool save_write_time,
                              uint8_t *buffer, uint32_t bytes,
                              uint32_t *xfer_bytes);

  /**
   * @brief Gets the timestamp for most recent write operation.
   * This API must be called only when save_write_time is 'true'
   * for the most recent write operation on the bus. It is
   * typically used for S4S mode of operation to get the start
   * time when the ST command is sent to the physical sensor. For
   * I2C and I3C, the timestamp is when the START condition occurs
   * for the write transfer. For SPI, the timestamp is an instance
   * during the write transfer.
   * This operation is valid only on a port that has been
   * successfully opened using sns_scp_open().
   *
   *  @param[in]  port_handle    Port handle for the bus.
   *  @param[out] write_time     System timestamp in ticks.
   *
   *  @note Cause for specific error returns:
   *  - SNS_RC_INVALID_VALUE - passed in port handle is invalid.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  passed in port handle is invalid.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_get_write_time)(sns_sync_com_port_handle *port_handle,
                                   sns_time *write_time);

  /**
   * @brief Issues a direct I3C Common Command Code (CCC) to a slave.
   * See Mipi Alliance Specificiation for I3C for more detailed information.
   * This API will either send or receive a direct CCC to a specified slave.
   * If the device is not I3C, this function will return an error.
   * All CCC commands save write time, so "sns_scp_get_write_time" may be
   * called later to get the transfer time on the bus.
   *
   *  @param[in]  port_handle     Port handle.
   *  @param[in]  ccc_cmd         CCC command to issue. Read or write will
   *                              depend on command.
   *  @param[out] buffer          Buffer for read/write operation. May be NULL
                                  if cmd has no data.
   *  @param[in]  bytes           Size of the buffer May be 0.
   *  @param[out] xfer_bytes      Total bytes actually read/written.
   *
   *  @note Cause for specific error returns:
   *  - SNS_RC_NOT_SUPPORTED - CCC is not supported on this bus
   *  - SNS_RC_INVALID_VALUE - invalid handle, unsupported CCC, or wrong buffer
   *                           size for CCC.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_NOT_SUPPORTED  CCC is not supported on this bus
   *  - SNS_RC_INVALID_VALUE  invalid handle, unsupported CCC, or wrong buffer
   *                          size for CCC.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_issue_ccc)(sns_sync_com_port_handle *port_handle,
                              sns_sync_com_port_ccc ccc_cmd, uint8_t *buffer,
                              uint32_t bytes, uint32_t *xfer_bytes);

  /**
   * @brief Full duplex read / write operation directly to the slave.
   * This operation is valid only on a port that has been successfully opened
   * using sns_scp_open() and supports full duplex transfers.
   *
   *  @param[in] port_handle      Port handle.
   *  @param[in] read_buffer      Pointer to the read buffer.
   *  @param[in] read_len_bytes   Length of the read buffer.
   *  @param[in] write_buffer     Pointer to the write buffer.
   *  @param[in] write_len_bytes  Length of the write buffer.
   *  @param[in] bits_per_word    SPI basic transaction unit size, usually 8 or
   *                              32 bit.
   *
   *  @note Cause for specific error returns:
   *  - SNS_RC_INVALID_VALUE - passed in port handle is invalid.
   *  - SNS_RC_NOT_AVAILABLE - action not supported.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_INVALID_VALUE  Passed in port handle is invalid.
   *  - SNS_RC_NOT_AVAILABLE  Action not supported.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_rw)(sns_sync_com_port_handle *port_handle,
                       uint8_t *read_buffer, uint32_t read_len_bytes,
                       const uint8_t *write_buffer, uint32_t write_len_bytes,
                       uint8_t bits_per_word);

  /**
   * @brief Get the list of slaves matching the input Manufacturer ID (MIPI(R))
   *
   * @param[in] com_config             Com port configuration structure.
   * @param[in] mipi_manufacturer_id   Manufacturer ID assigned by MIPI(R) 
   *                                   alliance. This ID uniquely identifies 
   *                                   each vendor.
   * @param[out] list                  List of slaves matching the input 
   *                                   Manufacturer ID (assigned by the MIPI(R) 
   *                                   Alliance).
   * @param[out] num_slaves            Actual number of slaves matching the 
   *                                   input manufacturer id.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_NOT_AVAILABLE  Action not supported.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*sns_scp_get_dynamic_addr)( sns_com_port_config const *com_config,
                                      uint32_t mipi_manufacturer_id,
                                      sns_com_port_i3c_slaves *list,
                                      uint32_t *num_slaves );

} sns_sync_com_port_service_api;
