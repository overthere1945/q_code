#pragma once
/** ===========================================================================
 * @file
 *
 * @brief A single linked list interface.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

#include "sns_rc.h"
#include "sns_island.h"
#include "sns_assert.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/*=============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * @brief List item consists of only a next pointer. Note that an item pointer
 * is not added here to reduce memory footprint. Any structure that needs
 * to be inserted into a sl_list, is required to keep this sns_sl_list_item
 * as the first item in the structure.
 *
 */
typedef struct sns_sl_list_item
{
  struct sns_sl_list_item *next; /*!< Pointer to next item on the list;
                                  *    nullptr if the tail
                                  */
} sns_sl_list_item;

/**
 * @brief Contains head tail and count of the singly linked list.
 *
 */
typedef struct sns_sl_list
{
  sns_sl_list_item *head; /*!< Pointer to Head of the list; may be null */
  sns_sl_list_item *tail; /*!< Pointer to Tail of the list; may be null */
  uint32_t cnt;           /*!< Number of entries presently collected
                           *    within this list
                           */
} sns_sl_list;

/**
 * @brief An accessor structure for the singly linked List.
 * All mutator and accessor actions to a sl_list
 * must be done through the Iterator interface.
 *
 */
typedef struct sns_sl_list_iter
{
  sns_sl_list_item *curr; /*!< Current Entry of this iterator. May be NULL. */
  sns_sl_list_item *prev; /*!< Previous Entry of this iterator. May be NULL. */
  sns_sl_list *list;      /*!< List over which this iterates */
} sns_sl_list_iter;

/*=============================================================================
  Single linked list public functions
  ===========================================================================*/

/**
 * @brief Initialize a list to its empty state.
 *
 * @param[in] list   This list.
 *
 * @return
 *  - SNS_RC_SUCCESS:
 *
 */
sns_rc sns_sl_list_init(sns_sl_list *list);

/**
 * @brief Initialize a list item.
 *
 * @param[in] item List entry item in this list
 * @param[in] data Data in this list item
 *
 * @return
 *   - SNS_RC_SUCCESS:
 *
 */
sns_rc sns_sl_list_item_init(sns_sl_list_item *item, void *data);

/**
 * @brief Create an iterator for this List.
 *
 * @param[in] iter     Iterator for this list.
 * @param[in] list     This list.
 * @param[in] head     Initialize iterator to first item in the list;
 *                     otherwise last.
 *
 * @return
 *  - First item: If head is True.
 *  - Last Item:  If head is False.
 *  - NULL:       If list is empty.
 *
 */
sns_sl_list_item *sns_sl_list_iter_init(sns_sl_list_iter *iter,
                                        sns_sl_list *list, bool head);

/**
 * @brief Advance the iterator to the next item in the list.
 * Remain at NULL, if iterator already reached the end of list.
 *
 * @param[in] iter Iterator for this list
 *
 * @return
 *  - Next item in this list.
 *  - NULL:  If no more items are present in this list.
 *
 */
sns_sl_list_item *sns_sl_list_iter_advance(sns_sl_list_iter *iter);

/**
 * @brief Return the current list_item.Will be NULL if current item is invalid.
 * E.g. If the list is empty, or has iterated past the head/tail.
 *
 * @param[in] iter Iterator for this list.
 *
 * @return
 *  - Current item in this list.
 *
 */
sns_sl_list_item *sns_sl_list_iter_curr(sns_sl_list_iter *iter);

/**
 * @brief Add this item to the specified List; advance iterator only if
 * the List was previously empty.
 *
 * @param[in] iter Iterator for this list.
 * @param[in] item Item to be inserted in this list.
 * @param[in] after If true insert after current item, false enter before.
 *
 * @return
 *   - SNS_RC_SUCCESS
 *
 */
sns_rc sns_sl_list_iter_insert(sns_sl_list_iter *iter,
                               struct sns_sl_list_item *item, bool after);

/**
 * @brief Remove the current Entry.  Advance iterator to next; if end of list
 * is reached, Stay at the end.Remove after the end of the list is ignored.
 *
 * @param[in] iter Iterator for this list.
 *
 * @return
 *   - Item removed from this List.
 *
 */
sns_sl_list_item *sns_sl_list_iter_remove(sns_sl_list_iter *iter);

/**
 * @brief Return the current length of the associated List.
 *
 * @param[in] iter Iterator for this list.
 *
 * @return
 *  - uint32_t:  Length of this list.
 *
 */
uint32_t sns_sl_list_iter_len(sns_sl_list_iter *iter);

/**
 * @brief Return a pointer to the data associated with the current list item.
 *
 * @note This is equivalent to the operation:
 *   sns_sl_list_iter_curr(iter)
 *
 * @param[in] iter Iterator for this list.
 *
 * @return
 *  - Data pointer:  To the current item in this list.
 *
 */
void *sns_sl_list_iter_get_curr_data(sns_sl_list_iter *iter);