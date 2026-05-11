#pragma once
/** ============================================================================
 * @file
 *
 * @brief Handles the storage of all Sensor events. The ThreadManager is
 * notified when new events are available for processing.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdint.h>
#include "sns_rc.h"
#include "sns_service.h"

/*==============================================================================
  Forward Declarations
  ============================================================================*/

struct sns_sensor_instance;
struct sns_request;
struct sns_sensor_event;
struct sns_sensor_uid;

/*==============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief The Event Service. Will be obtained from
 * sns_service_manager::get_service.
 *
 */
typedef struct sns_event_service
{
  sns_service service;                     /*!< Service information. */
  const struct sns_event_service_api *api; /*!< Public API provided by the
                                            *   Framework to be used by the
                                            *   Sensor.
                                            */
} sns_event_service;

/**
 * @brief Event Service APIs.
 *
 */
typedef struct sns_event_service_api
{
  uint32_t struct_len; /*!< Length of sns_event_service_api structure. */

  /**
   * @brief Get the maximum event size supported by the Framework.
   *
   * @param[in] service Event Service reference.
   *
   * @return
   *  - uint32_t The maximum event size supported by the Framework.
   *
   */
  uint32_t (*get_max_event_size)(sns_event_service const *service);

  /**
   * @brief Allocate an empty event buffer.
   * Output will not be sent to clients until publish_event is called.
   * alloc_event may not be called again until publish_event is called.
   * From whichever Sensor Instance API called alloc_event, within
   * that same function block publish_event must be called. If the function
   * block returns without doing so, the Framework will free the buffer and
   * return it to the pool.
   *
   * @note
   * An output Event may consist of multiple logical samples.  These
   * samples will be consumed individually and sequentially by the
   * client.
   *
   * @param[in] service   Event Service reference.
   * @param[in] instance  Sensor Instance which will form/publish the event.
   * @param[in] event_len Buffer space to allocate for the Event.
   *
   * @return
   *  - sns_sensor_event*  All allocations are guaranteed to succeed unless
   *                       event_len exceeds get_max_buffer_size().
   *
   */
  struct sns_sensor_event *(*alloc_event)(sns_event_service *service,
                                          struct sns_sensor_instance *instance,
                                          uint32_t event_len);

  /**
   * @brief Publish an event, previously allocated by alloc_event, to all
   * registered clients. Client may specify the SUID of the data being sent.
   * This SUID must match the SUID of the Sensor to which a client sent its
   * enable request. Must be called within the same block as alloc_event.
   *
   * @param[in] service     Event Service reference.
   * @param[in] instance    Sensor Instance which formed the event.
   * @param[in] event       Event to publish to all registered clients.
   * @param[in] sensor_uid  SUID of this published data, if set to
   *                        NULL, framework determines suid from instance
   *                        parameter.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*publish_event)(sns_event_service *service,
                          struct sns_sensor_instance *instance,
                          struct sns_sensor_event *event,
                          struct sns_sensor_uid const *sensor_uid);

  /**
   * @brief Publish a generic error event.  This event will be delivered to all
   * active clients.  Upon receipt, the Framework will destroy this Sensor
   * Instance, and all associated state.
   * This is a helper function; an error event could also be
   * generated and published using the alloc_event/publish_event API.
   *
   * @param[in]   service   Event Service reference.
   * @param[in]   instance  Sensor Instance.
   * @param[in]   reason    - SNS_RC_INVALID_STATE: The Sensor Instance entered
   *                        an invalid state.
   *                        - SNS_RC_NOT_AVAILABLE: A software and/or hardware
   *                        dependency has been lost.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*publish_error)(sns_event_service *service,
                          struct sns_sensor_instance *instance, sns_rc reason);
} sns_event_service_api;
