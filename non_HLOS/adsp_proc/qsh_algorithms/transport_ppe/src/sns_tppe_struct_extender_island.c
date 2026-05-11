/*=============================================================================
  @file sns_tppe_struct_extender_island.c

  Contains API implementations that can be used to extend a structure.

  Copyright (c) 2022-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_tppe_struct_extender.h"

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

bool sstruct_init(simple_struct_extender *thiz, size_t parent_struct_size)
{
  if(!thiz)
    return false;
  thiz->parent_struct_pointer = NULL;
  thiz->buffer_sizes[0] = ALIGN_8(parent_struct_size);
  thiz->items_added = 1;
  return true;
}

uint32_t sstruct_append_item_with_size(simple_struct_extender *thiz, size_t size)
{
  uint32_t retval = UINT32_MAX;
  if(thiz->items_added < MAX_BUFFERS_SUPPORTED)
  {
    thiz->buffer_sizes[thiz->items_added] = ALIGN_8(size);
    retval = thiz->items_added;
    ++thiz->items_added;
  }
  return retval;
}

size_t sstruct_get_total_size(simple_struct_extender *thiz)
{
  size_t total_size = 0;
  for(int i = 0; i < thiz->items_added; i++)
  {
    total_size += thiz->buffer_sizes[i];
  }
  return total_size;
}

bool sstruct_set_parent_pointer(simple_struct_extender *thiz, void *parent_struct_pointer)
{
  if(!thiz || !parent_struct_pointer)
  {
    return false;
  }
  thiz->parent_struct_pointer = parent_struct_pointer;
  return true;
}

void *sstruct_get_address_for_item(simple_struct_extender *thiz, uint32_t item_id)
{
  if(!thiz->parent_struct_pointer)
  {
    return NULL;
  }
  if(item_id >= thiz->items_added || thiz->buffer_sizes[item_id] == 0)
  {
    return NULL;
  }
  size_t size_until_item = 0;
  for(int i = 0; i < item_id; i++)
  {
    size_until_item += thiz->buffer_sizes[i];
  }
  return ((char *)thiz->parent_struct_pointer) + size_until_item;
}
