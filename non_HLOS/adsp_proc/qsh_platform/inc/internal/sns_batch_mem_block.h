#pragma once
/** ===========================================================================
 * @file
 *
 * @brief SNS batch mem block interface.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/

/*=============================================================================
  Include files
  ===========================================================================*/

#include "sns_memmgr.h"
#include "sns_sl_list.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*=============================================================================
  Macros
  ===========================================================================*/
/**
 * @brief Batch sensor Heap IDs
 *
 */
#define SNS_BATCH_SENSOR_HEAP_ID        SNS_ISLAND_BATCH
#define SNS_CLIENT_BATCH_SENSOR_HEAP_ID SNS_CLIENT_BATCH

/*=============================================================================
  Type definitions
  ===========================================================================*/
/**
 * @brief Batch memory block.
 *
 */
typedef struct _sns_batch_mem_block
{
  sns_sl_list_item list_entry; /*!< List entry. */
  uint16_t event_count;        /*!< Event count. */
  void *buffer;                /*!< Buffer pointer.*/
} sns_batch_mem_block;

/**
 * @brief Batch memory block.
 *
 */
typedef struct _sns_batch_mem_block_list
{
  sns_sl_list list;
  uint16_t write_index;
} sns_batch_mem_block_list;

/*=============================================================================
  Type definitions
  ===========================================================================*/

/**
 * @brief Initialize the block list.
 *
 * @param[in] list_handle Pointer to a list handle.
 *
 * @return
 *   - None.
 *
 */
void sns_batch_mem_block_list_init(sns_batch_mem_block_list *list_handle);

/**
 * @brief Get allocation requirement for the given block.
 *
 * @param[in] list_handle      Pointer to a list handle.
 * @param[in] block_alloc_size Size of each block in bytes.
 * @param[in] alloc_size       Size of the buffer required per block
 *
 * @return
 *   - size_t:     Number of blocks that can be allocated from this pool.
 *
 */
size_t sns_batch_mem_block_list_get_alloc_requirement(
    sns_batch_mem_block_list *list_handle, size_t block_alloc_size,
    size_t alloc_size);

/**
 * @brief Allocate one or more blocks from the list.
 *
 * @param[in] list_handle      Pointer to a list handle.
 * @param[in] block_alloc_size Size of each block in bytes.
 * @param[in] alloc_size       Size of the buffer required per block.
 * @param[in] heap_id          Heap ID.
 *
 * @return
 *   - None.
 *
 */
void *sns_batch_mem_block_list_alloc(sns_batch_mem_block_list *list_handle,
                                     size_t block_alloc_size, size_t alloc_size,
                                     sns_mem_heap_id heap_id);

/**
 * @brief Free all blocks in the list.
 *
 * @param[in] list_handle      Pointer to a list handle.
 *
 * @return
 *  - size_t:       Size of freed blocks.
 *
 */
size_t
sns_batch_mem_block_list_free_all_blocks(sns_batch_mem_block_list *list_handle);

/**
 * @brief Reset the list.
 *
 * @param[in] list_handle      Pointer to a list handle.
 *
 * @return
 *   - None.
 *
 */
void sns_batch_mem_block_list_reset(sns_batch_mem_block_list *list_handle);

/**
 * @brief Free blocks using an event count.
 *
 * @param[in] list_handle      Pointer to a list handle.
 * @param[in] event_count      Event count.
 *
 * @return
 *  -size_t:  Size of freed blocks.
 *
 */
size_t sns_batch_mem_block_free_blocks_using_event_count(
    sns_batch_mem_block_list *list_handle, uint32_t event_count);
