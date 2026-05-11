#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Memory manager abstraction layer for Sensors.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/
/*=============================================================================
  Includes
  ===========================================================================*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "sns_rc.h"
#include "sns_register.h"

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief Heap Ids
 *
 */
typedef enum
{
  SNS_HEAP_MAIN = 0,       /*!< Main heap.*/
  SNS_HEAP_BATCH,          /*!< Batch heap.*/
  SNS_CLIENT_BATCH,        /*!< Client batch heap. */
  SNS_HEAP_ISLAND,         /*!< Island heap. */
  SNS_HEAP_PRAM,           /*!< PRAM heap. */
  SNS_ISLAND_BATCH,        /*!< Island batch heap. */
  SNS_HEAP_ISLAND_SSC,     /*!< Island SSC heap. */
  SNS_HEAP_ISLAND_QSHTECH, /*!< Island QSHTECH heap. */
#ifdef SSC_TARGET_X86
  SNS_ISLAND_X86_HEAP,
#endif
  SNS_HEAP_MAX,
} sns_mem_heap_id;

/**
 * @brief Memmgr callback function type.
 *
 */
typedef void (*sns_mem_heap_lowmem_cb)(intptr_t args);

/*=============================================================================
  Macros
  ===========================================================================*/

/*!< Use main heap when QSH is not operating in island */
#ifdef SNS_DISABLE_ISLAND
#define SNS_ISLAND_BATCH    SNS_HEAP_BATCH
#define SNS_CLIENT_BATCH    SNS_HEAP_BATCH
#define SNS_HEAP_ISLAND     SNS_HEAP_MAIN
#define SNS_HEAP_ISLAND_SSC SNS_HEAP_MAIN
#endif

/*=============================================================================
  Global Data
  ===========================================================================*/

/**
 * @brief Peripheral memory, start address and size.
 *
 */
extern void *sns_pram_addr;
extern uint32_t sns_pram_size;
extern uint32_t sns_pram_heap_size;

/*=============================================================================
  Public Function Declarations
  ===========================================================================*/

/**
 * @brief Initializes all the heaps.
 *
 * @return
 *   - SNS_RC_SUCCESS:
 *   - SNS_RC_FAILED:
 *
 */
sns_rc sns_heap_init(void);

/**
 * @brief Perform delayed heap initialization.  Must be called after *both*
 * sns_heap_init and sns_osa_init.
 *
 * @return
 *   - SNS_RC_SUCCESS:
 *   - SNS_RC_FAILED:
 *
 */
sns_rc sns_heap_init_delayed(void);

/**
 * @brief Searches the heap for a caller pointer matching
 * instance and frees the block.
 *
 * @param[in] heap_id    Pointer to the heap which to
 *                       search and free memory.
 * @param[in] caller_ptr Pointer to the sensor instance which
 *                       allocated the memory.
 * @param[in] alloc_cnt  Number of allocations by the caller_ptr.
 *
 * @return
 *  - uint16_t:   Count of freed allocations for client ptr instance.
 *
 */
uint16_t sns_free_client_ptr_instances(const sns_mem_heap_id heap_id,
                                       void *caller_ptr, uint16_t alloc_cnt);

/**
 * @brief Allocates memory from the heap and returns a pointer to the
 *        newly allocated memory block.
 *
 * @note The memory is always zeroed out internally.
 *
 * @param [in] heap_id          Heap to allocate from.
 * @param [in] size             Number of bytes required.
 *
 * @return
 *   - Pointer to the block of memory if successful.
 *   - NULL if alloc failed
 *
 */
void *sns_malloc(sns_mem_heap_id heap_id, size_t size);

/**
 * @brief Allocates memory from the heap and returns a pointer to the
 *        newly allocated memory block.
 *
 * @note The memory is always zeroed out internally.
 *
 * @param [in] heap_id           Heap to allocate from.
 * @param [in] size              Number of bytes required.
 * @param [in] debug_caller_ptr  Optional debug such as caller
 *                               sns_sensor_instance ptr
 *                               If set to NULL, defaults to using the
 *                               caller .text address.
 *
 * @return
 *   - Pointer to the block of memory if successful.
 *   - NULL if alloc failed.
 *
 */
void *sns_malloc_debug(sns_mem_heap_id heap_id, size_t size,
                       void *debug_caller_ptr);

