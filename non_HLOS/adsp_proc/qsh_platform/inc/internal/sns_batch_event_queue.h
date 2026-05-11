#pragma once
/** ===========================================================================
 * @file
 *
 * @brief The SNS batch event queue is designed to store simplified or
 * reduced events.The queue format is header-events-header2-events2.Headers
 * are used to mark the beginning of new messages. They also contain information
 * necessary to reconstruct the actual events.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/

/*=============================================================================
  Include files
  ===========================================================================*/

#include "sns_batch_mem_block.h"
#include "sns_sensor_event.h"
#include "sns_sl_list.h"
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Check if BEQ is a data event msg ID
 *
 * @param x Message ID
 */
#define IS_BEQ_DATA_EVENT_MSGID(x) ((x >= 1024) && (x <= 1536))

/**
 * @brief The reconstructed event includes a message ID and actual timestamp,
 * differing from sns_sensor_event as the event is represented as a pointer.
 * This format allows for efficient event reconstruction using the provided
 * headers.
 *
 */
typedef struct _sns_beq_event
{
  uint64_t timestamp;  /*!< Timestamp of the event.*/
  uint32_t message_id; /*!< Message ID of the event.*/
  uint32_t event_len;  /*!< Length of the event.*/
  uint64_t *event;     /*!< Pointer to the event.*/
} sns_beq_event;

/**
 * @brief This structure represents the reduced version of the original event
 * which contains just the required fields.
 *
 */
typedef struct sns_beq_reduced_event
{
  uint32_t offset_ts; /*!< Time offset. */
  uint32_t event[1];  /*!< Event. */
} sns_beq_reduced_event;

/**
 * @brief This structure represents each element in the queue.
 *
 */
typedef struct sns_beq_item
{
  sns_sl_list_item list_item;          /*!< List item for the linked list. */
  sns_beq_reduced_event reduced_event; /*!< Reduced event. */
} sns_beq_item;

/**
 * @brief This structure holds number of bytes and blocks freed after removing
 * all the events.
 *
 */
typedef struct sns_beq_remove_events_rc
{
  uint16_t header_bytes_freed; /*!< Header bytes freed.*/
  uint16_t block_count;        /*!< Block count freed.*/
} sns_beq_remove_events_rc;

/**
 * @brief Header is a special event with metadata for the events that follow
 * header denotes start of an event of another message_id
 * header will be inserted in these scenerios.
 * 1. An Event with a different message id than current header.
 * 2. An Event with a different event length than current header.
 * 3. An Event with a ts_offset greater than uint32_max.
 *
 */
typedef struct _sns_beq_header
{
  sns_sl_list_item list_item; /*!< Linked list item. */
  uint64_t start_ts;          /*!< Start time of the event. */
  uint32_t message_id;        /*!< Message ID of the event. */
  uint32_t event_count;       /*!< Number of events following this header.*/
  uint32_t event_len;         /*!< Length of the event. */
  bool first_event_sent;      /*!< Indicates if it's the first event sent.*/
  sns_beq_reduced_event reduced_event; /*!< Reduced event. */
} sns_beq_header;

/**
 * @brief This structure represents the entire queue.
 * It consists of two lists: one for storing events and other for storing
 * headers.
 *
 */
typedef struct sns_batch_event_queue
{
  sns_sl_list events;                  /*!< List of events.*/
  sns_sl_list headers;                 /*!< List of headers.*/
  sns_batch_mem_block_list block_list; /*!< Block list used to store
                                        *   these events.
                                        */
  sns_beq_header *tail_header;         /*!< Tail header.*/

  size_t count;
} sns_batch_event_queue;

/**
 * @brief Header is a special event with metadata for the events that follow.
 *
 */
typedef struct _sns_beq_iter
{
  sns_sl_list_iter events_iter;  /*!< Actual list iterator pointing
                                  *   to the events.
                                  */
  sns_sl_list_iter headers_iter; /*!< Actual list iterator pointing
                                  *   to the headers.
                                  */
  uint64_t start_ts;             /*!< Start time of the current item. */
  uint32_t message_id;           /*!< Message Id for the current item. */
  sns_batch_event_queue *queue;  /*!< Queue pointer of this iterator */
  uint16_t event_len;            /*!< Event Len. */
  uint16_t events_in_header;     /*!< Events remaining with this
                                  *   message id from this point.
                                  */
  bool is_header_node; /*!< Flag to hold if iter is pointing at header node */
  bool event_iter_initialized; /*!< Flag to check if event iter is
                                *   initialized.
                                */
} sns_beq_iter;

/*=============================================================================
  Batch Event List public functions
  ===========================================================================*/

/**
 * @brief Initialize queue to its empty state.
 *
 * @param[in] queue This queue.
 *
 * @return
 * - SNS_RC_SUCCESS:
 *
 */
