#pragma once
/*=============================================================================
  @file qsh_audio_memmgr.h

  Memory manager abstraction layer for Sensors.

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Includes
  ===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "sns_rc.h"

//for sim disable island memory allocation
#ifdef SSC_TARGET_X86
#undef QSH_AUDIO_USE_ISLAND_MEM
#endif

#ifdef QSH_AUDIO_USE_ISLAND_MEM
#include "island_mem.h"
#endif

/*=============================================================================
  Type Definitions
  ===========================================================================*/
/* Heap Ids */
typedef enum
{
  QSH_AUDIO_HEAP_MAIN = 0,
  QSH_AUDIO_HEAP_ISLAND = 2,
  QSH_AUDIO_HEAP_MAX,
} qsh_audio_mem_heap_id;


/*=============================================================================
  Public Function Declarations
  ===========================================================================*/

/*!
  @brief allocates memory from the heap and returns a pointer to the
               newly allocated memory block

  @note The memory is always zeroed out internally.

  @param [i] heapID           heap to allocate from
  @param [i] size                number of bytes required

  @return
   pointer to the block of memory if successful
   NULL if alloc failed
*/
void *qsh_audio_malloc(qsh_audio_mem_heap_id heap_id, size_t size);

/*!
  @brief free's a block of memory

  @note The memory is not zeroed out in qsh_audio_free().

  @param [i] ptr pointer to memory block to be freed
*/
void qsh_audio_free(void *ptr);
