#pragma once
/*=============================================================================
  @file sns_ai_oem1_sensor.h
  
  The ai_oem1 virtual Sensor
  
  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/
  
/*=============================================================================
  Include Files
  ===========================================================================*/
  
#include "sns_sensor_uid.h"
#include "sns_memory_service.h"
#include "sns_sensor.h"
#include "sns_data_stream.h"
#include "sns_resampler.pb.h"
#include "sns_sensor_util.h"
#include "sns_diag_service.h"
#include "sns_suid_util.h"
#include "sns_island_mem.h"

/*=============================================================================
  Static Data Definitions
  ============================================================================*/

#define AI_OEM1_SUID \
{  \
  .sensor_uid =  \
  {  \
    0xee, 0xf4, 0x36, 0xf8, 0x28, 0x75, 0x16, 0xa7, \
    0xa1, 0x96, 0x4b, 0x50, 0x7b, 0xf7, 0xe3, 0xe0  \
  }                                                 \
}

//dependent sensors for ai_oem1 :: accel, resampler
#define SNS_AI_OEM1_NUM_MANDATORY_DEP_SENOSRS   2

//By default AI_OEM1 output is event type i.e. ON_CHANGE
//Minumum sample rate for ai_oem1
#define AI_OEM1_MINUMUM_SAMPLE_RATE 1.0

/*=============================================================================
  Type Definitions
  ===========================================================================*/

typedef struct sns_ai_oem1_sensor_state {

  // Points to our SUID
  sns_sensor_uid const *suid;

  // memory pool name
  sns_island_mem_util mem_pool_info;

  // resampler, accel
  SNS_SUID_LOOKUP_DATA(SNS_AI_OEM1_NUM_MANDATORY_DEP_SENOSRS) suid_lookup_data; 

  // handle to diag_service
  sns_diag_service *diag_service;

} sns_ai_oem1_sensor_state;

/*=============================================================================
  Public function declaration
  ============================================================================*/
  
/*!
  -----------------------------------------------------------------------------
  See sns_sensor::init
  -----------------------------------------------------------------------------
  */

sns_rc sns_ai_oem1_init(sns_sensor *const this);

/*!
  -----------------------------------------------------------------------------
  See sns_sensor::deinit
  -----------------------------------------------------------------------------
  */

sns_rc sns_ai_oem1_deinit(sns_sensor *const this);

/*!
  -----------------------------------------------------------------------------
  See sns_sensor::notify_event
  -----------------------------------------------------------------------------
  */

sns_rc sns_ai_oem1_notify_event(sns_sensor *const this);

/*!
  -----------------------------------------------------------------------------
  See sns_sensor::set_client_request
  -----------------------------------------------------------------------------
  */

sns_sensor_instance* sns_ai_oem1_set_client_request(
        sns_sensor *const this,
        struct sns_request const *exist_req,
        struct sns_request const *new_req,
        bool remove);

/* ---------------------------------------------------------------------------*/
