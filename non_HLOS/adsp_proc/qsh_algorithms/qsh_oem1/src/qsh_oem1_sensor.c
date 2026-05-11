/*=============================================================================
  @file qsh_oem1_sensor.c

  The qsh_oem1 virtual Sensor implementation

  Copyright (c) 2021-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_oem1_sensor.h"
#include "qsh_oem1_sensor_instance.h"
#include "qsh_oem1.pb.h"
#include "sns_attribute_util.h"
#include "sns_memory_service.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_qshtech_island.h"
#include "sns_service.h"
#include "sns_suid.pb.h"
#include "sns_types.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/
/**
 * Initialize attributes to their default state.  They may/will be updated
 * within qsh_oem1_notify.
 */
static void publish_attributes(sns_sensor *const this)
{
  {
    char const name[] = "qsh_oem1";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = name, .buf_len = sizeof(name)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
  }
  {
    char const type[] = "qsh_oem1";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = type, .buf_len = sizeof(type)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_TYPE, &value, 1, false);
  }
  {
    char const vendor[] = "qualcomm";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = vendor, .buf_len = sizeof(vendor)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VENDOR, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    char const proto1[] = "qsh_oem1.proto";
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = proto1, .buf_len = sizeof(proto1)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_API, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = 1;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VERSION, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = SNS_STD_SENSOR_STREAM_TYPE_ON_CHANGE;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_STREAM_TYPE, &value, 1, true);
  }
}

static void publish_available(sns_sensor *const this)
{
  sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
  value.has_boolean = true;
  value.boolean = true;
  sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_AVAILABLE, &value, 1, true);
}

/* See sns_sensor::init */
sns_rc qsh_oem1_init(sns_sensor *const this)
{
  qsh_oem1_sensor_state *state = (qsh_oem1_sensor_state *)this->state->state;
  state->suid = this->sensor_api->get_sensor_uid(this);

  SNS_SUID_LOOKUP_INIT(state->suid_lookup_data, NULL);
  sns_suid_lookup_add(this, &state->suid_lookup_data, "wifi_svc");
  sns_suid_lookup_add(this, &state->suid_lookup_data, "location");
  sns_suid_lookup_add(this, &state->suid_lookup_data, "amd");

  // Register callback function to vote for the island mode
#ifdef QSH_ISLAND_INCLUDE_OEM1
  sns_service_manager *service_mgr = NULL;
  sns_memory_service *memory_service = NULL;
  service_mgr = this->cb->get_service_manager(this);
  memory_service = (sns_memory_service *)service_mgr->get_service(service_mgr, SNS_MEMORY_SERVICE);
  if(NULL != memory_service)
  {
    memory_service->api->register_island_memory_vote_cb(this, &sns_qshtech_island_memory_vote);
  }
#endif // QSH_ISLAND_INCLUDE_OEM1

  publish_attributes(this);
  SNS_PRINTF(MED, this, "qsh_oem1 init success and attributes published");
  return SNS_RC_SUCCESS;
}

/* See sns_sensor::deinit */
sns_rc qsh_oem1_deinit(sns_sensor *const this)
{
  UNUSED_VAR(this);
  SNS_PRINTF(MED, this, "qsh_oem1_deinit");
  return SNS_RC_SUCCESS;
}

/* See sns_sensor::notify_event */
sns_rc qsh_oem1_notify_event(sns_sensor *const this)
{
  qsh_oem1_sensor_state *state = (qsh_oem1_sensor_state *)this->state->state;
  SNS_PRINTF(LOW, this, "qsh_oem1_notify_event");
  sns_suid_lookup_handle(this, &state->suid_lookup_data);

  if(sns_suid_lookup_complete(&state->suid_lookup_data))
  {
    publish_available(this);
    sns_suid_lookup_deinit(this, &state->suid_lookup_data);
  }
  return SNS_RC_SUCCESS;
}

/* See sns_sensor::set_client_request */
sns_sensor_instance *qsh_oem1_set_client_request(sns_sensor *const this,
                                                 sns_request const *curr_req,
                                                 sns_request const *new_req, bool remove)
{
  bool flush_req_handled = false;

  // get shared instance
  sns_sensor_instance *ret_inst = sns_sensor_util_get_shared_instance(this);

  SNS_PRINTF(HIGH, this, "qsh_oem1_set_client_request - msg_id=%d/%d remove=%u",
             curr_req ? curr_req->message_id : -1, new_req ? new_req->message_id : -1, remove);

  // Handle remove request
  if(remove)
  {
    if(NULL != ret_inst)
    {
      ret_inst->cb->remove_client_request(ret_inst, curr_req);
    }
  }
  else if(NULL != new_req)
  {
    if(new_req->message_id == SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG)
    {
      // Create instance if there isn't any
      if(NULL == ret_inst)
      {
        ret_inst = this->cb->create_instance(this, (sizeof(qsh_oem1_inst_state)));
      }
      if(NULL != ret_inst)
      {
        this->instance_api->set_client_config(ret_inst, new_req);
      }
      
      // Add the new request
      // Ignore any subsequent ON_CHANGE req from existing client
      if(NULL != ret_inst && NULL == curr_req)
      {
        ret_inst->cb->add_client_request(ret_inst, new_req);
      }
    }
    // Handle flush request
    else if(new_req->message_id == SNS_STD_MSGID_SNS_STD_FLUSH_REQ && NULL != ret_inst &&
            NULL != curr_req)
    {
      this->instance_api->set_client_config(ret_inst, new_req);
      flush_req_handled = true;
    }
    else
    {
      ret_inst = NULL;
    }
  }
  else
  {
    ret_inst = NULL;
  }

  // Remove the instance if there is no active request
  if(NULL != ret_inst && NULL == ret_inst->cb->get_client_request(
                                     ret_inst, this->sensor_api->get_sensor_uid(this), true))
  {
    this->cb->remove_instance(ret_inst);
  }

  if(flush_req_handled)
  {
    ret_inst = &sns_instance_no_error;
  }
  return ret_inst;
}
