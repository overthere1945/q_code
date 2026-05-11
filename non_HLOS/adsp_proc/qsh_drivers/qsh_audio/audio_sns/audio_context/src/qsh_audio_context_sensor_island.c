/**============================================================================
  @file qsh_audio_context_sensor_island.c

  @brief The Audio Context Sensor implementation island code

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_context_sensor.h"
#include "qsh_audio_context_sensor_instance.h"
#include "qsh_audio_context.pb.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

// publish the received events from each instance
static void qsh_audio_context_disaggregate_event(sns_sensor *const this, uint32_t payload_size, void *payload_ptr)
{
   struct acd_event_payload_t
   {
      event_id_acd_detection_event_t       event_header;
      acd_key_info_t                       key_info;
      acd_generic_key_id_event_t           gen_key_info;
      acd_context_specific_generic_event_t context_event_arr[];
   } *temp = payload_ptr;

   event_id_acd_detection_event_t *event_payload_ptr   = &temp->event_header;
   acd_key_info_t *                key_info_ptr        = &temp->key_info;
   acd_generic_key_id_event_t *    gen_key_payload_ptr = &temp->gen_key_info;

   uint32_t min_req_payload_size =
      sizeof(event_id_acd_detection_event_t) + sizeof(acd_key_info_t) + sizeof(acd_generic_key_id_event_t);

   if ((payload_size < min_req_payload_size) || (ACD_KEY_ID_GENERIC_INFO != key_info_ptr->key_id) ||
       (payload_size <
        (min_req_payload_size + gen_key_payload_ptr->num_contexts * sizeof(acd_context_specific_generic_event_t))))
   {
      SNS_PRINTF(ERROR, this, "Insuffcient payload received for context event.");
      return;
   }

   acd_context_specific_generic_event_t *context_event_arr = &temp->context_event_arr[0];

   // loop through all the sensor instances and publish requested context events.
   for (sns_sensor_instance *inst_ptr = this->cb->get_sensor_instance(this, true); NULL != inst_ptr;
        inst_ptr                      = this->cb->get_sensor_instance(this, false))
   {

      qsh_audio_context_inst_state *inst_state_ptr = (qsh_audio_context_inst_state *)inst_ptr->state->state;

      qsh_audio_context_event event_arr[MAX_CONTEXT_PER_INSTANCE] = { { 0 } };
      sns_time      event_timestamp =
         ((sns_time)event_payload_ptr->event_timestamp_msw << 32) + (sns_time)event_payload_ptr->event_timestamp_lsw;

      uint32_t num_contexts = 0;

      for (int i = 0; i < gen_key_payload_ptr->num_contexts; i++)
      {
         for (int j = 0; j < inst_state_ptr->inst_config.num_context; j++)
         {
            bool is_send_event = false;
            // if this instance-client requested this context
            if (context_event_arr[i].context_id == inst_state_ptr->inst_config.event_config[j].context_id)
            {
               // if event is ongoing in this instance then send the event.
               if (inst_state_ptr->inst_config.is_event_ongoing[j])
               {
                  is_send_event = true;
               }
               // if event is not ongoing in this instance then send the event only if threshold is crossed
               else if (context_event_arr[i].confidence_score >=
                        inst_state_ptr->inst_config.event_config[j].confidence_threshold)
               {
                  is_send_event = true;
                  // mark the event ongoing.
                  inst_state_ptr->inst_config.is_event_ongoing[j] = true;
               }

               if (is_send_event)
               {
                  event_arr[num_contexts].context_id = context_event_arr[i].context_id;
                  event_arr[num_contexts].event_type = (context_event_arr[i].event_type == 0)
                                                          ? QSH_AUDIO_CONTEXT_EVENT_STOPPED
                                                          : QSH_AUDIO_CONTEXT_EVENT_DETECTED;
                  event_arr[num_contexts].threshold = context_event_arr[i].confidence_score;
                  event_arr[num_contexts].start_timestamp =
                     ((uint64_t)context_event_arr[i].detection_timestamp_msw << 32) +
                     ((uint64_t)context_event_arr[i].detection_timestamp_lsw);
                  num_contexts++;
               }

               // if event stopped then make the ongoing flag false
               if (context_event_arr[i].event_type == 0)
               {
                  inst_state_ptr->inst_config.is_event_ongoing[j] = false;
               }

               break;
            }
         }
      }

      qsh_audio_context_inst_publish_event(inst_ptr, num_contexts, event_arr, event_timestamp);
   }
}

// func to handle response/events
static void qsh_audio_context_handle_resp(sns_sensor *const this,
                                          uint32_t opcode,
                                          uint32_t token,
                                          uint32_t payload_len,
                                          void *   payload_ptr)
{
   qsh_audio_context_sensor_state *sensor_state_ptr = (qsh_audio_context_sensor_state *)this->state->state;

   switch (opcode)
   {
      case ASPS_CMD_BASIC_RSP:
      {
         asps_basic_cmd_rsp_t *rsp_msg_ptr = (asps_basic_cmd_rsp_t *)payload_ptr;
         if (payload_len < sizeof(asps_basic_cmd_rsp_t))
         {
            SNS_PRINTF(ERROR, this, "Insufficient payload size for response.");
            return;
         }
         else if (rsp_msg_ptr->status != 0 && token != IGNORE_RSP_TOKEN)
         {
            switch (rsp_msg_ptr->opcode)
            {
               case ASPS_CMD_SET_CFG:
               case ASPS_CMD_GET_CFG:
               case ASPS_CMD_REGISTER_MODULE_EVENTS:
               {
                  SNS_PRINTF(ERROR,
                             this,
                             "Error %lu received from miid 0x%x in response of cmd 0x%x",
                             rsp_msg_ptr->status,
                             token,
                             rsp_msg_ptr->opcode);
                  qsh_audio_event_send_error(this, SNS_STD_ERROR_NOT_SUPPORTED);
                  return;
               }
               default:
                  break;
            }
         }
      }
      break;
      case ASPS_EVENT_TO_CLIENT:
      {
         if (payload_len < sizeof(asps_event_to_client_t))
         {
            SNS_PRINTF(ERROR, this, "Insufficient payload size for event.");
            return;
         }

         asps_event_to_client_t *event_msg_ptr = (asps_event_to_client_t *)payload_ptr;

         switch (event_msg_ptr->event_id)
         {
            case EVENT_ID_ASPS_SENSOR_USECASE_INFO:
            {
               struct acd_usecase_info_payload_t
               {
                  asps_event_to_client_t                  asps_event_header;
                  event_id_asps_sensor_usecase_info_t     usecase_info_header;
                  asps_acd_usecase_register_ack_payload_t acd_usecase_info;
               } *event_ptr = payload_ptr;

               if (event_msg_ptr->event_payload_size > sizeof(event_id_asps_sensor_usecase_info_t))
               {
                  if (ASPS_USECASE_ID_ACD == event_ptr->usecase_info_header.usecase_id)
                  {
                     if (event_ptr->usecase_info_header.payload_size > sizeof(asps_acd_usecase_register_ack_payload_t))
                     {
                        qsh_audio_island_exit();
                        qsh_audio_context_receive_usecase_info(this,
                                                               &event_ptr->acd_usecase_info,
                                                               event_ptr->usecase_info_header.payload_size);
                     }
                  }
               }
               else
               {
                  SNS_PRINTF(ERROR, this, "insufficient size for event %lu received", event_msg_ptr->event_id);
               }
            }
            break;

            case EVENT_ID_ACD_DETECTION_EVENT:
               qsh_audio_context_disaggregate_event(this, event_msg_ptr->event_payload_size, (event_msg_ptr + 1));
               break;

            default:
               SNS_PRINTF(ERROR, this, "Unsupported event %lu received", event_msg_ptr->event_id);
               break;
         }
      }
      break;
      case ASPS_CMD_RSP_GET_CFG:
      {
         struct ctx_list_get_cfg_rsp_payload_t
         {
            asps_cmd_rsp_get_cfg_t                    cmd_rsp_header;
            asps_module_param_data_t                  param_data_header;
            param_id_asps_get_supported_context_ids_t context_list;
         } *rsp_ptr = payload_ptr;

         sensor_state_ptr->is_context_list_query_pending = FALSE;

         if (payload_len < sizeof(struct ctx_list_get_cfg_rsp_payload_t))
         {
            SNS_PRINTF(ERROR, this, "Insufficient payload size received.");
            return;
         }
         if (0 != rsp_ptr->cmd_rsp_header.status)
         {
            SNS_PRINTF(ERROR, this, "Received error %lu.", rsp_ptr->cmd_rsp_header.status);
            qsh_audio_event_send_error(this, SNS_STD_ERROR_NOT_SUPPORTED);
         }
         else
         {
            if (rsp_ptr->param_data_header.param_id != PARAM_ID_ASPS_GET_SUPPORTED_CONTEXT_IDS)
            {
               SNS_PRINTF(ERROR, this, "Invalid param %lu received", rsp_ptr->param_data_header.param_id);
               return;
            }

            if (0 == rsp_ptr->context_list.num_supported_contexts)
            {
               SNS_PRINTF(ERROR, this, "no context supported.");
               qsh_audio_event_send_error(this, SNS_STD_ERROR_NOT_SUPPORTED);
            }
            else
            {
               // loop through all the sensor instances and publish supported context list.
               for (sns_sensor_instance *inst_ptr = this->cb->get_sensor_instance(this, true); NULL != inst_ptr;
                    inst_ptr                      = this->cb->get_sensor_instance(this, false))
               {
                  qsh_audio_context_inst_publish_supported_context(inst_ptr, &rsp_ptr->context_list);
               }
            }
         }
      }
      break;
      default:
         SNS_PRINTF(ERROR, this, "unsupported opcode %lu received", opcode);
         break;
   }
}

// structure for qsh callback
struct qsh_audio_context_qsh_invoke_cb_data
{
   uint32_t opcode;
   uint32_t token;
   uint32_t payload_len;
   void *   payload;
};

// qsh callback function to handle response and events.
static sns_rc qsh_audio_context_qsh_invoke_cb(sns_sensor *const this, void *user_arg_ptr)
{
   UNUSED_VAR(this);
   struct qsh_audio_context_qsh_invoke_cb_data *cb_data = user_arg_ptr;
   qsh_audio_context_handle_resp(this, cb_data->opcode, cb_data->token, cb_data->payload_len, cb_data->payload);
   return SNS_RC_SUCCESS;
}

// callback function to receive messages from ADSP
void qsh_audio_context_uqci_receive_msg(void *cb_ctx_ptr, uqsocket_msg_header_t *msg_header)
{
   sns_rc rc;

   struct qsh_audio_context_qsh_invoke_cb_data cb_data = { .opcode      = msg_header->opcode,
                                                           .token       = msg_header->token,
                                                           .payload_len = msg_header->payload_len,
                                                           .payload     = (void *)(msg_header + 1) };
   qsh_invoke(cb_ctx_ptr, qsh_audio_context_qsh_invoke_cb, &rc, &cb_data);
}

/*===========================================================================
  Public API Definitions
  ===========================================================================*/

/* See sns_sensor::get_sensor_uid */
static sns_sensor_uid const *qsh_audio_context_get_sensor_uid(sns_sensor const *this)
{
   UNUSED_VAR(this);
   static const sns_sensor_uid acs_suid = ACS_SUID;
   return &acs_suid;
}

const sns_sensor_api qsh_audio_context_api = {
   .struct_len         = sizeof(sns_sensor_api),
   .init               = &qsh_audio_context_init,
   .deinit             = &qsh_audio_context_deinit,
   .get_sensor_uid     = &qsh_audio_context_get_sensor_uid,
   .set_client_request = &qsh_audio_context_set_client_request,
   .notify_event       = &qsh_audio_context_notify_event,
};
