/**============================================================================
  @file qsh_audio_context_sensor_instance.c

  @brief The Audio Context Sensor Instance implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_context_sensor_instance.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_service_manager.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

static void qsh_audio_context_print_event_config(sns_sensor_instance *const this)
{
   qsh_audio_context_inst_state *state_ptr = (qsh_audio_context_inst_state *)this->state->state;

   for (int i = 0; i < state_ptr->inst_config.num_context; i++)
   {
      SNS_INST_PRINTF(LOW,
                      this,
                      "request for enabling context_id %d, "
                      "threshold %d, step_size %d",
                      state_ptr->inst_config.event_config[i].context_id,
                      state_ptr->inst_config.event_config[i].confidence_threshold,
                      state_ptr->inst_config.event_config[i].step_size);
   }
}

/* See sns_sensor_instance_api::init */
sns_rc qsh_audio_context_inst_init(sns_sensor_instance *this, sns_sensor_state const *state_ptr)
{
   sns_rc                          rc               = SNS_RC_SUCCESS;
   qsh_audio_context_inst_state *  inst_state_ptr   = (qsh_audio_context_inst_state *)this->state->state;
   qsh_audio_context_sensor_state *sensor_state_ptr = (qsh_audio_context_sensor_state *)state_ptr->state;
   sns_service_manager *           service_mgr_ptr  = this->cb->get_service_manager(this);

   inst_state_ptr->event_service =
      (sns_event_service *)service_mgr_ptr->get_service(service_mgr_ptr, SNS_EVENT_SERVICE);

   // one client is sufficient for all the instances.
   // aggregated requests from all the instances are sent.
   if (NULL == sensor_state_ptr->uqci_client_handle)
   {
      rc = uqci_register_client(sensor_state_ptr->audio_event_state.uqci_svc_handle_ptr,
                                qsh_audio_context_uqci_receive_msg,
                                sensor_state_ptr->audio_event_state.qsh_invoke_handle_ptr,
                                &sensor_state_ptr->uqci_client_handle);

      // register for the usecase info event
      if (SNS_RC_SUCCESS == rc)
      {
         qsh_audio_context_uqci_register_usecase_info_event(sensor_state_ptr, 1);
      }
   }

   SNS_INST_PRINTF(MED, this, "qsh_audio_context_inst_init");
   return rc;
}

/* See sns_sensor_instance_api::deinit */
sns_rc qsh_audio_context_inst_deinit(sns_sensor_instance *const this)
{
   UNUSED_VAR(this);
   SNS_INST_PRINTF(MED, this, "qsh_audio_context_inst_deinit");

   return SNS_RC_SUCCESS;
}

/* See sns_sensor_instance_api::set_client_config */
sns_rc qsh_audio_context_inst_set_client_config(sns_sensor_instance *const this, sns_request const *client_request_ptr)
{
   sns_rc                        rv        = SNS_RC_FAILED;
   qsh_audio_context_inst_state *state_ptr = (qsh_audio_context_inst_state *)this->state->state;

   switch (client_request_ptr->message_id)
   {
      case QSH_AUDIO_CONTEXT_MSGID_QSH_AUDIO_CONTEXT_REQ:
      {
         qsh_audio_context_req request = qsh_audio_context_req_init_default;

         // clear any previous configuration request.
         sns_memzero(&state_ptr->inst_config, sizeof(state_ptr->inst_config));

         if (!pb_decode_request(client_request_ptr, qsh_audio_context_req_fields, NULL, &request))
         {
            return SNS_RC_FAILED;
         }
         else
         {
            state_ptr->inst_config.num_context = request.config_count;
            for (int i = 0; i < request.config_count; i++)
            {
               state_ptr->inst_config.event_config[i].context_id           = request.config[i].context_id;
               state_ptr->inst_config.event_config[i].confidence_threshold = request.config[i].threshold;
               state_ptr->inst_config.event_config[i].step_size            = request.config[i].step_size;
            }
         }

         qsh_audio_context_print_event_config(this);
      }
      break;
      default:
         rv = SNS_RC_NOT_SUPPORTED;
         SNS_INST_PRINTF(MED, this, "unsupported config %d", client_request_ptr->message_id);
         break;
   }

   return rv;
}

