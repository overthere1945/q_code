/*==============================================================================
  @file sns_ai_oem1_sensor_instance.c

  AI_OEM1 Sensor Instance API implementation

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include "pb_encode.h"
#include "pb_decode.h"
#include "sns_ai_oem1.pb.h"
#include "sns_ai_oem1_algo_interface.h"
#include "sns_ai_oem1_sensor.h"
#include "sns_ai_oem1_sensor_instance.h"
#include "sns_attribute_service.h"
#include "sns_attribute_util.h"
#include "sns_diag_service.h"
#include "sns_event_service.h"
#include "sns_math_util.h"
#include "sns_mem_util.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_rc.h"
#include "sns_resampler.pb.h"
#include "sns_sensor.h"
#include "sns_sensor_instance.h"
#include "sns_sensor_util.h"
#include "sns_service.h"
#include "sns_service_manager.h"
#include "sns_std_sensor.pb.h"
#include "sns_stream_service.h"
#include "sns_time.h"
#include "sns_types.h"
#include "sns_island_mem.h"

/*==============================================================================
  Extern data
  ============================================================================*/

extern sns_sensor_uid sns_ai_oem1_suid;

/*=============================================================================
  Static data
  ============================================================================*/

/*=============================================================================
  Macro Definitions
  ============================================================================*/

#define ALIGN_256(size) ((((size_t)(size))+0xFF)&0xFFFFFF00)

/*=============================================================================
  Static Function Declaration
  ============================================================================*/

static void get_suids( sns_sensor_instance *const this,
                       sns_sensor_state const *state );

static void instance_cleanup( sns_sensor_instance *const this,
                              sns_ai_oem1_instance_state *state );

static sns_rc sns_ai_oem1_start_dep_sensors( sns_sensor_instance *const this );

static void sns_ai_oem1_input_init( sns_sensor_instance *const this );

static sns_rc sns_ai_oem1_ai_handle_init( sns_sensor_instance *const this );

static sns_rc sns_ai_oem1_ai_handle_deinit( sns_sensor_instance *const this );

/*=============================================================================
  Public Function Definitions
  ============================================================================*/

/*------------------------------------------------------------------------------
 * See sns_sensor_instance_api::init
 * ---------------------------------------------------------------------------*/

sns_rc sns_ai_oem1_instance_init( sns_sensor_instance *this, sns_sensor_state const *state )
{

  sns_rc rc = SNS_RC_SUCCESS;
  sns_ai_oem1_sensor_state     *sensor_state = (sns_ai_oem1_sensor_state*)state->state;
  sns_ai_oem1_instance_state *instance_state = (sns_ai_oem1_instance_state*)this->state->state;

  SNS_INST_PRINTF( HIGH, this, "init:" );
  
  //points to the start address of algo_state buffer
  instance_state->memory.algo_state = ((char*)instance_state) +
                                       sizeof(sns_ai_oem1_instance_state);
  //Lookup for dependent sensor
  get_suids( this, state );

  //Initialize the "accel" input buffer
  sns_ai_oem1_input_init( this );

  instance_state->suid          = sensor_state->suid;
  instance_state->mem_pool_info = sensor_state->mem_pool_info;

  //Initialize the device states in instance config
  instance_state->inst_config.previous_state = SNS_AI_OEM1_DEVICE_STATE_UNKNOWN;
  instance_state->inst_config.current_state = SNS_AI_OEM1_DEVICE_STATE_UNKNOWN;

  instance_state->is_orientation_change_detected = false;
  instance_state->is_ai_oem1_active = false;
  instance_state->is_first_time = true;
  
  instance_state->accel.unprocessed_samples = SNS_AI_OEM1_RESET_BUF_CNT;

  return rc;

}

/*------------------------------------------------------------------------------
 * See sns_sensor_instance_api::deinit
 * ---------------------------------------------------------------------------*/

sns_rc sns_ai_oem1_instance_deinit( sns_sensor_instance *const this )
{
  sns_ai_oem1_instance_state *intance_state = (sns_ai_oem1_instance_state*)this->state->state;

  SNS_INST_PRINTF( HIGH, this, "deinit:" );

  sns_ai_oem1_ai_handle_deinit( this );

  instance_cleanup( this, intance_state );

  return SNS_RC_SUCCESS;
}

/*------------------------------------------------------------------------------
 * See sns_sensor_instance_api::set_client_config
 * ---------------------------------------------------------------------------*/

sns_rc sns_ai_oem1_instance_set_client_config( sns_sensor_instance *const this, sns_request const *client_request )
{

  sns_rc rc = SNS_RC_SUCCESS;
  sns_ai_oem1_instance_state *state = (sns_ai_oem1_instance_state*)this->state->state;

  SNS_INST_PRINTF( HIGH, this, "set_client_config: msg_id(%d)", client_request->message_id );

  if( SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG == client_request->message_id )
  {
    if( false == state->is_ai_oem1_active )
    {
      state->is_ai_oem1_active = true;
      state->config_event_ts   = sns_get_system_time();

      if( SNS_RC_SUCCESS != sns_ai_oem1_ai_handle_init( this ))
      {
        rc = SNS_RC_FAILED;
      }
      else
      {
        rc = sns_ai_oem1_start_dep_sensors( this );
      }
    }
  }
  return rc;

}

