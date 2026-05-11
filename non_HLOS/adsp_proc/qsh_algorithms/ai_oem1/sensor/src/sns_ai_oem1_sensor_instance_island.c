/*==============================================================================
  @file sns_ai_oem1_sensor_instance_island.c

  AI_OEM1 Sensor Instance API implementation

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_ai_oem1.pb.h"
#include "sns_ai_oem1_sensor_instance.h"
#include "sns_island_service.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_resampler.pb.h"
#include "sns_service.h"
#include "sns_service_manager.h"
#include "sns_std_sensor.pb.h"
#include "sns_stream_service.h"

/*==============================================================================
  Extern data
  ============================================================================*/

extern sns_sensor_uid sns_ai_oem1_suid; // This sensor's suid.

/*=============================================================================
  Static Data Definitions
  ============================================================================*/

/*=============================================================================
  Static Function Declaration
  ============================================================================*/

static sns_rc sns_ai_oem1_instance_notify_event( sns_sensor_instance *const this );

static sns_rc sns_ai_oem1_handle_resampler_event( sns_sensor_instance *const this );

static sns_rc sns_ai_oem1_process_input( sns_sensor_instance *const this );

static sns_rc sns_ai_oem1_handle_run( sns_sensor_instance *const this );

/*===========================================================================
  Public Data Definitions
  ===========================================================================*/

sns_sensor_instance_api sns_ai_oem1_instance_api =
{
  .struct_len        = sizeof(sns_sensor_instance_api),
  .init              = &sns_ai_oem1_instance_init,
  .deinit            = &sns_ai_oem1_instance_deinit,
  .set_client_config = &sns_ai_oem1_instance_set_client_config,
  .notify_event      = &sns_ai_oem1_instance_notify_event
};

/*=============================================================================
  Static Function Definitions
  ============================================================================*/

/* -----------------------------------------------------------------------------
 * See sns_sensor_instance_api::notify_event
 * ---------------------------------------------------------------------------*/
 
static sns_rc sns_ai_oem1_instance_notify_event( sns_sensor_instance *const this )
{

  sns_rc rc = SNS_RC_SUCCESS;

  rc = sns_ai_oem1_handle_resampler_event(this);
  
  return rc;
}
/* ---------------------------------------------------------------------------*/

static sns_rc sns_ai_oem1_handle_resampler_event( sns_sensor_instance *const this )
{

  sns_rc rc = SNS_RC_SUCCESS;

  sns_ai_oem1_instance_state *state = (sns_ai_oem1_instance_state*)this->state->state;

  sns_sensor_event *event_in = NULL;

  event_in = state->accel.resampler_stream->api->peek_input(state->accel.resampler_stream);

  while(NULL != event_in)
  {

    pb_istream_t stream;
    sns_std_sensor_event accel_event = sns_std_sensor_event_init_default;

    if( SNS_STD_SENSOR_MSGID_SNS_STD_SENSOR_EVENT == event_in->message_id )
    {
    
      stream = pb_istream_from_buffer((pb_byte_t*)event_in->event, event_in->event_len);

      uint8_t arr_index = 0;

      pb_float_arr_arg arg = {
        .arr = state->accel.data,
        .arr_len = ARR_SIZE(state->accel.data),
        .arr_index = &arr_index
      };

      accel_event.data = (struct pb_callback_s){
        .funcs.decode = &pb_decode_float_arr_cb,
        .arg = &arg
      };

      if( !pb_decode( &stream, sns_std_sensor_event_fields, &accel_event) )
      {
        SNS_INST_PRINTF( ERROR, this, "pb_decode() failed for resampled accel event" );
        rc = SNS_RC_FAILED;
      }
      else
      {
  
        state->accel.status    = accel_event.status;
        state->accel.timestamp = event_in->timestamp;
  
        if(state->accel.status != SNS_STD_SENSOR_SAMPLE_STATUS_UNRELIABLE )
        {
          // process accel input
          sns_ai_oem1_process_input( this );
        }
      }
    }

    event_in = state->accel.resampler_stream->api->get_next_input(state->accel.resampler_stream);

  }

  return rc;

}


