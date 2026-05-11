/** =============================================================================
 * @file
 *
 * @brief Contains function definitions required for Caching all sent item crc
 * This is required by duplicate detection feature
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 * ===========================================================================*/

#include "sns_math_ops.h"
#include "sns_mem_util.h"
#include "sns_memory_service.h"
#include "sns_rc.h"
#include "sns_sensor_instance.h"
#include "sns_tppe_crc_cache.h"
#include "sns_array_list.h"
#include "sns_tppe_sensor.h"
#include "sns_types.h"

#define TRIGGER_ID_INVALID UINT8_MAX

static uint8_t *sns_tcc_get_trigger_ids(sns_tppe_crc_cache *thiz, uint32_t row)
{
  return (uint8_t *)sns_array_list_get_at(&thiz->trigger_ids_array, row);
}

/**
For reconfig requests, the previously allocated crc cache and trigger masks
should be freed
**/
static void sns_tcc_free_resources(sns_tppe_crc_cache *thiz)
{
  if(sns_array_list_get_count(&thiz->unique_crcs) > 0)
  {
    sns_array_list_deinit(&thiz->unique_crcs);
  }
  if(sns_array_list_get_count(&thiz->trigger_ids_array) > 0)
  {
    sns_array_list_deinit(&thiz->trigger_ids_array);
  }
}

size_t sns_tcc_get_tids_array_len(void)
{
  return MAX_TRIGGER_PER_CRC;
}

sns_rc sns_tcc_init(sns_tppe_crc_cache *thiz, uint32_t trigger_count, uint32_t max_crc_len,
                    sns_memory_service *const memory_service,
                    sns_sensor_instance *const tppe_instance_pointer)
{
  sns_rc rc = SNS_RC_SUCCESS;
  if(0 == trigger_count)
  {
    trigger_count = 1;
  }
  if(max_crc_len < MIN_CRC_CACHE_LEN)
  {
    max_crc_len = MIN_CRC_CACHE_LEN;
  }
  sns_tcc_free_resources(thiz);
  size_t mask_len = sns_tcc_get_tids_array_len();

  thiz->total_trigger_count = trigger_count;
  thiz->cache_count = 0;
  thiz->cur_index = -1;
  thiz->trigger_ids_arraylen = mask_len;
  thiz->crc_cache_max_len = max_crc_len;
  sns_array_list_param unique_crc_param = {
      .count_limit = max_crc_len, .count_per_block = MIN_CRC_CACHE_LEN, .item_size = 4};
  sns_array_list_param trigger_mask_param = {
      .count_limit = max_crc_len, .count_per_block = MIN_CRC_CACHE_LEN, .item_size = mask_len};

  rc = rc | sns_array_list_init(&thiz->unique_crcs, memory_service, tppe_instance_pointer,
                                &unique_crc_param);
  rc = rc | sns_array_list_init(&thiz->trigger_ids_array, memory_service, tppe_instance_pointer,
                                &trigger_mask_param);

  return rc;
}

uint32_t sns_tcc_get_crc_index(sns_tppe_crc_cache *thiz, uint32_t crc)
{
  return sns_array_list_get_index(&thiz->unique_crcs, &crc);
}

static uint32_t get_next_valid_crc_to_move(sns_tppe_crc_cache *thiz, uint32_t index,
                                           uint32_t end_index)
{
  uint32_t *crc = NULL;
  while(index > end_index && index < thiz->cache_count)
  {
    crc = (uint32_t *)sns_array_list_get_at(&thiz->unique_crcs, index);
    if(crc != NULL)
    {
      if(*crc)
      {
        return index;
      }
    }
    else
    {
      break;
    }
    index--;
  }
  return ARRAY_ITEM_INDEX_NOT_FOUND;
}

static uint32_t get_next_free_slot(sns_tppe_crc_cache *thiz, uint32_t index)
{
  uint32_t *crc = NULL;
  while(index < thiz->cache_count)
  {
    crc = (uint32_t *)sns_array_list_get_at(&thiz->unique_crcs, index);
    if(crc != NULL)
    {
      if(*crc == 0)
      {
        return index;
      }
    }
    else
    {
      break;
    }
    index--;
  }
  return ARRAY_ITEM_INDEX_NOT_FOUND;
}
/**
@brief :
This function will walk through the empty slots in between the table
Fill in those slots with items from back of the table
Once all slots are filled, remove elements from the end of the table
*/
static void sns_tcc_reclaim_empty_slots(sns_tppe_crc_cache *thiz, uint32_t freed_count)
{
  uint32_t tail = thiz->cache_count - 1;
  uint32_t new_tail = thiz->cache_count - freed_count - 1;

  uint32_t src_crc_index = get_next_valid_crc_to_move(thiz, tail, new_tail);
  uint32_t dest_crc_index = get_next_free_slot(thiz, new_tail);
  void *src_trigger_ids_array = NULL;
  void *dest_trigger_ids_array = NULL;
  while(src_crc_index != ARRAY_ITEM_INDEX_NOT_FOUND && dest_crc_index != ARRAY_ITEM_INDEX_NOT_FOUND)
  {
    // move crc
    sns_array_list_insert_at(&thiz->unique_crcs,
                             sns_array_list_get_at(&thiz->unique_crcs, src_crc_index),
                             dest_crc_index);
    // copy trigger indices
    src_trigger_ids_array = sns_array_list_get_at(&thiz->trigger_ids_array, src_crc_index);
    dest_trigger_ids_array = sns_array_list_get_at(&thiz->trigger_ids_array, dest_crc_index);

    if(src_trigger_ids_array && dest_trigger_ids_array)
    {
      sns_memscpy(dest_trigger_ids_array, sns_tcc_get_tids_array_len(), src_trigger_ids_array,
                  sns_tcc_get_tids_array_len());
    }
    src_crc_index--;
    src_crc_index = get_next_valid_crc_to_move(thiz, src_crc_index, new_tail);
    dest_crc_index = get_next_free_slot(thiz, dest_crc_index);
  }
  sns_array_list_remove_from_tail(&thiz->unique_crcs, freed_count);
  sns_array_list_remove_from_tail(&thiz->trigger_ids_array, freed_count);
  thiz->cache_count = thiz->cache_count - freed_count;
  thiz->cur_index = thiz->cache_count - 1;
}

