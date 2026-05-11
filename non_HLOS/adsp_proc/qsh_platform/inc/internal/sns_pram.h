/** ============================================================================
 * @file
 *
 * @brief PRAM data buffer manager.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*******************************************************************************
                               Includes
********************************************************************************
*/
#include <stdbool.h>
#include <stdint.h>
#include "sns_rc.h"
#include "sns_time.h"

/**
********************************************************************************
                                  Constants & Macros
********************************************************************************
*/

/**
********************************************************************************
                                  Structures
********************************************************************************
*/
/**
 * @brief Opaque memory block structure
 *
 */
typedef struct sns_pram_block_s sns_pram_block_s;

typedef void *sns_pram_cache;

/**
********************************************************************************
                                  Public Functions
********************************************************************************
*/

/**
 * @brief Called once to init the circular PRAM buffer for sensor data.
 *
 * @param [in] buffer_start_addr   Address of the start of the PRAM region
 *                                 dedicated to sensor data transfer.
 * @param [in] size                Size of the PRAM region for sensor data.
 *
 * @return
 * - None.
 *
 */
void sns_pram_data_init(void *buffer_start_addr, uint32_t size);

/**
 * @brief Copies PRAM data into local memory for processing. The region may be
 * re-used by the SDC after this function returns.
 * Call sns_pram_cache_release() when local processing is complete.
 *
 * @param [out] cache_handle Handle to cached data.
 * @param [in] start_offset  Start offset of data to cache.
 * @param [in] end_offset    End offset of data to cache.
 *
 * @return
 *  - SNS_RC_SUCCESS:     If successful.
 *  - SNS_RC_FAILED:      Otherwise.
 *
 * @note Allocates local memory to store the PRAM data.
 */
sns_rc sns_pram_cache_region(sns_pram_cache *cache_handle, int32_t start_offset,
                             int32_t end_offset);

/**
 * @brief Release copy of PRAM data in local memory after processing.
 *
 * @param[in] cache_handle Cached region on which to operate.
 *
 * @return
 *  - SNS_RC_SUCCESS:
 *
 */
sns_rc sns_pram_cache_release(sns_pram_cache cache_handle);

/**
 * @brief
 * Returns a pointer to a PRAM block for sending to a physical sensor instance.
 * Data may be freed and no longer accessible after the next call to
 * sns_pram_get_next_block(), so all returned data should be processed before
 * calling this function again.
 *
 * @note Region must be cached first. May allocate additional
 *       memory to store the block.
 *
 *
 * @param[in] cache_handle   Cached region on which to operate.
 * @param[out] sensor_handle Index of the sensor which created the data.
 * @param[out] timestamp     Timestamp assigned to this region.
 * @param[out] ts_type       Type of timestamp. See "TS_TYPE_*" #defines.
 * @param[out] data_size     Size of data block.
 * @param[out] sensor_data   Pointer to sensor data.
 *
 * @return
 *    - SNS_RC_FAILED:  when there is no more data in the cache
 *
 */
sns_rc sns_pram_get_next_block(sns_pram_cache cache_handle,
                               uint8_t *sensor_handle, sns_time *timestamp,
                               uint8_t *ts_type, uint16_t *data_size,
                               uint8_t **sensor_data);
