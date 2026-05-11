#pragma once
/*=============================================================================
  @file sns_tppe_crc_cache.h

  Contains function definitions required for Caching all sent item crc
  This is required by duplicate detection feature

  Copyright (c) 2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "sns_sensor_instance.h"
#include "sns_memory_service.h"
#include "sns_array_list.h"

#define MAX_CRC_CACHE_LEN_DEFAULT 100
#define MIN_CRC_CACHE_LEN         25
#define MAX_TRIGGER_PER_CRC       5
#define CRC_INDEX_NOT_FOUND       UINT32_MAX

typedef struct crc_type
{
  uint32_t crc;
} sns_tppe_crc_metadata;

typedef struct crc_cache
{
  int16_t cur_index;
  uint16_t total_trigger_count;
  uint16_t cache_count;
  uint16_t crc_cache_max_len;
  uint16_t trigger_ids_arraylen;
  sns_array_list unique_crcs;
  sns_array_list trigger_ids_array;
} sns_tppe_crc_cache;

/**
 * @name sns_tcc_init
 * @param thiz: the current crc cache instance
 * @param trigger_count: Number of triggers in this instance
 * @param max_crc_len: Number of crcs that can be stored
 * @param memory_service: Memory Service pointer for dynamic allocation
 * @param tppe_instance_pointer
 * @return void
 * @brief This function will allocate memory required for Unique crc array and
 *        trigger indices array
 **/
sns_rc sns_tcc_init(sns_tppe_crc_cache *thiz, uint32_t trigger_count, uint32_t max_crc_len,
                    sns_memory_service *const memory_service,
                    sns_sensor_instance *const tppe_instance_pointer);

/**
 * @name sns_tcc_get_crc_index
 * @param thiz: the current crc cache instance
 * @param crc: crc value whose index should be found
 * @return index of the crc in the unique crc array
 * @brief This function will loop through the crc array and returns the index
 *        of crc in the crc array. If not found, it will return uint32_max
 **/
uint32_t sns_tcc_get_crc_index(sns_tppe_crc_cache *thiz, uint32_t crc);

/**
 * @name sns_tcc_is_trigger_set
 * @param thiz: the current crc cache instance
 * @param crc_index: index of the crc in the crc_array
 * @param trigger_index: Index of the trigger in triggers array
 * @return true if trigger is set, false otherwise
 **/
bool sns_tcc_is_trigger_set(sns_tppe_crc_cache *thiz, uint32_t crc_index, uint32_t trigger_index);

/**
 * @name sns_tcc_set_trigger
 * @param thiz: the current crc cache instance
 * @param crc_index: index of the crc in the crc_array
 * @param trigger_index: Index of the trigger in triggers array
 * @return void
 * @brief This function will copy trigger index into an empty slot within
 *        trigger indices array. If no empty slot is present, trigger index is
 *        ignored
 **/
void sns_tcc_set_trigger(sns_tppe_crc_cache *thiz, uint32_t crc_index, uint32_t trigger_index);

/**
 * @name sns_tcc_insert_crc
 * @param thiz: the current crc cache instance
 * @param crc: the  crc to be inserted
 * @return index of the position where this crc is inserted
 * @brief This function will insert the CRC in the unique crc array
 *        All insertions will happen in a cyclic manner. Once the array is
 *        filled, all old items are at the beginning of the array. So replace
 *        those
 **/
uint32_t sns_tcc_insert_crc(sns_tppe_crc_cache *thiz, uint32_t crc);

/**
 * @name sns_tcc_clear_cache
 * @param thiz: the current crc cache instance
 * @return void
 * @brief This function will clear all crc from the array
 **/
void sns_tcc_clear_cache(sns_tppe_crc_cache *thiz);

/**
 * @name sns_tcc_deinit
 * @param thiz: the current crc cache instance
 * @return void
 **/
void sns_tcc_deinit(sns_tppe_crc_cache *thiz);

/**
 * @name sns_tcc_clear_cache_for_trigger
 * @param thiz: the current crc cache instance
 * @param trigger_index: trigger index for which cache items should be cleared
 * @return void
 **/
void sns_tcc_clear_cache_for_trigger(sns_tppe_crc_cache *thiz, uint32_t trigger_index);
