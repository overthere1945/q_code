/**============================================================================
  @file qsh_audio_data_sensor_instance_island.c

  @brief The Audio Data Sensor Instance implementation, island code

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qurt_sclk.h"
#include "qurt_timer.h"
#include "qsh_audio_data_sensor_instance.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_sensor_util.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/
// publish received events
static void qsh_audio_data_inst_publish_event(sns_sensor_instance *const this,
                                              uint32_t payload_size,
                                              uint8_t *buf_ptr,
                                              sns_time timestamp)
{
   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;
   qsh_audio_data_event         event          = qsh_audio_data_event_init_default;

   pb_buffer_arg data        = (pb_buffer_arg){ .buf = buf_ptr, .buf_len = payload_size };
   event.buffer.funcs.encode = &pb_encode_string_cb;
   event.buffer.arg          = &data;

   size_t encoded_len = payload_size + 32;

   //pb_get_encoded_size(&encoded_len, qsh_audio_data_event_fields, &event);

   //SNS_INST_PRINTF(ERROR, this, "Payload Size %lu and Encoded Len %lu", payload_size, encoded_len);
   sns_sensor_event *event_out_ptr =
      inst_state_ptr->event_service->api->alloc_event(inst_state_ptr->event_service, this, encoded_len);

   if (event_out_ptr != NULL)
   {
      pb_ostream_t ostream = pb_ostream_from_buffer((pb_byte_t *)event_out_ptr->event, event_out_ptr->event_len);

      if (pb_encode(&ostream, qsh_audio_data_event_fields, &event))
      {
         event_out_ptr->timestamp  = QURT_TIMER_TIMETICK_FROM_US(timestamp);
         event_out_ptr->message_id = QSH_AUDIO_DATA_MSGID_QSH_AUDIO_DATA_EVENT;
         event_out_ptr->event_len  = ostream.bytes_written;
         inst_state_ptr->event_service->api->publish_event(inst_state_ptr->event_service, this, event_out_ptr, NULL);
      }
   }
}

// publish the received packet from each instance
static void qsh_audio_data_packet_event(sns_sensor_instance *const this, uint32_t payload_size, void *payload_ptr)
{
   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;

   if (!inst_state_ptr->data_flow_enable)
   {
      return;
   }

   data_event_id_qsocket_rd_ep_buffer_done_t *payload = (data_event_id_qsocket_rd_ep_buffer_done_t *)payload_ptr;

   if (payload->data_size)
   {
      if (payload_size < sizeof(data_event_id_qsocket_rd_ep_buffer_done_t) + payload->data_size)
      {
         SNS_INST_UPRINTF(ERROR, this, "Insufficient payload size for response.");
         return;
      }

      sns_time timestamp = 0;
      if (payload->flags.is_timestamp_valid)
      {
         timestamp = ((sns_time)payload->timestamp_msw << 32) | (sns_time)payload->timestamp_lsw;
      }
      else
      {
         // timestamp = sns_get_system_time();
      }
      uint8_t *buf_ptr = (uint8_t *)((uint8_t *)payload + sizeof(data_event_id_qsocket_rd_ep_buffer_done_t));

      qsh_audio_data_inst_publish_event(this, payload->data_size, buf_ptr, timestamp);
      SNS_INST_UPRINTF(LOW,
                       this,
                       "DATA_EVENT_ID_QSOCKET_RD_EP_BUFFER_DONE TS LSW: %lu MSW: %lu EOF: %d data_size %lu",
                       payload->timestamp_lsw,
                       payload->timestamp_msw,
                       payload->flags.end_of_frame,
                       payload->data_size);
   }

   /* Check if history data is done when flush request is in process. If it is then disable data flow
    * and clear flush req */
   if ((inst_state_ptr->flush_req) && (payload->flags.end_of_frame))
   {
      // current flush request for history is done
      inst_state_ptr->flush_req = false;

      // disable data flow
      inst_state_ptr->data_flow_enable = false;

      // send out flush done event
      sns_sensor_util_send_flush_event(inst_state_ptr->suid_ptr, this);
   }
}

static void qsh_audio_data_update_encode_ch_info(uint8_t num_channels, uint8_t *ch_type_ptr, uint32_t *encode_ch_ptr)
{
   for (uint8_t ch = 0; ch < num_channels; ch++)
   {
      switch (ch_type_ptr[ch])
      {
         case PCM_CHANNEL_L:
         {
            encode_ch_ptr[ch] = QSH_AUDIO_DATA_CHANNEL_L;
            break;
         }
         case PCM_CHANNEL_R:
         {
            encode_ch_ptr[ch] = QSH_AUDIO_DATA_CHANNEL_R;
            break;
         }
         default:
         {
            encode_ch_ptr[ch] = QSH_AUDIO_DATA_CHANNEL_INVALID;
            break;
         }
      }
   }

   return;
}

