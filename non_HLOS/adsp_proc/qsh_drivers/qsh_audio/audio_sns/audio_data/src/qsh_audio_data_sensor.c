/*=============================================================================
  @file qsh_audio_data_sensor.c

  The Audio Data Sensor implementation

  Copyright (c) 2021-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_data_sensor.h"
#include "qsh_audio_data_sensor_instance.h"
#include "qsh_audio_data.pb.h"

#include "qsh_audio_memmgr.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

/**
 * Initialize attributes to their default state.
 */
static void qsh_audio_data_publish_attributes(sns_sensor *const this)
{
   {
      char const              name[] = "Audio Data";
      sns_std_attr_value_data value  = sns_std_attr_value_data_init_default;
      value.str.funcs.encode         = pb_encode_string_cb;
      value.str.arg                  = &((pb_buffer_arg){ .buf = name, .buf_len = sizeof(name) });
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
   }
   {
      char const              type[] = "audio_data";
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
      char const              proto_files[] = "qsh_audio_data.proto";
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
      value.sint                    = SNS_STD_SENSOR_STREAM_TYPE_STREAMING;
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_STREAM_TYPE, &value, 1, false);
   }
   {
      //publishing the rate attributes
      sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
      value.has_flt                 = true;
      value.flt                     = ADS_SAMPLING_RATE;
      sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_RATES, &value, 1, true);
    }
}

/* See sns_sensor::init */
sns_rc qsh_audio_data_init(sns_sensor *const this)
{
   qsh_audio_data_sensor_state_t *state_ptr = (qsh_audio_data_sensor_state_t *)this->state->state;

   memset(state_ptr, 0, sizeof(qsh_audio_data_sensor_state_t));

   // base class init
   char   sns_name[] = "qsh_audio_data";
   sns_rc rc         = qsh_audio_event_init(this, sns_name);

   // publish attribute except "available"
   qsh_audio_data_publish_attributes(this);

   return rc;
}

/* See sns_sensor::deinit */
sns_rc qsh_audio_data_deinit(sns_sensor *const this)
{
   // base class deinit
   return qsh_audio_event_deinit(this);
}

/* See sns_sensor::notify_event */
sns_rc qsh_audio_data_notify_event(sns_sensor *const this)
{
   return qsh_audio_event_notify_event(this);
}

/* See sns_sensor::set_client_request */
sns_sensor_instance *qsh_audio_data_set_client_request(sns_sensor *const this,
                                                       sns_request const *curr_req,
                                                       sns_request const *new_req,
                                                       bool               remove)
{
   sns_rc err = SNS_RC_SUCCESS;

   SNS_PRINTF(LOW, this, "qsh_audio_data_set_client_request");

   // qsh_audio_data_sensor_state_t *state_ptr = (qsh_audio_data_sensor_state_t *)this->state->state;

   sns_sensor_instance *cur_inst_ptr =
      sns_sensor_util_find_instance(this, curr_req, this->sensor_api->get_sensor_uid(this));

   if (remove)
   {
      if (cur_inst_ptr != NULL)
      {
         qsh_audio_data_inst_state_t *inst_state_ptr = (qsh_audio_data_inst_state_t *)cur_inst_ptr->state->state;

         // if flush is active then send flush event and disable data-flow
         if (inst_state_ptr->flush_req)
         {
            inst_state_ptr->flush_req        = false;
            inst_state_ptr->data_flow_enable = false;
            sns_sensor_util_send_flush_event(inst_state_ptr->suid_ptr, cur_inst_ptr);
         }
         else if (inst_state_ptr->data_flow_enable)
         {
            SNS_PRINTF(LOW, this, "Disabling Data flow");
            inst_state_ptr->data_flow_enable = false;
            qsh_audio_data_uqci_send_data_flow(inst_state_ptr);
         }

         // remove request (there should be only one request in the queue)
         SNS_PRINTF(LOW, this, "removing request");
         cur_inst_ptr->cb->remove_client_request(cur_inst_ptr, curr_req);
      }
   }
   else
   {
      switch (new_req->message_id)
      {
         case SNS_STD_MSGID_SNS_STD_FLUSH_REQ:
         {
            if (cur_inst_ptr == NULL)
            {
               SNS_PRINTF(ERROR, this, "no instance found to flush data");
               break;
            }

            SNS_PRINTF(LOW, this, "sending flush request to instance");
            err = this->instance_api->set_client_config(cur_inst_ptr, new_req);

            cur_inst_ptr = (err == SNS_RC_SUCCESS) ? &sns_instance_no_error : NULL;
            return cur_inst_ptr;
         }
         case QSH_AUDIO_DATA_MSGID_QSH_AUDIO_DATA_CONFIG:
         {
         
            // if new create new instance
            if (cur_inst_ptr == NULL)
            {
               SNS_PRINTF(LOW, this, "creating instance");
               cur_inst_ptr = this->cb->create_instance(this, sizeof(qsh_audio_data_inst_state_t));
               if (cur_inst_ptr == NULL)
               {
                  SNS_PRINTF(ERROR, this, "create_instance() failed");
                  break;
               }
            }

            // set instance config
            SNS_PRINTF(LOW, this, "sending request to instance");
            err = this->instance_api->set_client_config(cur_inst_ptr, new_req);

            if (err == SNS_RC_SUCCESS)
            {
               if (curr_req)
               {
                  SNS_PRINTF(LOW, this, "removing prev request");
                  cur_inst_ptr->cb->remove_client_request(cur_inst_ptr, curr_req);
               }

               // add request
               SNS_PRINTF(LOW, this, "adding request");
               cur_inst_ptr->cb->add_client_request(cur_inst_ptr, new_req);
            }

            break;
         }
         default:
         {
            SNS_PRINTF(ERROR, this, "unsupported message received!");
            cur_inst_ptr = NULL;
            break;
         }
      }
   }

   // remove instance if there is no request
   if (cur_inst_ptr &&
       (NULL == cur_inst_ptr->cb->get_client_request(cur_inst_ptr, qsh_audio_data_get_sensor_uid(this), true)))
   {
      this->cb->remove_instance(cur_inst_ptr);
      cur_inst_ptr = NULL;
   }

   // If there is no instance then unblock power collapse
   if (NULL == this->cb->get_sensor_instance(this, true))
   {
      qsh_audio_event_unblock_power_collapse(this);
   }
   else
   {
      qsh_audio_event_block_power_collapse(this);
   }

   return (SNS_RC_SUCCESS == err) ? cur_inst_ptr : NULL;
}
