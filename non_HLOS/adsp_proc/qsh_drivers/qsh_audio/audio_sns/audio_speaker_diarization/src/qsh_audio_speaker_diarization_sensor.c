/*=============================================================================
  @file qsh_audio_speaker_diarization_sensor.c

  Speaker Diarization implementation

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
/* register for use-case info update event.
 * Sensor register for the SDZ usecase update event with ASPS in the beginning.
 * Whenever SDZ module instance-ID is updated an event will be sent to this sensor.
 */
void qsh_audio_speaker_diarization_uqci_register_usecase_info_event(
   qsh_audio_speaker_diarization_sensor_state *sensor_state_ptr,
   bool                                        is_register)
{
   struct register_usecase_info_event_t
   {
      asps_cmd_header_t                 cmd_header;
      asps_cmd_register_module_events_t reg_payload;
   } *msg_ptr = NULL;

   uint32_t payload_size = sizeof(struct register_usecase_info_event_t);

   if (SNS_RC_SUCCESS == uqci_create_msg(sensor_state_ptr->uqci_client_handle,
                                         ASPS_CMD_REGISTER_MODULE_EVENTS,                            // opcode
                                         (is_register) ? ASPS_MODULE_INSTANCE_ID : IGNORE_RSP_TOKEN, // token
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size = sizeof(asps_cmd_register_module_events_t);

      msg_ptr->reg_payload.event_id                  = EVENT_ID_ASPS_SENSOR_USECASE_INFO;
      msg_ptr->reg_payload.event_config_payload_size = 0;
      msg_ptr->reg_payload.is_register               = is_register ? 1 : 0;
      msg_ptr->reg_payload.module_instance_id        = ASPS_MODULE_INSTANCE_ID;

      uqci_send_msg(sensor_state_ptr->uqci_client_handle, msg_ptr);
   }
}

// send command to asps/acm to setup or teardown the usecase/graph of SDZ
static void qsh_audio_speaker_diarization_setup_sdz_graph(sns_sensor *const this, bool is_setup)
{
   qsh_audio_speaker_diarization_sensor_state *state_ptr =
      (qsh_audio_speaker_diarization_sensor_state *)this->state->state;
   uint32_t param_size   = sizeof(param_id_asps_sensor_usecase_register_t);
   uint32_t payload_size = sizeof(asps_cmd_header_t) + sizeof(asps_module_param_data_t) + param_size;

   struct sdz_usecase_setup_paylaod_t
   {
      asps_cmd_header_t                       cmd_header;
      asps_module_param_data_t                param_header;
      param_id_asps_sensor_usecase_register_t usecase_header;
   } *msg_ptr = NULL;

   if (SNS_RC_SUCCESS == uqci_create_msg(state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG,                                        // opcode
                                         (is_setup) ? ASPS_MODULE_INSTANCE_ID : IGNORE_RSP_TOKEN, // token
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size = payload_size - sizeof(asps_cmd_header_t);

      msg_ptr->param_header.param_id           = PARAM_ID_ASPS_SENSOR_USECASE_REGISTER;
      msg_ptr->param_header.param_size         = param_size;
      msg_ptr->param_header.module_instance_id = ASPS_MODULE_INSTANCE_ID;

      msg_ptr->usecase_header.usecase_id   = ASPS_USECASE_ID_SDZ;
      msg_ptr->usecase_header.is_register  = is_setup;
      msg_ptr->usecase_header.payload_size = 0;

      uqci_send_msg(state_ptr->uqci_client_handle, msg_ptr);

      if (0 == msg_ptr->usecase_header.is_register)
      {
         state_ptr->module_iid = 0;
         SNS_PRINTF(HIGH, this, "sending sdz graph destroy command.");
      }
      else
      {
         SNS_PRINTF(HIGH, this, "sending sdz graph setup command.");
      }
   }
}

// send event registration to the sdz modules in ADSP.
static void qsh_audio_speaker_diarization_register_sdz_event(sns_sensor *const this, bool is_register)
{
   qsh_audio_speaker_diarization_sensor_state *state_ptr =
      (qsh_audio_speaker_diarization_sensor_state *)this->state->state;

   // send registration or deregistration command to sdz miid
   uint32_t module_iid = state_ptr->module_iid;

   uint32_t reg_payload_size = (is_register) ? sizeof(event_id_sdz_output_reg_cfg_t) : 0;

   uint32_t asps_cmd_payload_size = sizeof(asps_cmd_register_module_events_t) + reg_payload_size;
   uint32_t payload_size          = sizeof(asps_cmd_header_t) + asps_cmd_payload_size;

   struct sdz_event_register_paylaod_t
   {
      asps_cmd_header_t                 cmd_header;
      asps_cmd_register_module_events_t event_reg_header;
      event_id_sdz_output_reg_cfg_t     reg_payload;
   } *msg_ptr = NULL;

   if (SNS_RC_SUCCESS == uqci_create_msg(state_ptr->uqci_client_handle,
                                         ASPS_CMD_REGISTER_MODULE_EVENTS,
                                         module_iid,
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, sizeof(asps_cmd_header_t));
      msg_ptr->cmd_header.payload_size = asps_cmd_payload_size;

      msg_ptr->event_reg_header.module_instance_id        = module_iid;
      msg_ptr->event_reg_header.event_id                  = EVENT_ID_SDZ_OUTPUT;
      msg_ptr->event_reg_header.is_register               = is_register;
      msg_ptr->event_reg_header.event_config_payload_size = reg_payload_size;

      if (is_register)
      {
         msg_ptr->reg_payload.event_payload_type = 1; // inband non-buffered
         SNS_PRINTF(HIGH, this, "sending sdz event registration command.");
      }
      else
      {
         SNS_PRINTF(HIGH, this, "sending sdz event deregistration command.");
      }

      uqci_send_msg(state_ptr->uqci_client_handle, msg_ptr);
   }
}

// send enable/disable command to the sdz modules in ADSP.
static void qsh_audio_speaker_diarization_send_sdz_enable_disable(sns_sensor *const this)
{
   qsh_audio_speaker_diarization_sensor_state *state_ptr =
      (qsh_audio_speaker_diarization_sensor_state *)this->state->state;

   struct set_cfg_cmd_payload_t
   {
      asps_cmd_header_t        cmd_header;
      asps_module_param_data_t param_header;
      param_id_module_enable_t payload;
   } *msg_ptr = NULL;

   uint32_t payload_size = sizeof(struct set_cfg_cmd_payload_t);

   if (SNS_RC_SUCCESS == uqci_create_msg(state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG, // opcode
                                         IGNORE_RSP_TOKEN,
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size         = payload_size - sizeof(asps_cmd_header_t);
      msg_ptr->param_header.param_id           = PARAM_ID_MODULE_ENABLE;
      msg_ptr->param_header.param_size         = sizeof(param_id_module_enable_t);
      msg_ptr->param_header.module_instance_id = state_ptr->module_iid;
      msg_ptr->payload.enable                  = state_ptr->sdz_enable ? 1 : 0;

      SNS_PRINTF(HIGH, this, "Sending enable %d ", msg_ptr->payload.enable);

      uqci_send_msg(state_ptr->uqci_client_handle, msg_ptr);
   }
}

// function to update enable/disable sdz.
static sns_rc qsh_audio_speaker_diarization_update_send_sdz_enable_disable(sns_sensor *const this, bool force_send)
{
   qsh_audio_speaker_diarization_sensor_state *state_ptr =
      (qsh_audio_speaker_diarization_sensor_state *)this->state->state;

   uint32_t sdz_enable = 0;

   // loop through all the sensor instances config and aggregate
   for (sns_sensor_instance *inst_ptr = this->cb->get_sensor_instance(this, true); NULL != inst_ptr;
        inst_ptr                      = this->cb->get_sensor_instance(this, false))
   {
      qsh_audio_speaker_diarization_inst_state *inst_state_ptr =
         (qsh_audio_speaker_diarization_inst_state *)inst_ptr->state->state;
      sdz_enable |= inst_state_ptr->sdz_enable;
   }

   // 1. if enable to disable or disable to enable change is happening then update.
   // 2. if new graph setup and new miid is received then force send.
   if (sdz_enable != state_ptr->sdz_enable || force_send)
   {
      state_ptr->sdz_enable = sdz_enable;
      qsh_audio_speaker_diarization_send_sdz_enable_disable(this);
   }

   return SNS_RC_SUCCESS;
}

// receives the usecase info event. it updates the sdz miid
// and then send the sdz event registration.
void qsh_audio_speaker_diarization_receive_usecase_info(sns_sensor *const this,
                                                        asps_sdz_usecase_register_ack_payload_t *usecase_info_ptr)
{
   qsh_audio_speaker_diarization_sensor_state *state_ptr =
      (qsh_audio_speaker_diarization_sensor_state *)this->state->state;

   state_ptr->module_iid = usecase_info_ptr->module_instance_id;

   // send sdz event registration command
   qsh_audio_speaker_diarization_register_sdz_event(this, true);

   // force send enable/disable command as new module IID is received
   qsh_audio_speaker_diarization_update_send_sdz_enable_disable(this, true);
}

/**
 * Initialize attributes to their default state.
 */
static void qsh_audio_speaker_diarization_publish_attributes(sns_sensor *const this)
{
   {
      char const              name[] = "Audio Speaker Diarization";
      sns_std_attr_value_data value  = sns_std_attr_value_data_init_default;
      value.str.funcs.encode         = pb_encode_string_cb;
      value.str.arg                  = &((pb_buffer_arg){ .buf = name, .buf_len = sizeof(name) });
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
   }
   {
      char const              type[] = "audio_speaker_diarization";
      sns_std_attr_value_data value  = sns_std_attr_value_data_init_default;
      value.str.funcs.encode         = pb_encode_string_cb;
      value.str.arg                  = &((pb_buffer_arg){ .buf = type, .buf_len = sizeof(type) });
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_TYPE, &value, 1, false);
   }
   {
      char const              vendor[] = "QTI";
      sns_std_attr_value_data value    = sns_std_attr_value_data_init_default;
      value.str.funcs.encode           = pb_encode_string_cb;
      value.str.arg                    = &((pb_buffer_arg){ .buf = vendor, .buf_len = sizeof(vendor) });
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VENDOR, &value, 1, false);
   }
   {
      char const              proto_files[] = "qsh_audio_speaker_diarization.proto";
      sns_std_attr_value_data value         = sns_std_attr_value_data_init_default;
      value.str.funcs.encode                = pb_encode_string_cb;
      value.str.arg                         = &((pb_buffer_arg){ .buf = proto_files, .buf_len = sizeof(proto_files) });
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_API, &value, 1, false);
   }
   {
      sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
      value.has_sint                = true;
      value.sint                    = 1;
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VERSION, &value, 1, false);
   }
   {
      sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
      value.has_sint                = true;
      value.sint                    = SNS_STD_SENSOR_STREAM_TYPE_ON_CHANGE;
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_STREAM_TYPE, &value, 1, true);
   }
}

