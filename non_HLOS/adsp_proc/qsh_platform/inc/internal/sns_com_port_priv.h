#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Private definitions used by synch_com_port (SCP) and
 * async_com_port (ACP).
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/
#include <stdbool.h>
#include <stdint.h>
#include "sns_async_com_port_int.h"
#include "sns_com_port_types.h"

/*=============================================================================
  Macros and constants
  ===========================================================================*/
#define SNS_MIN_BUS_INSTANCE_VALUE     1
#define SNS_MIN_SSC_QUP_INSTANCE_VALUE GET_QUP_MINOR(QUP_SSC_0)
#define SNS_MAX_SSC_QUP_INSTANCE_VALUE GET_QUP_MINOR(QUP_SSC_N)
#define SNS_DEFAULT_SSC_QUP_INSTANCE   SNS_MIN_SSC_QUP_INSTANCE_VALUE + 1

#define SNS_GET_BUS_INSTANCE(se_idx) (se_idx + 1)
#define SNS_GET_QUP_INDEX(qup_inst)  (qup_inst - 1)

/**
 * @brief 64 byte aligned address required to do buses transaction using
 * DDR memory extra 64 bytes added to avoid cache dirty bit issue.
 */
#define SNS_GET_CACHE_ALIGNED_SIZE(size) ((((size) / 64) + 2) * 64)

#define SNS_GET_CACHE_ALIGNED_PTR(ptr)                                         \
  (uint8_t *)((intptr_t)(ptr + 64) & 0xFFFFFFC0)
#define SNS_GET_QUP_TYPE(qup, inst)                                            \
  SET_QUP_TYPE((qup ? TOP_QUP : SSC_QUP), SNS_GET_QUP_INDEX(inst))

/**----------------------------- I3C BUS ---------------------------------*/
// TODO: move I3C to bus specific files when we have QuPv3 drivers
/**
 * @brief Structure containing the information of an i3c device.
 *
 */
typedef struct sns_i3c_info
{
  // TODO
} sns_i3c_info;

/**---------------------------I2C BUS---------------------------------------*/
/**
 * @brief Structure containing the information of an i2c device.
 *
 */
typedef struct sns_i2c_info
{
  void *i2c_handle;
  sns_time txn_start_time;
  _Atomic unsigned int xfer_in_progress;
} sns_i2c_info;
/**------------------------------ BUS HANDLER ------------------------------*/
/**
 * @brief Structure containing the information of a bus instance.
 *
 */
typedef struct sns_bus_info
{
  sns_bus_type bus_type;
  uint8_t *bus_config; /*!< bus_config is allocated by bus
                        *   specific implementation.
                        */
  bool power_on : 1;
  bool opened : 1;
} sns_bus_info;

/**
 * @brief Structure containing the vector and xfer mapping.
 *
 */
typedef struct sns_com_port_vector_xfer_map
{
  sns_port_vector *orig_vector_ptr; /*!< Input vector*/
  uint8_t *xfer_buffer_ptr;         /*!< Memory allocated for reg+data
                                     *   for each vector
                                     */
  size_t xfer_buffer_len;           /*!< Length of xfer buffer*/
  uint8_t num_vectors;              /*!< Number of vectors */
} sns_com_port_vector_xfer_map;

/**
 * @brief Structure containing the private handle of the port.
 *
 */
typedef struct sns_com_port_priv_handle
{
  sns_com_port_config com_config;        /*!< Configuration parameters.*/
  sns_bus_info bus_info;                 /*!< Bus information.*/
  sns_ascp_callback callback;            /*!< Async function pointer. */
  sns_com_port_vector_xfer_map xfer_map; /*!< Vector map info.*/
  void *args;   /*!< User data passed in from client.*/
  void *caller; /*!< Pointer to caller object.*/
  void *prof;   /*!< Profile handle.*/
} sns_com_port_priv_handle;

/**
 * @brief Structure containing the proxy handle.
 *
 */
typedef struct sns_com_port_proxy_handle
{
  sns_com_port_config com_config;   /*!< Configuration parameters.*/
  sns_sync_com_port_handle *handle; /*!< Sync handle.*/
  uint8_t ref_count;                /*!< Reference count.*/
  bool high_perf_mode_on;           /*!< High performance mode on or off.*/
} sns_com_port_proxy_handle;
