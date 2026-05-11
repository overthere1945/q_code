/**============================================================================
  @file qsh_audio_event_sensor.c

  @brief Base Class of audio event sensor.

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_event_sensor.h"
#include "sns_qshtech_island.h"
#include "sns_service_manager.h"
#ifdef SSC_TARGET_HEXAGON
#include "mmpm.h"
#define QSH_AUDIO_BW_VOTE (1024*1024)
#endif

/*=============================================================================
  Function Definitions
  ===========================================================================*/


// publish the availability of the sensor
static void publish_available(sns_sensor *const this, bool is_available)
{
   sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
   value.has_boolean             = true;
   value.boolean                 = is_available;
   sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_AVAILABLE, &value, 1, true);

   SNS_PRINTF(LOW, this, "publish_available %d", is_available);
}

// publish error from a sensor instance
sns_rc qsh_audio_event_inst_send_error_(sns_sensor_instance *const instance_ptr,
                                       sns_std_error              error,
                                       sns_sensor_uid const *     suid_ptr)
{
   sns_rc              ret_val = SNS_RC_SUCCESS;
   sns_std_error_event error_event;
   error_event.error = error;

   SNS_INST_PRINTF(LOW, instance_ptr, "send_error %d", error);

   if (!pb_send_event(instance_ptr,
                      sns_std_error_event_fields,
                      &error_event,
                      sns_get_system_time(),
                      SNS_STD_MSGID_SNS_STD_ERROR_EVENT,
                      suid_ptr))
   {
      ret_val = SNS_RC_FAILED;
   }
   return ret_val;
}

// publish error events to all the clients of this sensor
sns_rc qsh_audio_event_send_error_(sns_sensor *const this, sns_std_error error)
{
   sns_sensor_instance *         instance;
   sns_rc                        ret_val   = SNS_RC_SUCCESS;
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;

   // loop through all the sensor instances and publish error events to all the clients
   for (instance = this->cb->get_sensor_instance(this, true); NULL != instance;
        instance = this->cb->get_sensor_instance(this, false))
   {
      ret_val |= qsh_audio_event_inst_send_error_(instance, error, state_ptr->suid_ptr);
   }
   return ret_val;
}

// callback function to handle successful connection to the remote process
static sns_rc qsh_invoke_uqci_init_cb(sns_sensor *const this, void *svc_handle_ptr)
{
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;
   state_ptr->uqci_svc_handle_ptr          = svc_handle_ptr;
   publish_available(this, true);
   return SNS_RC_SUCCESS;
}

// callback function to handle error received from ipc
static sns_rc qsh_invoke_uqci_init_cb_err(sns_sensor *const this, void *svc_handle_ptr)
{
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;
   state_ptr->uqci_svc_handle_ptr          = svc_handle_ptr;

   qsh_audio_event_send_error(this, SNS_STD_ERROR_NOT_SUPPORTED);
   publish_available(this, false);
   return SNS_RC_SUCCESS;
}

// init callback function for uqci svc init
static void qsh_audio_event_uqci_init_cb(void *cb_ctx_ptr, void *svc_handle_ptr, int err)
{
   sns_rc rc;

   // qsh invoke is used to protect the sensor's state
   if (0 == err)
   {
      qsh_invoke(cb_ctx_ptr, qsh_invoke_uqci_init_cb, &rc, svc_handle_ptr);
   }
   else
   {
      qsh_invoke(cb_ctx_ptr, qsh_invoke_uqci_init_cb_err, &rc, svc_handle_ptr);
   }
}

// register with uqci service.
static void qsh_audio_event_uqci_init(sns_sensor *const this)
{
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;
#ifdef QSH_AUDIO_QUERY_REGISTRY

   ipcr_name_t rp_name = { .service = state_ptr->config.rp_svc_id, .instance = state_ptr->config.rp_svc_instance_id };
#else
   ipcr_name_t rp_name = { .service = QSH_AUDIO_RP_SERVICE_ID, .instance = QSH_AUDIO_RP_INSTANCE_ID };
#endif
   uqci_init(&rp_name, qsh_audio_event_uqci_init_cb, state_ptr->qsh_invoke_handle_ptr);
}

