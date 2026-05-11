#pragma once
/** ============================================================================
 * @file
 *
 * @brief Data stream represents a unique connection between a client sensor and
 * a source sensor. The client sensor is responsible for creating, maintaining
 * and removing the data stream.
 * After successful creation of the data stream, it can be used by the client
 * sensor to send requests to and receive events from the source sensor.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <inttypes.h>
#include "sns_request.h"
#include "sns_sensor_event.h"

/*==============================================================================
  Type Definitions
  ============================================================================*/
/**
 * @brief Data Stream Manager.
 *
 */
typedef struct sns_data_stream
{

  const struct sns_data_stream_api *api; /*!< Data Stream APIs. */
} sns_data_stream;

/**
 * @brief Data Stream APIs.
 *
 */
typedef struct sns_data_stream_api
{

  uint32_t struct_len; /*!< Length of sns_data_stream_api structure. */

  /**
   * @brief Sends a request to the source Sensor. This request may
   * update or replace the existing request, depending on the Sensor
   * specification.
   *
   * @param[inout] data_stream  Data stream on which to send the request.
   * @param[in]    request      Request to be sent; Framework will copy request.
   *
   * @return
   *  - SNS_RC_SUCCESS          Success.
   *  - SNS_RC_INVALID_STATE    Stream is no longer available; create again.
   *
   */
  sns_rc (*send_request)(sns_data_stream *data_stream, sns_request *request);

  /**
   * @brief Send a flush request to the source Sensor.
   * This is a helper function; clients may also initiate a flush
   * by generating a flush request message, and sending it via send_request.
   *
   * @param[inout] data_stream Data stream on which to initiate the flush.
   *
   * @return
   *  - SNS_RC_SUCCESS          Success.
   *  - SNS_RC_INVALID_STATE    Stream is no longer available; create again.
   *
   */
  sns_rc (*initiate_flush)(sns_data_stream *data_stream);

  /**
   * @brief Retrieve a pointer to the oldest unprocessed event associated with
   * this data stream from the event queue.
   *
   * @note Multiple sequential calls to this function will return the same
   * event pointer.
   *
   * @param[inout] data_stream Data stream from which to get an event
   *
   * @return
   *  - sns_sensor_event*   Oldest unprocessed event on the queue.
   *  - NULL                if no events remain.
   *
   */
  sns_sensor_event *(*peek_input)(sns_data_stream *data_stream);

  /**
   * @brief Remove the current event from the event queue (the event that would
   * be returned via peek_input). Return the next unprocessed event from the
   * event queue.
   * Once this function returns, there is no means to retrieve the removed
   * Event again; the data has been freed, and its memory should not be
   * accessed.
   *
   * @param[inout] data_stream Data stream from which to get an event
   *
   * @return
   *  - sns_sensor_event* The next oldest unprocessed event on the queue
                          (after the removal occurs).
   *  - NULL              if no further events remain.
   *
   */
  sns_sensor_event *(*get_next_input)(sns_data_stream *data_stream);

  /**
   * @brief Lookup the current number of unprocessed events on this data stream.
   * This value may change at any time, and should not be treated as precise.
   *
   * @note Do no rely on this value to assume valid input from peek_input.
   *
   * @param[inout] data_stream Data stream from which to get the input count.
   *
   * @return
   *  - uint32_t Number of unprocessed events on this data stream.
   *
   */
  uint32_t (*get_input_cnt)(sns_data_stream *data_stream);
} sns_data_stream_api;
