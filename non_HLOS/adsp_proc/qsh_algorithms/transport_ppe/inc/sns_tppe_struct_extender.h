#pragma once
/*=============================================================================
  @file sns_tppe_struct_extender.h

  Contains APIs that can be used to extend a structure.

  Copyright (c) 2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Usage Guideline

  This util is used to make it easier to compartmentalize a memory block into
  structures. A top level structure will be called a parent. All the structures
  that follows will be children. We call it an extender since it is used
  to extend a parent structure memory and add children to it.

  These are the steps to use this util.
  1. Initialize extender using parent struct size
  2. Add the sizes of all the structures in the order you want to place them
  3. Get total size of the structure after all additions
  4. Allocate memory for this size using malloc
  5. Set parent pointer in extender using corresponding API
  6. Get the address of each item using the indices returned
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_mem_util.h"
#include "sns_types.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_BUFFERS_SUPPORTED 16

typedef struct simple_struct_extender
{
  size_t buffer_sizes[MAX_BUFFERS_SUPPORTED];
  void *parent_struct_pointer;
  size_t items_added;
} simple_struct_extender;

/**
 * @name sstruct_init
 * @param thiz: the current sstruct extender pointer
 * @param parent_struct_size: size of parent struct that is to be extended
 * @return true/false based on success or failure
 * @note
 * if thiz is NULL, false is returned
 **/
bool sstruct_init(simple_struct_extender *thiz, size_t parent_struct_size);

/**
 * @name sstruct_append_item_with_size
 * @param thiz: the current sstruct extender pointer
 * @param size: size of the item user wishes to append to parent structure
 * @return uint32_t : identifier/index of the item that was added. Use this
 * to get pointer in future.
 * @note
 * All sizes will be aligned using ALIGN_8 by the utility
 **/
uint32_t sstruct_append_item_with_size(simple_struct_extender *thiz, size_t size);

/**
 * @name sstruct_get_total_size
 * @param thiz: the current sstruct extender pointer
 * @return size_t : total size of the resulting structure after adding all items
 * @brief
 * Called after user appends all the items he wishes to add to the parent
 * structure. The resulting size is the total of all items including
 * parent structure. All sizes are aligned using ALIGN_8
 **/
size_t sstruct_get_total_size(simple_struct_extender *thiz);

/**
 * @name sstruct_set_parent_pointer
 * @param thiz: the current sstruct extender pointer
 * @param parent_struct_pointer: set the parent pointer
 * @return true/false based on success or failure
 * @brief
 * Called after user appends all the items he wishes to add to the parent
 * structure. Set the parent pointer so that the pointers to the sub items
 * can be obtained
 **/
bool sstruct_set_parent_pointer(simple_struct_extender *thiz, void *parent_struct_pointer);

/**
 * @name sstruct_get_address_for_item
 * @param thiz: the current sstruct extender pointer
 * @param item_id: identifier returned while appending item
 * @return actual pointer of the item within the extended structure
 *         NULL if parent pointer is not set
 * @brief
 * Called to get the actual pointer of the sub item. The sizes are used to
 * calculate the offset from the parent pointer. Thus it is important to set
 * parent pointer. If the item size is set to 0, then the pointer returned
 * will be NULL
 **/
void *sstruct_get_address_for_item(simple_struct_extender *thiz, uint32_t item_id);