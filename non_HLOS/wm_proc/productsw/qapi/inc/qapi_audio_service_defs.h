/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#ifndef _QAPI_AUDIO_SERVICE_DEFS_H_
#define _QAPI_AUDIO_SERVICE_DEFS_H_

/* Variable to represent the session. */
typedef int32_t qapi_as_session_t;

/* Variable to represent the cookie of a session. */
typedef int32_t qapi_as_cookie_t;

/* Variable to represent client callback cookie. */
typedef int32_t qapi_as_client_cb_cookie;

/*
 *  -----------------------------------------------------------------------------
 *                      Audio player defs
 *  -----------------------------------------------------------------------------
 */

 /* Number of devices supported */
#define QAPI_AS_PLAYER_MAX_OUTPUT_DEVICES   (1)

/* Invalid session id */
#define QAPI_AS_INVALID_SESSION_ID          (-1)

/* Maximum number of tones supported by service */
#define QAPI_AS_MAX_TONES                   (10)

/* Maximum length of tone description */
#define QAPI_AS_TONE_DESC_MAX_LEN           (32)

/* Max and min volume range */
#define QAPI_AS_VOLUME_MIN                  (0.0f)
#define QAPI_AS_VOLUME_MAX                  (1.0f)

#ifdef CONFIG_QC_PDTSW_STHAL_ENABLE
#define QAPI_AS_STHAL_MAX_CHANNELS          (64)
#define QAPI_AS_STHAL_MAX_LAB_SIZE          (3072)
#endif

/**
 * Enum to indicate out devices
 */
typedef enum
{
    /* DO NOT CHANGE THE ORDER. Enums aligned with one defined from audio manager */
    QAPI_AS_PLAYER_DEVICE_NONE,

    QAPI_AS_PLAYER_DEVICE_OUT_SPEAKER = 1,            /* speaker device */

    /* Note: Device not supported for now */
    QAPI_AS_PLAYER_DEVICE_OUT_BLUETOOTH_A2DP,         /* bluetooth A2DP device */

    QAPI_AS_PLAYER_DEVICE_MAX
} qapi_as_player_device_id_t;

/**
 * structure to store device configurations
 */
typedef struct
{
    /* Number of devices to route audio playback to */
    /* Note: Routing to only 1 device is supported for now */
    uint8_t num_devices;

    /* List of device ids */
    qapi_as_player_device_id_t id[QAPI_AS_PLAYER_MAX_OUTPUT_DEVICES];
} qapi_as_player_device_t;

#ifdef CONFIG_QC_PDTSW_STHAL_ENABLE
/* Note: All the below definitions are inline with audio_sthal_defs.h, so any modifications
 * here would need modifications on the original defs header as well.
 */

/* Defines the sound model type used for a voice UI session */
typedef enum {
    /* This is to keep the enum size 4 bytes to match with size on Apps */
    QAPI_AS_STHAL_SOUND_MODEL_TYPE_NONE = 0xFFFFFF,
    /* Use for unspecified sound model type */
    QAPI_AS_STHAL_SOUND_MODEL_TYPE_UNKNOWN = -1,
    /* Use for keyphrase sound models */
    QAPI_AS_STHAL_SOUND_MODEL_TYPE_KEYPHRASE = 0,
    /* Use for all sound models other than keyphrase
     * Currently unused.*/
    QAPI_AS_STHAL_SOUND_MODEL_TYPE_GENERIC = 1,
} qapi_as_sthal_sound_model_type_t;

/* Audio format definitions */
typedef enum {
    QAPI_AS_STHAL_AUDIO_FMT_NONE =          0xFFFFFF,
    QAPI_AS_STHAL_AUDIO_FMT_INVALID =       0x0,
    QAPI_AS_STHAL_AUDIO_FMT_DEFAULT_PCM =   0x1,
    QAPI_AS_STHAL_AUDIO_FMT_PCM_S16_LE =    QAPI_AS_STHAL_AUDIO_FMT_DEFAULT_PCM,
    QAPI_AS_STHAL_AUDIO_FMT_PCM_S8_LE =     0x2,
    QAPI_AS_STHAL_AUDIO_FMT_PCM_S24_LE =    0x3,
    QAPI_AS_STHAL_AUDIO_FMT_PCM_S24_3LE =   0x4,
    QAPI_AS_STHAL_AUDIO_FMT_PCM_S32_LE =    0x5,
} qapi_as_sthal_audio_fmt_t;

