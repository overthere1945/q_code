#pragma once
/** =============================================================================
 * @file sns_sensor_util.h
 *
 * @brief Utility functions for use by Sensor Developers.
 * All utilities in this file can be used in island mode.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 * ===========================================================================
 */

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_data_stream.h"
#include "sns_rc.h"
#include "sns_sensor_event.h"
#include "sns_sensor_instance.h"
#include "sns_sensor_uid.h"
#include "sns_registry_util.h"

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_data_stream;
struct sns_sensor;
struct sns_sensor_instance;
struct sns_request;
struct sns_sensor_instance_state;

typedef struct
{
  char *data_type_str;   /*!< input to SUID sensor */
  sns_sensor_uid *suid;  /*!< list of SUIDs returned from SUID sensor */
  uint32_t num_of_suids; /*!< number of items in suid list*/
} sns_suid_search;

typedef struct sns_timesync_event_info
{
  sns_sensor_event *event;
  int stream_index;
  bool pending;
} sns_timesync_event_info;

/**
 * @brief Callback function type definition for iterating over all instances of
 * a particular sensor.
 *
 * @param[in] this Instance pointer.
 * @param[in] arg  User-defined arguments.
 *
 * @return
 *  - SNS_RC_SUCCESS:      If successful.
 *  - SNS_RC_FAILED:       Otherwise
 *
 */
typedef sns_rc (*sns_do_for_all_instances_cb_func)(
    sns_sensor_instance *const this, void *const arg);

/**
 * @brief callback function used with sns_process_streams_in_timesync.
 *
 * @param[in] event:        A single event from one of the data streams
 * @param[in] stream_index  Index of the data stream associated with
 *                          this event
 * @param[in] userdata      Opaque userdata supplied by the caller
 */
typedef sns_rc (*process_stream_event)(sns_sensor_event *event,
                                       int stream_index, void *userdata);

/**
 * @brief Callback function used in sns_sensor_util_find_instance_match().
 *
 * @param[in] request The new incoming request which is looking for a home.
 * @param[in] an active Sensor Instance.
 *
 * @return
 *  -True   If request can be serviced by this Instance.
 *  -False  Otherwise.
 */
typedef bool (*sns_sensor_util_instance_match_cb)(
    struct sns_request const *request, sns_sensor_instance const *instance);

/*=============================================================================
  Public Function Declarations
  ===========================================================================*/
/**
 * @brief Removes a particular stream for a sensor and marks the stream as a
 * NULL.
 *
 * @param[in] sensor The Sensor for which the stream is to be removed
 * @param[in] stream The stream to be removed
 *
 * @return sns_rc SNS_RC_SUCCESS == If stream removed successfully, any other
 * value == Failure.
 */

sns_rc sns_sensor_util_remove_sensor_stream(sns_sensor *sensor,
                                            sns_data_stream **stream);

/**
 * @brief Removes a particular stream for a sensor instance and marks the stream
 * as NULL.
 *
 * @param[in] instance The sensor instance for which the stream is to be
 *                     removed.
 * @param[in] stream   The stream to be removed.
 *
 * @return sns_rc SNS_RC_SUCCESS == If stream removed successfully, any other
 * value == Failure.
 */

sns_rc
sns_sensor_util_remove_sensor_instance_stream(sns_sensor_instance *instance,
                                              sns_data_stream **stream);

/**
 * @brief Find the instance (if any) which is presently servicing this request.
 *
 * @param[in] sensor  The Sensor from which to search through
 * @param[in] request The request to search for
 * @param[in] suid    The specific suid for which clients are being searched.
 *
 * @return sns_sensor_instance Return instance pointer if found the Sensor
 * Instance currently servicing this request; or NULL.
 */
struct sns_sensor_instance *
sns_sensor_util_find_instance(struct sns_sensor const *sensor,
                              struct sns_request const *request,
                              sns_sensor_uid const *suid);