/**
 * @brief Allocates memory from ISLAND heap, or MAIN heap if ISLAND heap is
 * exhausted, and returns a pointer to the newly allocated memory block.
 *
 * @note The memory is always zeroed out internally.
 *
 * @param [in] size                Number of bytes required.
 *
 * @return
 *   - Pointer to the block of memory if successful.
 *   - NULL if alloc failed.
 *
 */
void *sns_malloc_island_or_main(size_t size);

/**
 * @brief Allocates memory from ISLAND_SSC heap, or MAIN heap if ISLAND_SSC
 * heap is exhausted,and returns a pointer to the newly allocated memory block.
 *
 * @note The memory is always zeroed out internally.
 *
 * @param[in] size                Number of bytes required
 *
 * @return
 *   - Pointer to the block of memory if successful.
 *   - NULL if alloc failed.
 *
 */
void *sns_malloc_island_ssc_or_main(size_t size);

/**
 * @brief Frees a block of memory.
 *
 * @note The memory is not zeroed out in sns_free().
 *
 * @param [in] ptr Pointer to memory block to be freed.
 *
 * @return
 *    - SNS_RC_SUCCESS:  If operation success.
 *    - SNS_RC_FAILED:   If ptr does not belong to any valid heap.
 *
 */
sns_rc sns_free(void *ptr);

/**
 * @brief Register to be notified of low memory.
 *
 * @param [in] heap_id    The heap to be monitored.
 * @param [in] limit      Amount of free memory below which to trigger callback.
 * @param [in] callback   The callback function to be registered.
 * @param [in] args       Pointer that will be returned as a parameter to
 *                        the callback
 *
 * @return
 *   - SNS_RC_SUCCESS:
 *   - SNS_RC_FAILED:
 *
 */
sns_rc sns_mem_heap_register_lowmem_cb(sns_mem_heap_id heap_id, uint32_t limit,
                                       sns_mem_heap_lowmem_cb callback,
                                       intptr_t args);

/**
 * @brief De-registers the given low mem callback.
 *
 * @param [in] callback The callback function to be de-registered.
 *
 * @return
 *    - None.
 *
 */
void sns_mem_heap_deregister_lowmem_cb(sns_mem_heap_lowmem_cb callback);

/**
 * @brief Get the heap ID for a pointer.
 *
 * @param [in] ptr Pointer to be checked.
 *
 * @return
 *   - Heap id of the ptr if it belongs to a heap managed by sns_memmgr.
 *   - SNS_HEAP_MAX otherwise
 *
 */
sns_mem_heap_id sns_memmgr_get_heap_id(void const *ptr);

/**
 * @brief Generate and submit heap usage log packet.
 *
 * @param [in] cookie Cookie that is used to populate the
 *                    user_defiend field in the log packet.
 *
 * @return
 *    - None.
 *
 */
void sns_memmgr_log_heap_usage(uint64_t cookie);

/**
 * @brief Get the current heap usage.
 *
 * @param[in] heap_id  Heap whose usage is to be measured.
 *
 * @return
 *  - size_t:      Number of bytes malloc'd in the heap.
 *
 */
size_t sns_memmgr_get_heap_usage(sns_mem_heap_id heap_id);

/**
 * @brief Get total heap size.
 *
 * @param[in] heap_id  Heap whose size is to be measured.
 *
 * @return
 *  - size_t:     Number of bytes available at maximum in the heap.
 *
 */
size_t sns_memmgr_get_heap_size(sns_mem_heap_id heap_id);

/**
 * @brief Get heap id from memory segment.
 *
 * @param[in] mem_segment     Memory segment information such as
 *                            QSH, SSC and QSHTECH
 *
 * @return
 *  - sns_mem_heap_id:    Memory heap ID.
 *
 */
sns_mem_heap_id sns_memmgr_get_sensor_heap_id(sns_mem_segment mem_segment);

/**
 * @brief Check if memory block of a given size is available in the given heap.
 *
 * @param[in] heap_id         Heap ID.
 * @param[in] block_size      Size of the block.
 *
 * @return
 *     - True:  If the block is found.
 *     - False: Otherwise.
 *
 */
bool sns_memmgr_block_available(sns_mem_heap_id heap_id, size_t block_size);
