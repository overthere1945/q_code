#pragma once
/*=============================================================================
  @file qsh_audio_data_sensor.h

  Audio Data Sensor (uses Audio Event Sensor base class)

  Copyright (c) 2021-2024 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_event_sensor.h"
#include "asps_api.h"
#include "qsocket_read_ep_api.h"
#include "asps_acd_usecase_api.h"
#include "asps_ads_usecase_api.h"

#define ADS_SUID                                                                                                       \
   {                                                                                                                   \
      .sensor_uid = { 0x9f, 0x6e, 0xf8, 0x99, 0x09, 0x51, 0x29, 0xf8, 0x7d, 0x35, 0xda, 0xb1, 0xf6, 0x0c, 0x4e, 0xf2 } \
   }

#define ADS_SAMPLING_RATE (16000.0f) //Hertz
#define ADS_DEFAULT_NUM_CHANNELS 1
#define ADS_DEFAULT_BIT_WIDTH 16
#define ADS_MAX_FRAME_SIZE_US (20000.0f)
#define ADS_MAX_CHANNELS 16
/*=============================================================================
  Type Definitions
  ===========================================================================*/
typedef struct
{
   // base class state structure.
   qsh_audio_event_sensor_state audio_event_state;

} qsh_audio_data_sensor_state_t;

// sensor callback function to receive the messages from adsp.
void qsh_audio_data_uqci_receive_msg(void *cb_ctx_ptr, uqsocket_msg_header_t *msg_header);

// get sensor uid
sns_sensor_uid const *qsh_audio_data_get_sensor_uid(sns_sensor const *this);

// sensor public api
sns_rc               qsh_audio_data_init(sns_sensor *const this);
sns_rc               qsh_audio_data_deinit(sns_sensor *const this);
sns_rc               qsh_audio_data_notify_event(sns_sensor *const this);
sns_sensor_instance *qsh_audio_data_set_client_request(sns_sensor *const this,
                                                       sns_request const *curr_req,
                                                       sns_request const *new_req,
                                                       bool               remove);
