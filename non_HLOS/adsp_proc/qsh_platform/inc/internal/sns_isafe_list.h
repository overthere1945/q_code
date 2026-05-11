#pragma once
/** ===========================================================================
 * @file
 *
 * @brief A simple linked list implementation for use by the Sensors Framework.
 * Iterators (and their associated API functions) are required for all access
 * and modifications to the list.  Any number of iterators may be simultaneously
 * active for a list.  Adding items to a list is guaranteed synchronous and
 * thread-safe.  Removing items from a list may block if the item is in-use
 * by another iterator.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "sns_rc.h"

/*=============================================================================
  Forward Declarations
  ===========================================================================*/
struct sns_osa_lock;
struct sns_isafe_list_item;

/*=============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * @brief A single entry in a list.
 *
 */
typedef struct sns_isafe_list_item
{
  void *item;
  struct sns_isafe_list_item *next; /*!< Pointer to subsequent item on
                                     *   the list; nullptr if the head
                                     */
  struct sns_isafe_list_item *prev; /*!< Pointer to previous item on the
                                     *   list; nullptr if the tail
                                     */
  struct sns_isafe_list *list;      /*!< Reference to the containing list;
                                     *   nullptr if not on any list
                                     */
} sns_isafe_list_item;

/**
 * @brief A doubly-linked list.
 *
 */
typedef struct sns_isafe_list
{
  sns_isafe_list_item item; /*!< Contains head (item::next) and tail
                             * (item::prev) of the list; item::item
                             * always NULL
                             */
  uint32_t cnt;             /*!< Number of entries presently collected
                             *   within this list
                             */
} sns_isafe_list;

/**
 * @brief An accessor class for the List.  All mutator and accessor actions
 * must be done through the Iterator interface.
 *
 */
typedef struct sns_isafe_list_iter
{
  struct sns_isafe_list_item *list_entry; /*!< Entry is used by the list to
                                           *   manage this iterator
                                           */
  struct sns_isafe_list_item *curr;       /*!< Current Entry of this iterator.
                                           *   May be NULL.
                                           */
  struct sns_isafe_list *list;            /*!< List over which this iterates */
  bool check_island;                      /*!< Check for island ptrs */
  bool always_check_island;               /*!< Always check for island ptrs*/
} sns_isafe_list_iter;

/*=============================================================================
  Public Functions
  ===========================================================================*/

/**
 * @brief Initialize a list to its empty state.
 *
 * @param[in] list           This List.
 *
 * @return
 *   - SNS_RC_SUCCESS: Always.
 *
 */
sns_rc sns_isafe_list_init(struct sns_isafe_list *list);

/**
 * @brief Initialize a list item.
 *
 * @param[in] item          Item in this List.
 * @param[in] data          Item initialization data.
 *
 * @return
 * -  SNS_RC_SUCCESS: Always.
 *
 */
sns_rc sns_isafe_list_item_init(sns_isafe_list_item *item, void *data);

/**
 * @brief Remove an item from a list.
 *
 * @param[in] item     List item to be removed if present on a list.
 *
 * @return
 *  - None.
 *
 */
void sns_isafe_list_item_remove(sns_isafe_list_item *item);

/**
 * @brief Return a pointer to the data associated with this list item.
 *
 * @param[in] item       Item in this List.
 *
 * @return
 *   - Pointer to Data associated with this List Item.
 *
 */
void *sns_isafe_list_item_get_data(struct sns_isafe_list_item *item);

/**
 * Check whether this item is presently on any list.
 *
 * @param[in] item Item whose list presence to check.
 *
 * @return
 *   - True:          If on any list.
 *   - False:         Otherwise.
 *
 */
bool sns_isafe_list_item_present(struct sns_isafe_list_item const *item);

/**
 * Create an iterator for this List.
 *
 * @param[in] iter Iterator for this List.
 * @param[in] list This List.
 * @param[in] head Initialize iterator to first item in the list; otherwise last
 *
 * @return
 *   - First item if head is True.
 *   - Last Item if head is False.
 *   - NULL if list is empty.
 *
 */
