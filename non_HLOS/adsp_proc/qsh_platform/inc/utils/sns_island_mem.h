#pragma once
/** =============================================================================
 * @file sns_island_mem.h
 *
 * @brief
 * SNS wrapper for TCM manager APIs
 *
 * @copyright
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *  ===========================================================================
 */

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdio.h>
#include <stdbool.h>

#ifndef SSC_TARGET_X86
#include "island_mem.h"
#endif

/*=============================================================================
  Macro definitions
  ===========================================================================*/
/*
 * Maximum character length of the memory pool name
 */
#define SNS_MAX_MEM_POOL_NAME_LEN   (255)

/*============================================================================
  Structure Definitions and Typedefs
  ===========================================================================*/
/**
 * @brief Memory util structure.
 */
typedef struct sns_island_mem_util
{
  char mem_pool_name[SNS_MAX_MEM_POOL_NAME_LEN];
} sns_island_mem_util;

/*=============================================================================
  Function Declarations
  ===========================================================================*/
/**
 * @brief Wrapper function for island_alloc()
 *
 * @param[in] size Size (in bytes) of the block that needs to be allocated
 * 
 * @return Returns pointer to the allocated memory, NULL otherwise
 */
void* sns_island_alloc(size_t size);

/**
 * @brief Wrapper function for island_free()
 * 
 * For freeing the allocated using island_alloc() API
 *
 * @param[in/out] **ptr Pointer to memory address which needs to be freed
 * 
 * @return None
 */
void sns_island_free(void **ptr);

/**
 * @brief Wrapper function for island_pool_alloc()
 * 
 * The island_pool_alloc will adjust requested memory size to 4K alignment if 
 * client requests memory unaligned to 4K size
 *
 * @param[in] *state Pointer to SNS memory util structure
 * @param[in] size Size (in bytes) of the block that needs to be allocated
 * 
 * @return Returns pointer to the allocated memory, NULL otherwise
 */
void* sns_island_pool_alloc(sns_island_mem_util *const state, size_t size);

/**
 * @brief Wrapper function for island_pool_free()
 * 
 * For freeing the allocated using island_pool_alloc() API
 * 
 * The pool must be initialized and client must have valid island_mem_handle
 *
 * @param[in] *state Pointer to SNS memory util structure
 * @param[in/out] **ptr Pointer to memory address which needs to be freed
 * 
 * @return None
 */
void sns_island_pool_free(sns_island_mem_util *const state, void **ptr);

/**
 * @brief Wrapper function for sns_island_pool_heap_alloc()
 * 
 * Allocates memory from the 'island_heap' from pool associated with handle passed
 *
 * @param[in] *state Pointer to SNS memory util structure
 * @param[in] size Size (in bytes) of the block that needs to be allocated
 * 
 * @return Returns pointer to the allocated memory, NULL otherwise
 */
void* sns_island_pool_heap_alloc(sns_island_mem_util *const state, size_t size);

/**
 * @brief Wrapper function for island_pool_heap_free()
 * 
 * For freeing the allocated using island_pool_heap_alloc() API
 * 
 * @param[in/out] **ptr Pointer to memory address which needs to be freed
 * 
 * @return None
 */
void sns_island_pool_heap_free(void **ptr);