/* ---------------------------------------------------------------------------*/

sns_rc sns_ai_oem1_start_resampled_accel( sns_sensor_instance *const this )
{

  sns_rc rc = SNS_RC_INVALID_VALUE;
  
  sns_ai_oem1_instance_state *state = (sns_ai_oem1_instance_state*)this->state->state;;
  sns_service_manager  *service_mgr = this->cb->get_service_manager( this );
  sns_stream_service    *stream_mgr = (sns_stream_service*)service_mgr->get_service( service_mgr, SNS_STREAM_SERVICE );

  uint8_t buffer[100];
  sns_request req;
  sns_std_request std_req = sns_std_request_init_default;
  sns_resampler_config resampler_cfg = sns_resampler_config_init_default;

  //Create a resampler stream if not already present
  if( NULL == state->accel.resampler_stream )
  {
    rc = stream_mgr->api->create_sensor_instance_stream(
        stream_mgr, this, state->accel.resampler_suid, &state->accel.resampler_stream );
  }

  if( NULL != state->accel.resampler_stream )
  {
    req = (sns_request) {
      .message_id = SNS_RESAMPLER_MSGID_SNS_RESAMPLER_CONFIG,
      .request    = buffer,
    };

    std_req = (sns_std_request) {
      .has_batching = true,
      .batching.batch_period = SNS_AI_OEM1_BATCH_US,
    };

    sns_memscpy( &resampler_cfg.sensor_uid,
                 sizeof(resampler_cfg.sensor_uid),
                 &state->accel.accel_suid,
                 sizeof(state->accel.accel_suid) );

    resampler_cfg.resampled_rate = SNS_AI_OEM1_SAMPLE_RATE;
    resampler_cfg.rate_type      = SNS_RESAMPLER_RATE_FIXED;
    resampler_cfg.filter         = true;

    SNS_INST_PRINTF( HIGH, this, "AI_OEM1 sns_ai_oem1_start_resampled_accel: msg_id:%d sample_rate:%d/100 Hz, batch_period:%d us",
                                 SNS_RESAMPLER_MSGID_SNS_RESAMPLER_CONFIG,
                                 (int32_t)(SNS_AI_OEM1_SAMPLE_RATE*100),
                                 (int32_t)SNS_AI_OEM1_BATCH_US );

    req.request_len = pb_encode_request( buffer,
                                         sizeof(buffer),
                                         &resampler_cfg,
                                         sns_resampler_config_fields,
                                         &std_req );
    if( req.request_len > 0 )
    {
      rc = state->accel.resampler_stream->api->send_request( state->accel.resampler_stream,
                                                             &req );
    }
  }
  return rc;

}

/*=============================================================================
  Static Function Definitions
  ============================================================================*/

static void get_suids( sns_sensor_instance *const this, sns_sensor_state const *state )
{

  void *suid_lookup_data;
  sns_ai_oem1_sensor_state *sensor_state;
  sns_ai_oem1_instance_state *instance_state;

  sensor_state   = (sns_ai_oem1_sensor_state*)state->state;
  instance_state = (sns_ai_oem1_instance_state*)this->state->state;

  suid_lookup_data =  &sensor_state->suid_lookup_data;
  sns_suid_lookup_get( suid_lookup_data, "accel",     &instance_state->accel.accel_suid );
  sns_suid_lookup_get( suid_lookup_data, "resampler", &instance_state->accel.resampler_suid );

}

/* ---------------------------------------------------------------------------*/

static void instance_cleanup( sns_sensor_instance *const this, sns_ai_oem1_instance_state *state )
{

  SNS_INST_PRINTF( HIGH, this, "instance cleanup:" );
  sns_sensor_util_remove_sensor_instance_stream( this, &state->accel.resampler_stream );

}

/* ---------------------------------------------------------------------------*/

static sns_rc sns_ai_oem1_start_dep_sensors( sns_sensor_instance *const this )
{

  sns_rc rc = SNS_RC_SUCCESS;
  rc = sns_ai_oem1_start_resampled_accel( this );

  if( SNS_RC_SUCCESS != rc)
  {
    SNS_INST_PRINTF( ERROR, this, "ai_oem1_start_dep_sensors failed: rc=%d", rc);
    rc = SNS_RC_INVALID_STATE;
  }
  return rc;
  
}
  
/* ---------------------------------------------------------------------------*/

static void sns_ai_oem1_input_init( sns_sensor_instance *const this )
{

  sns_ai_oem1_instance_state *state;
  state  = (sns_ai_oem1_instance_state*)this->state->state;
  
  SNS_CBUFFER_INIT( state->algo_input.accel_buffer );

}

