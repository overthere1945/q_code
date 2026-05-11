#pragma once
/** =============================================================================
 * @file
 *
 * @brief Provides access to all service objects. Sensors must query for the
 * needed service each time they need it; the Framework reserves the right
 * to move a service into/out of island mode, and pointers kept to it may
 * break.
 * It would be appropriate within sns_sensor::init to query for all required
 * services, to ensure the Sensor is capable of operating.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdint.h>
#include "sns_service.h"

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief Service Manager APIs.
 *
 */
typedef struct sns_service_manager
{
  uint32_t struct_len; /*!< Length of sns_service_manager structure. */

  /**
   * @brief Query the Framework for a particular service.
   *
   * @param[in] manager Reference to the Service Manager.
   * @param[in] service Service for which to query.
   *
   * @return 
   *  - sns_service* Service reference; shall be casted by the client to the 
   *                 appropriate service object.
   *  - NULL         If not present. 
   */
  sns_service *(*get_service)(struct sns_service_manager *manager,
                              sns_service_type service);
} sns_service_manager;
