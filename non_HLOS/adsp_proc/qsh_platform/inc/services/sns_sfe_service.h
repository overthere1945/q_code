#pragma once
/**=============================================================================
 * @file
 *
 * @brief Definitions for Sensing Front End Service
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/**
  ==============================================================================
  Include Files
  ==============================================================================
*/
#include "sns_dae.pb.h"
#include "sns_rc.h"
#include "sns_service.h"
#include <stdint.h>

/**
  ==============================================================================
  Macros
  ==============================================================================
*/
/** Notification Mask for this service */
#define SNS_SFE_SIG_CHANNEL_ACTIVE     0x01
#define SNS_SFE_SIG_CHANNEL_DATA_READY 0x02
#define SNS_SFE_SIG_CHANNEL_SUSPENDED  0x04
#define SNS_SFE_SIG_CHANNEL_DRAINED    0x08

/**
  ==============================================================================
  Forward Declarations
  ==============================================================================
*/
struct sns_sensor_instance;

/**
  ==============================================================================
  Function Definitions
  ==============================================================================
*/
/**
 * @brief Function prototype for SFE service instance events.
 *
 * @param[in] instance  The sensor instance to receive the callback
 * @param[in] signal    Signal mask.
 * @param[in] rc        Result code
 *
 * @return
 *   - None.
 *
 */
typedef void (*sns_sfe_instance_cb)(struct sns_sensor_instance *instance,
                                    uint32_t signal, sns_rc result);

/**
  ==============================================================================
  Structures
  ==============================================================================
*/

/**
 * @brief Vendor Driver Channel
 *
 */
typedef struct sns_sfe_vdc sns_sfe_vdc;

/**
 * @brief public state used by the Framework for the sensor file service.
 *
 */
typedef struct sns_sfe_service_api
{
  uint32_t struct_len; /*!< Length of this structure. */

  /**
   * @brief Registers a vendor driver channel
   *
   * @param[in]  this   The sensor instance for which to register a VDC
   * @param[in]  cfg    The configuration with which to initialize the VDC
   * @param[in]  cb     The callback for reporting activities on the VDC
   * @param[out] vdc    Destination for the VDC
   *
   * @return
   *  SNS_RC_SUCCESS        A VDC has been allocated and initialized
   *  SNS_RC_NOT_AVAILABLE  No resources available
   *  SNS_RC_NOT_SUPPORTED  Not implemented
   *
   */
  sns_rc (*channel_register)(struct sns_sensor_instance *const this,
                             sns_dae_set_static_config const *cfg,
                             sns_sfe_instance_cb cb, sns_sfe_vdc **out_vdc);

  /**
   * @brief Activates the given vendor driver channel
   *
   * @param[in]  vdc  The VDC to activate
   * @param[in]  req  The configuration with which to activate the VDC
   *
   * @return
   *  SNS_RC_SUCCESS        A VDC is being activated
   *  SNS_RC_INVALID_VALUE  No such VDC found
   *  SNS_RC_NOT_SUPPORTED  Not implemented
   *
   * @note Registered callback will be called when VDC is in active state
   *
   */
  sns_rc (*channel_activate)(sns_sfe_vdc *vdc,
                             sns_dae_set_streaming_config const *req);

  /**
   * @brief Drains the given vendor driver channel
   *
   * @param[in]  vdc  The VDC to drain
   *
   * @return
   *  SNS_RC_SUCCESS        The VDC is being drained
   *  SNS_RC_INVALID_VALUE  No such VDC found
   *  SNS_RC_NOT_SUPPORTED  Not implemented
   *
   * @note Registered callback will be called when/if there are samples and
   *       again to reported drain operation completion
   *
   *
   */
  sns_rc (*channel_drain)(sns_sfe_vdc *vdc);

  /**
   * @brief Suspends the given vendor driver channel
   *
   * @param[in]  vdc  The VDC to suspend
   *
   * @return
   *  SNS_RC_SUCCESS        The VDC is being suspended
   *  SNS_RC_INVALID_VALUE  No such VDC found
   *  SNS_RC_NOT_SUPPORTED  Not implemented
   *
   * @note Registered callback will be called when VDC is suspended
   *
   */
  sns_rc (*channel_suspend)(sns_sfe_vdc *vdc);

  /**
   * @brief Updates Camera Controller (todo: Camera team to update)
   *
   * @param[in]  vdc  The VDC to read from
   *
   * @return
   *  SNS_RC_SUCCESS        Successfully read ...
   *  SNS_RC_INVALID_VALUE  Invalid input parameters
   *  SNS_RC_NOT_SUPPORTED  Not implemented
   *
   * @note This API is synchronous
   *
   */
  sns_rc (*channel_cam_ctrl_update)(sns_sfe_vdc *vdc, uint32_t cam_ctrl);

  /**
   * todo: more functions for camera
   */

  /**
   * @brief Deregisters the given vendor driver channel
   *
   * @param[in]  vdc  Reference to the VDC to deregister
   *
   * @return
   *  SNS_RC_SUCCESS        Given VDC found and deregistered
   *  SNS_RC_INVALID_VALUE  No such VDC found
   *  SNS_RC_NOT_SUPPORTED  Not implemented
   *
   */
  sns_rc (*channel_deregister)(sns_sfe_vdc **vdc);

} sns_sfe_service_api;

/**
 * @brief The SFE Service
 * Will be obtained from sns_service_manager::get_service.
 *
 */
typedef struct sns_sfe_service
{
  sns_service service;      /*!< Service information. */
  sns_sfe_service_api *api; /*!< Public api to be used by the sensors.
                             */
} sns_sfe_service;
