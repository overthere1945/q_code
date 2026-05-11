#pragma once
/** =============================================================================
 * @file sns_pb_util.h
 *
 * @brief Utility functions provided by Qualcomm for use by Sensors in
 * association with nanoPB. All utilities in this file can be used in island
 * mode.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "pb_decode.h"
#include "sns_std.pb.h"
#include "sns_std_sensor.pb.h"
#include "sns_time.h"

/**
 * @brief Optimized macro for decoding a encoded varint.
 *
 * @param[out]   shifter  Provided the final shifter offset used by the decoder.
 *                       Starts at 0x00 and shifts by +0x07 until the end of the
 *                       varint is found. This is used for correctly
 *                       constructing the final value.
 * @param[in]    buf      buffer with the encoded payload containing the varint
 *                       to be decoded.
 * @param[out]   val      Will provided back the decoded varint.
 * @param[inout] idx      Provide the starting position in the buffer where the
 *                       varint begins. This index will then be overwritten to
 *                       to reflect the final index that was read to get the
 *                       full varint. Each Index in a buffer is equivalent  to a
 *                       byte. Detection of the continuation bit triggers the
 *                       reading of the next byte in the buffer.
 */
#define SNS_PB_DECODE_VARINT(shifter, buf, val, idx)                           \
  shifter = 0, val = 0;                                                        \
  do                                                                           \
  {                                                                            \
    idx++;                                                                     \
    val |= (uint64_t)(buf[idx] & 0x7F) << shifter;                             \
    shifter += 7;                                                              \
  } while(buf[idx] & 0x80);

/**
 * @brief Optimized macro for decoding a encoded fixed32.
 *
 * @param[in]    buf    buffer with the encoded payload containing the fixed32
 * to be decoded.
 * @param[in]    idx    Provide the starting position in the buffer where the
 *                     fixed32 value begins.
 * @param[out]   val    Will provided back the decoded fixed32.
 *
 */
#define SNS_PB_DECODE_FIXED32(buf, idx, val)                                   \
  val = (uint32_t)buf[idx] | (uint32_t)(buf[idx + 1]) << 8 |                   \
        (uint32_t)(buf[idx + 2]) << 16 | (uint32_t)(buf[idx + 3]) << 24;

/* This define indicates sns_pb_decode_dae_data_event is supported */
#define SNS_PB_UTIL_HAS_DAE_DECODE

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_sensor_instance;
struct sns_sensor_uid;
struct sns_sensor_event;
struct sns_request;

/*=============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * @brief Structure to be used with pb_decode_simple_cb.
 *
 */
typedef struct pb_simple_cb_arg
{
  void *decoded_struct;       /*!< Where to place the decoded message. */
  const pb_msgdesc_t *fields; /*   Message definition associated with
                               *   decoded_struct.
                               */
} pb_simple_cb_arg;

/**
 * @brief Structure to be used with pb_encode_uint32_arr_cb &
 * pb_decode_uint32_arr_cb.
 *
 */
typedef struct pb_uint_32_arr_arg
{

  uint32_t *arr;   /*!< Where to place the decoded uint32 array. */
  uint8_t arr_len; /*!< Length of the uint32 array. */

  uint8_t *arr_index; /*!< Array index used for decoding.
                       *   Must be initialized to 0 prior
                       *   to calling pb_decode_uint32_arr_cb.
                       */
} pb_uint_32_arr_arg;

/**
 * @brief Structure to be used with pb_encode_float_arr_cb &
 * pb_decode_float_arr_cb.
 *
 */
typedef struct pb_float_arr_arg
{
  float *arr;         /*!< Where to place the decoded float array. */
  uint8_t arr_len;    /*!< Length of the float array. */
  uint8_t *arr_index; /*!< Array index used for decoding.
                       * Must be initialized to 0 prior to
                       * calling pb_decode_float_arr_cb.
                       */
} pb_float_arr_arg;

/**
 * @brief Callback structure for decode_string and encode_string.
 *
 */