void sns_tcc_clear_cache_for_trigger(sns_tppe_crc_cache *thiz, uint32_t trigger_index)
{
  // walk through each cache item
  // get the trigger array for this cache item
  // if there is a trigger index clear it
  // zero out the cache if all trigger indices are unintialized
  // reclaim empty slots now

  uint8_t *trigger_indices = NULL;
  uint8_t trigger_len = sns_tcc_get_tids_array_len();
  uint8_t uninitialized_count = 0;
  uint32_t freed_count = 0;
  uint32_t invalid_crc = 0;
  for(int crc_index = 0; crc_index < thiz->cache_count; crc_index++)
  {

    trigger_indices = sns_tcc_get_trigger_ids(thiz, crc_index);
    if(trigger_indices)
    {
      for(int i = 0; i < trigger_len; i++)
      {
        if(trigger_index == trigger_indices[i])
        {
          trigger_indices[i] = TRIGGER_ID_INVALID;
        }
        if(trigger_indices[i] == TRIGGER_ID_INVALID)
        {
          uninitialized_count++;
        }
      }
      if(uninitialized_count == trigger_len)
      {
        //  no triggers against this crc
        // invalidate crc at this location
        sns_array_list_insert_at(&thiz->unique_crcs, &invalid_crc, crc_index);
        freed_count++;
      }
      uninitialized_count = 0;
    }
  }
  if(freed_count)
  {
    sns_tcc_reclaim_empty_slots(thiz, freed_count);
  }
}

bool sns_tcc_is_trigger_set(sns_tppe_crc_cache *thiz, uint32_t crc_index, uint32_t trigger_index)
{
  bool result = false;
  uint8_t *trigger_indices = sns_tcc_get_trigger_ids(thiz, crc_index);
  if(trigger_indices)
  {
    for(int i = 0; i < sns_tcc_get_tids_array_len(); i++)
    {
      if(trigger_indices[i] == trigger_index)
      {
        result = true;
        break;
      }
    }
  }
  return result;
}

void sns_tcc_reset_trigger_ids(sns_tppe_crc_cache *thiz, uint8_t *trigger_ids)
{
  UNUSED_VAR(thiz);
  if(trigger_ids)
  {
    for(int i = 0; i < sns_tcc_get_tids_array_len(); i++)
    {
      trigger_ids[i] = TRIGGER_ID_INVALID;
    }
  }
}

void sns_tcc_set_trigger(sns_tppe_crc_cache *thiz, uint32_t crc_index, uint32_t trigger_index)
{
  uint8_t *trigger_indices = sns_tcc_get_trigger_ids(thiz, crc_index);
  if(trigger_indices && trigger_index < thiz->total_trigger_count && trigger_index < UINT8_MAX)
  {
    for(int i = 0; i < sns_tcc_get_tids_array_len(); i++)
    {
      if(trigger_indices[i] == TRIGGER_ID_INVALID || trigger_indices[i] == trigger_index)
      {
        trigger_indices[i] = trigger_index;
        break;
      }
    }
  }
}

uint32_t sns_tcc_insert_crc(sns_tppe_crc_cache *thiz, uint32_t crc)
{
  uint32_t insert_index = sns_tcc_get_crc_index(thiz, crc);
  void *trigger_ids = NULL;

  if(insert_index == ARRAY_ITEM_INDEX_NOT_FOUND)
  {
    insert_index = (thiz->cur_index + 1) % thiz->crc_cache_max_len;
    if(sns_array_list_get_count(&thiz->unique_crcs) >= thiz->crc_cache_max_len)
    {
      sns_array_list_insert_at(&thiz->unique_crcs, &crc, insert_index);
      trigger_ids = sns_array_list_insert_at(&thiz->trigger_ids_array, NULL, insert_index);
      sns_tcc_reset_trigger_ids(thiz, trigger_ids);
    }
    else
    {
      sns_array_list_insert(&thiz->unique_crcs, &crc);
      trigger_ids = sns_array_list_insert(&thiz->trigger_ids_array, NULL);
      sns_tcc_reset_trigger_ids(thiz, trigger_ids);
    }

    thiz->cur_index = insert_index;
    thiz->cache_count = (thiz->cache_count < thiz->crc_cache_max_len) ? thiz->cache_count + 1
                                                                      : thiz->crc_cache_max_len;
  }
  return insert_index;
}

void sns_tcc_clear_cache(sns_tppe_crc_cache *thiz)
{
  sns_array_list_deinit(&thiz->trigger_ids_array);
  sns_array_list_deinit(&thiz->unique_crcs);
  thiz->cache_count = 0;
  thiz->cur_index = -1;
}

void sns_tcc_deinit(sns_tppe_crc_cache *thiz)
{
  sns_tcc_free_resources(thiz);
  thiz->trigger_ids_arraylen = 0;
  thiz->cache_count = 0;
  thiz->cur_index = -1;
  thiz->total_trigger_count = 0;
}