/**============================================================================
  @file qsh_audio_data_sensor_instance.c

  @brief The Audio Data Sensor Instance implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_data_sensor_instance.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_service_manager.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

// send set-cfg command to the Read Endpoint Module to send history in ms.
void qsh_audio_data_uqci_send_pcm_cnv_config(qsh_audio_data_inst_state_t *inst_state_ptr)
{
   struct set_cfg_cmd_paylaod_t
   {
      asps_cmd_header_t                cmd_header;
      asps_module_param_data_t         param_header;
      param_id_pcm_output_format_cfg_t payload_header;
      payload_pcm_output_format_cfg_t  payload;
   } *msg_ptr = NULL;

   uint32_t payload_size = sizeof(struct set_cfg_cmd_paylaod_t);

   if (SNS_RC_SUCCESS == uqci_create_msg(inst_state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG, // opcode
                                         inst_state_ptr->usecase_info.pcm_cnv_miid,
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size         = payload_size - sizeof(asps_cmd_header_t);
      msg_ptr->param_header.param_id           = PARAM_ID_PCM_OUTPUT_FORMAT_CFG;
      msg_ptr->param_header.param_size         = sizeof(param_id_pcm_output_format_cfg_t) + sizeof(payload_pcm_output_format_cfg_t);
      msg_ptr->param_header.module_instance_id = inst_state_ptr->usecase_info.pcm_cnv_miid;

      msg_ptr->payload_header.fmt_id           = MEDIA_FMT_ID_PCM;
	  msg_ptr->payload_header.data_format      = DATA_FORMAT_FIXED_POINT;
      msg_ptr->payload.alignment               = PARAM_VAL_NATIVE;
      msg_ptr->payload.bit_width               = PARAM_VAL_NATIVE;
      msg_ptr->payload.bits_per_sample         = PARAM_VAL_NATIVE;
      msg_ptr->payload.endianness              = PCM_LITTLE_ENDIAN;
      msg_ptr->payload.interleaved             = PCM_INTERLEAVED;
      msg_ptr->payload.num_channels            = PARAM_VAL_NATIVE;
      msg_ptr->payload.q_factor                = PARAM_VAL_NATIVE;

      uqci_send_msg(inst_state_ptr->uqci_client_handle, msg_ptr);
   }

}

// send set-cfg command to the Read Endpoint Module to send history in ms.
void qsh_audio_data_uqci_send_data_config_extn(qsh_audio_data_inst_state_t *inst_state_ptr)
{
   struct set_cfg_cmd_paylaod_t
   {
      asps_cmd_header_t                cmd_header;
      asps_module_param_data_t         param_header;
      read_ep_qsocket_extn_cfg_t       payload;
   } *msg_ptr = NULL;

   uint32_t payload_size = sizeof(struct set_cfg_cmd_paylaod_t);

   if (SNS_RC_SUCCESS == uqci_create_msg(inst_state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG, // opcode
                                         IGNORE_RSP_TOKEN,
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size         = payload_size - sizeof(asps_cmd_header_t);
      msg_ptr->param_header.param_id           = PARAM_ID_QSOCKET_RD_EP_EXTN_CFG;
      msg_ptr->param_header.param_size         = sizeof(read_ep_qsocket_extn_cfg_t);
      msg_ptr->param_header.module_instance_id = inst_state_ptr->usecase_info.sns_rd_ep_miid;
	  msg_ptr->payload.version		           = 1;
	  msg_ptr->payload.batch_size_us           = inst_state_ptr->inst_data_config.batch_us;

      uqci_send_msg(inst_state_ptr->uqci_client_handle, msg_ptr);
   }
}


// send set-cfg command to the Read Endpoint Module to send history in ms.
void qsh_audio_data_uqci_send_data_config(qsh_audio_data_inst_state_t *inst_state_ptr)
{
   struct set_cfg_cmd_paylaod_t
   {
      asps_cmd_header_t                cmd_header;
      asps_module_param_data_t         param_header;
      read_ep_qsocket_cfg_t       payload;
   } *msg_ptr = NULL;

   uint32_t payload_size = sizeof(struct set_cfg_cmd_paylaod_t);

   if (SNS_RC_SUCCESS == uqci_create_msg(inst_state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG, // opcode
                                         inst_state_ptr->usecase_info.sns_rd_ep_miid,
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size         = payload_size - sizeof(asps_cmd_header_t);
      msg_ptr->param_header.param_id           = PARAM_ID_QSOCKET_RD_EP_CFG;
      msg_ptr->param_header.param_size         = sizeof(read_ep_qsocket_cfg_t);
      msg_ptr->param_header.module_instance_id = inst_state_ptr->usecase_info.sns_rd_ep_miid;
      msg_ptr->payload.client_id               = inst_state_ptr->client_id;
      msg_ptr->payload.history_buffer_size_us  = inst_state_ptr->inst_data_config.history_us;
      msg_ptr->payload.frame_size_us           = ADS_MAX_FRAME_SIZE_US;

      uqci_send_msg(inst_state_ptr->uqci_client_handle, msg_ptr);
   }

   qsh_audio_data_uqci_send_data_config_extn(inst_state_ptr);
}

// send set-cfg command to the Read Endpoint Module to enable / disable dataflow.
void qsh_audio_data_uqci_send_data_flow(qsh_audio_data_inst_state_t *inst_state_ptr)
{
   struct set_cfg_cmd_paylaod_t
   {
      asps_cmd_header_t           cmd_header;
      asps_module_param_data_t    param_header;
      read_ep_qsocket_data_flow_t payload;
   } *msg_ptr = NULL;

   uint32_t payload_size = sizeof(struct set_cfg_cmd_paylaod_t);

   if (SNS_RC_SUCCESS == uqci_create_msg(inst_state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG, // opcode
                                         inst_state_ptr->usecase_info.sns_rd_ep_miid,
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);

      msg_ptr->param_header.module_instance_id = inst_state_ptr->usecase_info.sns_rd_ep_miid;
      if (inst_state_ptr->flush_req)
      {
         msg_ptr->param_header.param_id   = PARAM_ID_QSOCKET_RD_EP_DRAIN_HISTORY_DATA;
         msg_ptr->param_header.param_size = 0;
      }
      else
      {
         msg_ptr->param_header.param_id   = PARAM_ID_QSOCKET_RD_EP_ENABLE_DATA_FLOW;
         msg_ptr->param_header.param_size = sizeof(read_ep_qsocket_data_flow_t);
         msg_ptr->payload.enable          = inst_state_ptr->data_flow_enable;
      }

      msg_ptr->cmd_header.payload_size = sizeof(asps_module_param_data_t) + msg_ptr->param_header.param_size;

      uqci_send_msg(inst_state_ptr->uqci_client_handle, msg_ptr);
   }
}

/* register for use-case info update event.
 * Sensor register for the Audio Data usecase update event with ASPS in the beginning.
 * Whenever read ep instance-ID is updated an event will be sent to this
 * sensor.
 */