typedef struct pb_buffer_arg
{
  void const *buf; /*!< Buffer to be written, or reference to read buffer. */
  size_t buf_len;  /*!< Length of buf. */
} pb_buffer_arg;

/**
 * @brief Callback structure for decode string array and encode string array
 *
 */
typedef struct pb_string_arr_arg
{
  uint8_t **buf_arr;  /*!< Where to place the decoded string array */
  uint8_t arr_len;    /*!< Length of string array */
  uint8_t *arr_index; /*!< Array index used for decoding.
                       * Must be initialized to 0 prior to
                       * calling pb_decode_string_arr_cb
                       */
  size_t max_str_len; /*!< Max size of a single string,
                       *  including null terminator
                       */
} pb_string_arr_arg;

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/
/**
 * @brief Decode a nested message. This function is intended to be used as a
 * callback function during the decode process.
 * This utility function can only be used for simple messages without
 * nested structures or arrays.
 *
 * @see pb_callback_s::decode
 *
 * @param[in] stream The stream from which to decode nested message from.
 * @param[in] field  The format of the nested message value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 * - True  Successfully decode the message.
 * - False Fail to decode the message.
 *
 */
bool pb_decode_simple_cb(pb_istream_t *stream, const pb_field_t *field,
                         void **arg);

/**
 * @brief Encode a nested array of uint32. This function is intended to
 * be used as a callback function during the encode process.
 *
 * @see pb_callback_s::encode.
 *
 * @param[in] stream The stream to encode nested message.
 * @param[in] field  The format of the nested message value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 *  - True  Successfully encode the message.
 *  - False Fail to encode the message.
 *
 */
bool pb_encode_uint_32_arr_cb(pb_ostream_t *stream, const pb_field_t *field,
                              void *const *arg);

/**
 * @brief Decode a nested array of uint32. This function is intended to
 * be used as a callback function during the decode process.
 *
 * @see pb_callback_s::decode.
 *
 * @param[in] stream The stream from which to decode nested array of uint32
 *                   from.
 * @param[in] field  The format of the nested array of uint32 value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 * - True  Successfully decode the message.
 * - False Fail to decode the message.
 *
 */
bool pb_decode_uint_32_arr_cb(pb_istream_t *stream, const pb_field_t *field,
                              void **arg);

/**
 * @brief Encode a nested array of floats  This function is intended to
 * be used as a callback function during the encode process.
 *
 * @see pb_callback_s::encode.
 *
 * @param[in] stream The stream to encode nested array of floats.
 * @param[in] field  The format of the nested array of floats value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 *  - True  Successfully encode the message.
 *  - False Fail to encode the message.
 *
 */
bool pb_encode_float_arr_cb(pb_ostream_t *stream, const pb_field_t *field,
                            void *const *arg);

/**
 * @brief Decode a nested array of floats  This function is intended to
 * be used as a callback function during the decode process.
 *
 * @see pb_callback_s::decode
 *
 * @param[in] stream The stream from which to decode nested array of float from.
 * @param[in] field  The format of the nested array of float value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 * - True  Successfully decode the message.
 * - False Fail to decode the message.
 *
 */
bool pb_decode_float_arr_cb(pb_istream_t *stream, const pb_field_t *field,
                            void **arg);
/**
 * @brief Encode an array of string. This function is intended to
 * be used as a callback function during encode process.
 *
 * @see pb_callback_s::encode
 *
 * @param[in] stream The stream to encode nested array of strings.
 * @param[in] field  The format of the nested array of strings value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 *  - True  Successfully encode the message.
 *  - False Fail to encode the message.
 */
bool pb_encode_string_arr_cb(pb_ostream_t *stream, const pb_field_t *field,
                             void *const *arg);

/**
 * @brief Decode an array of strings. This function is intended to be used
 * as a callback function during the decode process
 *
 * @see pb_callback_s::decode
 *
 * @param[in] stream The stream from which to decode nested array of string
 * from.
 * @param[in] field  The format of the nested array of string value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 *  - True  Successfully encode the message.
 *  - False Fail to encode the message.
 */
