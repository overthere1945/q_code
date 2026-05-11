#pragma once
/** =============================================================================
 * @file
 *
 * @brief Manages stream requests from Sensors or Sensor Instances.
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
#include "sns_sensor_uid.h"
#include "sns_service.h"

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_sensor;
struct sns_sensor_instance;
struct sns_data_stream;

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief The Stream Manager.  Will be obtained from
 * sns_service_manager::get_service.
 *
 */
typedef struct sns_stream_service
{
  sns_service service;                      /*!< Service information. */
  const struct sns_stream_service_api *api; /*!< Public api provided by the
                                             *   framework to be used by the
                                             *   sensor. */
} sns_stream_service;

/**
 * @brief The Stream Service APIs.
 *
 */
typedef struct sns_stream_service_api
{
  uint32_t struct_len; /*!< Length of sns_stream_service_api structure. */

  /**
   * @brief Initialize a new stream to be used by a Sensor.
   *
   * @param[in]   service       Stream Service reference.
   * @param[in]   sensor        The Sensor requesting the stream.
   * @param[in]   sensor_uid    To whom to make the connection.
   * @param[out]  data_stream   Stream reference allocated by the Framework.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*create_sensor_stream)(sns_stream_service *service,
                                 struct sns_sensor *sensor,
                                 sns_sensor_uid sensor_uid,
                                 struct sns_data_stream **data_stream);

  /**
   * @brief Initialize a new stream to be used by a Sensor Instance.
   *
   * @param[in]   service       Stream Service reference.
   * @param[in]   sensor        The Sensor Instance requesting the stream.
   * @param[in]   sensor_uid    To whom to make the connection.
   * @param[out]  data_stream   Stream reference allocated by the Framework.
   *
   * @return
   *  - SNS_RC_SUCCESS        Success.
   *  - SNS_RC_POLICY         The Sensor has exceeded the maximum stream count.
   *  - SNS_RC_NOT_AVAILABLE  The given SUID is not presently available.
   *  - Any other value       Failure.
   *
   */
  sns_rc (*create_sensor_instance_stream)(sns_stream_service *service,
                                          struct sns_sensor_instance *instance,
                                          sns_sensor_uid sensor_uid,
                                          struct sns_data_stream **data_stream);

  /**
   * @brief Remove a stream created by create_sensor_stream/
   * create_sensor_instance_stream.
   *
   * @param[in] service     Stream Service reference.
   * @param[in] data_stream Data Stream to remove and free.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  sns_rc (*remove_stream)(sns_stream_service *service,
                          struct sns_data_stream *data_stream);
} sns_stream_service_api;
