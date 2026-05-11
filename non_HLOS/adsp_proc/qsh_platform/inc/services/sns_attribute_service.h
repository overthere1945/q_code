#pragma once
/** =============================================================================
 * @file
 *
 * @brief All Sensors may publish a series of attributes, where each is a
 * key-value pair.  The key is an enum value uniquely identifying the attribute,
 * while the value is an Protocol Buffer encoded message of type sns_std_attr.
 * These are attributes of the Sensor, not of the Sensor Instance, and hence
 * cannot contain client-specific information.  Most attributes are expected
 * to be published at boot-up, but Sensors are permitted to publish updated
 * attributes later, if the state of the system changes.  All clients of active
 * Sensor Instances, as well as any client who previously registered for
 * attribute information, will receive an update with the new values.
 * Some attributes are to be used solely by the Framework, others are intended
 * to be used by the Sensor client, and some will be used by both.  Note that
 * these attributes will be seen and shared between Sensor objects.  External
 * clients may receive a supersubset of this list, depending on the design of
 * the QMI proxy module (i.e. some attributes may be filtered-out, while others,
 * such as the maximum batch size, may be added-in).  However, all clients will
 * receive the same list, no filtering is performed on a per-client basis.
 * The Framework shall define a list of "standard" attributes IDs
 * (sns_std_sensor_attr_id).  These will be well known "keys" distributed by
 * Qualcomm.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "sns_rc.h"
#include "sns_service.h"

/*==============================================================================
  Forward Declarations
  ============================================================================*/

struct sns_sensor;

/*==============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief An unique identifier of a Sensor attribute.  See
 * sns_std_sensor_attr_id for standard types; Sensors may themselves define
 * their own.
 *
 */
typedef uint32_t sns_attribute_id;

/**
 * @brief The attribute service. Can be obtained from
 * sns_service_managere::getService.
 *
 */
typedef struct sns_attribute_service
{

  sns_service service;                         /*!< Service information. */
  const struct sns_attribute_service_api *api; /*!< Public API provided by the
                                                *   Framework to be used by the
                                                *   Sensor.
                                                */
} sns_attribute_service;

/**
 * @brief API made available to Sensors, to manage their published attributes.
 *
 */
typedef struct sns_attribute_service_api
{
  uint32_t struct_len; /*!< Length of sns_attribute_service_api structure. */

  /**
   * @brief Publish a new or updated attribute.
   * Set completed flag to true, once a complete set of attributes
   * has been published.  This does not preclude updating attributes again
   * in the future.
   * Standard attribute types are defined in sns_std_sensor_attr_id.
   *
   * @param[out] service        Attribute service reference.
   * @param[in]  sensor         Whose attributes to publish.
   * @param[in]  attribute      Encoded attribute (sns_std_attr) to publish.
   * @param[in]  completed      Whether this is the last attribute in this
   *                            publish set.
   *
   * @return sns_rc , SNS_RC_SUCCESS   Success, Any other value  Failure.
   *
   */
  sns_rc (*publish_attribute)(sns_attribute_service *service,
                              struct sns_sensor *sensor, void const *attribute,
                              uint32_t attribute_len,
                              sns_attribute_id attribute_id, bool completed);
} sns_attribute_service_api;