/**
 * @brief Find the shared instance for a Sensor.
 * Many physical drivers share a single Instance across all associated
 * Sensors.  This function finds and returns that single Instance if it exists.
 *
 * @param[in] sensor A Sensor which shares a single Instance.
 *
 * @return sns_sensor_instance Return the Shared Instance, or NULL.
 */
struct sns_sensor_instance *
sns_sensor_util_get_shared_instance(struct sns_sensor *sensor);

/**
 * @brief Find an existing instance that can handle this new request.
 *
 * @param[in] sensor  Sensor which received the request.
 * @param[in] request New incoming request from
 *                    sns_sensor_api::set_client_request.
 * @param[in] cb      Callback function for each existing Instance.
 *
 * @return sns_sensor_instance Instance reference if found, otherwise NULL.
 */
struct sns_sensor_instance *
sns_sensor_util_find_instance_match(struct sns_sensor const *sensor,
                                    struct sns_request const *request,
                                    sns_sensor_util_instance_match_cb cb);

/**
 * @brief Return the SUID of the SUID Lookup Sensor.

 * @par Parameters
 *    None.
 * @return sns_sensor_uid SUID of the SUID lookup sensor.
 */
sns_sensor_uid sns_get_suid_lookup(void);

/**
 * @brief Process data from the given streams in synchronized
 * chronological order.
 * - Non-data events will be processed as soon as they are
 *   available
 * - Data events on the stream will be processed only when there
 *   is at least one data event available on all streams
 * - Data events across streams will be processed in
 *   chronological order
 *
 * @param[in] streams          Pointer to array of data streams.
 * @param[in] info             TODO.
 * @param[in] num_streams      Number of data streams.
 * @param[in] process_event_cb Callback function for processing one
 *                             event from a data stream.
 * @param userdata Userdata to be supplied to each callback.
 *
 * @return sns_rc SNS_RC_SUCCESS == if everything went fine, error
 *                status otherwise.
 */
sns_rc sns_process_streams_in_timesync(sns_data_stream **streams,
                                       sns_timesync_event_info *info,
                                       int num_streams,
                                       process_stream_event process_event_cb,
                                       void *userdata);

/**
 * @brief Find the minimum batch period from among the clients of an
 * instance
 *
 * @param[in] instance         The Sensor instance being evaluated.
 * @param[in] suid             The specific suid for which clients are being
 *                             evaluated.
 * @param[in] min_batch_period The min period of batching.
 * @param[in] max_flush_period The max period of flushing.
 * @param[in] max_batch        The max batch flag.
 *
 * @return sns_rc
 *  - SNS_RC_SUCCESS if a minimum batch period and
 *                   minimum flush period was
 *                   successfully determined.
 *  - SNS_RC_FAILED  if there was an error in
 *                   determining the batch period or
 *                   flush period.
 *  - SNS_RC_INVALID_STATE if there are no clients
 *                         for the instance for this suid.
 *
 */
sns_rc sns_sensor_util_find_min_batch_period(sns_sensor_instance *instance,
                                             sns_sensor_uid const *suid,
                                             uint32_t *min_batch_period,
                                             uint32_t *max_flush_period,
                                             bool *max_batch);

/**
 * @brief Decide if all requests have max batch set to true.
 *
 * @param[in] instance  The Sensor instance being evaluated.
 * @param[in] suid      The specific suid for which clients are being
 *                      evaluated.
 *
 * @return
 * - True   If all requests have the max batch set to true.
 * - False  If there is a request which does not have max batch set to true.
 *
 */
bool sns_sensor_util_decide_max_batch(sns_sensor_instance *instance,
                                      sns_sensor_uid const *suid);

/**
 * @brief Send a flush event.
 *
 * @param[in] uid       UID of the Sensor.
 * @param[in] instance  Reference to the instance of the Sensor.
 *
 * @return Returns
 *  Nothing.
 */
void sns_sensor_util_send_flush_event(sns_sensor_uid const *uid,
                                      sns_sensor_instance *instance);

