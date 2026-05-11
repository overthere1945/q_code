#pragma once
/*=============================================================================
  @file qsh_upd_proxy_sensor_instance.h

  The upd proxy Sensor instance

  Copyright (c) 2021-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_sensor_instance.h"
#include "qsh_upd_proxy_sensor.h"
#include "sns_proximity.pb.h"

#include "asps_api.h"
#include "us_detect_api.h"
#include "asps_acd_usecase_api.h"
#include "asps_upd_usecase_api.h"

/*============================================================================
  Type Declarations
  ===========================================================================*/


//acs sensor state structure
typedef struct sns_upd_inst_state
{
  //handle of event service to publish upd proxy or error events.
  sns_event_service *event_service;
  
  //upd enable/disable.
  uint32_t upd_enable;
} qsh_upd_proxy_inst_state;

void qsh_upd_proxy_inst_publish_event(sns_sensor_instance *const this, event_id_upd_detection_event_t *event_ptr);

sns_rc qsh_upd_proxy_inst_init(sns_sensor_instance *this, sns_sensor_state const *state_ptr);
sns_rc qsh_upd_proxy_inst_deinit(sns_sensor_instance *const this);
sns_rc qsh_upd_proxy_inst_set_client_config(sns_sensor_instance *const this, sns_request const *client_request_ptr);