bool pb_decode_string_arr_cb(pb_istream_t *stream, const pb_field_t *field,
                             void **arg);

/**
 * @brief Encode a string into a field. This function is intended to be used
 * as a callback function during the encode process.
 *
 * @see pb_callback_s::encode.
 *
 * @param[in] stream The stream to encode string.
 * @param[in] field  The format of the string.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 * - True  Successfully encode the message.
 * - False Fail to encode the message.
 *
 */
bool pb_encode_string_cb(pb_ostream_t *stream, const pb_field_t *field,
                         void *const *arg);

/**
 * @brief Decode a string from a field. This function is intended to be used as
 * a callback function during the decode process.
 *
 * @see pb_callback_s::decode.
 *
 * @param[in] stream  The stream to decode string.
 * @param[in] field   The format of the string.
 * @param[in] arg     The argument defined by caller.
 *
 * @return
 * - True  Successfully decode the message.
 * - False Fail to decode the message.
 *
 */
bool pb_decode_string_cb(pb_istream_t *stream, const pb_field_t *field,
                         void **arg);

/**
 * @brief Encode a sns_std_request and its Sensor-specific payload onto the
 * given buffer.
 *
 * @param[in] buffer           Where the encoded sns_std_request shall be
 *                             placed.
 * @param[in] buffer_len       Size of buffer.
 * @param[in] request_payload  The decoded structure to be encoded and placed
 *                             within sns_std_request::payload.
 * @param[in] payload_fields   Message definition associated with
 *                             request_payload.
 * @param[in] request          sns_std_request configuration,
 *                             NULL to use default values.
 *
 * @return
 * - size_t of encoded message.
 *
 */
size_t pb_encode_request(void *buffer, size_t buffer_len,
                         void const *request_payload,
                         pb_msgdesc_t const *payload_fields,
                         sns_std_request *request);

/**
 * @brief Decodes a request message.
 *
 * @param[in] req             Request to decode.
 * @param[in] payload_fields  Message definition associated with
 *                            request_payload.
 * @param[out] request        sns_std_request configuration; NULL to use default
 * values.
 * @param[out] decoded_req    Destination for the decoded request; This will be
 *                            ignored if request argument is provided.
 *
 * @return
 * - True  Successfully decode the message.
 * - False Fail to decode the message.
 *
 */
bool pb_decode_request(struct sns_request const *req,
                       pb_msgdesc_t const *payload_fields,
                       sns_std_request *request, void *decoded_req);

/**
 * @brief Encode an outgoing Event message; then publish that Event.
 * This utility function can only be used for simple events without
 *
 * @param[in] instance        Source of the event.
 * @param[in] fields          Message definition.
 * @param[in] c_struct        Decoded Event to send.
 * @param[in] timestamp       Timestamp of the Event.
 * @param[in] message_id      Message ID of the Event.
 * @param[in] sensor_uid      Sensor UID of the published event.
 *                            This can be NULL if Instance is not shared between
 *                            Sensors.
 *
 * @return
 * - True   Upon success.
 * - False  Otherwise.
 *
 */
bool pb_send_event(struct sns_sensor_instance *instance,
                   pb_msgdesc_t const *fields, void const *c_struct,
                   sns_time timestamp, uint32_t message_id,
                   struct sns_sensor_uid const *sensor_uid);

/**
 * @brief Determine encoded size of sns_std_sensor_event with variable length
 * floating point data payload. Construct, sns_std_sensor_event message
 * is as defined in sns_std_sensor.proto.
 *
 * @param[in] data     Data array of floats to be used as the data
 *                     payload as defined in sns_std_sensor.proto.
 * @param[in] data_len Length of the data array.
 *
 * @return
 * - size_t Size of the encoded message.
 *
 */
size_t pb_get_encoded_size_sensor_stream_event(float const *data,
                                               uint8_t data_len);

