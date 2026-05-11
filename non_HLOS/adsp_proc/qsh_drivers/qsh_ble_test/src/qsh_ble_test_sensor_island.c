/**============================================================================
  @file qsh_ble_test_sensor_island.c

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
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
#include "sns_request.h"
#include "sns_sensor.h"
#include "sns_sensor_uid.h"
#include "sns_sensor_util.h"
#include "sns_time.h"
#include "sns_types.h"

#include "sns_suid.pb.h"

#include "qsh_ble_test.pb.h"
#include "qsh_ble_test_sensor.h"
#include "qsh_ble_test_sensor_instance.h"

const sns_sensor_uid qsh_ble_test_suid = QSH_BLE_TEST_SUID;

sns_rc qsh_ble_test_init(sns_sensor *const this);
sns_rc qsh_ble_test_deinit(sns_sensor *const this);
sns_rc qsh_ble_test_notify_event(sns_sensor *const this);

static sns_sensor_uid const*
qsh_ble_test_get_suid(sns_sensor const *this)
{
  UNUSED_VAR(this);

  return &qsh_ble_test_suid;
}

sns_sensor_instance*
qsh_ble_test_set_client_request(
  sns_sensor *const this,
  sns_request const *curr_req,
  sns_request const *new_req,
  bool remove)
{
  sns_time time_now = sns_get_system_time();
  uint64_t resu = sns_get_time_tick_resolution();
  SNS_UPRINTF(HIGH, this, "qsh_ble_test_set_client_request, now=0x%x%08x, resu=0x%x",
      (uint32_t)(time_now >> 32), (uint32_t)time_now, (uint32_t)resu);

  sns_sensor_instance *curr_inst = sns_sensor_util_find_instance(
    this, curr_req, this->sensor_api->get_sensor_uid(this));

  sns_sensor_instance *rv_inst = NULL;

  if (remove)
  {
    SNS_UPRINTF(HIGH, this, "qsh_ble_test_set_client_request, remove");
    if (curr_inst != NULL)
    {
      curr_inst->cb->remove_client_request(curr_inst, curr_req);
    }
  }
  else if (new_req != NULL &&
      new_req->message_id == QSH_BLE_TEST_MSGID_QSH_BLE_TEST_CONFIG)
  {
    if (curr_inst != NULL)
    {
      SNS_UPRINTF(ERROR, this, "qsh_ble_test_set_client_request, curr_inst already exists");
      return NULL;
    }

    rv_inst = this->cb->create_instance(this, sizeof(qsh_ble_test_inst_state));

    if (rv_inst != NULL)
    {
      rv_inst->cb->add_client_request(rv_inst, new_req);
      this->instance_api->set_client_config(rv_inst, new_req);
    }
  }
  else if (new_req != NULL &&
      new_req->message_id == QSH_BLE_TEST_MSGID_QSH_BLE_TEST_GET_RESULT)
  {
    if (curr_inst != NULL)
    {
      SNS_UPRINTF(HIGH, this, "qsh_ble_test_set_client_request, GET_RESULT");
      this->instance_api->set_client_config(curr_inst, new_req);
      rv_inst = &sns_instance_no_error;
    }
    else
    {
      SNS_UPRINTF(ERROR, this, "GET_RESULT, curr_inst is null");
      rv_inst = NULL;
    }
  }
  else if (curr_inst != NULL && new_req != NULL && curr_req != NULL &&
    new_req->message_id == SNS_STD_MSGID_SNS_STD_FLUSH_REQ)
  {
    this->instance_api->set_client_config(curr_inst, new_req);
    rv_inst = &sns_instance_no_error;
  }

  // Remove the old instance if there is no active request
  if (NULL != curr_inst && NULL == curr_inst->cb->get_client_request(
      curr_inst, this->sensor_api->get_sensor_uid(this), true))
  {
      this->cb->remove_instance(curr_inst);
  }

  return rv_inst;
}

const sns_sensor_api qsh_ble_test_sensor_api =
{
  .struct_len = sizeof(sns_sensor_api),
  .init = &qsh_ble_test_init,
  .deinit = &qsh_ble_test_deinit,
  .get_sensor_uid = &qsh_ble_test_get_suid,
  .set_client_request = &qsh_ble_test_set_client_request,
  .notify_event = &qsh_ble_test_notify_event,
};

