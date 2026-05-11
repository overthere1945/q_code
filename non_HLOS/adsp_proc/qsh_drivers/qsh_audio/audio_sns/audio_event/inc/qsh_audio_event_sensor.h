#pragma once
/*=============================================================================
  @file qsh_audio_event_sensor.h

  Base Class of audio event sensor.

  Copyright (c) 2020-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_sensor_uid.h"
#include "sns_sensor.h"
#include "sns_data_stream.h"
#include "sns_sensor_util.h"
#include "sns_diag_service.h"
#include "sns_suid_util.h"
#include "sns_std_type.pb.h"
#include "sns_std_sensor.pb.h"
#include "sns_event_service.h"
#include "sns_power_mgr_service.h"
#include "sns_types.h"
#include "sns_suid.pb.h"
#include "sns_pb_util.h"
#include "sns_attribute_util.h"
#include "sns_printf.h"
#include "sns_stream_service.h"
#include "sns_registry.pb.h"
#include "sns_math_util.h"
#include "sns_mem_util.h"
#include "qsh_audio_island.h"
#include "sns_service.h"
#include "sns_memory_service.h"

#include "uqci_api.h"
#include "qsh_invoke.h"

#include "qurt_timer.h"

//macro to enable register query to get remote process's Qsocket address.
//#define QSH_AUDIO_QUERY_REGISTRY


#ifndef QSH_AUDIO_QUERY_REGISTRY
// Qsocket service id of remote process
#define QSH_AUDIO_RP_SERVICE_ID (0x1008)
// Qsocket instance id of remote process
#define QSH_AUDIO_RP_INSTANCE_ID (0x0)
#endif

//MCPS requirement for qsh-audio sensor
#define QSH_AUDIO_MCPS (5)

/*=============================================================================
  Type Definitions
  ===========================================================================*/

#ifdef QSH_AUDIO_QUERY_REGISTRY

typedef struct
{
  bool is_config_received;
  uint32_t rp_svc_id;
  uint32_t rp_svc_instance_id;
} qsh_audio_event_config;

#endif

//acs sensor state structure
typedef struct sns_audio_event_sensor_state {
  // Points to our SUID
  sns_sensor_uid const *suid_ptr;

  // flag; power collapse is blocked
  bool is_blocking_pc;

#ifdef QSH_AUDIO_QUERY_REGISTRY
  // Requests to the registry sensor
  sns_data_stream *registry_stream_ptr;

  // registry
  SNS_SUID_LOOKUP_DATA(1) suid_lookup_data;

  bool first_pass;

  qsh_audio_event_config config;
#endif
  //power manager handle to vote for mcps
  sns_power_mgr_handle *power_mgr_client_ptr;

  //service handle of uQsocket client interface
  void *uqci_svc_handle_ptr;

  //qsh invoke handle
  qsh_invoke_handle *qsh_invoke_handle_ptr;

  //mmpm client id to place vote requests
  uint32_t mmpm_client_id;
} qsh_audio_event_sensor_state;


sns_rc qsh_audio_event_init(sns_sensor *const this, char* sns_name);
sns_rc qsh_audio_event_deinit(sns_sensor *const this);
sns_rc qsh_audio_event_notify_event(sns_sensor *const this);
sns_sensor_instance *qsh_audio_event_set_client_request(sns_sensor *const this,
                                                        sns_request const *curr_req,
                                                        sns_request const *new_req,
                                                        bool               remove,
                                                        uint32_t           size_of_instance_state);


sns_rc qsh_audio_event_send_error_(sns_sensor *const this, sns_std_error error);
sns_rc qsh_audio_event_send_error(sns_sensor *const this, sns_std_error error);
sns_rc qsh_audio_event_instance_send_error(sns_sensor_instance *const this,
                                           sns_std_error error,
                                           sns_sensor_uid const *suid_ptr);
sns_rc qsh_audio_event_inst_send_error_(sns_sensor_instance *const this,
                                       sns_std_error error,
                                       sns_sensor_uid const *suid_ptr);

void qsh_audio_event_block_power_collapse(sns_sensor *const this);
void qsh_audio_event_unblock_power_collapse(sns_sensor *const this);