/* See sns_sensor::init */
sns_rc qsh_audio_speaker_diarization_init(sns_sensor *const this)
{
   qsh_audio_speaker_diarization_sensor_state *state_ptr =
      (qsh_audio_speaker_diarization_sensor_state *)this->state->state;

   memset(state_ptr, 0, sizeof(qsh_audio_speaker_diarization_sensor_state));

   // base class init
   char   sns_name[] = "qsh_audio_speaker_diarization";
   sns_rc rc         = qsh_audio_event_init(this, sns_name);

   // publish attribute except "available"
   qsh_audio_speaker_diarization_publish_attributes(this);

   return rc;
}

/* See sns_sensor::deinit */
sns_rc qsh_audio_speaker_diarization_deinit(sns_sensor *const this)
{
   qsh_audio_speaker_diarization_sensor_state *state_ptr =
      (qsh_audio_speaker_diarization_sensor_state *)this->state->state;

   // uqci client deregistration
   if (state_ptr->uqci_client_handle)
   {
      uqci_deregister_client(state_ptr->uqci_client_handle);
      state_ptr->uqci_client_handle = NULL;
   }

   // base class deinit
   return qsh_audio_event_deinit(this);
}

/* See sns_sensor::notify_event */
sns_rc qsh_audio_speaker_diarization_notify_event(sns_sensor *const this)
{
   return qsh_audio_event_notify_event(this);
}

