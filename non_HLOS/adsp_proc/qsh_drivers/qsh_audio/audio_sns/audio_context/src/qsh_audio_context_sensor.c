/*=============================================================================
  @file qsh_audio_context_sensor.c

  The Audio Context Sensor implementation

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_context_sensor.h"
#include "qsh_audio_context_sensor_instance.h"
#include "qsh_audio_context.pb.h"

#include "qsh_audio_memmgr.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

/* register for use-case info update event.
 * Sensor register for the ACD usecase update event with ASPS in the beginning.
 * Whenever ACD module instance-ID is updated an event will be sent to this sensor.
 */
void qsh_audio_context_uqci_register_usecase_info_event(qsh_audio_context_sensor_state *sensor_state_ptr,
                                                        bool                            is_register)
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

// send get-cfg command to the ASPS to query the list of supported audio context ids.
static void qsh_audio_context_uqci_query_context_list(qsh_audio_context_sensor_state *sensor_state_ptr)
{
   struct get_cfg_cmd_paylaod_t
   {
      asps_cmd_header_t        cmd_header;
      asps_module_param_data_t param_header;
   } *msg_ptr = NULL;

   uint32_t payload_size = sizeof(struct get_cfg_cmd_paylaod_t);

   if (SNS_RC_SUCCESS == uqci_create_msg(sensor_state_ptr->uqci_client_handle,
                                         ASPS_CMD_GET_CFG, //opcode
                                         ASPS_MODULE_INSTANCE_ID, //token
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size = sizeof(asps_module_param_data_t);

      msg_ptr->param_header.param_id           = PARAM_ID_ASPS_GET_SUPPORTED_CONTEXT_IDS;
      msg_ptr->param_header.param_size         = 0;
      msg_ptr->param_header.module_instance_id = ASPS_MODULE_INSTANCE_ID;

      uqci_send_msg(sensor_state_ptr->uqci_client_handle, msg_ptr);

      sensor_state_ptr->is_context_list_query_pending = TRUE;
   }
}

// send command to asps to setup the usecase of requested context ids.
static void qsh_audio_context_uqci_setup_usecase(sns_sensor *const this)
{
   qsh_audio_context_sensor_state *state_ptr = (qsh_audio_context_sensor_state *)this->state->state;
   uint32_t                        usecase_payload_size =
      sizeof(asps_acd_usecase_register_payload_t) + (sizeof(uint32_t) * state_ptr->aggr_config.num_context);
   uint32_t param_size   = sizeof(param_id_asps_sensor_usecase_register_t) + usecase_payload_size;
   uint32_t payload_size = sizeof(asps_cmd_header_t) + sizeof(asps_module_param_data_t) + param_size;
   uint32_t is_register  = (state_ptr->aggr_config.num_context) ? 1 : 0;

   struct acd_usecase_setup_paylaod_t
   {
      asps_cmd_header_t                       cmd_header;
      asps_module_param_data_t                param_header;
      param_id_asps_sensor_usecase_register_t usecase_header;
      asps_acd_usecase_register_payload_t     usecase_payload;
   } *msg_ptr = NULL;

   if (SNS_RC_SUCCESS == uqci_create_msg(state_ptr->uqci_client_handle,
                                         ASPS_CMD_SET_CFG,                                    // opcode
                                         (is_register) ? ASPS_MODULE_INSTANCE_ID : IGNORE_RSP_TOKEN, // token, if deregistration failure is ignored.
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, payload_size);
      msg_ptr->cmd_header.payload_size = payload_size - sizeof(asps_cmd_header_t);

      msg_ptr->param_header.param_id           = PARAM_ID_ASPS_SENSOR_USECASE_REGISTER;
      msg_ptr->param_header.param_size         = param_size;
      msg_ptr->param_header.module_instance_id = ASPS_MODULE_INSTANCE_ID;

      msg_ptr->usecase_header.usecase_id   = ASPS_USECASE_ID_ACD;
      msg_ptr->usecase_header.is_register  = is_register;
      msg_ptr->usecase_header.payload_size = (is_register) ? usecase_payload_size : 0;

      if (msg_ptr->usecase_header.is_register)
      {
         asps_acd_usecase_register_payload_t *usecase_payload_ptr =
            (asps_acd_usecase_register_payload_t *)(&msg_ptr->usecase_payload);

         // setting up usecase for the requested contexts from all the sensor client.
         usecase_payload_ptr->num_contexts = state_ptr->aggr_config.num_context;
         for (int i = 0; i < usecase_payload_ptr->num_contexts; i++)
         {
            usecase_payload_ptr->context_ids[i] = state_ptr->aggr_config.event_config_arr[i].context_id;
         }

         // block power collapse if enabling events.
         qsh_audio_event_block_power_collapse(this);
      }
      else
      {
         // unblock power collapse if disabling events.
         qsh_audio_event_unblock_power_collapse(this);
      }

      uqci_send_msg(state_ptr->uqci_client_handle, msg_ptr);
   }
}

// returns the miid in ADSP for a given context ID.
static uint32_t qsh_audio_context_get_miid(qsh_audio_context_sensor_state *state_ptr, uint32_t context_id)
{
   for (int i = 0; i < state_ptr->usecase_info.num_context; i++)
   {
      if (state_ptr->usecase_info.context_id_arr[i] == context_id)
      {
         return state_ptr->usecase_info.module_iid_arr[i];
      }
   }
   return 0; // usecase is not setup for this context-id
}

// function to deregister ACD events with the given module instance.
static void qsh_audio_context_uqci_dregister_module_event(qsh_audio_context_sensor_state *state_ptr, uint32_t miid)
{
   struct acd_event_deregister_paylaod_t
   {
      asps_cmd_header_t                 cmd_header;
      asps_cmd_register_module_events_t event_reg_header;
   } *msg_ptr = NULL;

   uint32_t payload_size = sizeof(struct acd_event_deregister_paylaod_t);

   if (SNS_RC_SUCCESS == uqci_create_msg(state_ptr->uqci_client_handle,
                                         ASPS_CMD_REGISTER_MODULE_EVENTS, //opcode
                                         IGNORE_RSP_TOKEN, //token
                                         payload_size,
                                         (void **)&msg_ptr))
   {
      sns_memzero(msg_ptr, sizeof(asps_cmd_header_t));
      msg_ptr->cmd_header.payload_size = sizeof(asps_cmd_register_module_events_t);

      msg_ptr->event_reg_header.module_instance_id        = miid;
      msg_ptr->event_reg_header.event_id                  = EVENT_ID_ACD_DETECTION_EVENT;
      msg_ptr->event_reg_header.is_register               = 0;
      msg_ptr->event_reg_header.event_config_payload_size = 0;

      uqci_send_msg(state_ptr->uqci_client_handle, msg_ptr);
   }
}

// send context event registration to the ACD modules in ADSP.
static void qsh_audio_context_uqci_register_context_events(sns_sensor *const this, bool_t force_send)
{
   qsh_audio_context_sensor_state *         state_ptr        = (qsh_audio_context_sensor_state *)this->state->state;
   uint32_t                                 agrr_num_context = state_ptr->aggr_config.num_context;
   acd_context_specific_generic_reg_info_t *agrr_config_ptr  = state_ptr->aggr_config.event_config_arr;

   // if number of contexts are zero then send de-registration command.
   uint32_t is_register = (agrr_num_context == 0) ? 0 : 1;

   // if de-registering then send the command to all the miids
   // this to send the event deregistration command to all the miids for ACD usecase.
   force_send = (0 == is_register) ? true : force_send;

   // if force send is false then first try to setup the usecase for all the requested contexts.
   if (!force_send)
   {
      for (int i = 0; i < state_ptr->aggr_config.num_context; i++)
      {
         if (0 == qsh_audio_context_get_miid(state_ptr, state_ptr->aggr_config.event_config_arr[i].context_id))
         {
            // usecase is not setup for some of the contexts.
            // setup usecase now and return, event registration will be done when usecase is setup.
            qsh_audio_context_uqci_setup_usecase(this);
            return;
         }
      }
   }
   else
   {
      // at this point, acd usecsae is setup for the context ids, so send the event registration to each module.
   }

   // send registration request for each acd module instance
   for (int i = 0; i < state_ptr->usecase_info.num_context; i++)
   {
      // multiple contexts can be supported by one miid therefore find all the unique miids and send the context
      // event registration to them.
      bool is_unique_miid = true;
      for (int j = 0; j < i; j++)
      {
         if (state_ptr->usecase_info.module_iid_arr[j] == state_ptr->usecase_info.module_iid_arr[i])
         {
            is_unique_miid = false;
            break;
         }
      }
      // send registration or deregistration command to each unique miid
      if (is_unique_miid)
      {
         uint32_t module_iid = state_ptr->usecase_info.module_iid_arr[i];

         // first deregister any events.
         qsh_audio_context_uqci_dregister_module_event(state_ptr, module_iid);

         uint32_t gen_key_payload_size =
            sizeof(acd_generic_key_id_reg_cfg_t) + (agrr_num_context * sizeof(acd_context_specific_generic_reg_info_t));
         uint32_t event_payload_size =
            sizeof(event_id_acd_detection_cfg_t) + sizeof(acd_key_info_t) + gen_key_payload_size;
         uint32_t asps_cmd_payload_size = sizeof(asps_cmd_register_module_events_t) + event_payload_size;
         uint32_t payload_size          = sizeof(asps_cmd_header_t) + asps_cmd_payload_size;

         struct acd_event_register_paylaod_t
         {
            asps_cmd_header_t                       cmd_header;
            asps_cmd_register_module_events_t       event_reg_header;
            event_id_acd_detection_cfg_t            acd_reg_header;
            acd_key_info_t                          key_info_payload;
            acd_generic_key_id_reg_cfg_t            gen_key_payload;
            acd_context_specific_generic_reg_info_t context_config[0];
         } *msg_ptr = NULL;

         if (SNS_RC_SUCCESS == uqci_create_msg(state_ptr->uqci_client_handle,
                                               ASPS_CMD_REGISTER_MODULE_EVENTS, // opcode
                                               module_iid,                      // token
                                               payload_size,
                                               (void **)&msg_ptr))
         {
            sns_memzero(msg_ptr, sizeof(asps_cmd_header_t));
            msg_ptr->cmd_header.payload_size = asps_cmd_payload_size;

            msg_ptr->event_reg_header.module_instance_id        = module_iid;
            msg_ptr->event_reg_header.event_id                  = EVENT_ID_ACD_DETECTION_EVENT;
            msg_ptr->event_reg_header.is_register               = 0; // will be updated later
            msg_ptr->event_reg_header.event_config_payload_size = 0; // will be updated later

            if (is_register)
            {
               acd_generic_key_id_reg_cfg_t *           gen_key_payload_ptr = (&msg_ptr->gen_key_payload);
               acd_context_specific_generic_reg_info_t *context_config_arr  = (&msg_ptr->context_config[0]);

               msg_ptr->acd_reg_header.num_keys           = 1;
               msg_ptr->key_info_payload.key_id           = ACD_KEY_ID_GENERIC_INFO;
               msg_ptr->key_info_payload.key_payload_size = 0; // will be updated later

               gen_key_payload_ptr->num_contexts = 0;

               for (int k = 0; k < agrr_num_context; k++)
               {
                  if (module_iid == qsh_audio_context_get_miid(state_ptr, agrr_config_ptr[k].context_id))
                  {
                     context_config_arr[gen_key_payload_ptr->num_contexts].context_id = agrr_config_ptr[k].context_id;
                     context_config_arr[gen_key_payload_ptr->num_contexts].confidence_threshold =
                        agrr_config_ptr[k].confidence_threshold;

                     context_config_arr[gen_key_payload_ptr->num_contexts].step_size = agrr_config_ptr[k].step_size;

                     gen_key_payload_ptr->num_contexts++;
                     SNS_PRINTF(LOW,
                                this,
                                "request for enabling context_id 0x%x "
                                "threshold %d step_size %d with module iid 0x%x",
                                agrr_config_ptr[k].context_id,
                                agrr_config_ptr[k].confidence_threshold,
                                agrr_config_ptr[k].step_size,
                                module_iid);
                  }
               }

               // if there is any context for this cmd then send registration otherwise deregister.
               msg_ptr->event_reg_header.is_register = (gen_key_payload_ptr->num_contexts) ? 1 : 0;

               // update the payload size
               if (msg_ptr->event_reg_header.is_register)
               {
                  msg_ptr->key_info_payload.key_payload_size =
                     sizeof(acd_generic_key_id_reg_cfg_t) +
                     (gen_key_payload_ptr->num_contexts * sizeof(acd_context_specific_generic_reg_info_t));

                  msg_ptr->event_reg_header.event_config_payload_size = sizeof(event_id_acd_detection_cfg_t) +
                                                                        sizeof(acd_key_info_t) +
                                                                        msg_ptr->key_info_payload.key_payload_size;
               }
            }

            if (msg_ptr->event_reg_header.is_register)
            {
               // send registration command
               uqci_send_msg(state_ptr->uqci_client_handle, msg_ptr);
            }
            else
            {
               // deregistration is already done, free the msg.
               uqci_free_msg(state_ptr->uqci_client_handle, msg_ptr);
            }
         }
      }
   }

   // if no contexts are required then de-register for ACD usecase.
   // this should be done after sending deregistration to all the miids.
   if (0 == state_ptr->aggr_config.num_context)
   {
      qsh_audio_context_uqci_setup_usecase(this);

      if (state_ptr->usecase_info.context_id_arr)
         qsh_audio_free(state_ptr->usecase_info.context_id_arr);

      state_ptr->usecase_info.num_context    = 0;
      state_ptr->usecase_info.context_id_arr = NULL;
      state_ptr->usecase_info.module_iid_arr = NULL;
   }
}

// receives the usecase info event. it updates the acd miid for each context
// and then send the context event registration.
void qsh_audio_context_receive_usecase_info(sns_sensor *const this,
                                            asps_acd_usecase_register_ack_payload_t *usecase_info_ptr,
                                            uint32_t                                 payload_size)
{
   qsh_audio_context_sensor_state *state_ptr = (qsh_audio_context_sensor_state *)this->state->state;

   uint32_t                                 usecase_info_num_total_contexts = 0;
   asps_acd_usecase_register_ack_payload_t *temp_ptr                        = usecase_info_ptr;
   uint32_t                                 miid_count                      = 0;

   // get the total number of acd miid and contexte ids
   while (payload_size > sizeof(asps_acd_usecase_register_ack_payload_t))
   {
      uint32_t req_payload_size = sizeof(asps_acd_usecase_register_ack_payload_t) + (temp_ptr->num_contexts * sizeof(uint32_t));
      if (payload_size < req_payload_size)
      {
          break;
      }
      SNS_PRINTF(LOW,
                 this,
                 "USECASE info: miid 0x%x, num_contexts %lu",
                 temp_ptr->module_instance_id,
                 temp_ptr->num_contexts);

      miid_count++;
      usecase_info_num_total_contexts += temp_ptr->num_contexts;
      payload_size -= req_payload_size;
      temp_ptr = (asps_acd_usecase_register_ack_payload_t *)((int8_t*)temp_ptr + req_payload_size);
   }

   // allocate memory to store usecase info
   if (state_ptr->usecase_info.num_context != usecase_info_num_total_contexts)
   {
      if (state_ptr->usecase_info.context_id_arr)
         qsh_audio_free(state_ptr->usecase_info.context_id_arr);

      state_ptr->usecase_info.num_context    = 0;
      state_ptr->usecase_info.context_id_arr = NULL;
      state_ptr->usecase_info.module_iid_arr = NULL;

      uint32_t *ptr =
         (uint32_t *)qsh_audio_malloc(QSH_AUDIO_HEAP_MAIN, sizeof(uint32_t) * 2 * usecase_info_num_total_contexts);

      if (ptr)
      {
         state_ptr->usecase_info.context_id_arr = ptr;
         state_ptr->usecase_info.module_iid_arr = ptr + usecase_info_num_total_contexts;
      }
      else
      {
         SNS_PRINTF(LOW, this, "malloc failed!!");
         qsh_audio_event_send_error(this, SNS_STD_ERROR_NOT_SUPPORTED);
         return;
      }
   }

   state_ptr->usecase_info.num_context = 0;

   for (int i = 0; i < miid_count; i++)
   {
      for (int j = 0; j < usecase_info_ptr->num_contexts; j++)
      {
         state_ptr->usecase_info.module_iid_arr[state_ptr->usecase_info.num_context] =
            usecase_info_ptr->module_instance_id;
         state_ptr->usecase_info.context_id_arr[state_ptr->usecase_info.num_context] = usecase_info_ptr->context_ids[j];
         state_ptr->usecase_info.num_context++;
      }

      usecase_info_ptr =
         (asps_acd_usecase_register_ack_payload_t *)(&usecase_info_ptr->context_ids[usecase_info_ptr->num_contexts]);
   }

   // since we received updated usecase info therefore send the context request to the new acd modules.
   qsh_audio_context_uqci_register_context_events(this, true);
}

// function to aggregate and send the event configuration
static sns_rc qsh_audio_context_aggregate_req_send(sns_sensor *const this)
{
   qsh_audio_context_sensor_state *state_ptr = (qsh_audio_context_sensor_state *)this->state->state;

   // reset the aggregated config.
   state_ptr->aggr_config.num_context = 0;

   // loop through all the sensor instances config and aggregate
   for (sns_sensor_instance *inst_ptr = this->cb->get_sensor_instance(this, true); NULL != inst_ptr;
        inst_ptr                      = this->cb->get_sensor_instance(this, false))
   {
      qsh_audio_context_inst_state *inst_state_ptr = (qsh_audio_context_inst_state *)inst_ptr->state->state;

      for (int i = 0; i < inst_state_ptr->inst_config.num_context; i++)
      {
         uint32_t context_id = inst_state_ptr->inst_config.event_config[i].context_id;
         uint32_t threshold  = inst_state_ptr->inst_config.event_config[i].confidence_threshold;
         uint32_t step_size  = inst_state_ptr->inst_config.event_config[i].step_size;

         int j = 0;
         for (j = 0; j < state_ptr->aggr_config.num_context; j++)
         {
            // if context is present in the aggr config then update the threshold
            if (state_ptr->aggr_config.event_config_arr[j].context_id == context_id)
            {
               state_ptr->aggr_config.event_config_arr[j].confidence_threshold =
                  MIN(state_ptr->aggr_config.event_config_arr[j].confidence_threshold, threshold);

               state_ptr->aggr_config.event_config_arr[j].step_size =
                  MIN(state_ptr->aggr_config.event_config_arr[j].step_size, step_size);
               break;
            }
         }

         // if context is not present in the aggr config then add.
         if (j == state_ptr->aggr_config.num_context)
         {
            // allocate extra memory, if there is no sufficient
            if (j == state_ptr->aggr_config.event_config_arr_size)
            {
               // adding space for 8 more contexts
               uint32_t size =
                  (state_ptr->aggr_config.event_config_arr_size + 8) * sizeof(acd_context_specific_generic_reg_info_t);
               void *mem_ptr = qsh_audio_malloc(QSH_AUDIO_HEAP_MAIN, size); // island not required

               if (mem_ptr)
               {
                  if (state_ptr->aggr_config.event_config_arr)
                  {
                     sns_memscpy(mem_ptr,
                                 size,
                                 state_ptr->aggr_config.event_config_arr,
                                 state_ptr->aggr_config.event_config_arr_size *
                                    sizeof(acd_context_specific_generic_reg_info_t));

                     qsh_audio_free(state_ptr->aggr_config.event_config_arr);
                  }

                  state_ptr->aggr_config.event_config_arr = (acd_context_specific_generic_reg_info_t *)mem_ptr;
                  state_ptr->aggr_config.event_config_arr_size += 8;
               }
               else
               {
                  SNS_PRINTF(ERROR, this, "malloc failed!");
               }
            }

            if (j < state_ptr->aggr_config.event_config_arr_size)
            {
               state_ptr->aggr_config.event_config_arr[j].context_id           = context_id;
               state_ptr->aggr_config.event_config_arr[j].confidence_threshold = threshold;
               state_ptr->aggr_config.event_config_arr[j].step_size            = step_size;
               state_ptr->aggr_config.num_context++;
            }
         }
      }
   }

   // send aggregated request.
   // also sets up the usecase in ADSP first.
   qsh_audio_context_uqci_register_context_events(this, false);

   return SNS_RC_SUCCESS;
}

/**
 * Initialize attributes to their default state.
 */
static void qsh_audio_context_publish_attributes(sns_sensor *const this)
{
   {
      char const              name[] = "Audio Context";
      sns_std_attr_value_data value  = sns_std_attr_value_data_init_default;
      value.str.funcs.encode         = pb_encode_string_cb;
      value.str.arg                  = &((pb_buffer_arg){ .buf = name, .buf_len = sizeof(name) });
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
   }
   {
      char const              type[] = "audio_context";
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
      char const              proto_files[] = "qsh_audio_context.proto";
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
sns_rc qsh_audio_context_init(sns_sensor *const this)
{
   qsh_audio_context_sensor_state *state_ptr = (qsh_audio_context_sensor_state *)this->state->state;

   memset(state_ptr, 0, sizeof(qsh_audio_context_sensor_state));

   // base class init
   char   sns_name[] = "qsh_audio_context";
   sns_rc rc         = qsh_audio_event_init(this, sns_name);

   // publish attribute except "available"
   qsh_audio_context_publish_attributes(this);

   return rc;
}

/* See sns_sensor::deinit */
sns_rc qsh_audio_context_deinit(sns_sensor *const this)
{
   qsh_audio_context_sensor_state *state_ptr = (qsh_audio_context_sensor_state *)this->state->state;

   // uqci client deregistration
   if (state_ptr->uqci_client_handle)
   {
      uqci_deregister_client(state_ptr->uqci_client_handle);
      state_ptr->uqci_client_handle = NULL;
   }

   if (state_ptr->aggr_config.event_config_arr)
   {
      qsh_audio_free(state_ptr->aggr_config.event_config_arr);
   }

   if (state_ptr->usecase_info.num_context && state_ptr->usecase_info.context_id_arr)
   {
      state_ptr->usecase_info.num_context = 0;
      qsh_audio_free(state_ptr->usecase_info.context_id_arr);
   }

   // base class deinit
   return qsh_audio_event_deinit(this);
}

/* See sns_sensor::notify_event */
sns_rc qsh_audio_context_notify_event(sns_sensor *const this)
{
   return qsh_audio_event_notify_event(this);
}

/* See sns_sensor::set_client_request */
sns_sensor_instance *qsh_audio_context_set_client_request(sns_sensor *const this,
                                                          sns_request const *curr_req,
                                                          sns_request const *new_req,
                                                          bool               remove)
{
   qsh_audio_context_sensor_state *state_ptr = (qsh_audio_context_sensor_state *)this->state->state;
   sns_sensor_instance *           inst_ptr =
      qsh_audio_event_set_client_request(this, curr_req, new_req, remove, sizeof(qsh_audio_context_inst_state));

   // if instance is removed then send the re-aggregated request
   if (remove)
   {
      qsh_audio_context_aggregate_req_send(this);
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
         case QSH_AUDIO_CONTEXT_MSGID_QSH_AUDIO_CONTEXT_QUERY:
         {
            // if there is already a pending query then don't send it again, we are waiting for the response
            if (!state_ptr->is_context_list_query_pending)
            {
               qsh_audio_context_uqci_query_context_list(state_ptr);
            }
         }
         break;
         case QSH_AUDIO_CONTEXT_MSGID_QSH_AUDIO_CONTEXT_REQ:
         {
            qsh_audio_context_aggregate_req_send(this);
         }
         break;
         default:
            break;
      }
   }

   // If there is no instance then unblock power collapse
   if (NULL == this->cb->get_sensor_instance(this, true))
   {
      qsh_audio_event_unblock_power_collapse(this);
   }

   return inst_ptr;
}
