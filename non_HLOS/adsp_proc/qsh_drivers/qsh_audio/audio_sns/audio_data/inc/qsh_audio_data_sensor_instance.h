#pragma once
/*=============================================================================
  @file qsh_audio_data_sensor_instance.h

  The Audio Data Sensor instance

  Copyright (c) 2021-2024 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_sensor_instance.h"
#include "qsh_audio_data_sensor.h"
#include "qsh_audio_data.pb.h"

#include "asps_api.h"
#include "qsocket_read_ep_api.h"
#include "common_enc_dec_api.h"
#include "asps_acd_usecase_api.h"
#include "asps_ads_usecase_api.h"
/*============================================================================
  Type Declarations
  ===========================================================================*/
#define IGNORE_RSP_TOKEN (0x11001001) // some random number for command token.

// structure to cache the configuration requested by client
typedef struct
{
   uint32_t history_us;
   uint32_t stream_type;
   uint32_t batch_us;
   uint32_t sampling_rate;
   uint32_t bit_width;
   uint32_t num_channels;

   uint8_t mapped_ch_type[ADS_MAX_CHANNELS];

   bool_t has_media_fmt;
   bool_t requires_buffering;
} qsh_audio_data_inst_data_config_t;

// structure to cache the ReadEP module instance ID (in ADSP domain)
typedef struct
{
   uint32_t sns_rd_ep_miid;
   uint32_t mfc_miid;
   uint32_t pcm_cnv_miid;
} qsh_audio_data_usecase_info_t;

// ads sensor state structure
typedef struct sns_ads_inst_state
{
   // handle of event service to publish data or error events.
   sns_event_service *event_service;

   // event config received from the client
   qsh_audio_data_inst_data_config_t inst_data_config;

   // client handle of uQsocket client interface
   void *uqci_client_handle;

   // client-id
   uint32_t client_id;

   // ADSP-DATA usecase info
   qsh_audio_data_usecase_info_t usecase_info;

   sns_sensor_uid const *suid_ptr;

   //flag indicating if data-flow configuration is pending or not.
   bool_t data_flow_pending;

   //flag to indicate if streaming is active.
   bool_t data_flow_enable;

   //flag to indicate if streaming is active due to flush request.
   bool_t flush_req;
} qsh_audio_data_inst_state_t;

// function to handle the received event for usecase info from the asps-adsp.
void qsh_audio_data_receive_usecase_info(sns_sensor_instance *const this,
                                         asps_audio_data_usecase_register_ack_payload_t *pcm_usecase_info_ptr);

void qsh_audio_data_uqci_send_data_flow(qsh_audio_data_inst_state_t *inst_state_ptr);

void qsh_audio_data_handle_resp(sns_sensor_instance *const this,
                                uint32_t opcode,
                                uint32_t token,
                                uint32_t payload_len,
                                void *   payload_ptr);

sns_rc qsh_audio_data_inst_init(sns_sensor_instance *this, sns_sensor_state const *state_ptr);
sns_rc qsh_audio_data_inst_deinit(sns_sensor_instance *const this);
sns_rc qsh_audio_data_inst_set_client_config(sns_sensor_instance *const this, sns_request const *client_request_ptr);