static sns_rc qsh_audio_speaker_diarization_aggregate_req_send(sns_sensor *const this)
{
   qsh_audio_speaker_diarization_sensor_state *state_ptr =
      (qsh_audio_speaker_diarization_sensor_state *)this->state->state;
   sns_sensor_instance *inst_ptr = this->cb->get_sensor_instance(this, true);

   // if SDZ graph is not setup and there is at least one client (instance) then request ASPS/ACM to setup the graph.
   if (state_ptr->module_iid == 0 && inst_ptr)
   {
      // block power collapse if enabled use case.
      qsh_audio_event_block_power_collapse(this);

      // setup the sdz graph, event will be registered when module iid is received.
      qsh_audio_speaker_diarization_setup_sdz_graph(this, true);
   }
   // if use case is already setup then check and send enable/disable to the sdz.
   else if (inst_ptr)
   {
      // force send is false because graph is already setup.
      return qsh_audio_speaker_diarization_update_send_sdz_enable_disable(this, false);
   }
   // if there is no client (instance) and graph is setup then request ASPS/ACM to tear down the graph.
   else if (state_ptr->module_iid != 0)
   {
      // unblock power collapse if destroying the use case.
      qsh_audio_event_unblock_power_collapse(this);

      // unregister the sdz event and request ASPS/ACM to tear down the graph.
      qsh_audio_speaker_diarization_register_sdz_event(this, false);
      qsh_audio_speaker_diarization_setup_sdz_graph(this, false);
   }

   return SNS_RC_SUCCESS;
}

