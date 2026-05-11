/** ===========================================================================
 * @file
 *
 * @brief CCD Sensor Instance implementation.
 *
 * @note should only be accessed by CCD and DAE sensors
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/

#include "sns_osa_lock.h"
#include "sns_signal.h"
#include "sns_time.h"
#include "sns_rc.h"

#define BIN_DATA_EVENT_BUF_SIZE 10

#define MAX_NUM_BIN_INPUTS 8

/**
 * @brief Configuration event info structure
 *
 */
typedef struct config_event_info
{
  int8_t cd;         /*!< CD block number(1,2,3,or 4). */
  int8_t sdc_num;    /*!< proxy0 or proxy1. */
  char *data_source; /*!< Data source name. */
} config_event_info;

/**
 * @brief Binary data event info.
 *
 */
typedef struct bin_data_event_info
{
  int8_t bin_data;    /*!< Binary data value. */
  uint32_t cd_num;    /*!< CD block number(1,2,3,or 4). */
  uint32_t sdc_num;   /*!< proxy0 or proxy1. */
  sns_time timestamp; /*!< Timestamp when data was logged. */
} bin_data_event_info;

/**
 * @brief Data buffer for CCD binary inputs.
 *
 */
typedef struct
{
  bool cfg_updated;                             /*!< Flag indicating if
                                                 *    new config has been
                                                 *    received.
                                                 */
  bool cfg_valid[MAX_NUM_BIN_INPUTS];           /*!< Flag indicating if
                                                 *    valid config exists
                                                 *    for each binary input.
                                                 */
  config_event_info config[MAX_NUM_BIN_INPUTS]; /*!< Configurations for
                                                 *    all binary inputs.
                                                 */
  uint32_t read_idx;                            /*!< Read index. */
  uint32_t write_idx;                           /*!< Write index. */
  uint32_t proxy_log_buf_signal;                /*!< Buffer signal.*/
  sns_osa_lock *lock;                           /*!< Resource lock */
  sns_signal_handle *sig_handle;
  bin_data_event_info bin_event_info[BIN_DATA_EVENT_BUF_SIZE];
} sns_ccd_bin_data_buf;

/*==================================================================
  Public Functions
  ================================================================== */

/**
 * @brief Allocate data buf in island memory and initialize lock and signal
 * only a single data buffer can be allocated.
 *
 * @param[in] sig_handle signal handle from DAE.
 *
 * @return
 *  - SNS_RC_SUCCESS: Allocation and initialization successful.
 *  - SNS_RC_FAILED:  Buffer is already allocated or failed to allocate
 *                    the memory.
 *
 */
sns_rc sns_ccd_bin_data_buf_init(sns_signal_handle *sig_handle);

/**
 * @brief Free data buf memory, unregister signal, and delete lock.
 *
 * @return
 *   - None.
 *
 */
void sns_ccd_bin_data_buf_deinit(void);

/**
 * @brief Get data buf reference, should only be required in DAE.
 *
 * @return
 * - sns_ccd_bin_data_buf: Pointer to data buffer.
 *
 */
sns_ccd_bin_data_buf *sns_ccd_bin_data_buf_get_buf(void);

/**
 * @brief Log the current CCD binary input configuration.
 *
 * @param[in] proxy_name Name of the source proxy sensor.
 * @param[in] cd_num     CD block number (1,2,3,or 4).
 * @param[in] sdc_num    proxy0 or proxy1.
 *
 * @return
 *   - None.
 *
 */
void sns_ccd_bin_data_buf_log_input_cfg(char *proxy_name, uint32_t cd_num,
                                        uint32_t sdc_num);

/**
 * @brief Clear the input configuration associated with a block.
 *
 * @param[in] cd_num_to_delete Block whose configuration to delete.
 *
 * @return
 *   - None.
 *
 */
void sns_ccd_bin_data_buf_clear_input_config_block(uint32_t cd_num_to_delete);

/**
 * @brief Log the binary(non-IMU) data fed into CCD.
 *
 * @param[in] data        Data.
 * @param[in] timestamp   Timestamp.
 * @param[in] cd_num      CD number (1,2,3,or 4).
 * @param[in] sdc_num     proxy0 or proxy1.
 *
 * @return
 *   - None.
 *
 */
void sns_ccd_bin_data_buf_log_input_data(uint32_t data, sns_time timestamp,
                                         uint32_t cd_num, uint32_t sdc_num);
