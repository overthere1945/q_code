/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#ifndef _QAPI_AUDIO_SERVICE_H_
#define _QAPI_AUDIO_SERVICE_H_

#include "qapi_audio_service_defs.h"

/**
 * Open a playback session.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_PLAYER_OPEN once the request is processed.
 * Session will remain open for a duration of (3 seconds) as defined in audio service. Client can
 * issue start followed by stop any number of times provided the idle timeout duration is met b/w
 * consecutive stop followed by start calls.
 * After timeout QAPI_AS_CLIENT_CB_EVENT_SESSION_CLOSE will be issued as callback to indicate session
 * has been closed.
 * Once playback session is opened, client can issue set volume to override default volume set by
 * audio service.
 * Maximum 2 open requests are accepted by audio service. If a third request is made, the request
 * will be rejected with QAPI_AS_ERROR_TYPE_NO_RESOURCES. Client either needs to be manage the
 * ongoing sessions using policy function or retry utill a session is available.
 * @param[in] type- type of playback being requested. Will be used for managing audio policy.
 * @param[in] tone_id- ID of the tone which is offloaded to AM.
 * @param[in] device- output device to route audio stream to.
 * @param[in] cb- callback to receive the responses and events. Note that the application
 * must return from the callback quickly and process the data in its own context.
 * Do not use blocking calls in the callback.
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_session_open(qapi_as_pb_req_type type, uint16_t tone_id,
                              qapi_as_player_device_t *device, qapi_as_session_client_cb_t *cb);

/**
 * Closes a playback session identified by session_id and cookie.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_SESSION_CLOSE once the request is processed.
 * Releases resources allocated for the session
 * Client should ensure session is stopped successfully prior to this call.
 * Client should also ensure all the other API call responses are received before calling close,
 * otherwise results can be an undefined behavior.
 * session_id and cookie becomes invalid once QAPI_AS_CLIENT_CB_EVENT_SESSION_CLOSE returns success.
 * @param[in] session_id The session for which the close is requested.
 * @param[in] cookie The cookie of session for which the close is requested.
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_session_close(qapi_as_session_t session_id, qapi_as_cookie_t cookie);

/**
 * Starts playback session identified by session_id and cookie.
 * Should be issued ensuring the session is not closed.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_SESSION_START once the request is processed.
 * Audio is rendered on requested output device of the sink until end of stream(EOS)
 * is reached or playback error happens.
 * Playback error or EOS is reported to client via callback registered via qapi_audio_session_open()
 * Client can issue stop/close qapis post receiving response in event
 * QAPI_AS_CLIENT_CB_EVENT_SESSION_START as success.
 * On EOS, Audio Player will be in stopped, client can immediately issue a start request
 * post a successfull stop to restart same tone from the beginning.
 * @param[in] session_id The session for which the start is requested.
 * @param[in] cookie The cookie of session for which the start is requested.
 * @return qapi_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_session_start(qapi_as_session_t session_id, qapi_as_cookie_t cookie);

/**
 * Stops playback session identified by session_id and cookie.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_SESSION_STOP once the request is processed.
 * On receiving response in event AS_CLIENT_CB_EVENT_SESSION_STOP as success,
 * Audio Player will be in stopped state.
 * Client can immediately call start to restart playback ensuring stop has been successfully
 * acknowledged by service via registered callback.
 * Client should note that stop is not a low power state i.e., all the
 * hardware resource used by the playback session will be active, continue to consume power
 * throughout stop state. Client can consider closing the session in case of a longer stop
 * as a power optimization strategy. Otherwise session will anyway close the session after
 * certain default timeout.
 * @param[in] session_id The session for which the stop is requested.
 * @param[in] cookie The cookie of session for which the stop is requested.
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_session_stop(qapi_as_session_t session_id, qapi_as_cookie_t cookie);

/**
 * Sets new volume to playback session.
 * This is only the current stream volume and not the system volume.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_SESSION_SET_VOL once the request is processed.
 * This API would be relevant only for HW sinks rendering audio on an output sound device.
 * @param[in] session_id The session for which the set volume is requested.
 * @param[in] cookie The cookie of session for which the set volume is requested.
 * @param[in] new_volume volume to set. Value supported is between 0.0 and 1.0.
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_session_set_volume(qapi_as_session_t session_id, qapi_as_cookie_t cookie,
                                    float new_volume);

/**
 * Retrieves current volume level of the playback session identified.
 * This is only the current stream volume and not the system volume.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_SESSION_GET_VOL once the request is processed.
 * Call to qapi_audio_session_get_volume() is valid after a successfull- session open/start/stop operations.
 * @param[in] session_id The session for which the get volume is requested.
 * @param[in] cookie The cookie of session for which the get volume is requested.
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_session_get_volume(qapi_as_session_t session_id, qapi_as_cookie_t cookie);

/**
 * Use case policy function that can be overriden by client framework to indtroduce
 * their own use case rules. Audio service defines a default policy as part of the definition
 * for this function within the service.
 * If client doesn't define this policy function, default policy as defined in the serivce will be applied.
 * @param[in] session_id_table Session id table for respective session types. Indexing this table
 * with session type will give the session id of respective playback type.
 * @param[in] pb_type_requested Playback type requested by client
 * @param[in] avail_session_id Available session id which is not occupied any use case
 * Note: Client app can simply return available session id if it is valid, else can choose to cancel one
 * of the playback sessions running by returning session id of respective playback type.
 * @return qapi_as_session_t Session id of the session on which use case need to be started.
 */
