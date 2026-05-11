/*=============================================================================
  @file sns_array_list.c

  Contains function definitions required for array list implementation

  Copyright (c) 2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "sns_array_list.h"
#include "sns_mem_util.h"
#include "sns_pb_util.h"
#include "sns_rc.h"
#include "sns_list.h"
#include "sns_types.h"
#include "sns_tppe_sensor_instance.h"
#include <stdint.h>

static bool check_limits(sns_array_list_param *param_in)
{
  return param_in->count_limit && param_in->count_per_block && param_in->item_size &&
         param_in->count_limit < UINT16_MAX && param_in->count_per_block < UINT16_MAX &&
         param_in->item_size < UINT16_MAX && param_in->count_per_block <= param_in->count_limit;
}

static void free_all_blocks(sns_array_list *thiz)
{
  sns_list_iter iter;
  sns_list_iter_init(&iter, &thiz->blocks, true);

  while(sns_list_iter_curr(&iter))
  {
    thiz->mem_service->api->free(thiz->instance_pointer, sns_list_iter_remove(&iter));
  }
}

static bool is_item_equal(void *item1, void *item2, size_t len)
{
  return sns_memcmp(item1, item2, len) == 0;
}

sns_rc sns_array_list_init(sns_array_list *thiz, sns_memory_service *mem_service,
                           sns_sensor_instance *const instance, sns_array_list_param *param_in)
{
  if(!check_limits(param_in))
  {
    return SNS_RC_FAILED;
  }

  thiz->count_limit = param_in->count_limit;
  thiz->count_per_block = param_in->count_per_block;
  thiz->count_limit = param_in->count_limit;
  thiz->item_size = param_in->item_size;

  sns_list_init(&thiz->blocks);
  thiz->count = 0;
  thiz->cur_index = 0;

  thiz->instance_pointer = instance;
  thiz->mem_service = mem_service;
  return SNS_RC_SUCCESS;
}

void sns_array_list_deinit(sns_array_list *thiz)
{
  thiz->count_limit = 0;
  thiz->count_per_block = 0;
  thiz->count_limit = 0;

  free_all_blocks(thiz);
  thiz->count = 0;
  thiz->count_per_block = 0;
  thiz->item_size = 0;

  thiz->instance_pointer = NULL;
  thiz->mem_service = NULL;
}

static void *allocate_memory(sns_array_list *thiz)
{
  sns_array_list_block ref_block;
  size_t alloc_size =
      sizeof(ref_block) - sizeof(ref_block.mem_block) + thiz->count_per_block * thiz->item_size;
  alloc_size = ALIGN_8(alloc_size);
  return sns_tppe_memory_service_malloc(thiz->mem_service, thiz->instance_pointer, alloc_size);
}

void *sns_array_list_insert(sns_array_list *thiz, void *source_item)
{

  sns_list_iter iter;
  void *dest_item = NULL;
  sns_array_list_block *cur_block =
      (sns_array_list_block *)sns_list_iter_init(&iter, &thiz->blocks, false);

  if(thiz->count >= thiz->count_limit)
  {
    return NULL;
  }

  bool new_block_required = thiz->cur_index >= thiz->count_per_block || NULL == cur_block;

  if(new_block_required)
  {
    cur_block = allocate_memory(thiz);
    if(cur_block)
    {
      thiz->cur_index = 0;
      sns_list_item_init(&cur_block->list_item, &thiz->blocks);
      sns_list_iter_insert(&iter, &cur_block->list_item, true);
    }
  }
  if(cur_block)
  {
    char *mem_block = (char *)cur_block->mem_block;
    dest_item = mem_block + thiz->cur_index * thiz->item_size;
    if(source_item)
    {
      sns_memscpy(dest_item, thiz->item_size, source_item, thiz->item_size);
    }
    else
    {
      sns_memzero(dest_item, thiz->item_size);
    }
    ++thiz->count;
    ++thiz->cur_index;
  }
  return dest_item;
}

uint32_t sns_array_list_get_index(sns_array_list *thiz, void *item)
{
  sns_list_iter iter;
  int index = 0;
  int result_index = 0;
  bool item_found = false;
  int remaining_elements = thiz->count;
  char *item_in_array = NULL;
  char *mem_block;
  int elements_in_cur_block = 0;
  sns_array_list_block *cur_block =
      (sns_array_list_block *)sns_list_iter_init(&iter, &thiz->blocks, true);
  while(!item_found && remaining_elements && cur_block)
  {
    index = 0;
    mem_block = (char *)&cur_block->mem_block[0];
    elements_in_cur_block =
        (remaining_elements < thiz->count_per_block) ? remaining_elements : thiz->count_per_block;
    while(elements_in_cur_block)
    {
      item_in_array = &mem_block[index * thiz->item_size];
      if(is_item_equal(item, item_in_array, thiz->item_size))
      {
        item_found = true;
        break;
      }
      --elements_in_cur_block;
      --remaining_elements;
      index++;
      result_index++;
    }
    cur_block = (sns_array_list_block *)sns_list_iter_advance(&iter);
  }

  return item_found ? result_index : ARRAY_ITEM_INDEX_NOT_FOUND;
}

void *sns_array_list_get_at(sns_array_list *thiz, uint32_t index)
{
  if(index >= thiz->count)
  {
    return NULL;
  }
  uint32_t block_number = index / thiz->count_per_block;
  uint32_t index_within_block = index % thiz->count_per_block;

  sns_list_iter iter;
  char *mem_block = NULL;

  sns_array_list_block *cur_block =
      (sns_array_list_block *)sns_list_iter_init(&iter, &thiz->blocks, true);

  while(block_number)
  {
    cur_block = (sns_array_list_block *)sns_list_iter_advance(&iter);
    block_number--;
  }
  mem_block = (char *)cur_block->mem_block;
  return mem_block + (index_within_block * thiz->item_size);
}

void *sns_array_list_insert_at(sns_array_list *thiz, void *item, int index)
{
  void *dest_item = sns_array_list_get_at(thiz, index);
  if(dest_item)
  {
    if(item)
    {
      sns_memscpy(dest_item, thiz->item_size, item, thiz->item_size);
    }
    else
    {
      sns_memzero(dest_item, thiz->item_size);
    }
  }
  return dest_item;
}

size_t sns_array_list_get_count(sns_array_list *thiz)
{
  return thiz->count;
}

static void remove_blocks_from_tail(sns_array_list *thiz, uint32_t remove_block_count)
{
  int i = 0;
  sns_list_iter iter;
  sns_list_iter_init(&iter, &thiz->blocks, true);
  uint32_t cur_blocks = sns_list_iter_len(&iter);
  for(i = 0; i < cur_blocks - remove_block_count; i++)
  {
    sns_list_iter_advance(&iter);
  }
  for(i = 0; i < remove_block_count; i++)
  {
    thiz->mem_service->api->free(thiz->instance_pointer, sns_list_iter_remove(&iter));
  }
}

uint32_t sns_array_list_remove_from_tail(sns_array_list *thiz, uint32_t remove_count)
{
  sns_list_iter iter;
  uint32_t remaining_events = thiz->count;
  if(remove_count > thiz->count)
  {
    remaining_events = 0;
  }
  else
  {
    remaining_events = thiz->count - remove_count;
  }
  sns_list_iter_init(&iter, &thiz->blocks, true);
  uint32_t cur_num_of_blocks = sns_list_iter_len(&iter);
  uint32_t new_num_of_blocks = 0;
  if(remaining_events)
  {
    new_num_of_blocks = (remaining_events) / thiz->count_per_block;
    if(remaining_events % thiz->count_per_block != 0)
    {
      new_num_of_blocks += 1;
    }
  }
  remove_blocks_from_tail(thiz, cur_num_of_blocks - new_num_of_blocks);
  thiz->count = remaining_events;
  if(thiz->count == 0)
  {
    thiz->cur_index = 0;
  }
  else if(thiz->count % thiz->count_per_block == 0)
  {
    thiz->cur_index = thiz->count_per_block;
  }
  else
  {
    thiz->cur_index = thiz->count % thiz->count_per_block;
  }
  return remaining_events;
}