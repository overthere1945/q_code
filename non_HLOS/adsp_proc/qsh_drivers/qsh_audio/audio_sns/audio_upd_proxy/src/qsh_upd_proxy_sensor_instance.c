/**============================================================================
  @file qsh_upd_proxy_sensor_instance.c

  @brief The UPD PROXY Sensor Instance implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_upd_proxy_sensor_instance.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_service_manager.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/
/* See sns_sensor_instance_api::init */
sns_rc qsh_upd_proxy_inst_init(sns_sensor_instance *this, sns_sensor_state const *state_ptr)
{
   sns_rc                      rc               = SNS_RC_SUCCESS;
   qsh_upd_proxy_inst_state *  inst_state_ptr   = (qsh_upd_proxy_inst_state *)this->state->state;
   qsh_upd_proxy_sensor_state *sensor_state_ptr = (qsh_upd_proxy_sensor_state *)state_ptr->state;
   sns_service_manager *       service_mgr_ptr  = this->cb->get_service_manager(this);

   inst_state_ptr->event_service =
      (sns_event_service *)service_mgr_ptr->get_service(service_mgr_ptr, SNS_EVENT_SERVICE);

   // one client is sufficient for all the instances.
   // aggregated requests from all the instances are sent.
   if (NULL == sensor_state_ptr->uqci_client_handle)
   {
      rc = uqci_register_client(sensor_state_ptr->audio_event_state.uqci_svc_handle_ptr,
                                qsh_upd_proxy_uqci_receive_msg,
                                sensor_state_ptr->audio_event_state.qsh_invoke_handle_ptr,
                                &sensor_state_ptr->uqci_client_handle);

      // register for the usecase info event
      if (SNS_RC_SUCCESS == rc)
      {
         qsh_upd_proxy_uqci_register_usecase_info_event(sensor_state_ptr, 1);
      }
   }
   SNS_INST_PRINTF(MED, this, "qsh_upd_proxy_inst_init");
   return rc;
}

/* See sns_sensor_instance_api::deinit */
sns_rc qsh_upd_proxy_inst_deinit(sns_sensor_instance *const this)
{
   UNUSED_VAR(this);
   SNS_INST_PRINTF(MED, this, "qsh_upd_proxy_inst_deinit");

   return SNS_RC_SUCCESS;
}

/* See sns_sensor_instance_api::set_client_config */
sns_rc qsh_upd_proxy_inst_set_client_config(sns_sensor_instance *const this, sns_request const *client_request_ptr)
{
   sns_rc                    rv        = SNS_RC_FAILED;
   qsh_upd_proxy_inst_state *state_ptr = (qsh_upd_proxy_inst_state *)this->state->state;

   switch (client_request_ptr->message_id)
   {
      case SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG:
      {
         state_ptr->upd_enable = 1;
         return SNS_RC_SUCCESS;
      }
      break;
      default:
         rv = SNS_RC_NOT_SUPPORTED;
         SNS_INST_PRINTF(MED, this, "unsupported config %d", client_request_ptr->message_id);
         break;
   }

   return rv;
}