// publish received events
static void qsh_audio_data_inst_publish_media_format_event(sns_sensor_instance *const this, uint8_t *buf_ptr)
{
   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;
   qsh_audio_data_media_format_event event     = qsh_audio_data_media_format_event_init_default;

   payload_media_fmt_pcm_t * pcm = (payload_media_fmt_pcm_t*) buf_ptr;

   bool_t encode_ch_type = FALSE;
   uint32_t encode_ch_ptr[ADS_MAX_CHANNELS] = {0};

   if(pcm->num_channels <= ADS_MAX_CHANNELS)
   {
	   encode_ch_type = TRUE;
	   qsh_audio_data_update_encode_ch_info(pcm->num_channels, (uint8_t *)(pcm + 1), encode_ch_ptr);
   }

   uint8_t arr_index = 0;
   pb_uint_32_arr_arg  ch_type_arg = ( pb_uint_32_arr_arg ){.arr = encode_ch_ptr, .arr_len = pcm->num_channels, .arr_index = &arr_index};

   event.sampling_rate   = pcm->sample_rate;
   event.num_channels    = pcm->num_channels;
   event.bits_per_sample = pcm->bits_per_sample;
   event.endianness      = pcm->endianness;
   event.stream_type     = inst_state_ptr->inst_data_config.stream_type;

   if(encode_ch_type)
   {
	   event.channel_type.funcs.encode = &pb_encode_uint_32_arr_cb;
	   event.channel_type.arg = &ch_type_arg;
   }


   size_t encoded_len = 0;

   pb_get_encoded_size(&encoded_len, qsh_audio_data_media_format_event_fields, &event);

   sns_sensor_event *event_out_ptr =
      inst_state_ptr->event_service->api->alloc_event(inst_state_ptr->event_service, this, encoded_len);

   if (event_out_ptr != NULL)
   {
      pb_ostream_t ostream = pb_ostream_from_buffer((pb_byte_t *)event_out_ptr->event, event_out_ptr->event_len);

      if (pb_encode(&ostream, qsh_audio_data_media_format_event_fields, &event))
      {
         event_out_ptr->timestamp  = sns_get_system_time();
         event_out_ptr->message_id = QSH_AUDIO_DATA_MSGID_QSH_AUDIO_DATA_MEDIA_FORMAT_EVENT;
         event_out_ptr->event_len  = ostream.bytes_written;
         inst_state_ptr->event_service->api->publish_event(inst_state_ptr->event_service, this, event_out_ptr, NULL);
      }
      else
      {
         SNS_INST_PRINTF(ERROR, this, "Encode Media Format failed");
      }
   }
}


// publish the received packet from each instance
static void qsh_audio_data_publish_media_format_event(sns_sensor_instance *const this, uint32_t payload_size, void *payload_ptr)
{

   media_format_t *payload_media_fmt = (media_format_t *)payload_ptr;
   payload_media_fmt_pcm_t *pcm = (payload_media_fmt_pcm_t*) (payload_media_fmt + 1);

   if(payload_size < sizeof(media_format_t) + sizeof(payload_media_fmt_pcm_t) )
   {
      SNS_INST_PRINTF(ERROR, this, "Insufficient payload size for response.");
      return;
   }

   SNS_INST_UPRINTF(LOW, this, "qsh_audio_data_publish_media_format_event - Media Format received %lu %lu %lu %lu",
                               pcm->sample_rate,
                               pcm->num_channels,
                               pcm->bits_per_sample,
                               pcm->endianness);

   qsh_audio_data_inst_publish_media_format_event(this, (uint8_t *) pcm);
}


