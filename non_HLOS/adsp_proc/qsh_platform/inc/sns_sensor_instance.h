#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Represents a specific instance of a Sensor.  Each Sensor may have 0 or
 * more instances active at any given time.  The Sensor determines when multiple
 * client requests may be shared; it is expected to share aggressively.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdint.h>
#include "sns_rc.h"
#include "sns_request.h"
#include "sns_sensor.h"

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_request;
struct sns_sensor;
struct sns_service_manager;

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief Structure for the instance created by a sensor.
 *
 */
typedef struct sns_sensor_instance
{
  /**
   * @brief Functions which call back into the framework; provided by the
   * framework.
   *
   */
  struct sns_sensor_instance_cb const *cb;

  /**
   * @brief Fixed-size memory allocation made by the framework from the power
   * efficient heap appropriate for this Sensor.  Memory is for the sole use of
   * the Sensor Instance. Memory may be moved between Sensor registered function
   * calls. Therefore direct references should not be maintained.
   *
   */
  struct sns_sensor_instance_state *state;

  /**
   * @brief Fixed-sized memory allocation made by the framework from the
   * Non-Island, high power memory heap.  May not be dereferenced unless
   * Instance created via sns_sensor_cb::create_instance_v2.
   * Developer is responsible for invoking
   * sns_island_service::sensor_instance_island_exit before accessing.
   *
   */
  struct sns_sensor_instance_state *state_ni;
} sns_sensor_instance;

/**
 * @brief Accessor functions for Sensor Instance state managed by the Framework.
 *
 */
typedef struct sns_sensor_instance_cb
{
  uint32_t struct_len;

  /**
   * @brief Get a reference to the Service Manager.  With this object, a
   * reference to any other utility service can be obtained.
   *
   * @param[in] instance Sensor Instance reference.
   *
   * @return
   *  - sns_service_manager*  Service Manager reference.
   *
   */
  struct sns_service_manager *(*get_service_manager)(
      sns_sensor_instance *instance);

  /**
   * @brief Return the next client request associated with this Sensor Instance
   * and SUID.
   *
   * @details Each Sensor Instance has a list of client requests per SUID which
   * it is servicing.  Entries are added via calls to add_client_request;
   * removed via remove_client_request.
   * An Instance may be handling client requests for multiple
   * (related) Sensors; must use SUID parameter to filter.
   *
   * @param[in] instance  Sensor Instance reference.
   * @param[in] suid      Suid of the sensor associated with this instance.
   * @param[in] first     Return the first request; reset the internal iterator
   *                      Must be called first to initialize iteration.
   *
   * @return
   *  - sns_request const* Each call to this function iterates over the list,
   *                       and returns the next entry.
   *  - NULL               At the end of the list, or if the list is empty.
   *
   */
  struct sns_request const *(*get_client_request)(sns_sensor_instance *instance,
                                                  sns_sensor_uid const *suid,
                                                  bool first);

  /**
   * @brief Remove a client request from this Sensor Instance.
   *
   * @param[in] instance  Sensor Instance reference.
   * @param[in] request   Client request to be removed.
   *
   * @return
   *  - None.
   *
   */
  void (*remove_client_request)(sns_sensor_instance *instance,
                                struct sns_request const *request);

  /**
   * @brief Assign this Sensor Instance to service the client request. Replaces
   * any existing request from the same client.
   *
   * @note The SUID of the recepient Sensor will be noted upon addition;
   * this SUID must be used within get_client_request.
   *
   * @param[in] instance  Sensor Instance reference.
   * @param[in] request   Client request to be added.
   *
   * @return
   *  - None.
   *
   */
  void (*add_client_request)(sns_sensor_instance *instance,
                             struct sns_request const *request);
} sns_sensor_instance_cb;

/**
 * @brief Sensor Instance API; to be implemented by the Sensor developer.
 *
 */
typedef struct sns_sensor_instance_api
{
  uint32_t struct_len; /*!< Length of sns_sensor_instance_api structure. */

  /**
   * @brief Initialize a Sensor Instance to its default state. A call to
   * sns_sensor_instance_api::deinit will precede any subsequent calls to this
   * function.
   *
   * @note Persistent configuration can be made available using the
   * sensor_state.
   *
   * @param[in] instance      Sensor Instance reference.
   * @param[in] sensor_state  State of the Sensor which created this Instance.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*init)(sns_sensor_instance *const instance,
                 sns_sensor_state const *sensor_state);

  /**
   * @brief Release all hardware and software resources associated with this
   * Sensor Instance.
   *
   * @param[in] instance Sensor Instance reference.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success
   *  - Any other value  Failure
   *
   */
  sns_rc (*deinit)(sns_sensor_instance *const instance);

  /**
   * @brief Update a Sensor Instance configuration to this client request. The
   * Sensor Instance is expected to start all dependent streams, timers, etc..
   *
   * @note A Sensor may define any number of unique request types they support.
   * However, a client may only have a single active stream; an enable request
   * can inherently serve as a "reconfiguration" request.
   *
   * @param[in] instance        Sensor Instance reference.
   * @param[in] client_request  Client request reference.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*set_client_config)(sns_sensor_instance *const instance,
                              struct sns_request const *client_request);

  /**
   * @brief Notification to the client that some data has been received. The
   * client must use the sns_event_service to obtain this data for processing.
   *
   * @param[in] instance    Sensor Instance reference.
   *
   * @return
   * - SNS_RC_SUCCESS       The sensor instance handled the event(s), and is in
   *                        an operational state. This return code should be
   *                        used if a recoverable  error occurs (such as
   *                        recoverable message decode errors, etc).
   * - SNS_RC_INVALID_STATE Sensor instance is in bad or invalid state and is
   *   unable to recover. Framework will start removing this instance upon
   *   receiving this error.
   * - SNS_RC_NOT_AVAILABLE - Sensors should return this error if certain
   *   operation cann't be completed at this time. Upon receiving this error
   *   type, framework will drop all the events from all data streams except
   *   most recent SNS_STREAM_REMAINDER number of events.
   * - For all other errors, Framework will deinit the sensor which created this
   *   instance.
   *
   */
  sns_rc (*notify_event)(sns_sensor_instance *const instance);
} sns_sensor_instance_api;

/**
 * @brief State specifically allocated for the Sensor Instance, to be used by
 * the Sensor developer as needed.  May be relocated; no pointers into this
 * buffer may be saved.
 *
 */
typedef struct sns_sensor_instance_state
{
  uint32_t state_len; /*!< Length of sensor instance state. */
  uint64_t state[1];  /*!< Sensor instance state. */
} sns_sensor_instance_state;