/* See sns_sensor::set_client_request */
sns_sensor_instance *qsh_audio_speaker_diarization_set_client_request(sns_sensor *const this,
                                                                      sns_request const *curr_req,
                                                                      sns_request const *new_req,
                                                                      bool               remove)
{

   qsh_audio_speaker_diarization_sensor_state *state_ptr =
      (qsh_audio_speaker_diarization_sensor_state *)this->state->state;
   sns_sensor_instance *inst_ptr = qsh_audio_event_set_client_request(this,
                                                                      curr_req,
                                                                      new_req,
                                                                      remove,
                                                                      sizeof(qsh_audio_speaker_diarization_inst_state));

   // if instance is removed then send the re-aggregated request
   if (remove)
   {
      qsh_audio_speaker_diarization_aggregate_req_send(this);
   }
   else if (new_req)
   {
      switch (new_req->message_id)
      {
         case SNS_STD_MSGID_SNS_STD_FLUSH_REQ:
         {
            sns_sensor_util_send_flush_event(state_ptr->audio_event_state.suid_ptr, inst_ptr);
            break;
         }
         case SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG:
         case QSH_AUDIO_SPEAKER_DIARIZATION_MSGID_QSH_AUDIO_SPEAKER_DIARIZATION_ENABLE:
         {
            qsh_audio_speaker_diarization_aggregate_req_send(this);
         }
         break;
         default:
            break;
      }
   }

   return inst_ptr;
}