/**
 * @brief Update batching requests to sensor instances. This function is for
 * sensors using sns_std_request and sns_std_sensor_config, and it should be
 * called when removing/adding new sensor instances. It will call the
 * curr_inst's set_client_config function using the request with the smallest
 * batch and flush period among all the requests of the instance.
 *
 * @param[in] sensor:    Sensor whose instances are to be updated.
 * @param[in] curr_inst: A instance of the current sensor.
 *
 * @return sns_rc
 * - SNS_RC_SUCCESS if instance is successfully updated with new
 *                  batching parameters.
 * - SNS_RC_FAILED  if decode or encode fails.
 * - SNS_RC_INVALID_VALUE if no request in the instance request queue.
 *
 */
sns_rc
sns_sensor_util_update_std_sensor_batching(sns_sensor *const sensor,
                                           sns_sensor_instance *curr_inst);

/**
 * @brief Compare if new request can reuse existing instance. If a instance has
 * the same sample_rate, same flag for flush_only and max_batch, it can be
 * reused.
 *
 * @param[in] sample_rate Sample rate.
 * @param[in] new_req     The new request received
 * @param[in] exist_req   The existing request being serviced by the instance
 *
 * @retval
 * -True New request can be serviced by the existing instance.
 * -False Otherwise.
 *
 */
bool sns_sensor_util_instance_match(float const *sample_rate,
                                    struct sns_request const *new_req,
                                    struct sns_request const *exist_req);

/**
 * @brief Remove all client requests from the client request list in the sensor
 * instance for the provided suid.
 *
 * @param[in] instance   reference to the instance.
 * @param[in] uid        SUID of the sensor.
 *
 * @return Returns
 *   None
 *
 */
void sns_sensor_util_remove_all_client_req(sns_sensor_instance *instance,
                                           sns_sensor_uid const *uid);

/**
 * @brief Send an event with the given message id without any payload.
 *
 * @param[in] instance          reference to the instance generating this event.
 * @param[in] uid               SUID of the sensor.
 * @param[in] event_message_id  message id of the event to be sent.
 *
 * @return sns_rc SNS_RC_SUCCESS == If successfully send the event,
 *                any other value == Failure.
 *
 */
sns_rc sns_sensor_util_send_event_without_payload(sns_sensor_instance *instance,
                                                  sns_sensor_uid const *uid,
                                                  uint32_t event_message_id);

/**
 * @brief Iterates through the list of all instances for the sensor of this
 * input instance and calls the user-provided callback function with the
 * arguments and instances as input, iterating from instance 1 to N.
 *
 * @note: This callback will include the input instance itself. This function
 *        should not be called directly from a multithreaded library.
 *
 * @param[in] this           Pointer to current instance
 * @param[in] user_cb_func   Callback function that will be
 *                           called for each instance
 * @param[in] arg            Argument passed to the callback
 *
 * @return
 *  - SNS_RC_SUCCESS:      If successful
 *  - SNS_RC_FAILED:       Otherwise
 *
 */
sns_rc sns_sensor_util_do_for_all_instances(
    sns_sensor_instance *const this,
    sns_do_for_all_instances_cb_func user_cb_func, void *const arg);

/**
 * @brief Match the attributes of the data_type with the attributes in the
 * dep_sensor_list
 *
 * @param[in] sensor          reference to the sensor getting the
 * suid_lookup_cb().
 * @param[in] list            reference to the dep_sensor_list stored in sensor
 * state
 * @param[in] data_type       data_type for which suid_lookup_up() came
 * @param[in] event           event containing the attributes for data_type
 * @param[out] item           sns_dep_sensor_info item type for list indicating
 * if dep_sensor_list had the data_type or not
 *
 * @return
 * - True   If data_type was present in dep_sensor_list and all attributes
 * matched.
 * - False  Otherwise
 *
 */
bool sns_dep_sensor_match_attributes(struct sns_sensor *const sensor,
                                     struct sns_list *const list,
                                     const char *const data_type,
                                     const struct sns_sensor_event *const event,
                                     sns_dep_sensor_info **const item);
