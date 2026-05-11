/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#ifndef _QAPI_HAPTICS_SERVICE_DEFS_H_
#define _QAPI_HAPTICS_SERVICE_DEFS_H_

#define QAPI_SUPPORTED_EFFECTS_MAX          (22)
#define QAPI_HS_INVALID_SESSION_ID          (-1)
#define QAPI_HS_TONE_DESC_MAX_LEN           (32)

/* Variable to represent a pattern id. */
typedef uint16_t qapi_haptics_pattern_id_t;

/* Variable to represent the session. */
typedef int32_t qapi_hs_session_t;

/* Variable to represent the cookie of a session. */
typedef int32_t qapi_hs_cookie_t;

/* Variable to represent the client callback cookie of a session. */
typedef int32_t qapi_haptics_client_cb_cookie_t;

/* Maximum number of tones supported by service */
#define QAPI_HS_MAX_PATTERNS                (10)

/* Maximum length of tone description */
#define QAPI_HS_PATTERN_DESC_MAX_LEN        (32)

typedef enum
{
    /* Success.*/
    QAPI_HS_ERROR_TYPE_SUCCESS = 0,

    /* generic error.*/
    QAPI_HS_ERROR_TYPE_FAIL,

    /* Invalid parameters.*/
    QAPI_HS_ERROR_TYPE_INVALID_PARAMS,

    /* No free resource to accomodate request majorly due to maximum
     * clients being reached for that particular feature .*/
    QAPI_HS_ERROR_TYPE_NO_RESOURCES,

    /* Operation not permitted*/
    QAPI_HS_ERROR_TYPE_NOT_PERMITTED,

    /* Feature unavailable.*/
    QAPI_HS_ERROR_TYPE_FEATURE_UNAVAILABLE,

    /* Malloc failed.*/
    QAPI_HS_ERROR_TYPE_NO_MEMORY,

    /* Unable to put the request to haptics service pipe.*/
    QAPI_HS_ERROR_TYPE_QUEUE_ERROR,

    /* Requested session not found or expired.*/
    QAPI_HS_ERROR_TYPE_NO_SESSION_FOUND,

    /* Start session request failed since current playback stop returned error.*/
    QAPI_HS_ERROR_TYPE_UNABLE_TO_TERMINATE_ACTIVE_SESSION,

    /* Glink related failure */
    QAPI_HS_ERROR_TYPE_GLINK_FAILURE,

    /* Response timout for the command sent to adsp */
    QAPI_HS_ERROR_TYPE_ADSP_RESPONSE_TIMEOUT,

    /* Tone ID not identified among the offloaded list */
    QAPI_HS_ERROR_TYPE_INVALID_PATTERN_ID,

    /* Invalid operation */
    QAPI_HS_ERROR_TYPE_INVALID_OPERATION,

    /* Undefined error.*/
    QAPI_HS_ERROR_TYPE_UNDEFINED,
}qapi_hs_error_t;

/**
 * Reason for requesting playback.
 */
typedef enum
{
    QAPI_HS_PLAYBACK_REQUEST_TYPE_NONE = -1,
    QAPI_HS_PLAYBACK_REQUEST_TYPE_NOTIFICATION,
    QAPI_HS_PLAYBACK_REQUEST_TYPE_TONE,
    QAPI_HS_PLAYBACK_REQUEST_TYPE_MAX
}qapi_haptics_pb_req_type;

/**
 * Haptics service callback event type
 */
typedef enum
{
    QAPI_HS_CLIENT_CB_EVENT_NONE = -1,

    /* returns the response for qapi_haptics_service_play.
     * see qapi_hs_client_cb_event_play_t for data associated with this event.*/
    QAPI_HS_CLIENT_CB_EVENT_PLAY = 0x01,

    /* returns the response for qapi_haptics_service_turnon.
     * see qapi_hs_client_cb_event_turnon_t for data associated with this event.*/
    QAPI_HS_CLIENT_CB_EVENT_TURNON,

    /* returns the response for qapi_haptics_service_turnoff.
     * see qapi_hs_client_cb_event_turnoff_t for data associated with this event.*/
    QAPI_HS_CLIENT_CB_EVENT_TURNOFF,

    /* returns the response for qapi_haptics_service_set_amplitude.
     * see qapi_hs_client_cb_event_setamplitude_t for data associated with this event.*/
    QAPI_HS_CLIENT_CB_EVENT_SET_AMPLITUDE,

    /* returns the response for qapi_haptics_service_get_amplitude.
     * see qapi_hs_client_cb_event_getamplitude_t for data associated with this event.*/
    QAPI_HS_CLIENT_CB_EVENT_GET_AMPLITUDE,

    /* Max session event callbacks */
    QAPI_HS_CLIENT_CB_EVENT_PLAYER_EVENTS_MAX,

    /* see qapi_hs_client_cb_event_getsupported_effects_t for data associated with this event.*/
    QAPI_HS_CLIENT_CB_EVENT_GET_SUPPORTED_EFFECTS,

    /* Event to indicate playback has been stopped due to an error/eos.
     * see qapi_hs_client_cb_event_playback_stopped_t for data associated with this event.*/
    QAPI_HS_CLIENT_CB_EVENT_PLAYBACK_STOPPED,

    /* Playback terminated to schedule another client's playback or other reason.
     * session context info is no more valid.
     * see qapi_hs_client_cb_event_session_terminated_t for data associated
     * with this event.*/
    QAPI_HS_CLIENT_CB_EVENT_SESSION_TERMINATED,

    /* Requests received with invalid cookie. The session could have been expired
     * before the request can be processed. no data associated for this event.*/
    QAPI_HS_CLIENT_CB_EVENT_REQUESTS_ON_INVALID_COOKIE,

    QAPI_HS_CLIENT_CB_EVENT_MAX
}qapi_hs_client_cb_event;

