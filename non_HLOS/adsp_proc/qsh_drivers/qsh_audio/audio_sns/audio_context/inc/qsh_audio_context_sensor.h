#pragma once
/*=============================================================================
  @file qsh_audio_context_sensor.h

  Audio Context Sensor (uses Audio Event Sensor base class)

  Copyright (c) 2020-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_event_sensor.h"
#include "asps_api.h"
#include "acd_api.h"
#include "asps_acd_usecase_api.h"

#define ACS_SUID \
{  \
  .sensor_uid =  \
  {  \
    0x90, 0x11, 0xcb, 0x1f, 0x44, 0xb4, 0x38, 0x99, \
    0x06, 0x5f, 0xae, 0x6d, 0xbf, 0x75, 0x72, 0x80  \
  }  \
}

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define IGNORE_RSP_TOKEN (0x11001001) //some random number for command token.

/*=============================================================================
  Type Definitions
  ===========================================================================*/

// structure to cache the aggregated context event configuration requested by all sensor clients
typedef struct
{
   // required number of contexts
   uint32_t num_context;

   // array size of #event_config_arr
   uint32_t event_config_arr_size;

   // configuration of each context
   // this is dynamically allocated memory with array size of #event_config_arr_size
   acd_context_specific_generic_reg_info_t *event_config_arr;
} qsh_audio_context_aggr_config_t;

// structure to cache the ACD module instance ID (in ADSP domain) for each context ID
typedef struct
{
   // number of contexts for which miid in ADSP is known
   uint32_t num_context;

   // context id and corresponding miid in ADSP domain.
   // these are dynamically allocated array of size #num_context each.
   uint32_t *context_id_arr;
   uint32_t *module_iid_arr;
} qsh_audio_context_usecase_info;

// audio context sensor state structure.
typedef struct
{
   // base class state structure.
   qsh_audio_event_sensor_state audio_event_state;

   // client handle of uQsocket client interface
   void *uqci_client_handle;

   // flag to check if context list query is pending or not
   bool is_context_list_query_pending;

   // aggregated configuration
   qsh_audio_context_aggr_config_t aggr_config;

   // ADSP-ACD usecase info
   qsh_audio_context_usecase_info usecase_info;
} qsh_audio_context_sensor_state;

// sensor callback function to receive the messages from adsp.
void qsh_audio_context_uqci_receive_msg(void *cb_ctx_ptr, uqsocket_msg_header_t *msg_header);

// function to send usecase info event registration to asps-adsp.
void qsh_audio_context_uqci_register_usecase_info_event(qsh_audio_context_sensor_state *sensor_state_ptr,
                                                        bool                            is_register);

// function to handle the received event for usecase info from the asps-adsp.
void qsh_audio_context_receive_usecase_info(sns_sensor *const this,
                                            asps_acd_usecase_register_ack_payload_t *acd_usecase_info_ptr,
                                            uint32_t                                 payload_size);

// sensor public api
sns_rc               qsh_audio_context_init(sns_sensor *const this);
sns_rc               qsh_audio_context_deinit(sns_sensor *const this);
sns_rc               qsh_audio_context_notify_event(sns_sensor *const this);
sns_sensor_instance *qsh_audio_context_set_client_request(sns_sensor *const this,
                                                          sns_request const *curr_req,
                                                          sns_request const *new_req,
                                                          bool               remove);