qapi_as_session_t __attribute__((weak)) audio_usecase_policy(qapi_as_session_t *session_id_table,
    qapi_as_pb_req_type pb_type_requested, qapi_as_session_t avail_session_id);

/**
 * Get supported tone id list.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_GET_SUPPORTED_TONE_IDS on registered global client
 * callback, once the request is processed. Tone id list is received as an array of integers,
 * size of list will be less than or equal to QAPI_AS_MAX_TONES.
 * API is independent of session operations, hence can be called irrespective of session state.
 * @param[in] cb_data Callback data containing the callback funtion pointer and client context cookie
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed else an error code.
 */
qapi_as_error_t qapi_audio_get_supported_tones(qapi_as_global_client_cb_t cb_data);

#ifdef CONFIG_QC_PDTSW_STHAL_ENABLE
/**
 * Loads sound model to specified session instance.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_LOAD_SM once the request is processed.
 * Its the responsibility of the application to track the session it is using.
 * Only keyphrase model is currently supported. Calling this API with model type as
 * generic will return error. A valid soundmodel needs to be offloaded by APPs
 * as a pre-requisite.
 * @param[in] type Sound model type.
 *                 QAPI_AS_STHAL_SOUND_MODEL_TYPE_KEYPHRASE  for generic sound model
 *                 QAPI_AS_STHAL_SOUND_MODEL_TYPE_GENERIC    for keyphrase sound model
 * @param[in] cb callback to receive the responses and events. the cookie is unused for STHAL.
 *
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_sthal_load_soundmodel(qapi_as_sthal_sound_model_type_t type, qapi_as_session_client_cb_t *cb);
 
/**
 * Unload sound model from specified session instance.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_UNLOAD_SM once the request is processed.
 * Its the responsibility of the application to track the session it is using.
 * @param[in] session_id The session instance to load the model.
 * @param[in] cookie Session cookie.
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_sthal_unload_soundmodel(qapi_as_session_t session_id, qapi_as_cookie_t cookie);
 
/**
 * Start recognition.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_START_RECOG once the request is processed.
 * Its the responsibility of the application to track the session it is using.
 * @param[in] session_id The session instance to start the recognition.
 * @param[in] cookie Session cookie.
 * @param[in] lab_enable LAB capture enable/disable
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_sthal_start_recognition(qapi_as_session_t session_id,
    qapi_as_cookie_t cookie, bool lab_enable);
 
/**
 * Stop recognition.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_STOP_RECOG once the request is processed.
 * @param[in] session_id The session instance to stop the recognition.
 * @param[in] cookie Session cookie.
 * @return QAPI_AS_ERROR_TYPE_SUCCESS if the request can be processed.
 */
qapi_as_error_t qapi_audio_sthal_stop_recognition(qapi_as_session_t session_id, qapi_as_cookie_t cookie);
 
/**
 * Read lab data
 * This is a blocking call to read LAB data from audio service. API should be called only after
 * receiving QAPI_AS_CLIENT_CB_EVENT_LAB_QUEUED from audio service.
 * Client will receive QAPI_AS_CLIENT_CB_EVENT_LAB_QUEUED if LAB record is enabled while starting the session.
 * Its the responsibility of the application to track the session it is using, copy of lab data
 * to the buffer provided by client will happen in client's process context.
 * @param[in] session_id The session instance where recognition is started
 * @param[in] cookie Session cookie.
 * @param[in] buffer Buffer to be allocated and passed by client and the size of the buffer should
 * be strictly equal to the available buffer size that is notified to client earlier
 * with QAPI_AS_CLIENT_CB_EVENT_STHAL_LAB_QUEUE event. Max LAB buffer size that can be queued by
 * audio service would be QAPI_AS_STHAL_MAX_LAB_SIZE.
 * @param[in] size Size of the buffer allocated
 *
 * @return Size of buffer been read successfully. Else 0 or negative value indicating read failure.
 */
int32_t qapi_audio_sthal_lab_read(qapi_as_session_t session_id, qapi_as_cookie_t cookie,
    uint8_t *buffer, size_t size);

#endif //CONFIG_QC_PDTSW_STHAL_ENABLE

#endif //_QAPI_AUDIO_SERVICE_H_
