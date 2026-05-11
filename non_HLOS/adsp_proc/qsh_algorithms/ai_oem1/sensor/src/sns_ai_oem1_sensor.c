/*=============================================================================
  @file sns_ai_oem1_sensor.c

  The ai_oem1 virtual sensor implementation

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_ai_oem1.pb.h"
#include "sns_ai_oem1_algo_version.h"
#include "sns_ai_oem1_sensor.h"
#include "sns_ai_oem1_sensor_instance.h"
#include "sns_attribute_service.h"
#include "sns_attribute_util.h"
#include "sns_diag_service.h"
#include "sns_event_service.h"
#include "sns_mem_util.h"
#include "sns_memory_service.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_qshtech_island.h"
#include "sns_rc.h"
#include "sns_sensor.h"
#include "sns_sensor_instance.h"
#include "sns_sensor_util.h"
#include "sns_service.h"
#include "sns_service_manager.h"
#include "sns_stream_service.h"
#include "sns_suid.pb.h"
#include "sns_types.h"

/*=============================================================================
  Externs
  ============================================================================*/

extern const sns_sensor_uid sns_ai_oem1_suid;

/*=============================================================================
  Static Function Declaration
  ============================================================================*/

static void publish_attributes(sns_sensor * const this);
static void publish_available(sns_sensor * const this);

/*=============================================================================
  Public Function Definitions
  ============================================================================*/

/*------------------------------------------------------------------------------
 * See sns_sensor::init
 * ---------------------------------------------------------------------------*/

sns_rc sns_ai_oem1_init(sns_sensor *const this)
{

  sns_service_manager        *service_mgr    = NULL;
  sns_memory_service         *memory_service = NULL;
  sns_ai_oem1_sensor_state   *state          = (sns_ai_oem1_sensor_state*)this->state->state;

  //Lookup for Dependent Sensors
  SNS_SUID_LOOKUP_INIT(state->suid_lookup_data, NULL);
  sns_suid_lookup_add(this, &state->suid_lookup_data, "accel");
  sns_suid_lookup_add(this, &state->suid_lookup_data, "resampler");

  // Register callback function to vote for the island mode
  service_mgr = this->cb->get_service_manager(this);
  memory_service = (sns_memory_service*)service_mgr->get_service(service_mgr, SNS_MEMORY_SERVICE);
  if(NULL != memory_service)
  {
    memory_service->api->register_island_memory_vote_cb(this, &sns_qshtech_island_memory_vote);
    memory_service->api->get_memory_pool_name( this, state->mem_pool_info.mem_pool_name, 
                                                     sizeof( state->mem_pool_info.mem_pool_name ) );	
  }

  state->suid         = this->sensor_api->get_sensor_uid(this);
  state->diag_service = (sns_diag_service*)service_mgr->get_service(service_mgr, SNS_DIAG_SERVICE);  
  
  publish_attributes(this);
  
  SNS_PRINTF(MED, this, "sns_ai_oem1_init: done");
  return SNS_RC_SUCCESS;

}

/*-----------------------------------------------------------------------------
 * See sns_sensor::deinit
 * ---------------------------------------------------------------------------*/

/* See sns_sensor::deinit */
sns_rc sns_ai_oem1_deinit(sns_sensor *const this)
{

  sns_ai_oem1_sensor_state *state = (sns_ai_oem1_sensor_state*)this->state->state;
  sns_suid_lookup_deinit( this, &state->suid_lookup_data );

  UNUSED_VAR(this);
  SNS_PRINTF(MED, this, "sns_ai_oem1_deinit: done");
  return SNS_RC_SUCCESS;

}

/*------------------------------------------------------------------------------
 * See sns_sensor::set_client_request
 * ---------------------------------------------------------------------------*/