/* Audio channel map enumeration */
typedef enum {
    QAPI_AS_STHAL_CHMAP_CHANNEL_NONE = 0xFFFFFF,
    QAPI_AS_STHAL_CHMAP_CHANNEL_FL = 1,                      /**< Front right channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_FR = 2,                      /**< Front center channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_C = 3,                       /**< Left surround channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_LS = 4,                      /** Right surround channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_RS = 5,                      /** Low frequency effect channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_LFE = 6,                     /** Center surround channel; */
    QAPI_AS_STHAL_CHMAP_CHANNEL_RC = 7,                      /**< rear center channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_CB = QAPI_AS_STHAL_CHMAP_CHANNEL_RC,   /**< center back channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_LB = 8,                      /**< rear left channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_RB = 9,                      /**<  rear right channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_TS = 10,                     /**< Top surround channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_CVH = 11,                    /**< Center vertical height channel.*/
    QAPI_AS_STHAL_CHMAP_CHANNEL_TFC = QAPI_AS_STHAL_CHMAP_CHANNEL_CVH, /**< Top front center */
    QAPI_AS_STHAL_CHMAP_CHANNEL_MS = 12,                     /**< Mono surround channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_FLC = 13,                    /**< Front left of center channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_FRC = 14,                    /**< Front right of center channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_RLC = 15,                    /**< Rear left of center channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_RRC = 16,                    /**< Rear right of center channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_LFE2 = 17,                   /**< Secondary low freq effect channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_SL = 18,                     /**< Side left channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_SR = 19,                     /**< Side right channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_TFL = 20,                    /**< Top front left channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_LVH = QAPI_AS_STHAL_CHMAP_CHANNEL_TFL, /**< Right vertical height */
    QAPI_AS_STHAL_CHMAP_CHANNEL_TFR = 21,                    /**< Top front right channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_RVH = QAPI_AS_STHAL_CHMAP_CHANNEL_TFR, /**< Right vertical height */
    QAPI_AS_STHAL_CHMAP_CHANNEL_TC = 22,                     /**< Top center channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_TBL = 23,                    /**< Top back left channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_TBR = 24,                    /**< Top back right channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_TSL = 25,                    /**< Top side left channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_TSR = 26,                    /**< Top side right channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_TBC = 27,                    /**< Top back center channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_BFC = 28,                    /**< Bottom front center channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_BFL = 29,                    /**< Bottom front left channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_BFR = 30,                    /**< Bottom front right channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_LW = 31,                     /**< Left wide channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_RW = 32,                     /**< Right wide channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_LSD = 33,                    /**< Left side direct channel. */
    QAPI_AS_STHAL_CHMAP_CHANNEL_RSD = 34,                    /**< Left side direct channel. */
} qapi_as_sthal_channel_map_t;

/*
 * Callback function to get notified about detection event
 *
 * Based on sound model type, the event payload would differ.
 * If type == QAPI_AS_STHAL_SOUND_MODEL_TYPE_GENERIC
 *      event data -> qapi_as_sthal_recognition_event_t
 * If type == QAPI_AS_STHAL_SOUND_MODEL_TYPE_KEYPHRASE
 *      event data -> qapi_as_sthal_phrase_recognition_event_t
 */
typedef void (*qapi_as_sthal_recognition_callback_t)(qapi_as_session_t session_id,
        qapi_as_sthal_sound_model_type_t type, void *event, void *cookie);

/* Audio channel info data structure */
typedef struct __attribute__((packed, aligned(4)))
{
    /* Number of channels */
    uint16_t channels;
    /* Channel map per channel */
    uint8_t ch_map[QAPI_AS_STHAL_MAX_CHANNELS];
} qapi_as_sthal_channel_info_t;

/* Audio media format configuration */
typedef struct __attribute__((packed, aligned(4)))
{
    /* Sample rate */
    uint32_t sample_rate;
    /* Bit width */
    uint32_t bit_width;
    /* Audio format id */
    qapi_as_sthal_audio_fmt_t aud_fmt_id;
    /* Channel info */
    qapi_as_sthal_channel_info_t ch_info;
} qapi_as_sthal_media_config_t;

/*
 * Payload corresponding to a detection event applicable for
 * generic and keyphrase soundmodels
 */
