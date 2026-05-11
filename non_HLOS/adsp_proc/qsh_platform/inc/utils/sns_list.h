#pragma once
/** ============================================================================
 * @file
 *
 * @brief A simple linked list implementation for use by the Sensors Framework.
 * Iterators (and their associated API functions) are required for all access
 * and modifications to the list.  Any number of iterators may be simultaneously
 * active for a list.  Adding items to a list is guaranteed synchronous and
 * thread-safe.  Removing items from a list may block if the item is in-use
 * by another iterator.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "sns_rc.h"

/*==============================================================================
  Forward Declarations
  ============================================================================*/
struct sns_osa_lock;
struct sns_list_item;

/*=============================================================================
  Type Definitions
  ===========================================================================*/

typedef struct sns_list_item
{
  void *item; /*!< Pointer to current selected item on the
               *   list.
               */

  struct sns_list_item *next; /*!< Pointer to subsequent item on the list;
                               *   nullptr if the head.
                               */

  struct sns_list_item *prev; /*!< Pointer to previous item on the list;
                               *   nullptr if the tail.
                               */

  struct sns_list *list; /*!< Reference to the containing list; nullptr
                          *   if not on any list.
                          */
} sns_list_item;

typedef struct sns_list
{
  sns_list_item item; /*!< Contains head (item::next) and tail (item::prev) of
                       *   the list; item::item always NULL.
                       */

  uint32_t cnt; /* Number of entries presently collected within this
                 * list.
                 */
} sns_list;

/**
 * @brief An accessor class for the List.  All mutator and accessor actions
 * must be done through the Iterator interface.
 *
 */
typedef struct sns_list_iter
{
  struct sns_list_item *list_entry; /*!< Entry is used by the list to manage
                                     *   this iterator.
                                     */

  struct sns_list_item *curr; /*!< Current Entry of this iterator. May be
                               *   NULL.
                               */

  struct sns_list *list; /*!< List over which this iterates.
                          */
} sns_list_iter;

/*==============================================================================
  Public Functions
  ============================================================================*/

/**
 * @brief Initialize a list to its empty state.
 *
 * @param[in] list This List.
 *
 * @return
 * - sns_rc SNS_RC_SUCCESS
 *
 */
sns_rc sns_list_init(struct sns_list *list);

/**
 * @brief Initialize a list item.
 *
 * @param[in] item Item in this List.
 * @param[in] data Item initialization data.
 *
 * @return
 * - sns_rc SNS_RC_SUCCESS.
 * - Any other value  Failure.
 *
 */
sns_rc sns_list_item_init(sns_list_item *item, void *data);

/**
 * @brief Remove an item from a list.
 *
 * @param[in] item List item to be removed if present on a list.
 *
 * @return
 * - None.
 *
 */
void sns_list_item_remove(sns_list_item *item);

/**
 * @brief Return a pointer to the data associated with this list item.
 *
 * @param[in] item Item in this List.
 *
 * @return
 * - void *ptr - Data associated with this List Item.
 *
 */
void *sns_list_item_get_data(struct sns_list_item *item);

/**
 * @brief Check whether this item is presently on any list.
 *
 * @param[in] item Item whose list presence to check.
 *
 * @return
 * - bool - True if on any list; false otherwise.
 *
 */
bool sns_list_item_present(struct sns_list_item const *item);

/**
 * @brief Create an iterator for this List.
 *
 * @param[in] iter Iterator for this list.
 * @param[in] list This list.
 * @param[in] head Initialize iterator to first item in the list;
 * otherwise last.
 *
 * @return
 * - sns_list_item *ptr - First item if head is True. Last Item if head is
 * false.
 * - NULL if list is empty.
 *
 */
sns_list_item *sns_list_iter_init(sns_list_iter *iter, struct sns_list *list,
                                  bool head);

/**
 * @brief Add this item to the specified List; advance iterator only if
 * the List was previously empty.
 *
 * @param[in] iter Iterator for this list.
 * @param[in] item Item to be inserted in this list.
 * @param[in] after If true insert after current item, false enter before.
 *
 * @return
 * - SNS_RC_SUCCESS - Added item to list sucessfully.
 * - SNS_RC_FAILED  - Item is already present in list.
 *
 */
sns_rc sns_list_iter_insert(sns_list_iter *iter, struct sns_list_item *item,
                            bool after);

/**
 * @brief Remove the current Entry.  Advance iterator to next; if next is NULL,
 * advance to prev.
 *
 * @param[in] iter Iterator for this list.
 *
 * @return
 * - sns_list_item *ptr - Item removed from this list.
 *
 */
sns_list_item *sns_list_iter_remove(sns_list_iter *iter);

/**
 * @brief Return the current length of the associated list.
 *
 * @param[in] iter Iterator for this list.
 *
 * @return
 * - uint32_t - Length of this list.
 *
 */
uint32_t sns_list_iter_len(sns_list_iter *iter);

/**
 * @brief Return the current list_item.  Will be NULL if current item is
 * invalid. E.g. If the list is empty, or has iterated past the head/tail.
 *
 * @param[in] iter Iterator for this list.
 *
 * @return
 * - struct sns_list_item *ptr - Current item in this list.
 *
 */
struct sns_list_item *sns_list_iter_curr(sns_list_iter *iter);

/**
 * @brief Advance the iterator to the next item in the list.
 *
 * @param[in] iter Iterator for this list.
 *
 * @return
 * - struct sns_list_item *ptr - Next item in this list.
 *
 */
struct sns_list_item *sns_list_iter_advance(sns_list_iter *iter);

/**
 * @brief Return the iterator to the previous item in the list.
 *
 * @param[in] iter Iterator for this list.
 *
 * @return Previous item in this list.
 * - struct sns_list_item *ptr - Previous item in this list.
 *
 */
struct sns_list_item *sns_list_iter_return(sns_list_iter *iter);

/**
 * @brief Return a pointer to the data associated with the current list item.
 * This is equivalent to the operation:
 * sns_list_item_get_data(sns_list_iter_curr(iter)).
 *
 * @param[in] iter Iterator for this list.
 *
 * @return
 * - void *ptr - Data pointer to the current item in this list.
 *
 */
void *sns_list_iter_get_curr_data(sns_list_iter *iter);
