/*=============================================================================
  @file qsh_audio_speaker_diarization_sensor_island.c

  The SDZ Sensor Instance implementation

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_speaker_diarization_sensor.h"
#include "qsh_audio_speaker_diarization_sensor_instance.h"
#include "qsh_audio_speaker_diarization.pb.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

// publish the received events from each instance
static void qsh_audio_speaker_diarization_disaggregate_event(sns_sensor *const this,
                                                             uint32_t payload_size,
                                                             void    *payload_ptr)
{
   uint32_t                     req_event_payload_size = sizeof(event_id_sdz_output_event_t);
   event_id_sdz_output_event_t *event_ptr              = (event_id_sdz_output_event_t *)payload_ptr;
   if (payload_size < req_event_payload_size)
   {
      SNS_PRINTF(ERROR, this, "Insuffcient payload received for sdz event.");
      return;
   }

   req_event_payload_size += (event_ptr->num_outputs * sizeof(sdz_output_status_t));
   if (payload_size < req_event_payload_size)
   {
      SNS_PRINTF(ERROR, this, "Insuffcient payload received for sdz event.");
      return;
   }

   sdz_output_status_t *status_ptr = (sdz_output_status_t *)(event_ptr + 1);
   for (int i = 0; i < event_ptr->num_outputs; i++)
   {
      req_event_payload_size += (status_ptr->num_speakers * sizeof(sdz_speaker_info_t));
      if (payload_size < req_event_payload_size)
      {
         SNS_PRINTF(ERROR, this, "Insuffcient payload received for sdz event.");
         return;
      }
      status_ptr =
         (sdz_output_status_t *)((uint8_t *)(status_ptr + 1) + (status_ptr->num_speakers * sizeof(sdz_speaker_info_t)));
   }

   // loop through all the sensor instances and publish requested sdz proxy events.
   for (sns_sensor_instance *inst_ptr = this->cb->get_sensor_instance(this, true); NULL != inst_ptr;
        inst_ptr                      = this->cb->get_sensor_instance(this, false))
   {
      qsh_audio_speaker_diarization_inst_publish_event(inst_ptr, event_ptr);
   }
}

// func to handle response/events
static void qsh_audio_speaker_diarization_handle_resp(sns_sensor *const this,
                                                      uint32_t opcode,
                                                      uint32_t token,
                                                      uint32_t payload_len,
                                                      void    *payload_ptr)
{
   // qsh_audio_speaker_diarization_sensor_state *sensor_state_ptr = (qsh_audio_speaker_diarization_sensor_state
   // *)this->state->state;

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
               struct sdz_usecase_info_payload_t
               {
                  asps_event_to_client_t                  asps_event_header;
                  event_id_asps_sensor_usecase_info_t     usecase_info_header;
                  asps_sdz_usecase_register_ack_payload_t sdz_usecase_info;
               } *event_ptr = payload_ptr;

               if (ASPS_USECASE_ID_SDZ == event_ptr->usecase_info_header.usecase_id)
               {
                  qsh_audio_island_exit();
                  qsh_audio_speaker_diarization_receive_usecase_info(this, &event_ptr->sdz_usecase_info);
               }
               else
               {
                  SNS_PRINTF(ERROR, this, "received different use case id", event_msg_ptr->event_id);
               }
            }
            break;

            case EVENT_ID_SDZ_OUTPUT:
               qsh_audio_speaker_diarization_disaggregate_event(this,
                                                                event_msg_ptr->event_payload_size,
                                                                (event_msg_ptr + 1));
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
struct qsh_audio_speaker_diarization_qsh_invoke_cb_data
{
   uint32_t opcode;
   uint32_t token;
   uint32_t payload_len;
   void    *payload;
};

// qsh callback function to handle response and events.
static sns_rc qsh_audio_speaker_diarization_qsh_invoke_cb(sns_sensor *const this, void *user_arg_ptr)
{
   UNUSED_VAR(this);
   struct qsh_audio_speaker_diarization_qsh_invoke_cb_data *cb_data = user_arg_ptr;
   qsh_audio_speaker_diarization_handle_resp(this,
                                             cb_data->opcode,
                                             cb_data->token,
                                             cb_data->payload_len,
                                             cb_data->payload);
   return SNS_RC_SUCCESS;
}

// callback function to receive messages from ADSP
void qsh_audio_speaker_diarization_uqci_receive_msg(void *cb_ctx_ptr, uqsocket_msg_header_t *msg_header)
{
   sns_rc rc;

   struct qsh_audio_speaker_diarization_qsh_invoke_cb_data cb_data = { .opcode      = msg_header->opcode,
                                                                       .token       = msg_header->token,
                                                                       .payload_len = msg_header->payload_len,
                                                                       .payload     = (void *)(msg_header + 1) };
   qsh_invoke(cb_ctx_ptr, qsh_audio_speaker_diarization_qsh_invoke_cb, &rc, &cb_data);
}

/*===========================================================================
  Public API Definitions
  ===========================================================================*/
/* See sns_sensor::get_sensor_uid */
sns_sensor_uid const *qsh_audio_speaker_diarization_get_sensor_uid(sns_sensor const *this)
{
   UNUSED_VAR(this);
   static const sns_sensor_uid sdz_suid = SDZ_SUID;
   return &sdz_suid;
}

const sns_sensor_api qsh_audio_speaker_diarization_api = {
   .struct_len         = sizeof(sns_sensor_api),
   .init               = &qsh_audio_speaker_diarization_init,
   .deinit             = &qsh_audio_speaker_diarization_deinit,
   .get_sensor_uid     = &qsh_audio_speaker_diarization_get_sensor_uid,
   .set_client_request = &qsh_audio_speaker_diarization_set_client_request,
   .notify_event       = &qsh_audio_speaker_diarization_notify_event,
};