static void qsh_audio_data_uqci_register_usecase_info_event(qsh_audio_data_inst_state_t *inst_state_ptr,
                                                            bool                         is_register)
{
   struct register_usecase_info_event_t
   {
      asps_cmd_header_t                 cmd_header;
      asps_cmd_register_module_events_t reg_payload;
   } *msg_ptr = NULL;

   uint32_t payload_size = sizeof(struct register_usecase_info_event_t);

   if (SNS_RC_SUCCESS == uqci_create_msg(inst_state_ptr->uqci_client_handle,
                                         ASPS_CMD_REGISTER_MODULE_EVENTS, // opcode
                                         (is_register) ? ASPS_MODULE_INSTANCE_ID : IGNORE_RSP_TOKEN,
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size = sizeof(asps_cmd_register_module_events_t);

      msg_ptr->reg_payload.event_id                  = EVENT_ID_ASPS_SENSOR_USECASE_INFO;
      msg_ptr->reg_payload.event_config_payload_size = 0;
      msg_ptr->reg_payload.is_register               = is_register ? 1 : 0;
      msg_ptr->reg_payload.module_instance_id        = ASPS_MODULE_INSTANCE_ID;

      uqci_send_msg(inst_state_ptr->uqci_client_handle, msg_ptr);
   }
}

// send command to asps to setup the usecase
static void qsh_audio_data_uqci_setup_usecase(sns_sensor_instance *const this, bool_t is_register)
{
   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;

   uint32_t usecase_payload_size = sizeof(asps_pcm_data_usecase_register_payload_t);
   uint32_t param_size           = sizeof(param_id_asps_sensor_usecase_register_t) + usecase_payload_size;
   uint32_t payload_size         = sizeof(asps_cmd_header_t) + sizeof(asps_module_param_data_t) + param_size;

   struct audio_data_usecase_setup_paylaod_t
   {
      asps_cmd_header_t                        cmd_header;
      asps_module_param_data_t                 param_header;
      param_id_asps_sensor_usecase_register_t  usecase_header;
      asps_pcm_data_usecase_register_payload_t usecase_payload;
   } *msg_ptr = NULL;

   if (SNS_RC_SUCCESS == uqci_create_msg(inst_state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG,
                                         (is_register) ? ASPS_MODULE_INSTANCE_ID : IGNORE_RSP_TOKEN,
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size = payload_size - sizeof(asps_cmd_header_t);

      msg_ptr->param_header.param_id           = PARAM_ID_ASPS_SENSOR_USECASE_REGISTER;
      msg_ptr->param_header.param_size         = param_size;
      msg_ptr->param_header.module_instance_id = ASPS_MODULE_INSTANCE_ID;

      msg_ptr->usecase_header.usecase_id   = ASPS_USECASE_ID_PCM_DATA;
      msg_ptr->usecase_header.is_register  = is_register;
      msg_ptr->usecase_header.payload_size = (is_register) ? sizeof(asps_pcm_data_usecase_register_payload_t) : 0;

      msg_ptr->usecase_payload.stream_type = (is_register) ? inst_state_ptr->inst_data_config.stream_type : 0;

      if(0 == is_register)
      {
         SNS_INST_PRINTF(LOW, this, "Sending ASPS Usecase Deregister");
         inst_state_ptr->usecase_info.sns_rd_ep_miid = 0;
         //todo: qsh_audio_event_unblock_power_collapse(this);
      }
      else
      {
         SNS_INST_PRINTF(LOW, this, "Sending ASPS Usecase Register");
         //todo: qsh_audio_event_block_power_collapse(this);
      }

      uqci_send_msg(inst_state_ptr->uqci_client_handle, msg_ptr);
   }
}

// send command to asps to setup the usecase
static void qsh_audio_data_uqci_setup_v2_usecase(sns_sensor_instance *const this, bool_t is_register)
{

   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;

   uint32_t ch_type_size         = sizeof(uint8_t) * inst_state_ptr->inst_data_config.num_channels;
   uint32_t usecase_payload_size = sizeof(asps_pcm_data_v2_usecase_register_payload_t) + ch_type_size;
   uint32_t param_size           = sizeof(param_id_asps_sensor_usecase_register_t) + usecase_payload_size;
   uint32_t payload_size         = sizeof(asps_cmd_header_t) + sizeof(asps_module_param_data_t) + param_size;

   SNS_INST_PRINTF(LOW,
                   this,
                   "Sending ASPS Usecase Register %lu %lu %lu",
                   usecase_payload_size,
                   inst_state_ptr->inst_data_config.num_channels,
                   sizeof(asps_pcm_data_usecase_register_payload_t));

   struct audio_data_usecase_setup_paylaod_t
   {
      asps_cmd_header_t                           cmd_header;
      asps_module_param_data_t                    param_header;
      param_id_asps_sensor_usecase_register_t     usecase_header;
      asps_pcm_data_v2_usecase_register_payload_t usecase_payload;
   } *msg_ptr = NULL;

   if (SNS_RC_SUCCESS == uqci_create_msg(inst_state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG,
                                         (is_register) ? ASPS_MODULE_INSTANCE_ID : IGNORE_RSP_TOKEN,
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size = payload_size - sizeof(asps_cmd_header_t);

      msg_ptr->param_header.param_id           = PARAM_ID_ASPS_SENSOR_USECASE_REGISTER;
      msg_ptr->param_header.param_size         = param_size;
      msg_ptr->param_header.module_instance_id = ASPS_MODULE_INSTANCE_ID;

      msg_ptr->usecase_header.usecase_id   = ASPS_USECASE_ID_PCM_DATA_V2;
      msg_ptr->usecase_header.is_register  = is_register;
      msg_ptr->usecase_header.payload_size = (is_register) ? usecase_payload_size : 0;
      msg_ptr->usecase_payload.stream_type = (is_register) ? inst_state_ptr->inst_data_config.stream_type : 0;

      if (is_register)
      {
         msg_ptr->usecase_payload.sample_rate  = inst_state_ptr->inst_data_config.sampling_rate;
         msg_ptr->usecase_payload.bit_width    = inst_state_ptr->inst_data_config.bit_width;
         msg_ptr->usecase_payload.num_channels = inst_state_ptr->inst_data_config.num_channels;

         msg_ptr->usecase_payload.requires_buffering = inst_state_ptr->inst_data_config.requires_buffering;

         uint8_t *ch_ptr = (uint8_t *)(msg_ptr + 1);
         sns_memscpy(ch_ptr,
        		 	 ch_type_size,
                     inst_state_ptr->inst_data_config.mapped_ch_type,
                     inst_state_ptr->inst_data_config.num_channels);

         SNS_INST_PRINTF(LOW, this, "Sending ASPS Usecase Register");
         // todo: qsh_audio_event_block_power_collapse(this);
      }
      else
      {
         SNS_INST_PRINTF(LOW, this, "Sending ASPS Usecase Deregister");
         inst_state_ptr->usecase_info.sns_rd_ep_miid = 0;
         // todo: qsh_audio_event_unblock_power_collapse(this);
      }

      uqci_send_msg(inst_state_ptr->uqci_client_handle, msg_ptr);
   }
}

static void qsh_audio_data_uqci_register_data_config(sns_sensor_instance *const this, bool_t force_send, bool_t ads_enable)
{
   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;

   // if ads_enable is zero then send de-registration command.
   uint32_t is_register = (ads_enable == 0) ? 0 : 1;

   // if de-registering then send the command to miid
   // this to send the event deregistration command to miid for upd usecase.
   force_send = (0 == is_register) ? true : force_send;

   // if force send is false then first try to setup the usecase for all the requested.
   if (!force_send)
   {
      if (0 == inst_state_ptr->usecase_info.sns_rd_ep_miid)
      {
         // usecase is not setup.
         // setup usecase now and return, event registration will be done when usecase is setup.
    	  SNS_INST_PRINTF(LOW, this, "Setting up Usecase with stream type = %lu", inst_state_ptr->inst_data_config.stream_type);

         if(!inst_state_ptr->inst_data_config.has_media_fmt)
         {
        	 qsh_audio_data_uqci_setup_usecase(this, ads_enable);
         }
         else
         {
        	 qsh_audio_data_uqci_setup_v2_usecase(this, ads_enable);
         }

         return;
      }
   }
   else
   {
      // at this point usecase is set up
   }

   if (inst_state_ptr->usecase_info.pcm_cnv_miid && ads_enable)
   {
      SNS_INST_PRINTF(LOW, this, "Sending PCM CNV Output Config");
      qsh_audio_data_uqci_send_pcm_cnv_config(inst_state_ptr);
   }

   if (inst_state_ptr->usecase_info.sns_rd_ep_miid && ads_enable)
   {
      SNS_INST_PRINTF(LOW, this, "Sending Data Config history in us = %lu", inst_state_ptr->inst_data_config.history_us);
      qsh_audio_data_uqci_send_data_config(inst_state_ptr);
	  SNS_INST_PRINTF(LOW, this, "Sending Data Config Extn batch in us = %lu", inst_state_ptr->inst_data_config.batch_us);
   }

   // if disabled then de-register for usecase.
   // this should be done after sending deregistration to the miid.
   if (0 == ads_enable)
   {
      SNS_INST_PRINTF(LOW, this, "Unsetting Usecase");
      if(!inst_state_ptr->inst_data_config.has_media_fmt)
      {
     	 qsh_audio_data_uqci_setup_usecase(this, ads_enable);
      }
      else
      {
     	 qsh_audio_data_uqci_setup_v2_usecase(this, ads_enable);
      }
   }
}

// receives the usecase info event. it updates the miid .
void qsh_audio_data_receive_usecase_info(sns_sensor_instance *const this,
                                         asps_audio_data_usecase_register_ack_payload_t *usecase_info_ptr)
{
   SNS_INST_PRINTF(LOW, this, "ASPS_USECASE_ID_PCM_DATA Received");
   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;
   // only one module id instance specific
   inst_state_ptr->usecase_info.sns_rd_ep_miid = usecase_info_ptr->sns_rd_ep_miid;
   inst_state_ptr->usecase_info.mfc_miid       = usecase_info_ptr->mfc_miid;
   inst_state_ptr->usecase_info.pcm_cnv_miid   = usecase_info_ptr->pcm_cnv_miid;

   SNS_INST_PRINTF(LOW, this, "Sending Read EP Data Config");
   qsh_audio_data_uqci_register_data_config(this, true, true);
}

/* See sns_sensor_instance_api::init */
sns_rc qsh_audio_data_inst_init(sns_sensor_instance *this, sns_sensor_state const *state_ptr)
{
   sns_rc                         rc               = SNS_RC_SUCCESS;
   qsh_audio_data_inst_state_t *  inst_state_ptr   = (qsh_audio_data_inst_state_t *)this->state->state;
   qsh_audio_data_sensor_state_t *sensor_state_ptr = (qsh_audio_data_sensor_state_t *)state_ptr->state;
   sns_service_manager *          service_mgr_ptr  = this->cb->get_service_manager(this);

   inst_state_ptr->event_service =
      (sns_event_service *)service_mgr_ptr->get_service(service_mgr_ptr, SNS_EVENT_SERVICE);

   inst_state_ptr->suid_ptr = sensor_state_ptr->audio_event_state.suid_ptr;

   if(NULL == inst_state_ptr->uqci_client_handle)
   {
      rc = uqci_register_client(sensor_state_ptr->audio_event_state.uqci_svc_handle_ptr,
                                qsh_audio_data_uqci_receive_msg,
                                sensor_state_ptr->audio_event_state.qsh_invoke_handle_ptr,
                                &inst_state_ptr->uqci_client_handle);
      if (SNS_RC_SUCCESS == rc)
      {
         // get client id and store in inst_state_ptr
         rc = uqci_get_client_id(inst_state_ptr->uqci_client_handle, &inst_state_ptr->client_id);
      }

      if (SNS_RC_SUCCESS == rc)
      {
         qsh_audio_data_uqci_register_usecase_info_event(inst_state_ptr, true);
      }

   }

   sns_memzero(&inst_state_ptr->inst_data_config, sizeof(inst_state_ptr->inst_data_config));

   inst_state_ptr->inst_data_config.sampling_rate = ADS_SAMPLING_RATE;
   inst_state_ptr->inst_data_config.bit_width = ADS_DEFAULT_BIT_WIDTH;
   inst_state_ptr->inst_data_config.num_channels = ADS_DEFAULT_NUM_CHANNELS;
   inst_state_ptr->inst_data_config.requires_buffering = true;
   SNS_INST_PRINTF(MED, this, "qsh_audio_data_inst_init");
   return rc;
}

/* See sns_sensor_instance_api::deinit */
sns_rc qsh_audio_data_inst_deinit(sns_sensor_instance *const this)
{
   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;

   // dereg here
   qsh_audio_data_uqci_register_data_config(this, false, false);
   qsh_audio_data_uqci_register_usecase_info_event(inst_state_ptr, false);

   uqci_deregister_client(inst_state_ptr->uqci_client_handle);
   inst_state_ptr->uqci_client_handle = NULL;

   SNS_INST_PRINTF(MED, this, "qsh_audio_data_inst_deinit");

   return SNS_RC_SUCCESS;
}

static void qsh_audio_data_fill_ep_mf_channel_type(sns_sensor_instance *const this,
                                                   uint32_t *src_ch_ptr,
                                                   uint8_t   src_ch_len)
{
   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;

   if (inst_state_ptr->inst_data_config.num_channels != src_ch_len)
   {
      SNS_INST_PRINTF(LOW,
                      this,
                      "Warning! requested ch num %lu and num types %lu do no match!",
                      inst_state_ptr->inst_data_config.num_channels,
                      src_ch_len);
   }

   if(ADS_MAX_CHANNELS < inst_state_ptr->inst_data_config.num_channels)
   {
	      SNS_INST_PRINTF(LOW,
	                      this,
	                      "Warning! Too many num_channels %lu so ch_type not supported",
						  inst_state_ptr->inst_data_config.num_channels);
	      return;
   }

   uint8_t * dest_ch_ptr = inst_state_ptr->inst_data_config.mapped_ch_type;

   for (int i = 0; i < inst_state_ptr->inst_data_config.num_channels; i++)
   {
      switch (src_ch_ptr[i])
      {
         case QSH_AUDIO_DATA_CHANNEL_L:
         {
            dest_ch_ptr[i] = PCM_CHANNEL_L;
            break;
         }
         case QSH_AUDIO_DATA_CHANNEL_R:
         {
            dest_ch_ptr[i] = PCM_CHANNEL_R;
            break;
         }
         default:
         {
        	 SNS_INST_PRINTF(LOW,
        	         		 this,
        	                 "Unsupported channel type set from client %lu!.", src_ch_ptr[i]);
            break;
         }

         SNS_INST_PRINTF(LOW,
        		 	 	 this,
                         "request for media format channel %lu mapped to channel %lu",
                         src_ch_ptr[i],
                         dest_ch_ptr[i]);
      }
   }
}

/* See sns_sensor_instance_api::set_client_config */
sns_rc qsh_audio_data_inst_set_client_config(sns_sensor_instance *const this, sns_request const *client_request_ptr)
{
   sns_rc                       rv             = SNS_RC_FAILED;
   qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)this->state->state;

   switch (client_request_ptr->message_id)
   {
      case SNS_STD_MSGID_SNS_STD_FLUSH_REQ:
      {
         if (inst_state_ptr->flush_req)
         {
            SNS_INST_PRINTF(MED, this, "flush is already active. ignoring new flush request.");
            return SNS_RC_SUCCESS;
         }
         else if (inst_state_ptr->data_flow_enable)
         {
            SNS_INST_PRINTF(MED, this, "streaming is already active, sending flush event now.");
            sns_sensor_util_send_flush_event(inst_state_ptr->suid_ptr, this);
            return SNS_RC_SUCCESS;
         }
         else if ((0 == inst_state_ptr->inst_data_config.history_us) ||
        		 (!inst_state_ptr->inst_data_config.requires_buffering))
         {
            SNS_INST_PRINTF(MED, this, "flush-period is zero. sending flush event now.");
            sns_sensor_util_send_flush_event(inst_state_ptr->suid_ptr, this);
         }
         else
         {
            /* enable data flow now.
             * flush event will be sent when EOF is received */
            inst_state_ptr->flush_req        = true;
            inst_state_ptr->data_flow_enable = true;

            SNS_INST_PRINTF(MED, this, "received flush request.");

            /* enable data flow */
            qsh_audio_data_uqci_send_data_flow(inst_state_ptr);
         }

         return SNS_RC_SUCCESS;
      }
      case QSH_AUDIO_DATA_MSGID_QSH_AUDIO_DATA_CONFIG:
      {
         const uint32_t INVALID     = 0xFFFFFFFF;
         uint32_t       history_us  = INVALID, batch_us = INVALID;
         uint32_t       stream_type = INVALID;
         bool_t requires_buffering = true, has_ep_mf_info = false;
         uint32_t sampling_rate = INVALID, bit_width = INVALID, num_channels = INVALID;

         uint8_t decoded_ch_index = 0;
         uint32_t decoded_ch_type[ADS_MAX_CHANNELS] = {0};
         pb_uint_32_arr_arg  ch_type_arg = (pb_uint_32_arr_arg) {.arr = decoded_ch_type, .arr_len = ARR_SIZE(decoded_ch_type), .arr_index = &decoded_ch_index};

         pb_istream_t    stream;
         sns_std_request std_request = sns_std_request_init_default;
         stream = pb_istream_from_buffer(client_request_ptr->request, client_request_ptr->request_len);

         // Decode sns std request for history/batch/buffering info
         if (pb_decode(&stream, sns_std_request_fields, &std_request))
         {
            if (std_request.has_batching)
            {
               if (std_request.batching.has_flush_only)
               {
                  /* if flush_only == TRUE
                   * 	then disable data flow
                   * if flush_only == FALSE
                   * 	then enable data flow
                   */
                  bool_t data_flow = (std_request.batching.flush_only) ? false : true;

                  if (data_flow != inst_state_ptr->data_flow_enable)
                  {
                     SNS_INST_PRINTF(LOW, this, "received data flow %d ", data_flow);

                     inst_state_ptr->data_flow_enable = data_flow;
                     if (0 != inst_state_ptr->usecase_info.sns_rd_ep_miid) // use case is already established.
                     {
                        qsh_audio_data_uqci_send_data_flow(inst_state_ptr);

                        return SNS_RC_SUCCESS;
                     }
                     else
                     {
                        inst_state_ptr->data_flow_pending = true;
                     }
                  }
               }

               if (std_request.batching.has_flush_period)
               {
                  history_us = std_request.batching.flush_period;
               }

               batch_us = std_request.batching.batch_period;
            }
         }
         else
         {
        	 return SNS_RC_FAILED;
         }

         //if std request had batch period is unset or set specifically to zero then the client does not require buffering
         if ( ((0 == history_us) && (0 == batch_us)) || ((INVALID == history_us) && (INVALID == batch_us)))
         {
         	requires_buffering = false;
         }

         /* If batch us is not set as a part of std cfg / if batch us is set as invalid by client we
          * use the default batching size available. */
         batch_us = (batch_us == INVALID)? ADS_MAX_FRAME_SIZE_US : batch_us;
         history_us = (history_us == INVALID)? 0 : history_us;

         /* Only batch periods that are multiples of frame size are currently supported */
         batch_us = SNS_MAX((batch_us - (batch_us % ((uint32_t) ADS_MAX_FRAME_SIZE_US))),ADS_MAX_FRAME_SIZE_US);

         /* flush_period or history should be greater than or equal to batch_period as mentioned
          * in sns_std.proto file */
         history_us = SNS_MAX(batch_us, history_us);

         // Decode qsh_audio_data_config request for stream type / ep mf info info
         qsh_audio_data_config request = qsh_audio_data_config_init_default;
         request.hwep_mf_cfg.channel_type.funcs.decode = &pb_decode_uint_32_arr_cb;
         request.hwep_mf_cfg.channel_type.arg = &ch_type_arg;

         if (pb_decode_request(client_request_ptr, qsh_audio_data_config_fields, NULL, &request))
         {
            stream_type = request.stream_type;

            if (request.has_hwep_mf_cfg)
            {
               has_ep_mf_info = true;
               sampling_rate  = request.hwep_mf_cfg.has_sampling_rate ? request.hwep_mf_cfg.sampling_rate : sampling_rate;
               bit_width      = request.hwep_mf_cfg.has_bit_width ? request.hwep_mf_cfg.bit_width : bit_width;
               num_channels   = request.hwep_mf_cfg.has_num_channels ? request.hwep_mf_cfg.num_channels : num_channels;
            }
         }
         else
         {
        	 return SNS_RC_FAILED;
         }


         if (INVALID != history_us && INVALID != stream_type && INVALID != batch_us)
         {
            SNS_INST_PRINTF(MED, this, "request for history in us %lu stream type %d batch_us %lu.", history_us, stream_type, batch_us);

            if (0 != inst_state_ptr->usecase_info.sns_rd_ep_miid) //use case is already setup
            {
               if (inst_state_ptr->inst_data_config.stream_type == stream_type &&
                   inst_state_ptr->inst_data_config.history_us == history_us &&
                   inst_state_ptr->inst_data_config.batch_us == batch_us &&
                   ((!has_ep_mf_info) ||
                    ((has_ep_mf_info) && ((inst_state_ptr->inst_data_config.sampling_rate == sampling_rate) &&
                                          (inst_state_ptr->inst_data_config.bit_width == bit_width) &&
                                          (inst_state_ptr->inst_data_config.num_channels == num_channels)))))
               {
                  // if same config is received then don't need to return error
                  return SNS_RC_SUCCESS;
               }
               else
               {
                  SNS_INST_PRINTF(ERROR,
                                  this,
                                  "request for history in ms %lu stream type %d rejected.",
                                  history_us,
                                  stream_type);

                  // if use case is already enabled then reject the history and stream type configuration change.
                  return SNS_RC_FAILED;
               }
            }

            inst_state_ptr->inst_data_config.stream_type = stream_type;
            inst_state_ptr->inst_data_config.history_us  = history_us;
            inst_state_ptr->inst_data_config.batch_us    = batch_us;

            /* Indicates if MF was sent via request */
            if(has_ep_mf_info)
            {
               inst_state_ptr->inst_data_config.has_media_fmt = has_ep_mf_info;

               inst_state_ptr->inst_data_config.sampling_rate =
                  (INVALID == sampling_rate) ? ADS_SAMPLING_RATE : sampling_rate;
               inst_state_ptr->inst_data_config.bit_width = (INVALID == bit_width) ? ADS_DEFAULT_BIT_WIDTH : bit_width;
               inst_state_ptr->inst_data_config.num_channels =
                  ((INVALID == num_channels)) ? ADS_DEFAULT_NUM_CHANNELS : num_channels;
               inst_state_ptr->inst_data_config.requires_buffering = requires_buffering;

               SNS_INST_PRINTF(LOW,
                               this,
                               "request for media format %lu %lu %lu buffering %lu ch_index %lu",
                               inst_state_ptr->inst_data_config.sampling_rate,
                               inst_state_ptr->inst_data_config.bit_width,
                               inst_state_ptr->inst_data_config.num_channels,
                               inst_state_ptr->inst_data_config.requires_buffering,
							   decoded_ch_index);

               qsh_audio_data_fill_ep_mf_channel_type(this, decoded_ch_type, decoded_ch_index);

            }


            qsh_audio_data_uqci_register_data_config(this, false, true);

            return SNS_RC_SUCCESS;
         }
         else
         {
            return SNS_RC_FAILED; // invalid configuration
         }

         break;
      }
      default:
         rv = SNS_RC_NOT_SUPPORTED;
         SNS_INST_PRINTF(ERROR, this, "unsupported config %d", client_request_ptr->message_id);
         break;
   }
   return rv;
}
