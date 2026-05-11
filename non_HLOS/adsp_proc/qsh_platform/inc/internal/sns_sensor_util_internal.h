#pragma once
/** ============================================================================
 * @file
 *
 * @brief Internal utility functions for use by Qualcomm Sensor Developers.
 *
 * @note All utilities in this file can be used in island mode.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

#include "sns_sensor_instance.h"
#include "sns_sensor_uid.h"
#include "sns_std.pb.h"

#define SNS_ATTR_ARRAY_MAX_LEN  4
#define SNS_ATTR_STRING_MAX_LEN 32
/**
 * @brief Attribute structure extension.
 *
 */
typedef struct
{
  struct sns_sensor *sensor;                    /*!< Sensor pointer. */
  int32_t attr_id;                              /*!< Attribute ID.*/
  int32_t attr_value[SNS_ATTR_ARRAY_MAX_LEN];   /*!< Attribute value.*/
  float flt_attr_value[SNS_ATTR_ARRAY_MAX_LEN]; /*!< Float attribute value.*/
  char attr_string[SNS_ATTR_ARRAY_MAX_LEN][SNS_ATTR_STRING_MAX_LEN];
  uint8_t attr_array_cnt; /*!< Attribute count.*/
} sns_sensor_util_attrib_ext;

/**
 * @brief Consolidate the standard request from among all clients of the
 * instance Find minimum batch period ,maximum flush period, max batch flag and
 * flush only flag.
 *
 * @param[in] instance     The sensor instance.
 * @param[in] suid         The suid for which clients are being evaluated.
 * @param[out] std_req     The standard request to be modified.
 *
 * @return
 *  - SNS_RC_SUCCESS:      If consolidated request was successfully determined.
 *  - SNS_RC_FAILED:       If there was an error in decoding the requests.
 *  - SNS_RC_INVALID_STATE:  If the instance for this suid has no clients.
 *
 */
sns_rc sns_sensor_util_find_consolidated_req(sns_sensor_instance *instance,
                                             sns_sensor_uid const *suid,
                                             sns_std_request *std_req);
/**
 * @brief Decode sensor attribute value, currently only support int.
 *
 * @param[in] stream The stream to decode attribute value from.
 * @param[in] field  The format of the attribute value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 *  - True:  If decoded successfully.
 *  - False: Otherwise.
 *
 */
bool sns_sensor_util_decode_attr_value(pb_istream_t *stream,
                                       const pb_field_t *field, void **arg);

/**
 * @brief Decode sensor attributes in an array.
 *
 * @param[in] stream The stream to decode attribute value from.
 * @param[in] field  The format of the attribute value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 *  - True:  If decoded successfully.
 *  - False: Otherwise.
 *
 */
bool sns_sensor_util_decode_attr_ext(pb_istream_t *stream,
                                     const pb_field_t *field, void **arg);

/**
 * @brief Decode sensor attribute values for arrays of attributes.
 *
 * @param[in] stream The stream to decode attribute value from.
 * @param[in] field  The format of the attribute value.
 * @param[in] arg    The argument defined by caller.
 *
 * @return
 *  - True:  If decoded successfully.
 *  - False: Otherwise.
 *
 */
bool sns_sensor_util_decode_attr_value_ext(pb_istream_t *stream,
                                           const pb_field_t *field, void **arg);

/**
 * @brief Send a response event for on-change requests for the case where
 * there is no relevant config information to be shared with the client.
 *
 * @param[in] instance   Reference to the instance generating this event.
 * @param[in] uid        SUID of the sensor.
 *
 * @return
 *   - None.
 *
 */
void sns_sensor_util_send_onchange_resp_event(sns_sensor_instance *instance,
                                              sns_sensor_uid const *uid);