/**
 * @brief Construct, Encode and Publish an outgoing sns_std_sensor_event as
 * defined in sns_std_sensor.proto.
 *
 * @param[in] instance        Source of the event.
 * @param[in] sensor_uid      Sensor UID of the published event.
 * @param[in] timestamp       Timestamp of the published event.
 * @param[in] message_id      Message ID of the published event.
 * @param[in] sample_status   The sample status of the published
 *                            event as defined in sns_std_sensor.proto.
 * @param[in] data            Data array of floats to be used as the data
 *                            payload as defined in sns_std_sensor.proto.
 * @param[in] data_len        Length of the data array.
 * @param[in] encoded_len     Length of the encoded message as
 *                            determined by
 *                            pb_get_encoded_size_sensor_stream_event.
 *
 * @return
 * - sns_rc SNS_RC_SUCCESS on successfully sending event.
 *
 * - All other values are equal to failure.
 *
 */
sns_rc pb_send_sensor_stream_event(struct sns_sensor_instance *instance,
                                   struct sns_sensor_uid const *sensor_uid,
                                   sns_time timestamp, uint32_t message_id,
                                   sns_std_sensor_sample_status sample_status,
                                   float const *data, uint8_t data_len,
                                   size_t encoded_len);

/**
 * @brief Decode an sns_suid_event with variable number of SUIDs.
 *
 * @see pb_callback_s::decode.
 *
 * @param[in] stream nanopb input stream.
 * @param[in] field  nanopb field for decoding.
 * @param[in] arg    type sns_suid_search.
 *
 * @return
 * - True   On success.
 * - False  On failure.
 *
 */
bool pb_decode_suid_event(pb_istream_t *stream, const pb_field_t *field,
                          void **arg);