/**
 * Struct to represent strength value to be set while playing haptics buzz.
 */
typedef enum
{
    HS_LIGHT,
    HS_MEDIUM,
    HS_STRONG
} qapi_haptics_strength_t;

/**
 * Struct to represent generic client request responses.
 */
typedef struct
{
    /* Response for the request.*/
    qapi_hs_error_t response;

    /* Haptics api return value for debug purposes.*/
    int32_t haptics_err_code;
} qapi_hs_client_cb_event_res_t;

/**
 * Struct to represent QAPI_HS_CLIENT_CB_EVENT_PLAY event data.
 */
typedef qapi_hs_client_cb_event_res_t qapi_hs_client_cb_event_play_t;

/**
 * Struct to represent QAPI_HS_CLIENT_CB_EVENT_TURNON event data.
 */
typedef qapi_hs_client_cb_event_res_t qapi_hs_client_cb_event_turnon_t;

/**
 * Struct to represent QAPI_HS_CLIENT_CB_EVENT_TURNOFF event data.
 */
typedef qapi_hs_client_cb_event_res_t qapi_hs_client_cb_event_turnoff_t;

/**
 * Struct to represent QAPI_HS_CLIENT_CB_EVENT_SET_AMPLITUDE event data.
 */
typedef qapi_hs_client_cb_event_res_t qapi_hs_client_cb_event_setamplitude_t;

/**
 * Struct to represent QAPI_HS_CLIENT_CB_EVENT_GET_AMPLITUDE event data.
 */
typedef struct
{
    /* Basic response */
    qapi_hs_client_cb_event_res_t result;

    /* amplitude of the haptics playback.*/
    float amplitude;
} qapi_hs_client_cb_event_getamplitude_t;

typedef struct
{
    int32_t pattern_id;

    /* Pattern name or description */
    int8_t pattern_desc[QAPI_HS_PATTERN_DESC_MAX_LEN];
} hs_pattern_info;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_GET_SUPPORTED_TONE_IDS event data.
 */
typedef struct
{
    /* list of supported tones info */
    hs_pattern_info pattern_info[QAPI_HS_MAX_PATTERNS];

    /* Number of patterns */
    uint16_t num_patterns;
} qapi_hs_client_cb_event_getsupported_effects_t;

/**
 * Type to indicate reason for playback stop
 */
typedef enum
{
    QAPI_HS_PLAYBACK_STOP_REASON_ERROR = 1,
    QAPI_HS_PLAYBACK_STOP_REASON_EOS,
} qapi_hs_playback_stop_reason_t;

/**
 * Struct to represent QAPI_HS_CLIENT_CB_EVENT_PLAYBACK_STOPPED event data.
 */
typedef struct
{
    /* stop reason.*/
    qapi_hs_playback_stop_reason_t reason;

    /* data associated with the reason. Unused currently */
    int32_t data;
} qapi_hs_client_cb_event_playback_stopped_t;

/**
 * Type to indicate reason for playback termination
 */
typedef enum
{
    QAPI_HS_SESSION_TERMINATION_REASON_PLAYBACK_ERROR = 1,
    QAPI_HS_SESSION_TERMINATION_REASON_FOCUS_CHANGE,
    QAPI_HS_SESSION_TERMINATION_REASON_SSR,
} qapi_hs_session_termination_reason_t;

/**
 * Struct to represent QAPI_HS_CLIENT_CB_EVENT_SESSION_TERMINATED event data.
 */
typedef struct
{
    /* termination reason.*/
    qapi_hs_session_termination_reason_t reason;

    /* data associated with the reason. Unused currently */
    int32_t data;
} qapi_hs_client_cb_event_session_terminated_t;

/**
 * Haptics service client callback prototype.
 * @param[in] evt type of event received.
 * @param[in] data data associated with the event. this is only valid within the callback.
 * @param[in] cb_cookie callback cookie of session for which the event is received.
 * @param[in] session_id session for which the event is received.
 * @param[in] session_cookie session cookie of session for which the event is received.
 */
typedef void (*qapi_hs_session_client_cb)(qapi_hs_client_cb_event evt, void* data,
    qapi_haptics_client_cb_cookie_t cb_cookie, qapi_hs_session_t session_id,
    qapi_hs_cookie_t session_cookie);

/**
 * Haptics service client callback prototype.
 * @param[in] evt type of event received.
 * @param[in] data data associated with the event. This is only valid within the callback.
 * @param[in] cb_cookie callback cookie of session for which the event is received.
 */
typedef void (*qapi_hs_global_client_cb)(qapi_hs_client_cb_event evt, void* data,
    qapi_haptics_client_cb_cookie_t cb_cookie);

/**
 * Haptics session playback callbacks
 * Client should not invoke haptics service QAPIs within the callback.
 */
typedef struct
{
    qapi_hs_session_client_cb evt_cb;
    qapi_haptics_client_cb_cookie_t cookie;
} qapi_haptics_session_client_cb_t;

/**
 * Haptics global callbacks
 * Client should not invoke haptics service QAPIs within the callback.
 */
typedef struct
{
    qapi_hs_global_client_cb evt_cb;
    qapi_haptics_client_cb_cookie_t cookie;
} qapi_haptics_global_client_cb_t;

#endif //_QAPI_HAPTICS_SERVICE_DEFS_H_