sns_isafe_list_item *sns_isafe_list_iter_init(sns_isafe_list_iter *iter,
                                              struct sns_isafe_list *list,
                                              bool head);

/**
 * @brief Add this item to the specified List; advance iterator only if
 * the List was previously empty.
 *
 * @param[in] iter    Iterator for this List.
 * @param[in] item    Item to be inserted in this list
 * @param[in] after   If true insert after current item, false enter before.
 *
 * @return
 *  - SNS_RC_SUCCESS: If the insertion succeeded.
 *  - SNS_RC_FAILED:  If the item already exists on some other list.
 *
 */
sns_rc sns_isafe_list_iter_insert(sns_isafe_list_iter *iter,
                                  struct sns_isafe_list_item *item, bool after);

/**
 * @brief Remove the current Entry.  Advance iterator to next; if next is NULL,
 * advance to prev.
 *
 * @param[in] iter Iterator for this List.
 *
 * @return
 *  - sns_isafe_list_item:   Item removed from this List.
 *
 */
sns_isafe_list_item *sns_isafe_list_iter_remove(sns_isafe_list_iter *iter);

/**
 * @brief Return the current length of the associated List.
 *
 * @param[in] iter    Iterator for this List.
 *
 * @return
 *   - uint32_t:       Length of this List.
 *
 */
uint32_t sns_isafe_list_iter_len(sns_isafe_list_iter *iter);

/**
 * @brief Return the current list_item. Will be NULL if current item is invalid.
 * @example If the list is empty, or has iterated past the head/tail.
 *
 * @param[in] iter    Iterator for this List.
 *
 * @return
 *   - sns_isafe_list_item:   Current item in this List.
 *
 */
struct sns_isafe_list_item *sns_isafe_list_iter_curr(sns_isafe_list_iter *iter);

/**
 * @brief Advance the iterator to the next item in the list.
 *
 * @param[in] iter      Iterator for this List.
 *
 * @return
 *  - sns_isafe_list_item:   Next item in this List.
 *
 */
struct sns_isafe_list_item *
sns_isafe_list_iter_advance(sns_isafe_list_iter *iter);

/**
 * @brief Return the iterator to the previous item in the list.
 *
 * @param[in] iter Iterator for this List.
 *
 * @return
 *   - sns_isafe_list_item:  Previous item in this List.
 *
 */
struct sns_isafe_list_item *
sns_isafe_list_iter_return(sns_isafe_list_iter *iter);

/**
 * Set if iterator must always check for island ptrs
 *
 * @brief A list might consist of elements that are located in
 *        island memory and outside island memory. To safely
 *        iterate over the list, multiple checks are performed
 *        to determine if an exit from island mode is required
 *        to proceed with a list operation.
 *        When the system exits from island mode, these pointer
 *        checks will no longer be performed for an iterator
 *        unless the iterator is reinitialized when the system
 *        is in island mode.
 *        Some iterators can remain initialized through multiple
 *        entries and exits from island mode. For these
 *        iterators, it is never safe to skip performing the
 *        pointer checks. Other iterators are known to be always
 *        accessed outside island mode. Here it is always safe
 *        to skip performing the pointer checks.
 *        This function provides a mechanism to either always
 *        perform pointer checks (always_check = true) or to
 *        always skip the pointer checks (always_check = false).
 *        This setting remains valid until the iterator is
 *        reinitialized.
 *
 * @param[in] iter          The iterator.
 * @param[in] always_check  If the iterator must always check ptrs.
 *
 * @return
 *  - None.
 *
 */
void sns_isafe_list_iter_set_always_check_island_ptrs(sns_isafe_list_iter *iter,
                                                      bool always_check);

/**
 * @brief Return a pointer to the data associated with the current list item.
 * This is equivalent to the operation:
 *  @code 
 *   sns_isafe_list_item_get_data(sns_isafe_list_iter_curr(iter)) 
 *  @endcode
 * 
 *
 * @param[in] iter         Iterator for this List.
 *
 * @return
 *   - Data pointer to the current item in this List.
 *
 */
void *sns_isafe_list_iter_get_curr_data(sns_isafe_list_iter *iter);
