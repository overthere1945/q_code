#pragma once
/*=============================================================================
  @file sns_ai_oem1_sensor_instance.h
  
  The ai_oem1 virtual Sensor Instance
  
  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/
  
/*=============================================================================
  Include Files
  ===========================================================================*/
  
#include "sns_ai_oem1.pb.h"
#include "sns_diag_service.h"
#include "sns_ai_oem1_algo_input.h"
#include "sns_ai_oem1_algo_interface.h"
#include "sns_std_sensor.pb.h"
#include "sns_memory_service.h"
#include "sns_sensor_instance.h"
#include "sns_ai_oem1_sensor.h"
#include "sns_sensor_uid.h"
#include "sns_types.h"

/*=============================================================================
  Static Data Definitions
  ============================================================================*/

#define SNS_AI_OEM1_ACCEL_AXIS            (3) // Number of axis in the accelerometer i/p

#define SNS_AI_OEM1_SAMPLE_RATE      (100.0f) // Dependent sensors sample rate (Hz)
#define SNS_AI_OEM1_BATCH_US        (1000000) // 1sec 

/*=============================================================================
  Type Definitions
  ============================================================================*/

typedef struct sns_accel_info
{

  sns_sensor_uid                  accel_suid;
  sns_sensor_uid              resampler_suid;
  sns_time                         timestamp;
  sns_data_stream          *resampler_stream;
  float         data[SNS_AI_OEM1_ACCEL_AXIS];
  int32_t                unprocessed_samples;
  sns_std_sensor_sample_status        status;

} sns_accel_info;

typedef struct sns_ai_oem1_inst_config
{

  sns_ai_oem1_device_state    previous_state;
  sns_ai_oem1_device_state    current_state;

} sns_ai_oem1_inst_config;

typedef struct sns_ai_oem1_memory_info
{

  void                *algo_state;
  void                 *model_buf;
  void       *model_buf_allocated;
  void         *model_scratch_buf;
  void      *model_persistent_buf;
  void           *model_input_buf;
  void          *model_output_buf;

} sns_ai_oem1_memory_info;

typedef struct sns_ai_oem1_instance_state
{

  sns_ai_oem1_memory_info           memory;
  sns_accel_info                    accel;
  sns_ai_oem1_algo_input            algo_input;
  sns_sensor_uid const             *suid;
  sns_diag_service                 *diag_mgr;
  sns_std_sensor_config             client_config;
  sns_ai_oem1_inst_config           inst_config;
  uint32_t                          eai_timedelta;
  sns_time                          config_event_ts;
  sns_time                          event_ts;
  bool                              is_ai_oem1_active;
  bool                              is_first_time;
  bool                              is_orientation_change_detected;
  sns_island_mem_util               mem_pool_info;
  
} sns_ai_oem1_instance_state;

/*=============================================================================
  Public function declaration
  ============================================================================*/

/*!
  -----------------------------------------------------------------------------
  See sns_sensor_instance_api::init
  -----------------------------------------------------------------------------
  */

sns_rc sns_ai_oem1_instance_init( sns_sensor_instance* const this,
                                  sns_sensor_state const *sensor_state );

/*!
  -----------------------------------------------------------------------------
  See sns_sensor_instance_api::deinit
  -----------------------------------------------------------------------------
  */

sns_rc sns_ai_oem1_instance_deinit( sns_sensor_instance* const this );

/*!
  -----------------------------------------------------------------------------
  See sns_sensor_instance_api::set_client_config
  -----------------------------------------------------------------------------
  */

sns_rc sns_ai_oem1_instance_set_client_config( sns_sensor_instance* const this,
                                               sns_request const* client_request );

/*!
  -----------------------------------------------------------------------------
  sns_ai_oem1_start_resampled_accel()
  -----------------------------------------------------------------------------
  @brief  The API to start accel sensor

  @param [i] this    : sensor instance pointer

  @return
  SNS_RC_SUCCESS     : Operation successful
  SNS_RC_FAILED      : otherwise
  -----------------------------------------------------------------------------
  */

sns_rc sns_ai_oem1_start_resampled_accel( sns_sensor_instance *const this );

/*!
  -----------------------------------------------------------------------------
  sns_ai_oem1_publish_event()
  -----------------------------------------------------------------------------
  @brief  The API to publish AI_OEM1 event. This API will publish AI_OEM1 output state
          event to the client

  @param [i] this        : sensor instance pointer
  @param [i] event_ts    : event timestamp in ticks

  @return
  -----------------------------------------------------------------------------
  */
sns_rc sns_ai_oem1_publish_event( sns_sensor_instance *const this, sns_time event_ts );

/* ---------------------------------------------------------------------------*/

