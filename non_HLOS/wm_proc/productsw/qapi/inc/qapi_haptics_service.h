/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#ifndef _QAPI_HAPTICS_SERVICE_H_
#define _QAPI_HAPTICS_SERVICE_H_

#include "qapi_haptics_service_defs.h"

/**
 qapi_haptics_service_play used by clients of Haptics service to play pre-defined/pre-canned
 haptics buzz using a pattern ID.
 Any pattern which is played using this API cannot be stopped/terminated by the client. Use case
 will end only after EOS. Play request placed without having received the acknowledgement to
 previous play request will be rejected by the service.
 @param[in] type Type of playback request, either notification or touch
 @param[in] pattern_id pattern id of haptics buzz to be played
 @param[in] strength  strength level of the playback as per the enums exposed by qapi_haptics_strength_t
 @param[in] clint_cb which gets called with the session id and cookie info once playback requested
 has been submitted to the service queue successfully. Callback indicating session notification as
 in playback error/EOS will be sent to the client over same client callback.
 @return QAPI error code
 */
qapi_hs_error_t qapi_haptics_service_play(qapi_haptics_pb_req_type type,
    qapi_haptics_pattern_id_t pattern_id, qapi_haptics_strength_t strength,
    qapi_haptics_session_client_cb_t clint_cb);

/**
 qapi_haptics_service_turnon used by clients of Haptics service to turn on pre-defined/pre-canned
 haptics buzz using a pattern ID.
 @param[in] type Type of playback request, either notification or touch
 @param[in] pattern_id pattern id of haptics buzz to be played
 @param[in] strength  strength level of the playback as per the enums exposed by qapi_haptics_strength_t
 @param[in] duration_ms duration of playback in ms
 @param[in] priority  Boolean value to indicate if the play request is high priority or not
                      if it is true --> haptics service will stop ongoing play and start this request
                      if it is false--> haptics service will return busy if play is in progress
 @param[in]  clint_cb which gets called with the session id and cookie info once playback requested
 has been submitted to the service queue successfully. Callback indicating session notification as
 in playback error/EOS etc will be sent to the client over same client callback.
 @return QAPI error code
*/
qapi_hs_error_t qapi_haptics_service_turnon(qapi_haptics_pb_req_type type,
    qapi_haptics_pattern_id_t pattern_id, qapi_haptics_strength_t strength, uint16_t duration_ms,
    bool priority, qapi_haptics_session_client_cb_t cb);

/**
qapi_haptics_service_turnoff used by clients of Haptics Service to stop the playback initiated using
qapi_haptics_service_turnon. Client callback will be received upon successfull closure of haptics
session on adsp.
 @param[in] session_id session_id of haptics buzz to be turned off.
 @param[in] session_cookie cookie of haptics session
 @return QAPI error code
*/
qapi_hs_error_t qapi_haptics_service_turnoff(qapi_hs_session_t session_id,
    qapi_hs_cookie_t session_cookie);

/**
Set amplitude for ongoing haptics session
 @param[in] session_id session_id of haptics session
 @param[in] session_cookie cookie of haptics session
 @param[in] amplitude new amplitude value to be set for the session. Acceptable range is 0.0 to 1.0
 @return Generic QAPI_CODE
*/
qapi_hs_error_t qapi_haptics_service_set_amplitude(qapi_hs_session_t session_id,
    qapi_hs_cookie_t session_cookie, float amplitude);

/**
Get amplitude for ongoing haptics session
 @param[in] session_id session_id of haptics session
 @param[in] session_cookie cookie of haptics session
 Amplitude info will be returned through the callback function that is registered during play/turnon
 qapi function calls.
 @return QAPI error code
*/
qapi_hs_error_t qapi_haptics_service_get_amplitude(qapi_hs_session_t session_id,
    qapi_hs_cookie_t session_cookie);

/**
Get list of supported effects
 @param[in] cb callback structure having callback function pointer and cookie.
 Callback function will be called with the supported effects info.
 @return QAPI error code
*/
qapi_hs_error_t qapi_haptics_service_get_supported_effects(qapi_haptics_global_client_cb_t client_cb);

#endif //_QAPI_HAPTICS_SERVICE_H_