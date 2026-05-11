/**============================================================================
  @file qsh_upd_proxy_sensor_island.c

  @brief The Upd Proxy Sensor implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_upd_proxy_sensor.h"
#include "qsh_upd_proxy_sensor_instance.h"
#include "sns_proximity.pb.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

// publish the received events from each instance
static void qsh_upd_proxy_disaggregate_event(sns_sensor *const this, uint32_t payload_size, void *payload_ptr)
{
   event_id_upd_detection_event_t *event_ptr = (event_id_upd_detection_event_t *)payload_ptr;
   if ((payload_size < sizeof(event_id_upd_detection_event_t)))
   {
      SNS_PRINTF(ERROR, this, "Insuffcient payload received for upd proxy event.");
      return;
   }

   if (event_ptr->proximity_event_type == US_DETECT_INVALID)
   {
      SNS_PRINTF(ERROR, this, "Invalid detection!");
      return;
   }

   // loop through all the sensor instances and publish requested upd proxy events.
   for (sns_sensor_instance *inst_ptr = this->cb->get_sensor_instance(this, true); NULL != inst_ptr;
        inst_ptr                      = this->cb->get_sensor_instance(this, false))
   {
      qsh_upd_proxy_inst_publish_event(inst_ptr, event_ptr);
   }
}

// func to handle response/events
static void qsh_upd_proxy_handle_resp(sns_sensor *const this,
                                      uint32_t opcode,
                                      uint32_t token,
                                      uint32_t payload_len,
                                      void *   payload_ptr)
{
   // qsh_upd_proxy_sensor_state *sensor_state_ptr = (qsh_upd_proxy_sensor_state *)this->state->state;

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
               struct upd_usecase_info_payload_t
               {
                  asps_event_to_client_t                  asps_event_header;
                  event_id_asps_sensor_usecase_info_t     usecase_info_header;
                  asps_upd_usecase_register_ack_payload_t upd_usecase_info;
               } *event_ptr = payload_ptr;

               if (ASPS_USECASE_ID_UPD == event_ptr->usecase_info_header.usecase_id)
               {
                  qsh_audio_island_exit();
                  qsh_upd_proxy_receive_usecase_info(this, &event_ptr->upd_usecase_info);
               }
               else
               {
                  SNS_PRINTF(ERROR, this, "received different use case id", event_msg_ptr->event_id);
               }
            }
            break;

            case EVENT_ID_GENERIC_US_DETECTION:
               qsh_upd_proxy_disaggregate_event(this, event_msg_ptr->event_payload_size, (event_msg_ptr + 1));
               break;

            default:
               SNS_PRINTF(ERROR, this, "Unsupported event %lu received", event_msg_ptr->event_id);
               break;
         }
      }
      break;
      default:
         SNS_PRINTF(ERROR, this, "unsupported opcode %lu received", opcode);
         break;
   }
}

// structure for qsh callback
struct qsh_upd_proxy_qsh_invoke_cb_data
{
   uint32_t opcode;
   uint32_t token;
   uint32_t payload_len;
   void *   payload;
};

// qsh callback function to handle response and events.
static sns_rc qsh_upd_proxy_qsh_invoke_cb(sns_sensor *const this, void *user_arg_ptr)
{
   UNUSED_VAR(this);
   struct qsh_upd_proxy_qsh_invoke_cb_data *cb_data = user_arg_ptr;
   qsh_upd_proxy_handle_resp(this, cb_data->opcode, cb_data->token, cb_data->payload_len, cb_data->payload);
   return SNS_RC_SUCCESS;
}

// callback function to receive messages from ADSP
void qsh_upd_proxy_uqci_receive_msg(void *cb_ctx_ptr, uqsocket_msg_header_t *msg_header)
{
   sns_rc rc;

   struct qsh_upd_proxy_qsh_invoke_cb_data cb_data = { .opcode      = msg_header->opcode,
                                                       .token       = msg_header->token,
                                                       .payload_len = msg_header->payload_len,
                                                       .payload     = (void *)(msg_header + 1) };
   qsh_invoke(cb_ctx_ptr, qsh_upd_proxy_qsh_invoke_cb, &rc, &cb_data);
}

/*===========================================================================
  Public API Definitions
  ===========================================================================*/
/* See sns_sensor::get_sensor_uid */
sns_sensor_uid const *qsh_upd_proxy_get_sensor_uid(sns_sensor const *this)
{
   UNUSED_VAR(this);
   static const sns_sensor_uid upd_suid = UPD_SUID;
   return &upd_suid;
}

const sns_sensor_api qsh_upd_proxy_api = {
   .struct_len         = sizeof(sns_sensor_api),
   .init               = &qsh_upd_proxy_init,
   .deinit             = &qsh_upd_proxy_deinit,
   .get_sensor_uid     = &qsh_upd_proxy_get_sensor_uid,
   .set_client_request = &qsh_upd_proxy_set_client_request,
   .notify_event       = &qsh_upd_proxy_notify_event,
};
