/**============================================================================
  @file sns_ccd_ml_decision_tree_demo_sensor.c

  @brief Example dummy AR algo integrated with ccd_mldt

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_attribute_util.h"
#include "sns_ccd_ml_decision_tree_demo_algo_interface.h"
#include "sns_ccd_ml_decision_tree_demo_sensor.h"
#include "sns_ccd_ml_decision_tree_demo_sensor_instance.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_rc.h"
#include "sns_sensor_util.h"

const sns_sensor_uid sns_ccd_ml_decision_tree_demo_suid =
    CCD_ML_DECISION_TREE_DEMO_SUID;

/*=============================================================================
  Static Function Definitions
  ===========================================================================*/

/* Publish all saved attributes except available attribute */
static void publish_attributes(sns_sensor *const this)
{
  {
    char const name[] = "ccd_ml_decision_tree_demo";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = name, .buf_len = sizeof(name)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
  }
  {
    char const vendor[] = "QTI";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg =
        &((pb_buffer_arg){.buf = vendor, .buf_len = sizeof(vendor)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VENDOR, &value, 1, false);
  }
  {
    char const type[] = "ml_demo";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = type, .buf_len = sizeof(type)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_TYPE, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = 1;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VERSION, &value, 1,
                          false);
  }
  {
    char const proto_files[] = "sns_ml_demo.proto";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg =
        &((pb_buffer_arg){.buf = proto_files, .buf_len = sizeof(proto_files)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_API, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = SNS_STD_SENSOR_STREAM_TYPE_ON_CHANGE;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_STREAM_TYPE, &value, 1,
                          true);
  }
}

static void publish_available(sns_sensor *const this)
{
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_boolean = true;
    value.boolean = true;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_AVAILABLE, &value, 1,
                          true);
  }
}

static sns_sensor_uid const *
sns_ccd_ml_decision_tree_demo_get_sensor_uid(sns_sensor const *this)
{
  UNUSED_VAR(this);
  return &sns_ccd_ml_decision_tree_demo_suid;
}

static sns_rc sns_ccd_ml_decision_tree_demo_init(sns_sensor *const this)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_ccd_ml_decision_tree_demo_sensor_state *state =
      (sns_ccd_ml_decision_tree_demo_sensor_state *)this->state->state;
  publish_attributes(this);
  SNS_SUID_LOOKUP_INIT(state->lookup_data, NULL);
  sns_suid_lookup_add(this, &state->lookup_data, "accel");
  sns_suid_lookup_add(this, &state->lookup_data, "gyro");
  sns_suid_lookup_add(this, &state->lookup_data, "ccd_mldt");
  return rc;
}

static sns_rc sns_ccd_ml_decision_tree_demo_deinit(sns_sensor *const this)
{
  sns_ccd_ml_decision_tree_demo_sensor_state *state =
      (sns_ccd_ml_decision_tree_demo_sensor_state *)this->state->state;
  sns_suid_lookup_deinit(this, &state->lookup_data);
  return SNS_RC_SUCCESS;
}

static sns_sensor_instance *sns_ccd_ml_decision_tree_demo_set_client_request(
    sns_sensor *const this, sns_request const *curr_req,
    sns_request const *new_req, bool remove)
{
  sns_sensor_instance *curr_inst = this->cb->get_sensor_instance(this, true);
  sns_rc rc = SNS_RC_SUCCESS;

  if(remove)
  {
    if(curr_inst != NULL)
    {
      curr_inst->cb->remove_client_request(curr_inst, curr_req);
    }
  }
  else if(NULL != new_req)
  {
    if(new_req->message_id == SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG)
    {
      if(curr_inst == NULL)
      {
        size_t state_size =
          sizeof(sns_ccd_ml_decision_tree_demo_sensor_inst_state) +
          sns_ccd_ml_decision_tree_demo_algo_get_state_size();

        curr_inst = this->cb->create_instance(
            this, state_size);
      }
      if(curr_inst != NULL)
      {
        curr_inst->cb->add_client_request(curr_inst, new_req);
        rc = this->instance_api->set_client_config(curr_inst, new_req);
        if(rc != SNS_RC_SUCCESS)
        {
          curr_inst->cb->remove_client_request(curr_inst, new_req);
          if(curr_req != NULL)
          {
            curr_inst->cb->add_client_request(curr_inst, curr_req);
          }
        }
      }
    }
    else if(new_req->message_id == SNS_STD_MSGID_SNS_STD_FLUSH_REQ)
    {
      if(curr_req == NULL)
      {
        curr_inst = NULL;
      }
      else if(curr_inst != NULL)
      {
        sns_sensor_util_send_flush_event(this->sensor_api->get_sensor_uid(this),
                                         curr_inst);
      }
    }
  }

  if(curr_inst &&
     NULL == curr_inst->cb->get_client_request(
                 curr_inst, this->sensor_api->get_sensor_uid(this), true))
  {
    this->cb->remove_instance(curr_inst);
  }

  return (rc == SNS_RC_SUCCESS) ? curr_inst : NULL;
}

static sns_rc sns_ccd_ml_decision_tree_demo_notify_event(sns_sensor *const this)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_ccd_ml_decision_tree_demo_sensor_state *state =
      (sns_ccd_ml_decision_tree_demo_sensor_state *)this->state->state;

  bool completed = sns_suid_lookup_complete(&state->lookup_data);
  rc = sns_suid_lookup_handle(this, &state->lookup_data);
  if(rc == SNS_RC_NOT_AVAILABLE)
  {
    rc = SNS_RC_INVALID_LIBRARY_STATE;
  }
  else if(rc == SNS_RC_SUCCESS)
  {
    if(completed != sns_suid_lookup_complete(&state->lookup_data))
    {
      publish_available(this);
      sns_suid_lookup_deinit(this, &state->lookup_data);
    }
  }
  return SNS_RC_SUCCESS;
}

const sns_sensor_api sns_ccd_ml_decision_tree_demo_sensor_api = {
    .struct_len = sizeof(sns_sensor_api),
    .init = &sns_ccd_ml_decision_tree_demo_init,
    .deinit = &sns_ccd_ml_decision_tree_demo_deinit,
    .get_sensor_uid = &sns_ccd_ml_decision_tree_demo_get_sensor_uid,
    .set_client_request = &sns_ccd_ml_decision_tree_demo_set_client_request,
    .notify_event = &sns_ccd_ml_decision_tree_demo_notify_event,
};
