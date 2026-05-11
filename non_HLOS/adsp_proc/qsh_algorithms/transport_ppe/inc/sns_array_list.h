#pragma once
/*=============================================================================
  @file sns_array_list.h

  Contains function definitions required for array list implementation

  Copyright (c) 2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/**
Array list is a data structure that is used to emulate a resizable array using
a linked list of smaller arrays.
- This data structure takes care of allocation and freeing up of memory
- All array operations are valid in arraylist also
- Limits are capped at a realistic uint16_max

Implementation details:
This is a generic implementation of the array list,
User provides the size of an individual element in bytes
User is expected to take care of memory alignment
This size will be used to move around in the array

An array list consist of a linked list of mini-arrays.
The size of mini-arrays will be equal to count_per_block number of elements
User can continously insert into this array, when the mini-array runs out of
empty slots, arraylist will allocate another block and insert the new item
in the new block, until the count_limit is reached.

User can access elements using an index, as if it was an array. the array list
will perform the math to identify and return the item correctly.

Access will be a function of the number of blocks.O(block_count).
Block count is assumed to be small, hence almost constant time operation.

Insertion is a constant time operation


**/

#include "sns_memory_service.h"
#include "sns_sensor_instance.h"
#include "sns_list.h"
#include <stdint.h>

#define ARRAY_ITEM_INDEX_NOT_FOUND UINT32_MAX

typedef struct sns_array_list_block
{
  sns_list_item list_item;
  uint64_t mem_block[1];
} sns_array_list_block;

typedef struct sns_array_list_param
{
  uint32_t item_size;
  // Number elements required per block of allocation
  uint32_t count_per_block;
  // Limit to the number elements in the array list
  uint32_t count_limit;
} sns_array_list_param;

typedef struct sns_array_list
{
  // size of individual element in the array
  uint16_t item_size;
  // Number elements required per block of allocation
  uint16_t count_per_block;
  // Limit to the number elements in the array list
  uint16_t count_limit;
  // Index in the cur block where the new item should be inserted
  uint16_t cur_index;
  // Total Number of elements inserted in this array
  uint16_t count;
  // The linked list of small arrays
  sns_list blocks;

  sns_memory_service *mem_service;
  sns_sensor_instance *instance_pointer;
} sns_array_list;

/**
 * @name sns_array_list_init
 * @param thiz: the current al pointer
 * @param mem_service : dynamic memory manager
 * @param instance: Pointer to the instance using this al
 * @param param_in : array list parameters
 * @return void
 * @brief this function will initialize the array list with required params
 **/
sns_rc sns_array_list_init(sns_array_list *thiz, sns_memory_service *mem_service,
                           sns_sensor_instance *const instance, sns_array_list_param *param_in);

/**
 * @name sns_array_list_deinit
 * @param thiz: the current al pointer
 * @return void
 * @brief this function will free all the memory allocated for the array list
          and reset the variables in the array list structure
 **/
void sns_array_list_deinit(sns_array_list *thiz);

/**
 * @name sns_array_list_insert
 * @param thiz: the current al pointer
 * @param item: pointer to the item to be inserted
                if item is NULL, 0s are inserted
 * @return void* Pointer to the item just inserted
 * @brief this function will copy the item into the mini array
          if mini array has run out of all slots it will allocate new block
          insertion beyond count_limit will fail
          insertion fails if new block allocation fails
          user will have to take care of this failure

 **/
void *sns_array_list_insert(sns_array_list *thiz, void *item);

/**
 * @name sns_array_list_insert_at
 * @param thiz: the current al pointer
 * @param item: pointer to the item to be inserted
                 if item is NULL, 0s are inserted
 * @param index: index where the item should be inserted
 * @return void* Pointer to the item just inserted
 * @brief replace the item at the specified index with the new item provided
          if index is out of bounds, return NULL
 **/
void *sns_array_list_insert_at(sns_array_list *thiz, void *item, int index);

/**
 * @name sns_array_list_get_index
 * @param thiz: the current al pointer
 * @param item: pointer to the item whose index should be found
 * @return uint32_t index of the item, ARRAY_ITEM_INDEX_NOT_FOUND if not found
 * @brief
    For non primitive types, memcmp will be used to figure out equality
 **/
uint32_t sns_array_list_get_index(sns_array_list *thiz, void *item);

/**
 * @name sns_array_list_get_at
 * @param thiz: the current al pointer
 * @param index: Index from which the item should be accessed
 * @return void* pointer to the item at index
 **/
void *sns_array_list_get_at(sns_array_list *thiz, uint32_t index);

/**
 * @name sns_array_list_get_count
 * @param thiz: the current al pointer
 * @return Number of elements inserted in the array
 **/
size_t sns_array_list_get_count(sns_array_list *thiz);

/**
 * @name sns_array_list_remove_from_tail
 * @param thiz: the current al pointer
 * @param remove_count: Number of elements to be removed
 * @return Number of items in the array after remove
 **/
uint32_t sns_array_list_remove_from_tail(sns_array_list *thiz, uint32_t remove_count);