typedef struct __attribute__((packed, aligned(4)))
{
    /* Recognition status- 0 for success. Any other value for failure */
    int32_t status;
    /* Event type, Same as sound model type */
    qapi_as_sthal_sound_model_type_t type;
    /*
     * Delay in ms b/n end of model detection and start of audio available
     * for capture  * A negative value is also possible. (Ex: If key phrase is also
     * available for capture)
     */
    int32_t capture_delay_ms;
    /*
     * Duration in ms of audio captured before the start of the trigger. 0
     * if none
     */
    int32_t capture_preamble_ms;
    /*
     * Media format of either trigger in event data or to use for capture
     * of the rest of the utterance
     */
    qapi_as_sthal_media_config_t media_config;
    /*
     * Is it possible to capture audio from this utterance buffered by
     * implementation.
     */
    bool capture_available;
    /* The opaque data is the capture of the trigger sound */
    bool trigger_in_data;
    /* Size of opaque event data */
    uint32_t data_size;
    /* Offset of opaque data struct from the start of this struct */
    uint32_t data_offset;
} qapi_as_client_cb_event_sthal_recog_event_t;
#endif
/* -------------------- Audio player defs end----------------------------- */

/*
 *  -------------------------------------------------------------------------
 *                      Audio service defs
 *  -------------------------------------------------------------------------
 */

/**
 * Audio service error codes
 */
typedef enum
{
    /* Success.*/
    QAPI_AS_ERROR_TYPE_SUCCESS = 0,

    /* generic error.*/
    QAPI_AS_ERROR_TYPE_FAIL,

    /* Invalid parameters.*/
    QAPI_AS_ERROR_TYPE_INVALID_PARAMS,

    /* No free resource to accomodate request majorly due to maximum
     * clients being reached for that particular feature .*/
    QAPI_AS_ERROR_TYPE_NO_RESOURCES,

    /* Operation not permitted*/
    QAPI_AS_ERROR_TYPE_NOT_PERMITTED,

    /* Feature unavailable.*/
    QAPI_AS_ERROR_TYPE_FEATURE_UNAVAILABLE,

    /* Malloc failed.*/
    QAPI_AS_ERROR_TYPE_NO_MEMORY,

    /* Unable to put the request to audio service pipe.*/
    QAPI_AS_ERROR_TYPE_QUEUE_ERROR,

    /* Requested session not found or expired.*/
    QAPI_AS_ERROR_TYPE_NO_SESSION_FOUND,

    /* Start session request failed since current playback stop returned error.*/
    QAPI_AS_ERROR_TYPE_UNABLE_TO_TERMINATE_ACTIVE_SESSION,

    /* Glink related failure */
    QAPI_AS_ERROR_TYPE_GLINK_FAILURE,

    /* Response timout for the command sent to adsp */
    QAPI_AS_ERROR_TYPE_ADSP_RESPONSE_TIMEOUT,

    /* Tone ID not identified among the offloaded list */
    QAPI_AS_ERROR_TYPE_INVALID_TONE_ID,

    /* Invalid operation */
    QAPI_AS_ERROR_TYPE_INVALID_OPERATION,

    /* Undefined error.*/
    QAPI_AS_ERROR_TYPE_UNDEFINED,
} qapi_as_error_t;

/**
 * Type of playback request
 */
typedef enum
{
    QAPI_AS_PLAYBACK_REQUEST_TYPE_NONE = -1,
    QAPI_AS_PLAYBACK_REQUEST_TYPE_NOTIFICATION = 1,
    QAPI_AS_PLAYBACK_REQUEST_TYPE_TONE,
    QAPI_AS_PLAYBACK_REQUEST_TYPE_MAX
} qapi_as_pb_req_type;

/**
 * Audio service callback event type
 */