sns_sensor_instance* sns_ai_oem1_set_client_request(
            sns_sensor *const this,
            sns_request const *exist_req,
            sns_request const *new_req,
            bool remove)
{

  bool flush_req_handled = false;
  sns_sensor_instance *instance = sns_sensor_util_get_shared_instance(this);

  SNS_PRINTF(HIGH, this, "AI_OEM1 set_client_req: current(%x), new(%x), remove(%d)", exist_req, new_req, remove);

  if(remove)
  {
    if( NULL != instance )
    {
      instance->cb->remove_client_request(instance, exist_req);
    }
  }
  else if( NULL != new_req )
  {
    SNS_PRINTF( HIGH, this, "AI_OEM1 set_client_req: msg_id=(%d)", new_req->message_id );

    if( SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG == new_req->message_id )
    {

      // Create new instance if it's not already created.
      if( NULL == instance )
      {
        size_t algo_state_size = sns_ai_oem1_algo_get_mem_req(SNS_AI_OEM1_MEM_TYPE_ALGO_STATE_SIZE);

        SNS_PRINTF( HIGH, this, "ai_oem1_memory: algo_state_size(%d), ai_oem1_inst_state_size(%d)",
                   				algo_state_size, sizeof(sns_ai_oem1_instance_state));

        instance = this->cb->create_instance(this, sizeof(sns_ai_oem1_instance_state) + algo_state_size);
      }

      if( NULL != instance && NULL == exist_req )
      {
        instance->cb->add_client_request( instance, new_req );

        this->instance_api->set_client_config( instance, new_req );
      }
    }
    else if( SNS_STD_MSGID_SNS_STD_FLUSH_REQ == new_req->message_id &&
             NULL != instance &&
             NULL != exist_req )
    {
      sns_sensor_util_send_flush_event( this->sensor_api->get_sensor_uid(this),
                                        instance );
      flush_req_handled = true;
    }
    else
    {
      instance = NULL;
    }
  }
  else
  {
    instance =  NULL;
  }

  if( NULL != instance &&
      NULL == instance->cb->get_client_request( instance,
                                                this->sensor_api->get_sensor_uid(this),
                                                true) )
  {
    this->cb->remove_instance( instance );
  }

  if( flush_req_handled )
  {
    instance = &sns_instance_no_error;
  }
  return instance;

}

/*------------------------------------------------------------------------------
 * See sns_sensor::notify_event
 * ---------------------------------------------------------------------------*/

sns_rc sns_ai_oem1_notify_event(sns_sensor *const this)
{

  sns_ai_oem1_sensor_state *state = (sns_ai_oem1_sensor_state*)this->state->state;
  sns_suid_lookup_handle(this, &state->suid_lookup_data);

  if(sns_suid_lookup_complete(&state->suid_lookup_data))
  {
    SNS_PRINTF( HIGH, this, "sns_ai_oem1_notify_event: suid lookup complete" );
    publish_available(this);
    sns_suid_lookup_deinit(this, &state->suid_lookup_data);
  }
  return SNS_RC_SUCCESS;

}

/*=============================================================================
  Static Function Definitions
  ============================================================================*/

/*
 * Initialize attributes to their default state. They may/will be updated
 * within ai_oem1_notify.
 */
static void publish_attributes(sns_sensor *const this)
{
  {
    char const name[] = "ai_oem1";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg)
        { .buf = name, .buf_len = sizeof(name) });
    sns_publish_attribute(
        this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
  }
  {
    char const type[] = "ai_oem1";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg)
        { .buf = type, .buf_len = sizeof(type) });
    sns_publish_attribute(
        this, SNS_STD_SENSOR_ATTRID_TYPE, &value, 1, false);
  }
  {
    char const vendor[] = "QTI";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg)
        { .buf = vendor, .buf_len = sizeof(vendor) });
    sns_publish_attribute(
        this, SNS_STD_SENSOR_ATTRID_VENDOR, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    char const proto1[] = "sns_ai_oem1.proto";
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg)
        { .buf = proto1, .buf_len = sizeof(proto1) });
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_API, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = (int64_t)SNS_AI_OEM1_VERSION;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VERSION, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint     = SNS_STD_SENSOR_STREAM_TYPE_ON_CHANGE;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_STREAM_TYPE, &value, 1, false);
  }
  {
    sns_std_attr_value_data values[] = {SNS_ATTR};
    values[0].has_flt = true;
    values[0].flt = AI_OEM1_MINUMUM_SAMPLE_RATE;
    sns_publish_attribute( this, SNS_STD_SENSOR_ATTRID_RATES, values, ARR_SIZE(values), true );
  }
}

/* --------------------------------------------------------------------------*/

static void publish_available(sns_sensor *const this)
{

  sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
  value.has_boolean = true;
  value.boolean = true;
  sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_AVAILABLE, &value, 1, true);
  SNS_PRINTF( HIGH, this, "publish ai_oem1 sensor available" );

}

/*------------------------------------------------------------------------------
 * See sns_sensor::get_sensor_uid
 * ---------------------------------------------------------------------------*/

static sns_sensor_uid const* sns_ai_oem1_get_sensor_uid(sns_sensor const *this)
{
  UNUSED_VAR(this);
  return &sns_ai_oem1_suid;
}

/*===========================================================================
  Public Data Definitions
  ===========================================================================*/

sns_sensor_api sns_ai_oem1_sensor_api =
{
    .struct_len = sizeof(sns_sensor_api),
    .init = &sns_ai_oem1_init,
    .deinit = &sns_ai_oem1_deinit,
    .get_sensor_uid = &sns_ai_oem1_get_sensor_uid,
    .set_client_request = &sns_ai_oem1_set_client_request,
    .notify_event = &sns_ai_oem1_notify_event,
};