/* ---------------------------------------------------------------------------*/

static sns_rc sns_ai_oem1_ai_handle_init( sns_sensor_instance *const this )
{

  sns_rc rc = SNS_RC_SUCCESS;

  size_t alignment = 0;
  size_t model_size = 0;
  size_t buffer_size = 0;

  SNS_INST_PRINTF( LOW, this, "sns_ai_oem1_ai_handle_init() start");

  sns_ai_oem1_instance_state *state = (sns_ai_oem1_instance_state*)this->state->state;

  // Allocate EAI Model memory
  alignment  = sns_ai_oem1_algo_get_mem_req( SNS_AI_OEM1_MEM_TYPE_MODEL_ALIGNMENT );
  model_size = sns_ai_oem1_algo_get_mem_req( SNS_AI_OEM1_MEM_TYPE_MODEL_SIZE );

  SNS_INST_PRINTF( HIGH, this, "ai_oem1_algo_init: model_size=%d", model_size );
  SNS_INST_PRINTF( HIGH, this, "ai_oem1_algo_init: model_alignment=%d", alignment );
  
  if( 0 < alignment && 0 < model_size )
  {
    state->memory.model_buf_allocated = sns_island_pool_alloc( &state->mem_pool_info, model_size + alignment - 1 );
  }

  if( NULL == state->memory.model_buf_allocated )
  {
    SNS_INST_PRINTF( ERROR, this, "sns_ai_oem1_ai_handle_init(): model_buf_allocated is NULL" );
    rc = SNS_RC_FAILED;
  }
  else
  {
    state->memory.model_buf = (void *)ALIGN_256(state->memory.model_buf_allocated);
    // Initialize EAI model
    if( false == sns_ai_oem1_algo_model_init( state->memory.algo_state,
                                              state->memory.model_buf,
                                              SNS_AI_OEM1_MEM_LOCATION_LLC ) )
    {
      SNS_INST_PRINTF( ERROR, this, "sns_ai_oem1_algo_model_init() failed");
      rc = SNS_RC_FAILED;
    }
    else
    {
      // Allocate EAI buffer memory
      buffer_size = sns_ai_oem1_algo_get_model_mem_req( state->memory.algo_state,
                                         				SNS_AI_OEM1_MODEL_MEM_TYPE_SCRATCH_SIZE );

      SNS_INST_PRINTF( HIGH, this, "sns_ai_oem1_ai_handle_init: scratch_buf_size=%d", buffer_size );
      
	  // Allocate Scratch Buffer
      state->memory.model_scratch_buf = sns_island_pool_alloc( &state->mem_pool_info, buffer_size );

      buffer_size = sns_ai_oem1_algo_get_model_mem_req( state->memory.algo_state,
                                        				SNS_AI_OEM1_MODEL_MEM_TYPE_PERSISTENT_SIZE );

      SNS_INST_PRINTF( HIGH, this, "sns_ai_oem1_ai_handle_init: persistent_buf_size=%d", buffer_size );
      
	  // Allocate Persistent Buffer
      state->memory.model_persistent_buf = sns_island_pool_alloc( &state->mem_pool_info, buffer_size );

      if( NULL == state->memory.model_scratch_buf ||
          NULL == state->memory.model_persistent_buf )
      {
        SNS_INST_PRINTF( ERROR, this, "scratch(%x) or persist(%x) is null");
        rc = SNS_RC_FAILED;
      }
      else
      {
        // Initialize EAI buffers
        if( false == sns_ai_oem1_algo_state_init( state->memory.algo_state,
                                                  state->memory.model_scratch_buf,
                                                  state->memory.model_persistent_buf ))
        {
          SNS_INST_PRINTF( ERROR, this, "sns_ai_oem1_algo_state_init() failed");
          rc = SNS_RC_FAILED;
        }
      }
    }
  }
  return rc;
  
}

/* ---------------------------------------------------------------------------*/

static sns_rc sns_ai_oem1_ai_handle_deinit( sns_sensor_instance *const this )
{
  sns_rc rc = SNS_RC_SUCCESS;
  UNUSED_VAR(this);
  sns_ai_oem1_instance_state *state = (sns_ai_oem1_instance_state*)this->state->state;

  // deinitialize handle/algorithm
  if( false == sns_ai_oem1_algo_deinit( state->memory.algo_state ))
  {
    SNS_INST_PRINTF( ERROR, this, "sns_ai_oem1_ai_handle_deinit() failed");
    rc = SNS_RC_FAILED;
  }
  
  // Free Model, Scratch & Persistent Buffers
  sns_island_pool_free( &state->mem_pool_info, &state->memory.model_buf_allocated );
  sns_island_pool_free( &state->mem_pool_info, &state->memory.model_scratch_buf );
  sns_island_pool_free( &state->mem_pool_info, &state->memory.model_persistent_buf );

  return rc;
}
/* ---------------------------------------------------------------------------*/