/* function to register with mmpm */
static void qsh_audio_event_mmpm_register(sns_sensor *const this, char* client_name)
{
#ifdef SSC_TARGET_HEXAGON
//don't need to register with mmpm and place vote if in island
#ifndef QSH_AUDIO_IN_ISLAND
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;

   MmpmRegParamType regParam = { 0 };

   regParam.rev         = MMPM_REVISION;
   regParam.instanceId  = MMPM_CORE_INSTANCE_0;
   regParam.pwrCtrlFlag = PWR_CTRL_NONE;

   // Clear callback info if we're synchronous
   regParam.callBackFlag   = CALLBACK_NONE;
   regParam.MMPM_Callback  = NULL;
   regParam.cbFcnStackSize = 0;

   // Set core info and client name
   regParam.coreId      = MMPM_CORE_ID_LPASS_ADSP;
   regParam.pClientName = client_name;

   /* Make MMPM call to register client */
   state_ptr->mmpm_client_id = MMPM_Register_Ext(&regParam);

   if (0 == state_ptr->mmpm_client_id)
   {
      SNS_PRINTF(ERROR, this, "Error registering with mmpm");
   }
#else
   UNUSED_VAR(this);
   UNUSED_VAR(client_name);
#endif //QSH_AUDIO_IN_ISLAND
#endif //SSC_TARGET_HEXAGON
}

#ifndef QSH_AUDIO_IN_ISLAND
//Function to place bw vote
static void qsh_audio_event_request_bw_vote(sns_sensor *const this, bool is_enable)
{
#ifdef SSC_TARGET_HEXAGON
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;
   if (0 == state_ptr->mmpm_client_id)
   {
      return;
   }

   uint32_t mmpm_client_id = state_ptr->mmpm_client_id;

   static const uint8_t NUM_REQUEST = 1; // BW bundled request
   MmpmRscParamType     rscParam[NUM_REQUEST];
   MMPM_STATUS          retStats[NUM_REQUEST];
   MmpmRscExtParamType  reqParam;
   MmpmGenBwValType     bwReqVal;
   MmpmGenBwReqType     bwReq;

   reqParam.numOfReq = 0;

   //async votes to avoid MMPM deep call stack in qsh-audio context
   reqParam.apiType = MMPM_API_TYPE_ASYNC;

   reqParam.pExt      = NULL;
   reqParam.pReqArray = rscParam;
   reqParam.pStsArray = retStats;

   // For async where tracking is needed only
   reqParam.reqTag = 0;

   /* Populate bandwidth request */
   {
      bwReqVal.busRoute.masterPort                = MMPM_BW_PORT_ID_ADSP_MASTER;
      bwReqVal.busRoute.slavePort                 = MMPM_BW_PORT_ID_DDR_SLAVE;
      bwReqVal.bwValue.busBwValue.bwBytePerSec    = QSH_AUDIO_BW_VOTE;
      bwReqVal.bwValue.busBwValue.usagePercentage = 100;
      bwReqVal.bwValue.busBwValue.usageType       = MMPM_BW_USAGE_LPASS_DSP;

      rscParam[reqParam.numOfReq].rscId              = MMPM_RSC_ID_GENERIC_BW;
      rscParam[reqParam.numOfReq].rscParam.pGenBwReq = &bwReq;
      bwReq.pBandWidthArray                          = &bwReqVal;
      bwReq.numOfBw                                  = 1;
      reqParam.numOfReq++;
   }

   if (is_enable)
   {
      MMPM_Request_Ext(mmpm_client_id, &reqParam);
   }
   else
   {
      MMPM_Release_Ext(mmpm_client_id, &reqParam);
   }
#endif //SSC_TARGET_HEXAGON
}
#endif //QSH_AUDIO_IN_ISLAND

