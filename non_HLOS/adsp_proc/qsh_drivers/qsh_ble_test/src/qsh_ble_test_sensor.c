/*=============================================================================
  @file qsh_ble_test_sensor.c

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "sns_attribute_util.h"
#include "sns_printf.h"
#include "sns_rc.h"
#include "sns_sensor.h"
#include "sns_sensor_uid.h"
#include "sns_sensor_util.h"
#include "sns_types.h"

#include "sns_pb_util.h"
#include "sns_std_sensor.pb.h"
#include "sns_std_type.pb.h"

#include "qsh_ble_test_sensor.h"
#include "qsh_ble_test_sensor_instance.h"

static void
qsh_ble_test_publish_attributes(sns_sensor *const this)
{
  {
    char const name[] = "ble_test";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg)
        { .buf = name, .buf_len = sizeof(name) });
    sns_publish_attribute(
        this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
  }
  {
    char const type[] = "ble_test";
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
    char const proto_files[] = "qsh_ble_test.proto";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg)
        { .buf = proto_files, .buf_len = sizeof(proto_files) });
    sns_publish_attribute(
        this, SNS_STD_SENSOR_ATTRID_API, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = SNS_STD_SENSOR_STREAM_TYPE_ON_CHANGE;
    sns_publish_attribute(
        this, SNS_STD_SENSOR_ATTRID_STREAM_TYPE, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = QSH_BLE_TEST_SENSOR_VERSION;
    sns_publish_attribute(
        this, SNS_STD_SENSOR_ATTRID_VERSION, &value, 1, true);
  }
}

static void
qsh_ble_test_publish_available(sns_sensor *const this)
{
  sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
  value.has_boolean = true;
  value.boolean = true;
  sns_publish_attribute(
      this, SNS_STD_SENSOR_ATTRID_AVAILABLE, &value, 1, true);
}

sns_rc
qsh_ble_test_init(sns_sensor *const this)
{
  sns_rc rc = SNS_RC_SUCCESS;
  qsh_ble_test_sensor_state *state = (qsh_ble_test_sensor_state *)this->state->state;
  memset(state, 0, sizeof(qsh_ble_test_sensor_state));

  state->suid = this->sensor_api->get_sensor_uid(this);

  SNS_SUID_LOOKUP_INIT(state->suid_lookup_data, NULL);
  sns_suid_lookup_add(this, &state->suid_lookup_data, "ble");

  qsh_ble_test_publish_attributes(this);

  SNS_PRINTF(HIGH, this, "qsh_ble_test_init, done");
  return rc;
}

sns_rc
qsh_ble_test_deinit(sns_sensor *const this)
{
  UNUSED_VAR(this);

  SNS_PRINTF(HIGH, this, "qsh_ble_test_deinit, done");

  return SNS_RC_SUCCESS;
}

sns_rc
qsh_ble_test_notify_event(sns_sensor *const this)
{
  sns_rc rc = SNS_RC_SUCCESS;
  bool completed;
  SNS_PRINTF(HIGH, this, "qsh_ble_test_notify_event");

  qsh_ble_test_sensor_state *state = (qsh_ble_test_sensor_state*)this->state->state;

  completed = sns_suid_lookup_complete(&state->suid_lookup_data);

  sns_suid_lookup_handle(this, &state->suid_lookup_data);

  if (completed != sns_suid_lookup_complete(&state->suid_lookup_data))
  {
    SNS_PRINTF(HIGH, this, "qsh_ble_test_notify_event, suid lookup complete");
    qsh_ble_test_publish_available(this);
    sns_suid_lookup_deinit(this, &state->suid_lookup_data);
  }

  return rc;
}

