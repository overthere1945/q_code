#pragma once
/*=============================================================================
  @file qsh_audio_context_sensor_instance.h

  The Audio Context Sensor instance

  Copyright (c) 2020-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_sensor_instance.h"
#include "qsh_audio_context_sensor.h"
#include "qsh_audio_context.pb.h"

#include "asps_api.h"
#include "acd_api.h"
#include "asps_acd_usecase_api.h"

/*============================================================================
  Type Declarations
  ===========================================================================*/
#define MAX_CONTEXT_PER_INSTANCE (3)

// structure to cache the event configuration requested by client
typedef struct
{
   uint32_t num_context;

   bool is_event_ongoing[MAX_CONTEXT_PER_INSTANCE];

   acd_context_specific_generic_reg_info_t event_config[MAX_CONTEXT_PER_INSTANCE];
} qsh_audio_context_config_t;

// acs sensor state structure
typedef struct sns_acs_inst_state
{
   // handle of event service to publish context or error events.
   sns_event_service *event_service;

   // event config received from the client
   qsh_audio_context_config_t inst_config;

} qsh_audio_context_inst_state;

void qsh_audio_context_inst_publish_supported_context(sns_sensor_instance *const this,
                                                      param_id_asps_get_supported_context_ids_t *context_list_ptr);

void qsh_audio_context_inst_publish_event(sns_sensor_instance *const this,
                                          uint32_t       num_context,
                                          qsh_audio_context_event *event_buf_ptr,
                                          sns_time       timestamp);

sns_rc qsh_audio_context_inst_init(sns_sensor_instance *this, sns_sensor_state const *state_ptr);
sns_rc qsh_audio_context_inst_deinit(sns_sensor_instance *const this);
sns_rc qsh_audio_context_inst_set_client_config(sns_sensor_instance *const this, sns_request const *client_request_ptr);
