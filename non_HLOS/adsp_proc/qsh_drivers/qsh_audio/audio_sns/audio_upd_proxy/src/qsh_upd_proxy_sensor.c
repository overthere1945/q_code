/*=============================================================================
  @file qsh_upd_proxy_sensor.c

  The Upd Proxy Sensor implementation

  Copyright (c) 2021-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
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
/* register for use-case info update event.
 * Sensor register for the UPD usecase update event with ASPS in the beginning.
 * Whenever UPD module instance-ID is updated an event will be sent to this sensor.
 */
void qsh_upd_proxy_uqci_register_usecase_info_event(qsh_upd_proxy_sensor_state *sensor_state_ptr, bool is_register)
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

// send command to asps to setup the usecase of requested context ids.
static void qsh_upd_proxy_uqci_setup_usecase(sns_sensor *const this)
{
   qsh_upd_proxy_sensor_state *state_ptr    = (qsh_upd_proxy_sensor_state *)this->state->state;
   uint32_t                    param_size   = sizeof(param_id_asps_sensor_usecase_register_t);
   uint32_t                    payload_size = sizeof(asps_cmd_header_t) + sizeof(asps_module_param_data_t) + param_size;
   uint32_t                    is_register  = (state_ptr->upd_enable) ? 1 : 0;

   struct upd_usecase_setup_paylaod_t
   {
      asps_cmd_header_t                       cmd_header;
      asps_module_param_data_t                param_header;
      param_id_asps_sensor_usecase_register_t usecase_header;
   } *msg_ptr = NULL;

   if (SNS_RC_SUCCESS == uqci_create_msg(state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG,                                           // opcode
                                         (is_register) ? ASPS_MODULE_INSTANCE_ID : IGNORE_RSP_TOKEN, // token
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size = payload_size - sizeof(asps_cmd_header_t);

      msg_ptr->param_header.param_id           = PARAM_ID_ASPS_SENSOR_USECASE_REGISTER;
      msg_ptr->param_header.param_size         = param_size;
      msg_ptr->param_header.module_instance_id = ASPS_MODULE_INSTANCE_ID;

      msg_ptr->usecase_header.usecase_id   = ASPS_USECASE_ID_UPD;
      msg_ptr->usecase_header.is_register  = (state_ptr->upd_enable) ? 1 : 0;
      msg_ptr->usecase_header.payload_size = 0;

      if (msg_ptr->usecase_header.is_register)
      {
         // block power collapse if enabling events.
         qsh_audio_event_block_power_collapse(this);
      }
      else
      {
         // unblock power collapse if disabling events.
         qsh_audio_event_unblock_power_collapse(this);
      }

      uqci_send_msg(state_ptr->uqci_client_handle, msg_ptr);

      if (0 == msg_ptr->usecase_header.is_register)
      {
         state_ptr->module_iid = 0;
      }
   }
}

// send context event registration to the upd modules in ADSP.
static void qsh_upd_proxy_uqci_register_upd_event(sns_sensor *const this, bool_t force_send)
{
   qsh_upd_proxy_sensor_state *state_ptr  = (qsh_upd_proxy_sensor_state *)this->state->state;
   uint32_t                    upd_enable = state_ptr->upd_enable;

   // if upd_enable is zero then send de-registration command.
   uint32_t is_register = (upd_enable == 0) ? 0 : 1;

   // if de-registering then send the command to miid
   // this to send the event deregistration command to miid for upd usecase.
   force_send = (0 == is_register) ? true : force_send;

   // if force send is false then first try to setup the usecase for all the requested.
   if (!force_send)
   {
      if (0 == state_ptr->module_iid)
      {
         // usecase is not setup.
         // setup usecase now and return, event registration will be done when usecase is setup.
         qsh_upd_proxy_uqci_setup_usecase(this);
         return;
      }
   }
   else
   {
      // at this point, upd usecsae is setup, so send the event registration to each module.
   }
   // send registration request for upd module instance

   // send registration or deregistration command miid
   if (state_ptr->module_iid)
   {
      uint32_t module_iid = state_ptr->module_iid;

      uint32_t asps_cmd_payload_size = sizeof(asps_cmd_register_module_events_t);
      uint32_t payload_size          = sizeof(asps_cmd_header_t) + asps_cmd_payload_size;

      struct upd_event_register_paylaod_t
      {
         asps_cmd_header_t                 cmd_header;
         asps_cmd_register_module_events_t event_reg_header;
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
         msg_ptr->event_reg_header.event_id                  = EVENT_ID_GENERIC_US_DETECTION;
         msg_ptr->event_reg_header.is_register               = is_register; // will be updated later
         msg_ptr->event_reg_header.event_config_payload_size = 0;           // will be updated later

         uqci_send_msg(state_ptr->uqci_client_handle, msg_ptr);
      }
   }

   // if upd is disabled then de-register for upd usecase.
   // this should be done after sending deregistration to the miid.
   if (0 == state_ptr->upd_enable)
      qsh_upd_proxy_uqci_setup_usecase(this);
}

// receives the usecase info event. it updates the upd miid
// and then send the upd event registration.
void qsh_upd_proxy_receive_usecase_info(sns_sensor *const this,
                                        asps_upd_usecase_register_ack_payload_t *usecase_info_ptr)
{
   qsh_upd_proxy_sensor_state *state_ptr = (qsh_upd_proxy_sensor_state *)this->state->state;

   state_ptr->module_iid = usecase_info_ptr->module_instance_id;

   // since we received updated usecase info therefore send event registration to the new upd modules.
   qsh_upd_proxy_uqci_register_upd_event(this, true);
}

// function to update enable/disable upd and send the event configuration.
sns_rc qsh_upd_proxy_req_send(sns_sensor *const this)
{
   qsh_upd_proxy_sensor_state *state_ptr = (qsh_upd_proxy_sensor_state *)this->state->state;

   uint32_t upd_enable = 0;

   sns_sensor_instance *inst_ptr = this->cb->get_sensor_instance(this, true);
   if (NULL != inst_ptr)
   {
      qsh_upd_proxy_inst_state *inst_state_ptr = (qsh_upd_proxy_inst_state *)inst_ptr->state->state;
      upd_enable |= inst_state_ptr->upd_enable;
   }
   if (upd_enable != state_ptr->upd_enable)
   {
      state_ptr->upd_enable = upd_enable;
      qsh_upd_proxy_uqci_register_upd_event(this, false);
   }

   return SNS_RC_SUCCESS;
}

/**
 * Initialize attributes to their default state.
 */
static void qsh_upd_proxy_publish_attributes(sns_sensor *const this)
{
   {
      char const              name[] = "UPD Proxy";
      sns_std_attr_value_data value  = sns_std_attr_value_data_init_default;
      value.str.funcs.encode         = pb_encode_string_cb;
      value.str.arg                  = &((pb_buffer_arg){ .buf = name, .buf_len = sizeof(name) });
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
   }
   {
      char const              type[] = "upd_proximity";
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
      char const              proto_files[] = "qsh_upd_proximity.proto";
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
sns_rc qsh_upd_proxy_init(sns_sensor *const this)
{
   qsh_upd_proxy_sensor_state *state_ptr = (qsh_upd_proxy_sensor_state *)this->state->state;

   memset(state_ptr, 0, sizeof(qsh_upd_proxy_sensor_state));

   // base class init
   char   sns_name[] = "qsh_upd_proximity";
   sns_rc rc         = qsh_audio_event_init(this, sns_name);

   // publish attribute except "available"
   qsh_upd_proxy_publish_attributes(this);

   return rc;
}

/* See sns_sensor::deinit */
sns_rc qsh_upd_proxy_deinit(sns_sensor *const this)
{
   qsh_upd_proxy_sensor_state *state_ptr = (qsh_upd_proxy_sensor_state *)this->state->state;

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
sns_rc qsh_upd_proxy_notify_event(sns_sensor *const this)
{
   return qsh_audio_event_notify_event(this);
}

//single instance for all the clients.
static bool find_instance_match(struct sns_request const *request,
                                sns_sensor_instance const *instance)
{
   UNUSED_VAR(request);
   UNUSED_VAR(instance);

   return true;
}

/* See sns_sensor::set_client_request */
sns_sensor_instance *qsh_upd_proxy_set_client_request(sns_sensor *const this,
                                                      sns_request const *curr_req,
                                                      sns_request const *new_req,
                                                      bool               remove)
{
   qsh_upd_proxy_sensor_state *state_ptr = (qsh_upd_proxy_sensor_state *)this->state->state;

   sns_sensor_instance *curr_inst =
      sns_sensor_util_find_instance(this, curr_req, this->sensor_api->get_sensor_uid(this));
   sns_sensor_instance *qsh_upd_inst = NULL;
   sns_sensor_instance *match_inst   = NULL;

   if (remove)
   {
      if (curr_inst != NULL)
      {
         SNS_PRINTF(LOW, this, "removing request");
         curr_inst->cb->remove_client_request(curr_inst, curr_req);
         if (NULL == curr_inst->cb->get_client_request(curr_inst, qsh_upd_proxy_get_sensor_uid(this), true))
         {
            this->cb->remove_instance(curr_inst);
            qsh_upd_proxy_req_send(this);
         }
      }
   }
   else if ((NULL != curr_inst) && (new_req->message_id == SNS_STD_MSGID_SNS_STD_FLUSH_REQ))
   {
	  sns_sensor_util_send_flush_event(state_ptr->audio_event_state.suid_ptr, curr_inst);
      qsh_upd_inst = &sns_instance_no_error;
   }
   else if (NULL != new_req)
   {
      if (new_req->message_id == SNS_STD_SENSOR_MSGID_SNS_STD_ON_CHANGE_CONFIG)
      {
         match_inst = sns_sensor_util_find_instance_match(this, new_req, &find_instance_match);
         /* If this is a request from a new client, check if an existing
         matching instance can be reused */
         if (NULL == curr_inst)
         {
            // If this is a request from a new client
            if (NULL == match_inst)
            {
               SNS_PRINTF(LOW, this, "creating instance");
               qsh_upd_inst = this->cb->create_instance(this, sizeof(qsh_upd_proxy_inst_state));
               if (qsh_upd_inst == NULL)
               {
                  SNS_PRINTF(ERROR, this, "create_instance() failed");
                  return NULL;
               }
            }
            else
            {
               qsh_upd_inst = match_inst;
            }
         }
         else
         {
            curr_inst->cb->remove_client_request(curr_inst, curr_req);
            if (NULL != match_inst)
            {
               qsh_upd_inst = match_inst;
            }
            else
            {
               qsh_upd_inst = this->cb->create_instance(this, sizeof(qsh_upd_proxy_inst_state));
               if (NULL == qsh_upd_inst)
               {
                  SNS_PRINTF(ERROR, this, "create_instance() failed");
                  return NULL;
               }
            }
         }
         qsh_upd_inst->cb->add_client_request(qsh_upd_inst, new_req);
         this->instance_api->set_client_config(qsh_upd_inst, new_req);
         qsh_upd_proxy_req_send(this);
      }
   }

   // If there is no instance then unblock power collapse
   if (NULL == this->cb->get_sensor_instance(this, true))
   {
      qsh_audio_event_unblock_power_collapse(this);
   }

   return qsh_upd_inst;
}