sns_rc sns_beq_init(sns_batch_event_queue *queue);

/**
 * @brief Initialize a queue item.
 *
 * @param[in] item List entry item in this queue.
 * @param[in] data Data in this item.
 *
 * @return
 * - SNS_RC_SUCCESS:
 *
 */
sns_rc sns_beq_item_init(sns_beq_item *item, void *const data);

/**
 * @brief Create an iterator for this queue.
 *
 * @param[in] iter  Iterator for this queue.
 * @param[in] queue This queue.
 * @param[in] head  Initialize iterator to first item in the queue; otherwise
 *                  last.
 *
 * @return
 *   - First item: If head is True.
 *   - Last Item:  If head is False.
 *   - NULL:       If queue is empty.
 *
 */
sns_beq_item *sns_beq_iter_init(sns_beq_iter *iter,
                                sns_batch_event_queue *queue, bool head);

/**
 * @brief Advance the iterator to the next item.Remain at NULL, if iterator
 * already reached the end.
 *
 * @param[in] iter Iterator.
 *
 * @return
 *   - Next item in the queue.
 *
 */
void *sns_beq_iter_advance(sns_beq_iter *iter);

/**
 * @brief Return the current length of the associated queue.
 *
 * @param[in] iter Iterator.
 *
 * @return
 *   - uint32_t:  Length of this queue.
 *
 */
uint32_t sns_beq_iter_len(sns_beq_iter *iter);

/**
 * @brief Insert Header into the queue.
 *
 * @param[in] iter         Iterator for this queue.
 * @param[in] header       Header to be inserted.
 * @param[in] actual_event Event to be inserted.
 *
 * @return
 *   - SNS_RC_SUCCESS:
 *
 */
sns_rc sns_beq_iter_insert_header(sns_beq_iter *iter, sns_beq_header *header,
                                  const sns_beq_event *const actual_event);

/**
 * @brief Insert Event into the queue.
 *
 * @param[in] iter         Iterator for this queue.
 * @param[in] event        Event to be inserted.
 * @param[in] actual_event Event to be inserted.
 *
 * @return
 *   - SNS_RC_SUCCESS:
 */
sns_rc sns_beq_iter_insert_event(sns_beq_iter *iter, sns_beq_item *event,
                                 const sns_beq_event *const actual_event);

/**
 * @brief Check whether header insertion is needed before inserting an event.
 *
 * @param[in] iter  Iterator for this queue.
 * @param[in] event Event to be inserted.
 *
 * @return
 *  - True:      If header needs to be inserted.
 *  - False:     If only event suffices.
 *
 */
bool sns_beq_iter_is_header_required(sns_beq_iter *iter,
                                     const sns_sensor_event *const event);

/**
 * @brief Get Current Event from the queue.
 *
 * @param[in] iter      Iterator for this queue.
 * @param[in] event_out Event to be populated.
 *
 * @return
 *   - SNS_RC_SUCCESS:   If event was populated.
 *
 */
sns_rc sns_beq_iter_get_curr_event(sns_beq_iter *iter,
                                   sns_beq_event *event_out);

/**
 * @brief Remove data events of the specified timestamp.
 *
 * @param[in] iter   Iterator for this queue. After removal this iter will
                     will be pointing to tail of the queue.
 * @param[in] min_ts Events older than this timestamp will be removed.
 *
 * @return
 *  - uint32_t:       Number of events removed.
 *
 */
uint32_t sns_beq_iter_remove_data_events(sns_beq_iter *iter, sns_time min_ts);

/**
 * @brief Remove n events from the queue.
 *
 * @param[in] iter  Iterator for this Queue. After removal this iter will
                    will be pointing to tail of the queue.
 * @param[in] count Number of elements that should be removed from this queue.
 *
 * @return
 *  - uint32_t:      Number of events removed.
 *
 */
uint32_t sns_beq_iter_remove_n_events(sns_beq_iter *iter, size_t count);

/**
 * @brief Remove all events from the queue.
 *
 * @param[in] queue          The queue.
 * @param[in] tail_iter      Tail iterator for this queue.
 * @param[out] retval_out    Bytes and Blocks that were freed as a result of
 *                           removal.
 *
 * @return
 *   - None.
 */
void sns_beq_remove_all_events(sns_batch_event_queue *queue,
                               sns_beq_iter *tail_iter,
                               sns_beq_remove_events_rc *retval_out);

/**
 * @brief Convert sensor event to queued event.
 *
 * @param[in] event        Sensor event to be convert to sns_beq_event.
 * @param[out] event_out   Queued event with data from sns_sensor_event.
 *
 * @return
 *   - None.
 *
 */
void sns_convert_event_to_beq_event(sns_sensor_event *event,
                                    sns_beq_event *event_out);