// func to handle response/events
void qsh_audio_data_handle_resp(sns_sensor_instance *const this,
                                uint32_t opcode,
                                uint32_t token,
                                uint32_t payload_len,
                                void *   payload_ptr)
{

   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;

   SNS_INST_UPRINTF(LOW, this, "qsh_audio_data_handle_resp opcode = %lx ", opcode);
   switch (opcode)
   {
      case ASPS_CMD_BASIC_RSP:
      {
         asps_basic_cmd_rsp_t *rsp_msg_ptr = (asps_basic_cmd_rsp_t *)payload_ptr;
         if (payload_len < sizeof(asps_basic_cmd_rsp_t))
         {
            SNS_INST_PRINTF(ERROR, this, "Insufficient payload size for response.");
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
                  SNS_INST_PRINTF(HIGH,
                             this,
                             "Error %lu received from miid 0x%x in response of cmd 0x%x",
                             rsp_msg_ptr->status,
                             token,
                             rsp_msg_ptr->opcode);

                  qsh_audio_event_instance_send_error(this, SNS_STD_ERROR_NOT_SUPPORTED, inst_state_ptr->suid_ptr);
                  return;
               }
               default:
                  break;
            }
         }
         else if (ASPS_CMD_SET_CFG == rsp_msg_ptr->opcode && token == inst_state_ptr->usecase_info.sns_rd_ep_miid)
         {
            qsh_audio_island_exit();
            if (inst_state_ptr->data_flow_pending)
            {
               SNS_INST_PRINTF(LOW, this, "requesting pending data flow %d ", inst_state_ptr->data_flow_enable);
               // if data flow is pending then first send data-flow command.
               qsh_audio_data_uqci_send_data_flow(inst_state_ptr);
               inst_state_ptr->data_flow_pending = false;
            }
            else
            {
               // notify client that the config is successful
               sns_sensor_util_send_event_without_payload(this,
                                                          inst_state_ptr->suid_ptr,
                                                          QSH_AUDIO_DATA_MSGID_QSH_AUDIO_DATA_CONFIG);
            }
         }
      }
      break;
      case ASPS_EVENT_TO_CLIENT:
      {
         if (payload_len < sizeof(asps_event_to_client_t))
         {
            SNS_INST_PRINTF(ERROR, this, "Insufficient payload size for event.");
            return;
         }

         asps_event_to_client_t *event_msg_ptr = (asps_event_to_client_t *)payload_ptr;

         switch (event_msg_ptr->event_id)
         {
            case EVENT_ID_ASPS_SENSOR_USECASE_INFO:
            {
               struct audio_data_usecase_info_payload_t
               {
                  asps_event_to_client_t                 asps_event_header;
                  event_id_asps_sensor_usecase_info_t    usecase_info_header;
                  asps_audio_data_usecase_register_ack_payload_t audio_data_usecase_info;
               } *event_ptr = payload_ptr;

               if (event_msg_ptr->event_payload_size > sizeof(event_id_asps_sensor_usecase_info_t))
               {
                  if ((ASPS_USECASE_ID_PCM_DATA == event_ptr->usecase_info_header.usecase_id) ||
                      (ASPS_USECASE_ID_PCM_DATA_V2 == event_ptr->usecase_info_header.usecase_id))
                  {
                     if (event_ptr->usecase_info_header.payload_size >= sizeof(asps_audio_data_usecase_register_ack_payload_t))
                     {
                        qsh_audio_island_exit();
                        qsh_audio_data_receive_usecase_info(this, &event_ptr->audio_data_usecase_info);
                     }
                     else
                     {
                        SNS_INST_PRINTF(ERROR, this, "insufficient size for ack payload %lu received",
                                        event_ptr->usecase_info_header.payload_size);
                     }
                  }
               }
               else
               {
                  SNS_INST_PRINTF(ERROR, this, "insufficient size for event %lu received", event_msg_ptr->event_id);
               }
            }
            break;
            default:
            {
               SNS_INST_PRINTF(ERROR, this, "Unsupported event %lu received", event_msg_ptr->event_id);
               break;
            }
         }
      }
      break;
      case DATA_EVENT_ID_QSOCKET_RD_EP_BUFFER_DONE:
      {
         qsh_audio_data_packet_event(this, payload_len, payload_ptr);
         break;
      }
      case DATA_EVENT_ID_QSOCKET_RD_EP_MEDIA_FORMAT:
      {
         qsh_audio_data_publish_media_format_event(this, payload_len, payload_ptr);
         break;
      }
      default:
         SNS_INST_PRINTF(ERROR, this, "unsupported opcode %lu received", opcode);
         break;
   }
}

/*===========================================================================
  Public API Definitions
  ===========================================================================*/
/**
 * Process incoming events to the Sensor Instance.
 */
static sns_rc qsh_audio_data_inst_notify_event(sns_sensor_instance *const this)
{
   UNUSED_VAR(this);
   sns_rc rc = SNS_RC_SUCCESS;
   return rc;
}

const sns_sensor_instance_api qsh_audio_data_sensor_instance_api = { .struct_len = sizeof(sns_sensor_instance_api),
                                                               .init       = &qsh_audio_data_inst_init,
                                                               .deinit     = &qsh_audio_data_inst_deinit,
                                                               .set_client_config =
                                                                  &qsh_audio_data_inst_set_client_config,
                                                               .notify_event = &qsh_audio_data_inst_notify_event };

