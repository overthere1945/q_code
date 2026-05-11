#pragma once
/*=============================================================================
  @file qsh_upd_proxy_sensor.h

  UPD Proxy Sensor

  Copyright (c) 2021-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_event_sensor.h"
#include "asps_api.h"
#include "us_detect_api.h"
#include "asps_acd_usecase_api.h"
#include "asps_upd_usecase_api.h"

#define UPD_SUID \
{  \
  .sensor_uid =  \
  {  \
    0xc4, 0xab, 0x70, 0xd8, 0x82, 0x3c, 0x40, 0xc7, \
    0xb1, 0x83, 0x34, 0xd3, 0xaa, 0xd6, 0x33, 0xca  \
  }  \
}

#ifndef MIN
#define MIN(a, b)  (((a)<(b))?(a):(b))
#endif

#define IGNORE_RSP_TOKEN (0x11001001) //some random number for command token.

/*=============================================================================
  Type Definitions
  ===========================================================================*/
//audio context sensor state structure.
typedef struct
{
  //base class state structure.
  qsh_audio_event_sensor_state audio_event_state;
  
  //client handle of uQsocket client interface
  void *uqci_client_handle;
  
  //0: detection is disabled (remove usecase)
  //1: detection is enabled (start usecase)
  uint32_t upd_enable;
  
  //module_iid of the upd module in ADSP
  uint32_t module_iid;
} qsh_upd_proxy_sensor_state;

// sensor callback function to receive the messages from adsp.
void qsh_upd_proxy_uqci_receive_msg(void *cb_ctx_ptr, uqsocket_msg_header_t *msg_header);

// function to send usecase info event registration to asps-adsp.
void qsh_upd_proxy_uqci_register_usecase_info_event(qsh_upd_proxy_sensor_state *sensor_state_ptr, bool is_register);

// function to handle the received event for usecase info from the asps-adsp.
void qsh_upd_proxy_receive_usecase_info(sns_sensor *const this, asps_upd_usecase_register_ack_payload_t *usecase_info_ptr);

// sensor callback function to send the event registration to the module.
sns_rc qsh_upd_proxy_req_send(sns_sensor *const this);

// sensor public api
sns_rc qsh_upd_proxy_init(sns_sensor *const this);
sns_rc qsh_upd_proxy_deinit(sns_sensor *const this);
sns_rc  qsh_upd_proxy_notify_event(sns_sensor *const this);
sns_sensor_instance * qsh_upd_proxy_set_client_request(sns_sensor *const this,
                                                       sns_request const *curr_req,
                                                       sns_request const *new_req,
                                                       bool               remove);
sns_sensor_uid const *qsh_upd_proxy_get_sensor_uid(sns_sensor const *this);