/**
 * @brief Optimized function for decoding a std_sensor_event.
 *
 * @param[in]  instance            Instance calling this function
 * @param[in]  encoded_event       encoded event data buffer.
 *                                 ( E.g: (sns_sensor_event *)event->event ).
 * @param[in]  encoded_event_len   Length of the event being encoded.
 * @param[in]  float_array_len     Available space in the float array.
 * @param[out] float_array         Will contain the decoded floats.
 * @param[out] decoded_array_size  Will contain the number of decoded floats.
 * @param[out] decoded_event       Will contain decoded fields.
 *
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool pb_decode_std_sensor_event(struct sns_sensor_instance *instance,
                                const char *encoded_event,
                                size_t encoded_event_len, float *float_array,
                                size_t float_array_len,
                                size_t *decoded_array_size,
                                sns_std_sensor_event *decoded_event);

/**
 * @brief Optimized function for decoding a interrupt event.
 * It is recommended that clients use the v2_version of this utility.
 *
 * @param[in]  instance      Instance calling this function.
 * @param[in]  encoded_event Pointer with encoded event (sns_sensor_event).
 * @param[out] decoded_event Pointer provided for outputing the decoded event
 *                           (sns_interrupt_event).
 *
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool pb_decode_interrupt_event(struct sns_sensor_instance *instance,
                               void const *encoded_event, void *decoded_event);

/**
 * @brief Optimized function for decoding a sns_timer_sensor_event.
 *        It is recommended that clients use the v2_version of this utility.
 *
 * @param[in]  instance      Instance calling this function.
 * @param[in]  event         pointer of event to be encoded of type:
 *                           (sns_sensor_event *).
 * @param[out] decoded_event Pointer provided for outputing the decoded event
 * of type: (sns_timer_sensor_event *).
 *
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool pb_decode_timer_event(struct sns_sensor_instance *instance,
                           void const *event, void *decoded_event);

/**
 * @brief Optimized function for encoding a varint into a buffer.
 *
 * @param[out]   buf buffer for encoding pb data.
 * @param[in]    buf_size size of buffer.
 * @param[in]    value Value to encode.
 * @param[out]   bytes_written Number of bytes encoded into buffer.
 *
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool sns_pb_encode_varint(uint8_t *buf, size_t buf_size, uint64_t value,
                          size_t *bytes_written);

/**
 * @brief Optimized function for encoding a fixed32 into a buffer.
 *
 * @param[out]   buf      Buffer for encoding pb data.
 * @param[in]    buf_size Size of buffer.
 * @param[in]    val      value to encode.
 *
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool sns_pb_encode_fixed32(uint8_t *buf, size_t buf_size, const uint32_t val);

/**
 * @brief Optimized function for encoding a fixed64 into a buffer.
 *
 * @param[out]   buf      Buffer for encoding pb data.
 * @param[in]    buf_size Size of buffer.
 * @param[in]    val      value to encode.
 *
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool sns_pb_encode_fixed64(uint8_t *buf, size_t buf_size, const uint64_t val);

/**
 * @brief Optimized function for encoding a dae data event into a buffer
 *
 *
 * @param[in]    timestamp      timestamp
 * @param[in]    ts_type        timestamp type
 * @param[in]    data_size      size of sensor data
 * @param[in]    sensor_data    sensor data buffer
 * @param[in]    buf_size       size of encoded event buffer
 * @param[out]   encoded_event  Buffer for encoding pb data
 * @param[out]   bytes_written  Bytes written to encoded_event
 *
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool pb_encode_dae_data_event(sns_time timestamp, uint8_t ts_type,
                              uint16_t data_size, uint8_t *sensor_data,
                              size_t buf_size, uint8_t *encoded_event,
                              size_t *bytes_written);

/**
 * @brief Optimized function for decoding a dae data event
 *
 * @param[in]  instance      Instance calling this function.
 * @param[in]  fields        Message descriptor of type
                             sns_dae_data_event_fields
 * @param[in]  event         pointer of event to be decoded of type:
 *                           (sns_sensor_event *).
 * @param[out] decoded_event Pointer provided for outputing the decoded event
 *                           of type: (sns_dae_data_event *).
 * @param[in] pb_proto_header_version Version of generated header file used.
 *                                    Should be set to PB_PROTO_HEADER_VERSION
 *
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool pb_decode_dae_data_event(struct sns_sensor_instance *instance,
                              const pb_msgdesc_t *fields, void const *event,
                              void *decoded_event,
                              uint32_t pb_proto_header_version);

/**
 * @brief Optimized function for decoding a sns_timer_sensor_event.
 *
 * @param[in]  instance      Instance calling this function.
 * @param[in]  fields        Message descriptor of type
                             sns_timer_sensor_event_fields
 * @param[in]  encoded_event pointer of event to be encoded of type:
 *                           (sns_sensor_event *).
 * @param[out] decoded_event Pointer provided for outputing the decoded event
 * of type: (sns_timer_sensor_event *).
 *
 * @param[in] pb_proto_header_version Version of generated header file used.
 *                                    Should be set to PB_PROTO_HEADER_VERSION
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool pb_decode_timer_event_v2(struct sns_sensor_instance *instance,
                              const pb_msgdesc_t *fields,
                              void const *encoded_event, void *decoded_event,
                              uint32_t pb_proto_header_version);

/**
 * @brief Optimized function for decoding a interrupt event.
 *
 * @param[in]  instance      Instance calling this function.
 * @param[in]  fields        Message descriptor of type
                             sns_interrupt_event_fields
 * @param[in]  encoded_event Pointer with encoded event (sns_sensor_event).
 * @param[out] decoded_event Pointer provided for outputing the decoded event
 *                           (sns_interrupt_event).
 * @param[in] pb_proto_header_version Version of generated header file used.
 *                                    Should be set to PB_PROTO_HEADER_VERSION
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool pb_decode_interrupt_event_v2(struct sns_sensor_instance *instance,
                                  const pb_msgdesc_t *fields,
                                  void const *encoded_event,
                                  void *decoded_event,
                                  uint32_t pb_proto_header_version);