/* function to block powr collapse so that DSP can go in island. */
void qsh_audio_event_block_power_collapse(sns_sensor *const this)
{
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;

   // return if already blocking
   if (state_ptr->is_blocking_pc)
   {
      return;
   }
   
#ifdef QSH_AUDIO_IN_ISLAND
   //enable island entry
   qsh_audio_island_enable_npa_request();
#else
   //request bw vote to avoid power collapse and island entry.
   qsh_audio_event_request_bw_vote(this, true);
#endif

   sns_service_manager *service_mgr_ptr = this->cb->get_service_manager(this);
   sns_power_mgr_service *power_mgr_svc_ptr = (sns_power_mgr_service*)service_mgr_ptr->get_service(service_mgr_ptr, SNS_POWER_MGR_SERVICE);
   power_mgr_svc_ptr->api->sns_register_client("qsh_audio", &state_ptr->power_mgr_client_ptr);
   power_mgr_svc_ptr->api->sns_update_mcps_vote(state_ptr->power_mgr_client_ptr, QSH_AUDIO_MCPS);

   state_ptr->is_blocking_pc = TRUE;
}

/* function to unblock powr collapse. */
void qsh_audio_event_unblock_power_collapse(sns_sensor *const this)
{
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;

   // return if already not-blocking
   if (!state_ptr->is_blocking_pc)
   {
      return;
   }

#ifdef QSH_AUDIO_IN_ISLAND
   //disable island entry, so that it can go in power collapse.
   qsh_audio_island_disable_npa_request();
#else
   //release bw vote to allow power collapse or island entry.
   qsh_audio_event_request_bw_vote(this, false);
#endif

   sns_service_manager *service_mgr_ptr = this->cb->get_service_manager(this);
   sns_power_mgr_service *power_mgr_svc_ptr = (sns_power_mgr_service*)service_mgr_ptr->get_service(service_mgr_ptr, SNS_POWER_MGR_SERVICE);
   power_mgr_svc_ptr->api->sns_deregister_client(&state_ptr->power_mgr_client_ptr);

   state_ptr->is_blocking_pc = FALSE;
}

/* See sns_sensor::init */
sns_rc qsh_audio_event_init(sns_sensor *const this, char* sns_name)
{
   sns_rc                        rc        = SNS_RC_SUCCESS;
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;

   // state memset is done by the parent class.

   state_ptr->suid_ptr = this->sensor_api->get_sensor_uid(this);

   rc = qsh_invoke_register(this, &state_ptr->qsh_invoke_handle_ptr);
   if (rc != SNS_RC_SUCCESS)
   {
      SNS_PRINTF(ERROR, this, "qsh_invoke_register failed, rc=%d", rc);
      return rc;
   }

   // register with mmpm
   qsh_audio_event_mmpm_register(this, sns_name);

#ifdef QSH_AUDIO_QUERY_REGISTRY
   state_ptr->config.is_config_received = FALSE;
   state_ptr->first_pass                = true;

   SNS_SUID_LOOKUP_INIT(state_ptr->suid_lookup_data, NULL);
   sns_suid_lookup_add(this, &state_ptr->suid_lookup_data, "registry");
#else
   qsh_audio_event_uqci_init(this);
#endif

   sns_service_manager *manager= this->cb->get_service_manager(this);
   sns_memory_service *memory_service = (sns_memory_service *)manager->get_service(manager , SNS_MEMORY_SERVICE);
   if( NULL != memory_service)
   {
	  memory_service->api->register_island_memory_vote_cb(this, &sns_qshtech_island_memory_vote);
   }

   SNS_PRINTF(MED, this, "qsh_audio_event_init.");
   return SNS_RC_SUCCESS;
}

/* See sns_sensor::deinit */
sns_rc qsh_audio_event_deinit(sns_sensor *const this)
{
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;
   qsh_invoke_deregister(&state_ptr->qsh_invoke_handle_ptr);

   uqci_deinit(state_ptr->uqci_svc_handle_ptr);

   SNS_PRINTF(MED, this, "qsh_audio_event_deinit");
   return SNS_RC_SUCCESS;
}