typedef enum
{
    QAPI_AS_CLIENT_CB_EVENT_NONE = -1,

    /* returns the response for qapi_audio_session_open.
     * see qapi_as_client_cb_event_player_open_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_PLAYER_OPEN = 0x01,

    /* returns the response for qapi_audio_session_close.
     * see qapi_as_client_cb_event_player_close_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_PLAYER_CLOSE,

    /* returns the response for qapi_audio_session_start.
     * see qapi_as_client_cb_event_player_start_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_PLAYER_START,

    /* returns the response for qapi_audio_session_stop.
     * see qapi_as_client_cb_event_player_stop_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_PLAYER_STOP,

    /* returns the response for qapi_audio_session_set_volume.
     * see qapi_as_client_cb_event_player_set_vol_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_PLAYER_SET_VOL,

    /* returns the response for qapi_audio_session_get_volume.
     * see qapi_as_client_cb_event_player_get_vol_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_PLAYER_GET_VOL,

    /* Max session event callbacks */
    QAPI_AS_CLIENT_CB_EVENT_PLAYER_EVENTS_MAX,

    /* Event to indicate player has been stopped due to an error/eos.
     * see qapi_as_client_cb_event_player_stopped_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_PLAYER_STOPPED,

    /* Playback terminated to schedule another client's playback or other reason.
     * session context info is no more valid.
     * see qapi_as_client_cb_event_session_terminated_t for data associated
     * with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_SESSION_TERMINATED,

    /* Requests received with invalid cookie. The session could have been expired
     * before the request can be processed. no data associated for this event.*/
    QAPI_AS_CLIENT_CB_EVENT_REQUESTS_ON_INVALID_COOKIE,

    /* returns the response for qapi_audio_get_supported_tone_ids.
     * see qapi_as_client_cb_event_getsupported_effects_t for data associated with this event */
    QAPI_AS_CLIENT_CB_EVENT_GET_SUPPORTED_TONE_IDS,

#ifdef CONFIG_QC_PDTSW_STHAL_ENABLE
    /* returns the response for qapi_audio_sthal_load_soundmodel.
    * see qapi_as_client_cb_event_sthal_load_sm_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_STHAL_LOAD_SM,

    /* returns the response for qapi_audio_sthal_start_recog.
    * see qapi_as_client_cb_event_sthal_start_recog_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_STHAL_START_RECOG,

    /* returns the response for qapi_audio_sthal_stop_recog.
    * see qapi_as_client_cb_event_sthal_stop_recog_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_STHAL_STOP_RECOG,

    /* returns the response for qapi_audio_sthal_unload_soundmodel.
    * see qapi_as_client_cb_event_sthal_unload_sm_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_STHAL_UNLOAD_SM,

    /* Notif from service when KW detection event is received from adsp.
    * see qapi_as_client_cb_event_sthal_recog_event_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_STHAL_KW_DETECTION = 18,

    /* Notif from service when LAB data is received from adsp.
    * see qapi_as_client_cb_event_sthal_lab_notif_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_STHAL_LAB_QUEUE,

    /* Notif from service when LAB data read fails
    * see qapi_as_client_cb_event_labread_status_t for data associated with this event.*/
    QAPI_AS_CLIENT_CB_EVENT_STHAL_LAB_READ_FAILED,
#endif
    QAPI_AS_CLIENT_CB_EVENT_MAX
} qapi_as_client_cb_event;

/**
 * Struct to represent generic client request responses.
 */
typedef struct
{
    /* Response for the request.*/
    qapi_as_error_t response;

    /* Audio api return value for debug purposes.*/
    int32_t audio_err_code;
} qapi_as_client_cb_event_res_t;

/**
 * Struct to represent qapi_as_client_cb_event_player_res_t event data.
 */
typedef qapi_as_client_cb_event_res_t qapi_as_client_cb_event_player_res_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_PLAYER_OPEN event data.
 * The Session id and cookie in the callback are only valid if response is success.
 * The session ID and cookie returned by this event should be used to perform any
 * further further operations.
 */
typedef qapi_as_client_cb_event_player_res_t qapi_as_client_cb_event_player_open_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_PLAYER_CLOSE event data.
 * Client must not use the session id and cookie if the response value is success.
 */
typedef qapi_as_client_cb_event_player_res_t qapi_as_client_cb_event_player_close_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_PLAYER_START event data.
 */
typedef qapi_as_client_cb_event_player_res_t qapi_as_client_cb_event_player_start_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_PLAYER_STOP event data.
 */
typedef qapi_as_client_cb_event_player_res_t qapi_as_client_cb_event_player_stop_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_PLAYER_SET_VOL event data.
 */
typedef qapi_as_client_cb_event_player_res_t qapi_as_client_cb_event_player_set_vol_t;

#ifdef CONFIG_QC_PDTSW_STHAL_ENABLE
/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_STHAL_LOAD_SM event data.
 */
typedef qapi_as_client_cb_event_res_t qapi_as_client_cb_event_sthal_load_sm_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_STHAL_UNLOAD_SM event data.
 */
typedef qapi_as_client_cb_event_res_t qapi_as_client_cb_event_sthal_unload_sm_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_STHAL_START_RECOG event data.
 */
