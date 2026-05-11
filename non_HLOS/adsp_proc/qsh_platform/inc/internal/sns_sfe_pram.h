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
#include "sns_dae.pb.h"
#include "sns_rc.h"
#include "sns_time.h"

#include <stdint.h>

/**
********************************************************************************
                                  Constants & Macros
********************************************************************************
*/
// Per section 6.4.1.1 of SFE Arch Specs
typedef enum sns_sfe_client_id_type
{
  SFE_CLIENT_ID_VDC_INVALID,
  SFE_CLIENT_ID_VDC_1,
  SFE_CLIENT_ID_VDC_2,
  SFE_CLIENT_ID_VDC_3,
  SFE_CLIENT_ID_VDC_4,
  SFE_CLIENT_ID_VDC_5,
  SFE_CLIENT_ID_VDC_6,
  SFE_CLIENT_ID_VDC_7,
  SFE_CLIENT_ID_VDC_8,
  SFE_CLIENT_ID_VDC_9,
  SFE_CLIENT_ID_VDC_10,
  SFE_CLIENT_ID_VDC_11,
  SFE_CLIENT_ID_VDC_12,
  SFE_CLIENT_ID_DAE_EV = 27,
  SFE_CLIENT_ID_F3 = 29,
  SFE_CLIENT_ID_DEBUG = 30,
} sns_sfe_client_id_type;

typedef enum sns_sfe_pram_partition
{
  SNS_PRAM_PARTITION_VDC_1,
  SNS_PRAM_PARTITION_VDC_2,
  SNS_PRAM_PARTITION_VDC_3,
  SNS_PRAM_PARTITION_VDC_4,
  SNS_PRAM_PARTITION_VDC_5,
  SNS_PRAM_PARTITION_DEBUG,
  // add more as needed
  SNS_PRAM_PARTITION_MAX

} sns_sfe_pram_partition;

/**
********************************************************************************
                                  Structures
********************************************************************************
*/
typedef void *sns_sfe_pram_handle;

typedef struct sns_sfe_pram_block_info
{
  sns_time ts;
  sns_dae_timestamp_type ts_type;
  sns_sfe_client_id_type client_id;
  size_t data_size;
  uint8_t *data;
} sns_sfe_pram_block_info;

/**
********************************************************************************
                                  Public Functions
********************************************************************************
*/

/**
 * @brief Initializes the circular PRAM buffer for the given PRAM partition
 *
 * @param [in] partition           The PRAM partition to initialize
 * @param [in] buffer_start_addr   Address of the start of the PRAM partition
 * @param [in] size                Size of the PRAM partition
 * @param [out] pram_handle        Opaque handle to PRAM partition
 *
 * @return
 *  SNS_RC_SUCCESS                PRAM partition initialized
 *  SNS_RC_INVALID_VALUE          Invalid partition provided
 *
 */
sns_rc sns_sfe_pram_partition_init(sns_sfe_pram_partition partition,
                                   void *buffer_start_addr, size_t size,
                                   sns_sfe_pram_handle *pram_handle);

/**
 * @brief Frees any resources used for the  given PRAM partition
 *
 * @param [inout] pram_handle     Handle to PRAM partition to deinitialize
 *
 * @return
 *  SNS_RC_SUCCESS                PRAM partition deinitialized
 *  SNS_RC_INVALID_VALUE          Invalid partition provided
 *
 */
sns_rc sns_sfe_pram_partition_deinit(sns_sfe_pram_handle *pram_handle);

/**
 * @brief Copies PRAM region into local memory for processing.
 *
 * @note A valid region contains one or more PRAM blocks
 * @note The cache will be overriden on the next call to
 *       sns_sfe_pram_cache_region() or sns_sfe_pram_cache_block_at_address().
 * @note To access the cached blocks call sns_sfe_pram_get_next_block()
 *       repeatedly while it returns SNS_RC_SUCCESS
 *
 * @param [in] pram_handle      Handle to PRAM partition
 * @param [in] start_offset     Start offset of data to cache
 * @param [in] end_offset       End offset of data to cache
 *
 * @return
 *  SNS_RC_SUCCESS              PRAM region successfully cached
 *  SNS_RC_INVALID_VALUE        Invalid partition or offsets
 *  SNS_RC_NOT_AVAILABLE        No available local memory
 *
 */
sns_rc sns_sfe_pram_cache_region(sns_sfe_pram_handle pram_handle,
                                 int32_t start_offset, int32_t end_offset);

/**
 * @brief
 * Returns the next block of data in the cache.
 *
 * @note Region must be cached first.
 * @note Should be called until function returns SNS_RC_FAILED
 *
 * @param[in] cache_handle    The cached region on which to operate.
 * @param[out] client_id      Identifies owner of the data
 * @param[out] timestamp      Timestamp assigned to this block
 * @param[out] ts_type        Type of timestamp. See sns_dae_timestamp_type
 * @param[out] data_size      Size of data block.
 * @param[out] data           Pointer to data block.
 *
 * @return
 *  SNS_RC_SUCCESS            Data successfully extracted from a valid block
 *  SNS_RC_INVALID_VALUE      Invalid cache provided
 *  SNS_RC_FAILED             No more valid block
 *
 */
sns_rc sns_sfe_pram_get_next_block(sns_sfe_pram_handle pram_handle,
                                   sns_sfe_pram_block_info *block_info)
    __attribute__((nonnull(2)));

/**
 * @brief Copies the PRAM block at the given address into local memory for
 * processing.
 *
 * @note The cache will be overriden on the next call to
 *       sns_sfe_pram_cache_region() or sns_sfe_pram_cache_block_at_address().
 *
 * @param [in] pram_handle      Handle to PRAM partition
 * @param [in] pram_addr        Address of the PRAM block
 * @param [out] block_info      Destination for the extracted PRAM block info
 *
 * @return
 *  SNS_RC_SUCCESS              PRAM block successfully read
 *  SNS_RC_INVALID_VALUE        Invalid handle or address
 *  SNS_RC_NOT_AVAILABLE        No available local memory
 *
 */
sns_rc sns_sfe_pram_cache_block_at_address(sns_sfe_pram_handle pram_handle,
                                           uintptr_t pram_addr,
                                           sns_sfe_pram_block_info *block_info)
    __attribute__((nonnull(3)));
