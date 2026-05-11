/**============================================================================
  @file qsh_audio_data_sensor_island.c

  @brief The Audio Data Sensor implementation island code

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_data_sensor.h"
#include "qsh_audio_data_sensor_instance.h"
#include "qsh_audio_data.pb.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

// qsh callback function to handle response and events.
static sns_rc qsh_audio_data_qsh_invoke_cb(sns_sensor *const this, void *user_arg_ptr)
{

   struct uqsocket_msg_header_t *cb_data = user_arg_ptr;

   // loop through all the sensor instances config
   sns_sensor_instance *inst_ptr = NULL;
   for (inst_ptr = this->cb->get_sensor_instance(this, true); NULL != inst_ptr;
        inst_ptr = this->cb->get_sensor_instance(this, false))
   {
      qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)inst_ptr->state->state;
      if (inst_state_ptr->client_id == cb_data->client_id)
      {
         break;
      }
   }

   if (NULL == inst_ptr)
   {
      SNS_PRINTF(LOW, this, "NULL POINTER");
      return SNS_RC_FAILED;
   }

   qsh_audio_data_handle_resp((sns_sensor_instance *)inst_ptr,
                              cb_data->opcode,
                              cb_data->token,
                              cb_data->payload_len,
                              (cb_data + 1));

   return SNS_RC_SUCCESS;
}

// callback function to receive messages from ADSP
void qsh_audio_data_uqci_receive_msg(void *cb_ctx_ptr, uqsocket_msg_header_t *msg_header)
{
   sns_rc rc;

   qsh_invoke(cb_ctx_ptr, qsh_audio_data_qsh_invoke_cb, &rc, msg_header);
}

/*===========================================================================
  Public API Definitions
  ===========================================================================*/

/* See sns_sensor::get_sensor_uid */
sns_sensor_uid const *qsh_audio_data_get_sensor_uid(sns_sensor const *this)
{
   UNUSED_VAR(this);
   static const sns_sensor_uid ads_suid = ADS_SUID;
   return &ads_suid;
}

const sns_sensor_api qsh_audio_data_api = {
   .struct_len         = sizeof(sns_sensor_api),
   .init               = &qsh_audio_data_init,
   .deinit             = &qsh_audio_data_deinit,
   .get_sensor_uid     = &qsh_audio_data_get_sensor_uid,
   .set_client_request = &qsh_audio_data_set_client_request,
   .notify_event       = &qsh_audio_data_notify_event,
};