typedef qapi_as_client_cb_event_res_t qapi_as_client_cb_event_sthal_start_recog_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_STHAL_STOP_RECOG event data.
 */
typedef qapi_as_client_cb_event_res_t qapi_as_client_cb_event_sthal_stop_recog_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_STHAL_LAB_READ_FAILED event data.
 */
typedef qapi_as_client_cb_event_res_t qapi_as_client_cb_event_labread_status_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_STHAL_LAB_QUEUED event data.
 */
typedef struct
{
    /* Size of LAB data.*/
    size_t size;
} qapi_as_client_cb_event_sthal_lab_notif_t;
#endif //CONFIG_QC_PDTSW_STHAL_ENABLE

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_PLAYER_GET_VOL event data.
 */
typedef struct
{
    /* Response form executing the request.*/
    qapi_as_client_cb_event_player_res_t result;

    /* volume of the playback.*/
    float volume;
} qapi_as_client_cb_event_player_get_vol_t;

/**
 * Type to indicate reason for player stop
 */
typedef enum
{
    QAPI_AS_PLAYER_STOP_REASON_ERROR = 1,
    QAPI_AS_PLAYER_STOP_REASON_EOS,
} qapi_as_player_stop_reason_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_PLAYER_STOPPED event data.
 */
typedef struct
{
    /* stop reason.*/
    qapi_as_player_stop_reason_t reason;

    /* data associated with the reason */
    int32_t data;
} qapi_as_client_cb_event_player_stopped_t;

typedef enum
{
    QAPI_AS_SESSION_TERMINATION_REASON_NONE = 0,
    QAPI_AS_SESSION_TERMINATION_REASON_AUDIO_ERROR = 1,
    QAPI_AS_SESSION_TERMINATION_REASON_FOCUS_CHANGE,
    QAPI_AS_SESSION_TERMINATION_REASON_AUDIO_SSR,
    QAPI_AS_SESSION_TERMINATION_REASON_TIMEOUT
}qapi_as_session_termination_reason_t;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_SESSION_TERMINATED event data.
 */
typedef struct
{
    /* termination reason.*/
    qapi_as_session_termination_reason_t reason;

    /* data associated with the reason */
    int32_t data;
} qapi_as_client_cb_event_session_terminated_t;

typedef struct
{
    int32_t tone_id;

    /* Tone name or description */
    int8_t tone_desc[QAPI_AS_TONE_DESC_MAX_LEN];
} as_tone_info;

/**
 * Struct to represent QAPI_AS_CLIENT_CB_EVENT_GET_SUPPORTED_TONE_IDS event data.
 */
typedef struct
{
    /* list of supported tones info */
    as_tone_info tone_info[QAPI_AS_MAX_TONES];

    /* Number of tones */
    uint16_t num_tones;
} qapi_as_client_cb_event_getsupported_effects_t;

/**
 * Audio service client callback prototype.
 * @param[in] evt type of event received.
 * @param[in] data data associated with the event. this is only valid within the callback.
 * @param[in] session_id session for which the event is received.
 * @param[in] cb_cookie callback cookie for client context
 * @param[in] cookie cookie of session for which the event is received.
 */
typedef void (*qapi_as_session_client_cb)(qapi_as_client_cb_event evt, void* data,
    qapi_as_client_cb_cookie cb_cookie, qapi_as_session_t session_id, qapi_as_cookie_t session_cookie);

/**
 * Audio service client callback for global events
 * @param[in] evt type of event received.
 * @param[in] data data associated with the event. This is only valid within the callback.
 * @param[in] cb_cookie callback cookie for client context
 */
typedef void (*qapi_as_global_client_cb)(qapi_as_client_cb_event evt, void* data,
    qapi_as_client_cb_cookie cb_cookie);

/**
 * Client callback struct for an audio session.
 * Client should not invoke audio service APIs from the callback.
 */
typedef struct
{
    /* Event callback handle */
    qapi_as_session_client_cb evt_cb;

    /* Callback cookie */
    qapi_as_client_cb_cookie cookie;
} qapi_as_session_client_cb_t;

/**
 * Audio global callback struct
 * Client should not invoke Audio service QAPIs within the callback.
 */
typedef struct
{
    qapi_as_global_client_cb evt_cb;
    qapi_as_client_cb_cookie cookie;
} qapi_as_global_client_cb_t;

#endif //_QAPI_AUDIO_SERVICE_DEFS_H_