#ifdef QSH_AUDIO_QUERY_REGISTRY
// send request to registry
static void qsh_audio_event_send_registry_req(sns_sensor *const this)
{
   qsh_audio_event_sensor_state *state_ptr       = (qsh_audio_event_sensor_state *)this->state->state;
   sns_service_manager *         service_mgr_ptr = this->cb->get_service_manager(this);
   sns_stream_service *          stream_service_ptr =
      (sns_stream_service *)service_mgr_ptr->get_service(service_mgr_ptr, SNS_STREAM_SERVICE);
   uint8_t        buffer[100];
   size_t         encoded_len;
   sns_sensor_uid registry_suid;
   sns_suid_lookup_get(&state_ptr->suid_lookup_data, "registry", &registry_suid);

   // Create a data stream to Registry
   stream_service_ptr->api->create_sensor_stream(stream_service_ptr,
                                                 this,
                                                 registry_suid,
                                                 &state_ptr->registry_stream_ptr);
   // Send a request to Registry
   sns_registry_read_req sns_audio_event_registry;
   pb_buffer_arg         data        = (pb_buffer_arg){ .buf = "qsh_audio", .buf_len = (strlen("qsh_audio") + 1) };
   sns_audio_event_registry.name.arg = &data;
   sns_audio_event_registry.name.funcs.encode = pb_encode_string_cb;
   sns_std_request registry_std_req           = sns_std_request_init_default;
   encoded_len                                = pb_encode_request(buffer,
                                   sizeof(buffer),
                                   &sns_audio_event_registry,
                                   sns_registry_read_req_fields,
                                   &registry_std_req);
   if (0 < encoded_len && NULL != state_ptr->registry_stream_ptr)
   {
      sns_request request = (sns_request){ .message_id  = SNS_REGISTRY_MSGID_SNS_REGISTRY_READ_REQ,
                                           .request_len = encoded_len,
                                           .request     = buffer };
      state_ptr->registry_stream_ptr->api->send_request(state_ptr->registry_stream_ptr, &request);
   }
}

// parse the config received from registry
static bool qsh_audio_event_parse_registry_event(pb_istream_t *stream_ptr, const pb_field_t *field_ptr, void **arg_pptr)
{
   UNUSED_VAR(field_ptr);
   sns_registry_data_item reg_item;
   pb_buffer_arg          item_name   = { 0, 0 };
   reg_item.name.arg                  = &item_name;
   reg_item.name.funcs.decode         = pb_decode_string_cb;
   bool                    rv         = pb_decode(stream_ptr, sns_registry_data_item_fields, &reg_item);
   qsh_audio_event_config *config_ptr = (qsh_audio_event_config *)*arg_pptr;

   if (reg_item.has_sint)
   {
      if (0 == strncmp((char *)item_name.buf, "rp_svc_id", item_name.buf_len))
      {
         config_ptr->rp_svc_id          = reg_item.sint;
         config_ptr->is_config_received = TRUE;
      }
      else if (0 == strncmp((char *)item_name.buf, "rp_svc_instance_id", item_name.buf_len))
      {
         config_ptr->rp_svc_instance_id = reg_item.sint;
      }
   }
   else
   {
      rv = false;
   }

   return rv;
}