/* ---------------------------------------------------------------------------*/

static sns_rc sns_ai_oem1_process_input( sns_sensor_instance *const this )
{

  sns_rc rc = SNS_RC_SUCCESS;
  sns_ai_oem1_instance_state *state = (sns_ai_oem1_instance_state*)this->state->state;

  // Copy accel sample to circular buffer
  SNS_CBUFFER_PUSH( state->algo_input.accel_buffer,
                    state->accel.data,
                    sizeof(float) * SNS_AI_OEM1_ACCEL_AXIS );

  state->accel.unprocessed_samples++;

  if( SNS_RC_SUCCESS == rc )
  {
    rc = sns_ai_oem1_handle_run( this );
  }
  
  return rc;
  
}

/* ---------------------------------------------------------------------------*/

static sns_rc sns_ai_oem1_handle_run( sns_sensor_instance *const this )
{

  sns_rc rc = SNS_RC_SUCCESS;
  sns_ai_oem1_device_state output;
  bool orientation_change_detected = false;
  int16_t result;
  
  sns_ai_oem1_instance_state *state = (sns_ai_oem1_instance_state*)this->state->state;
  
  if( state->accel.unprocessed_samples >= SNS_AI_OEM1_BATCH_SAMPLES )
  {
  	SNS_INST_PRINTF( LOW, this, "sns_ai_oem1_handle_run: begin");
  
  	state->accel.unprocessed_samples = 0;
  
  	result = sns_ai_oem1_algo_run( state->memory.algo_state,
  								   &state->algo_input,
  								   &output,
  								   &orientation_change_detected );
  
  	if( 0 == result )
  	{
  
  	  if( true == orientation_change_detected )
  	  {
  		state->is_orientation_change_detected = true;
  		state->event_ts 			          = sns_get_system_time();
  		state->inst_config.current_state      = output;
		
  		// Publish event if, new output state is not same as last published state.
  		if( (output != state->inst_config.previous_state) && (SNS_AI_OEM1_DEVICE_STATE_UNKNOWN != output) )
  		{
          SNS_INST_PRINTF( HIGH, this, "current_orientation : %d [0:Down|1:Up]",  output );
		  sns_ai_oem1_publish_event( this, state->event_ts );
  		  state->inst_config.previous_state = output;
  		}
  	  }
  	}
  	else
  	{
  	  SNS_INST_PRINTF( ERROR, this, "sns_ai_oem1_algo_run: failed. result(%d)", result );
  	}
  	SNS_INST_PRINTF( LOW, this, "sns_ai_oem1_handle_run: end");
  }
  
  return rc;

}

/* ---------------------------------------------------------------------------*/

sns_rc sns_ai_oem1_publish_event( sns_sensor_instance *const this, 
                                  sns_time event_ts )
{

  sns_rc rc = SNS_RC_SUCCESS;
  sns_ai_oem1_instance_state *state = (sns_ai_oem1_instance_state*)this->state->state;

  sns_ai_oem1_event ai_oem1_algo_data = sns_ai_oem1_event_init_default;
  sns_sensor_uid ai_oem1_suid = AI_OEM1_SUID;

  if(SNS_AI_OEM1_DEVICE_STATE_FACING_UP == state->inst_config.current_state)
  {
    ai_oem1_algo_data.orientation = SNS_AI_OEM1_EVENT_TYPE_ORIENTATION_UP;
  }
  else
  {
    ai_oem1_algo_data.orientation = SNS_AI_OEM1_EVENT_TYPE_ORIENTATION_DOWN;
  }
  
  if( rc == SNS_RC_SUCCESS )
  {
  
    pb_send_event(this, 
  				  sns_ai_oem1_event_fields,
  				  &ai_oem1_algo_data,
  				  event_ts,
  				  SNS_AI_OEM1_MSGID_SNS_AI_OEM1_EVENT,
  				  &ai_oem1_suid);
  
  }
  
  return rc;

}

/* ---------------------------------------------------------------------------*/

