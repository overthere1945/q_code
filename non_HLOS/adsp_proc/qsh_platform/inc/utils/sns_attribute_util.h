#pragma once
/** =============================================================================
 * @file sns_attribute_util.h
 *
 * @brief Utility functions provided by Qualcomm for use by Sensors to handle
 * and publish attributes. All utilities in this file can be used in island
 * mode.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 * ===========================================================================
 */

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "pb_decode.h"
#include "sns_std.pb.h"
#include "sns_std_sensor.pb.h"

/*=============================================================================
  Macros
  ===========================================================================*/

/* Macro to shorten the default initializer */
#define SNS_ATTR             sns_std_attr_value_data_init_default
#define SNS_DEFAULT_MIN_RATE (5.0f)
#define SNS_DEFAULT_MAX_RATE (200.0f)

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_sensor;

/*=============================================================================
  Public Function Declarations
  ===========================================================================*/

typedef struct
{
  struct sns_sensor *sensor;   /*!< Pointer to the sensor */
  int32_t attr_id;             /*!< Attribute ID. */
  int32_t attr_value;          /*!< Int Attribute value. */
  float flt_attr_value;        /*!< Float Attribute value. */
  char attr_string[32];        /*!< String Attribute value. */
} sns_sensor_util_attrib;

typedef bool (*sns_attr_comp_fcn)(
    const sns_sensor_util_attrib *restrict attrib_decoded,
    const sns_sensor_util_attrib *restrict attrib_match);

/**
 * @brief Structure containing attributes and their associated compare
 * function to be used in the generic_lookup_cb.
 */
typedef struct
{
  sns_sensor_util_attrib *attrib;    /*!< Pointer to the attribute. */
  sns_attr_comp_fcn attr_comp_fcn;   /*!< Compare function pointer. */
} sns_sensor_util_generic_attrib;

/**
 * @brief Decode sensor attribute.
 *
 * @param[inout] stream  The stream to decode attribute value from.
 * @param[in] field      The format of the attribute value.
 * @param[in] arg        The argument defined by caller.
 *
 * @return bool
 *  - True if decoded successfully.
 *  - False if fail to decode.
 *
 */
bool sns_sensor_util_decode_attr(pb_istream_t *stream, const pb_field_t *field,
                                 void **arg);

/**
 * @brief Decode sensor attribute list.
 *
 * @param[in,out] stream The stream to decode attribute value from.
 * @param[in] field      The format of the attribute value.
 * @param[in] arg        The argument defined by caller.
 *                       arg is an array of sns_sensor_util_attrib data 
 *                       structure. For last entry, attribute id must be -1
 * @example
 *     sns_sensor_util_attrib attrib_list[] =
 *      { {.sensor = sensor, .attr_id = SNS_STD_SENSOR_ATTRID_RIGID_BODY},
 *        {.sensor = sensor, .attr_id = SNS_STD_SENSOR_ATTRID_RATES},
 *        {.sensor = NULL,   .attr_id = -1}
 *      };
 *
 * @return bool
 *  - True if decoded successfully.
 *  - False if fail to decode.
 *
 */
bool sns_sensor_util_decode_attr_list(pb_istream_t *stream,
                                      const pb_field_t *field, void **arg);

/**
 * @brief Encode an array of attribute values into an attribute. This function
 * is intended to be used when publishing attributes.
 *
 * @param[inout] stream  The stream to encode attribute value to.
 * @param[in] field       The format of the attribute value.
 * @param[in] arg         Type pb_buffer_arg (.buf = array, .buf_len = array
 * length).
 *
 * @see pb_callback_s::encode
 *
 * @return bool
 * - True if encoded successfully.
 * - False if fail to encode.
 *
 */
bool sns_pb_encode_attr_cb(pb_ostream_t *stream, const pb_field_t *field,
                           void *const *arg);

/**
 * @brief Publish a simple attribute using the Attribute Service.
 *
 * @param[in] sensor        Whose attributes to publish.
 * @param[in] attribute_id  ID corresponding to attribute.
 * @param[in] values        Array of decoded attribute values.
 * @param[in] values_len    Number of entries in values array.
 * @param[in] completed     Whether this is the last attribute in this publish
 *                          set.
 *
 * @return
 *  None
 */
void sns_publish_attribute(struct sns_sensor *const sensor,
                           uint32_t attribute_id,
                           struct _sns_std_attr_value_data const *values,
                           uint32_t values_len, bool completed);