// handle the event from registry
static sns_rc qsh_audio_event_handle_registry_events(sns_sensor *const this)
{
   sns_rc                        rv = SNS_RC_SUCCESS;
   sns_sensor_event *            event_ptr;
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;

   for (event_ptr = state_ptr->registry_stream_ptr->api->peek_input(state_ptr->registry_stream_ptr); NULL != event_ptr;
        event_ptr = (NULL == state_ptr->registry_stream_ptr)
                       ? NULL
                       : state_ptr->registry_stream_ptr->api->get_next_input(state_ptr->registry_stream_ptr))
   {

      if (SNS_REGISTRY_MSGID_SNS_REGISTRY_READ_EVENT == event_ptr->message_id)
      {
         pb_istream_t            stream     = pb_istream_from_buffer((void *)event_ptr->event, event_ptr->event_len);
         sns_registry_read_event read_event = sns_registry_read_event_init_default;
         pb_buffer_arg           group_name = { 0, 0 };
         read_event.name.arg                = &group_name;
         read_event.name.funcs.decode       = pb_decode_string_cb;
         read_event.data.items.arg          = &state_ptr->config;
         read_event.data.items.funcs.decode = qsh_audio_event_parse_registry_event;

         if (!pb_decode(&stream, sns_registry_read_event_fields, &read_event))
         {
            SNS_PRINTF(ERROR, this, "Error decoding registry event");
         }
         else if (0 == strncmp((char *)group_name.buf, "qsh_audio", group_name.buf_len))
         {
            SNS_PRINTF(LOW,
                       this,
                       "rp svc id %d, rp inst id %d",
                       state_ptr->config.rp_svc_id,
                       state_ptr->config.rp_svc_instance_id);
            sns_sensor_util_remove_sensor_stream(this, &state_ptr->registry_stream_ptr);
            state_ptr->registry_stream_ptr = NULL;
         }
         else
         {
            SNS_PRINTF(ERROR, this, "config from registry failed");
         }
      }
      else if (SNS_STD_MSGID_SNS_STD_ERROR_EVENT == event_ptr->message_id)
      {
         SNS_PRINTF(ERROR,
                    this,
                    "Error event received %d",
                    event_ptr->message_id);

         qsh_audio_event_send_error(this, SNS_STD_ERROR_INVALID_STATE);
         // Consume all remaining events
         while (NULL != state_ptr->registry_stream_ptr->api->get_next_input(state_ptr->registry_stream_ptr))
            ;
         rv = SNS_RC_INVALID_STATE;
         break;
      }
      else
      {
         SNS_PRINTF(ERROR,
                    this,
                    "qsh_audio_event_handle_registry_events: Unknown event received %d",
                    event_ptr->message_id);
      }
   }

   return rv;
}
#endif

/* See sns_sensor::notify_event */
sns_rc qsh_audio_event_notify_event(sns_sensor *const this)
{
   UNUSED_VAR(this);
#ifdef QSH_AUDIO_QUERY_REGISTRY
   qsh_audio_event_sensor_state *state_ptr = (qsh_audio_event_sensor_state *)this->state->state;

   sns_suid_lookup_handle(this, &state_ptr->suid_lookup_data);

   if (NULL != state_ptr->registry_stream_ptr)
   {
      qsh_audio_event_handle_registry_events(this);

      if (state_ptr->config.is_config_received)
      {
         qsh_audio_event_uqci_init(this);
      }
   }

   if (sns_suid_lookup_complete(&state_ptr->suid_lookup_data))
   {
      if (state_ptr->first_pass == true)
      {
         state_ptr->first_pass = false;
         qsh_audio_event_send_registry_req(this);
      }
      sns_suid_lookup_deinit(this, &state_ptr->suid_lookup_data);
   }
#endif
   return SNS_RC_SUCCESS;
}

/* See sns_sensor::set_client_request */
sns_sensor_instance *qsh_audio_event_set_client_request(sns_sensor *const this,
                                                        sns_request const *curr_req_ptr,
                                                        sns_request const *new_req_ptr,
                                                        bool               remove,
                                                        uint32_t           size_of_instance_state)
{
   sns_sensor_instance *inst_ptr =
      sns_sensor_util_find_instance(this, curr_req_ptr, this->sensor_api->get_sensor_uid(this));

   if (NULL != inst_ptr)
   {
      SNS_PRINTF(LOW, this, "removing request");
      inst_ptr->cb->remove_client_request(inst_ptr, curr_req_ptr);
   }
   else
   {
      SNS_PRINTF(LOW, this, "creating instance");
      inst_ptr = this->cb->create_instance(this, size_of_instance_state);
   }

   if (remove)
   {
      SNS_PRINTF(LOW, this, "removing instance");
      this->cb->remove_instance(inst_ptr);
      inst_ptr = NULL;
   }
   else
   {
      SNS_PRINTF(LOW, this, "adding request");
      inst_ptr->cb->add_client_request(inst_ptr, new_req_ptr);
      this->instance_api->set_client_config(inst_ptr, new_req_ptr);
   }

   return inst_ptr;
}
