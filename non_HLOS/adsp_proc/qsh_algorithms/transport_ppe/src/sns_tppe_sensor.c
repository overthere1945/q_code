/**============================================================================
  @file sns_tppe_sensor.c

  @brief The tppe virtual Sensor implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_attribute_util.h"
#include "sns_diag_service.h"
#include "sns_memory_service.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_qshtech_island.h"
#include "sns_registry.pb.h"
#include "sns_sensor_util.h"
#include "sns_service_manager.h"
#include "sns_stream_service.h"
#include "sns_suid.pb.h"
#include "sns_tppe_sensor_instance.h"
#include "sns_tppe_sensor.h"
#include "sns_tppe_struct_extender.h"
#include "sns_types.h"

/*=============================================================================
  Extern data
  ===========================================================================*/

extern const sns_sensor_uid tppe_suid;

/*=============================================================================
  Static Function Definitions
  ===========================================================================*/

/**
 * Initialize attributes to their default state.  They may/will be updated
 * within tppe_notify.
 */
static void publish_attributes(sns_sensor *const this)
{
  {
    char const name[] = "sns_transport_ppe";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = name, .buf_len = sizeof(name)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
  }
  {
    char const type[] = "transport_ppe";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = type, .buf_len = sizeof(type)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_TYPE, &value, 1, false);
  }
  {
    char const vendor[] = "QTI";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = vendor, .buf_len = sizeof(vendor)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VENDOR, &value, 1, false);
  }
  {
    char const proto_files[] = "sns_transport_ppe.proto";
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((pb_buffer_arg){.buf = proto_files, .buf_len = sizeof(proto_files)});
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_API, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = 1;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VERSION, &value, 1, false);
  }
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = SNS_STD_SENSOR_STREAM_TYPE_ON_CHANGE;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_STREAM_TYPE, &value, 1, true);
  }
}

static void publish_available(sns_sensor *const this, bool available)
{
  sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
  value.has_boolean = true;
  value.boolean = available;
  sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_AVAILABLE, &value, 1, true);
  SNS_PRINTF(HIGH, this, "Publish available : %u", available);
}

static void reg_config_set_filter_msg_id(sns_tppe_registry_config *reg_config, int data)
{
  if(reg_config->msg_id_cnt < SNS_TPPE_MAX_MSG_ID_LEN)
  {
    reg_config->msg_ids[reg_config->msg_id_cnt] = data;
    reg_config->msg_id_cnt++;
  }
}
static void reg_config_set_crc_cache_max_len(sns_tppe_registry_config *reg_config, int data)
{
  reg_config->crc_cache_max_len = data;
}

static void reg_config_set_batch_period(sns_tppe_registry_config *reg_config, int data)
{
  reg_config->batch_period = data;
}

static void reg_config_set_api(sns_tppe_registry_config *reg_config, char *data)
{
  sns_strlcpy(reg_config->api, data, sizeof(reg_config->api));
}

static void reg_config_set_type(sns_tppe_registry_config *reg_config, char *data)
{
  sns_strlcpy(reg_config->type, data, sizeof(reg_config->type));
}

static bool parse_registry(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
  UNUSED_VAR(field);
  sns_tppe_registry_config *reg_config = (sns_tppe_registry_config *)*arg;
  bool decoded = false;
  bool match = false;

  const struct
  {
    char *reg_item_name;
    void (*set_string_fcn)(sns_tppe_registry_config *, char *);
    void (*set_int_fcn)(sns_tppe_registry_config *, int);
  } tppe_reg_items[] = {{"type", reg_config_set_type, NULL},
                        {"api", reg_config_set_api, NULL},
                        {"batch_period_us", NULL, reg_config_set_batch_period},
                        {"crc_cache_max_len", NULL, reg_config_set_crc_cache_max_len},
                        {"filter_msg_id", NULL, reg_config_set_filter_msg_id}};

  pb_buffer_arg item_name = {0, 0};
  pb_buffer_arg item_string = {0, 0};
  sns_registry_data_item reg_item = sns_registry_data_item_init_default;
  pb_istream_t stream_cpy = *stream;

  reg_item.name.arg = &item_name;
  reg_item.name.funcs.decode = pb_decode_string_cb;
  reg_item.str.arg = &item_string;
  reg_item.str.funcs.decode = pb_decode_string_cb;

  decoded = pb_decode_noinit(stream, sns_registry_data_item_fields, &reg_item);

  if(reg_item.has_subgroup)
  {
    sns_registry_data_item temp_item = sns_registry_data_item_init_default;
    temp_item.subgroup.items.funcs.decode = parse_registry;
    temp_item.subgroup.items.arg = *arg;
    pb_decode_noinit(&stream_cpy, sns_registry_data_item_fields, &temp_item);
  }
  else if(decoded)
  {
    for(int i = 0; i < ARR_SIZE(tppe_reg_items); i++)
    {
      match = (0 == strncmp((char *)item_name.buf, tppe_reg_items[i].reg_item_name,
                            strlen(tppe_reg_items[i].reg_item_name)));

      if(match)
      {
        if(reg_item.has_sint)
        {
          tppe_reg_items[i].set_int_fcn(reg_config, reg_item.sint);
        }
        else if(0 != item_string.buf_len)
        {
          tppe_reg_items[i].set_string_fcn(reg_config, (char *)item_string.buf);
        }
        break;
      }
    }
  }
  return decoded;
}

/* Handle registry event */
static void handle_registry_event(sns_sensor *const this)
{
  sns_tppe_sensor_state *state = (sns_tppe_sensor_state *)this->state->state;
  if(NULL != state->registry_stream)
  {
    sns_sensor_event *event;
    for(event = state->registry_stream->api->peek_input(state->registry_stream); NULL != event;
        event = state->registry_stream->api->get_next_input(state->registry_stream))
    {
      if(SNS_REGISTRY_MSGID_SNS_REGISTRY_READ_EVENT == event->message_id)
      {
        pb_istream_t stream = pb_istream_from_buffer((void *)event->event, event->event_len);
        sns_registry_read_event read_event = sns_registry_read_event_init_default;
        pb_buffer_arg group_name = {0, 0};
        read_event.name.funcs.decode = pb_decode_string_cb;
        read_event.name.arg = &group_name;
        read_event.data.items.funcs.decode = parse_registry;
        read_event.data.items.arg = &state->reg_config;

        if(!pb_decode_noinit(&stream, sns_registry_read_event_fields, &read_event))
        {
          SNS_PRINTF(ERROR, this, "Error decoding registry event");
        }
        else
        {
          // add suid lookup request for reg config data type
          sns_suid_lookup_add_v2(this, &state->transport_suid_lookup_data, state->reg_config.type,
                                 true);
        }
      }
      else
      {
        SNS_PRINTF(ERROR, this, "Unknown event from registry stream received %d",
                   event->message_id);
      }
    }
  }
}

static void send_registry_request(sns_sensor *const this)
{
  sns_service_manager *service_mgr = this->cb->get_service_manager(this);
  sns_stream_service *stream_service =
      (sns_stream_service *)service_mgr->get_service(service_mgr, SNS_STREAM_SERVICE);
  sns_tppe_sensor_state *state = (sns_tppe_sensor_state *)this->state->state;
  sns_registry_read_req sns_tppe_registry_req = sns_registry_read_req_init_default;
  sns_std_request std_req = sns_std_request_init_default;
  uint8_t buffer[100];
  size_t encoded_len = 0;

  if(NULL == state->registry_stream)
  {
    sns_sensor_uid registry_suid;
    sns_suid_lookup_get(&state->tppe_suid_lookup_data, "registry", &registry_suid);
    // Create a data stream to Registry
    stream_service->api->create_sensor_stream(stream_service, this, registry_suid,
                                              &state->registry_stream);
  }

  // Send a request to registry
  pb_buffer_arg data =
      (pb_buffer_arg){.buf = "sns_tppe_ble", .buf_len = (strlen("sns_tppe_ble") + 1)};

  sns_tppe_registry_req.name.arg = &data;
  sns_tppe_registry_req.name.funcs.encode = pb_encode_string_cb;

  encoded_len = pb_encode_request(buffer, sizeof(buffer), &sns_tppe_registry_req,
                                  sns_registry_read_req_fields, &std_req);
  if(0 < encoded_len && NULL != state->registry_stream)
  {
    sns_request request = (sns_request){.message_id = SNS_REGISTRY_MSGID_SNS_REGISTRY_READ_REQ,
                                        .request_len = encoded_len,
                                        .request = buffer};
    state->registry_stream->api->send_request(state->registry_stream, &request);
  }
}

/* See sns_sensor::get_sensor_uid */
static sns_sensor_uid const *sns_tppe_get_sensor_uid(sns_sensor const *this)
{
  UNUSED_VAR(this);
  return &tppe_suid;
}

static bool transport_suid_lookup_cb(sns_sensor *const this, char const *data_type,
                                     sns_sensor_event *event)
{
  sns_tppe_sensor_state *state = (sns_tppe_sensor_state *)this->state->state;
  pb_istream_t stream = pb_istream_from_buffer((void *)event->event, event->event_len);
  sns_std_attr_event attr_event = sns_std_attr_event_init_default;
  sns_sensor_util_attrib attrib_list[] = {
      {.sensor = this, .attr_id = SNS_STD_SENSOR_ATTRID_API},
      {.sensor = this, .attr_id = SNS_STD_SENSOR_ATTRID_TRANSPORT_MTU_SIZE},
      {.sensor = NULL, .attr_id = -1}};

  bool ret = false;

  if(0 == strcmp(data_type, state->reg_config.type))
  {
    ret = true;
    attr_event.attributes.funcs.decode = &sns_sensor_util_decode_attr_list;
    attr_event.attributes.arg = (void *)&attrib_list;

    if(!pb_decode_noinit(&stream, sns_std_attr_event_fields, &attr_event))
    {
      SNS_PRINTF(ERROR, this, "Error decoding attr event");
      ret = false;
    }
    if(ret)
    {
      for(int i = 0; i < ARR_SIZE(attrib_list); i++)
      {
        if(SNS_STD_SENSOR_ATTRID_API == attrib_list[i].attr_id)
        {
          ret = (0 == strcmp(state->reg_config.api, attrib_list[i].attr_string));
        }
        else if(SNS_STD_SENSOR_ATTRID_TRANSPORT_MTU_SIZE == attrib_list[i].attr_id)
        {
          state->raw_adv_data_max_len = attrib_list[i].attr_value;
        }
      }
    }
  }

  return ret;
}

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

/* See sns_sensor::init */
sns_rc sns_tppe_init(sns_sensor *const this)
{
  sns_rc rc = SNS_RC_SUCCESS;
  sns_tppe_sensor_state *state = (sns_tppe_sensor_state *)this->state->state;
  SNS_SUID_LOOKUP_INIT(state->tppe_suid_lookup_data, NULL);
  SNS_SUID_LOOKUP_INIT(state->tppe_batch_suid_lookup_data, NULL);
  SNS_SUID_LOOKUP_INIT(state->transport_suid_lookup_data, transport_suid_lookup_cb);
  sns_suid_lookup_add(this, &state->tppe_suid_lookup_data, "registry");
  sns_suid_lookup_add(this, &state->tppe_batch_suid_lookup_data, "batch");

  // Register island memory vote callback only if island flag is enabled
#ifdef SNS_ISLAND_INCLUDE_TPPE
  sns_service_manager *service_mgr = NULL;
  sns_memory_service *memory_service = NULL;
  service_mgr = this->cb->get_service_manager(this);
  memory_service = (sns_memory_service *)service_mgr->get_service(service_mgr, SNS_MEMORY_SERVICE);

  if(NULL != memory_service)
  {
    memory_service->api->register_island_memory_vote_cb(this, &sns_qshtech_island_memory_vote);
  }
#endif // SNS_ISLAND_INCLUDE_TPPE

  publish_attributes(this);
  return rc;
}

/* See sns_sensor::deinit */
sns_rc sns_tppe_deinit(sns_sensor *const this)
{
  sns_tppe_sensor_state *state = (sns_tppe_sensor_state *)this->state->state;
  sns_suid_lookup_deinit(this, &state->tppe_suid_lookup_data);
  sns_suid_lookup_deinit(this, &state->tppe_batch_suid_lookup_data);
  sns_suid_lookup_deinit(this, &state->transport_suid_lookup_data);
  sns_sensor_util_remove_sensor_stream(this, &state->registry_stream);
  return SNS_RC_SUCCESS;
}

/*
 * 1. Check if suid lookup is completed for tppe and transport
 * 2. Handle any outstanding events on suid/ attribute streams
 * 3. Check if suid lookup complete flag has toggled
 * 4. If toggled,
 *    a. tppe : send registry request and deinit lookup request
 *    b. transport : publish available
 * 5. Handle registry events
 */
sns_rc sns_tppe_notify_event(sns_sensor *const this)
{
  sns_tppe_sensor_state *state = (sns_tppe_sensor_state *)this->state->state;
  bool tppe_completed = sns_suid_lookup_complete(&state->tppe_suid_lookup_data);
  bool transport_completed_before_lookup =
      sns_suid_lookup_complete(&state->transport_suid_lookup_data);
  bool transport_completed_after_lookup = false;
  sns_suid_lookup_handle(this, &state->tppe_suid_lookup_data);
  sns_suid_lookup_handle(this, &state->tppe_batch_suid_lookup_data);
  sns_suid_lookup_handle(this, &state->transport_suid_lookup_data);

  if(tppe_completed != sns_suid_lookup_complete(&state->tppe_suid_lookup_data))
  {
    send_registry_request(this);
    sns_suid_lookup_deinit(this, &state->tppe_suid_lookup_data);
  }

  transport_completed_after_lookup = sns_suid_lookup_complete(&state->transport_suid_lookup_data);
  if(transport_completed_before_lookup != transport_completed_after_lookup)
  {
    // do not deinitialize suid lookup for transport layers
    // the transport layers can potentially publish unavailable/ available in
    // run time
    publish_available(this, transport_completed_after_lookup);
  }

  handle_registry_event(this);

  return SNS_RC_SUCCESS;
}

static void populate_prop_id_decode_buff(sns_transport_decode_buffer *decode_buffer)
{
  for(int i = 1; i <= SNS_TRANSPORT_PPE_PROP_COUNT; i++)
  {
    decode_buffer->prop.prop[i - 1].which_prop = i;
  }
}

/**
 * @name setup_struct_extender
 * @param [i] sizing
 * @param [o] extender_out
 * @return void
 *
 * @brief
 * Instead of allocating each item separately,
 * A bigger chunk of memory will be malloc'd &
 * It will be divided into smaller parts in this order
 * This functions uses the struct extender utility to setup the buffer in the
 * order mentioned above.
 **/
void setup_struct_extender(sns_triggers_arg_sizing *sizing, simple_struct_extender *extender_out)
{
  size_t total_bytes_count = (sizing->service_df_bytes_count + sizing->manuf_df_bytes_count) * 2;
  size_t total_pf_count = sizing->prop_filter_count + sizing->dup_pf_count;
  size_t total_sdf_count = sizing->service_df_count + sizing->dup_sdf_count;
  size_t total_mdf_count = sizing->manuf_df_count + sizing->dup_mdf_count;
  size_t total_crc_data_len = sizing->decoded_raw_adv_data_len + sizing->transport_address_len;

  sstruct_init(extender_out, 0);
  // 1 property filters
  sstruct_append_item_with_size(extender_out, sizeof(sns_prop_filter) * total_pf_count);
  // 2 service data filters
  sstruct_append_item_with_size(extender_out, sizeof(sns_data_filter) * total_sdf_count);
  // 3 manuf_data_filters
  sstruct_append_item_with_size(extender_out, sizeof(sns_data_filter) * total_mdf_count);
  // 4 value_mask_array
  sstruct_append_item_with_size(extender_out, total_bytes_count);
  // 5 Event Trigger Id Array
  sstruct_append_item_with_size(extender_out, sizeof(uint32_t) * sizing->trigger_count);
  // 6 Unique Property Ids
  sstruct_append_item_with_size(extender_out,
                                sizeof(sns_transport_property) * SNS_TRANSPORT_PPE_PROP_COUNT);
  // 7 decoded_raw_adv_data
  sstruct_append_item_with_size(extender_out, total_crc_data_len);
  // 8 wakeup triggers
  sstruct_append_item_with_size(extender_out, sizing->trigger_count * sizeof(sns_single_trigger));
  // 9 dup Trigger Id Array
  sstruct_append_item_with_size(extender_out, sizeof(uint32_t) * sizing->trigger_count);
}

/**
 * @name setup_instance_state_variables
 * @param [i] inst_state : The state of the instance to be updated
 * @param [i] sizing : A structure holding sizes of decoded request
 * @param [i] extender : a struct extender that has been setup already
 * @param [i] mem_block : allocated memory to hold elements of struct extender
 * @return void
 *
 * @brief
 * Is called after an instance is created. This function will setup variables
 * in the instance state with valid pointers from the tppe mem block.
 **/
void setup_instance_state_variables(sns_tppe_inst_state *inst_state,
                                    sns_triggers_arg_sizing *sizing,
                                    simple_struct_extender *extender, void *mem_block)
{
  sns_all_filters *all_filters = &inst_state->all_filters;
  sns_tppe_event_trigger *event_triggers = &inst_state->event_triggers;
  sns_transport_decode_buffer *decode_buffer = &inst_state->decode_buffer;

  inst_state->tppe_config_memory_block = mem_block;
  inst_state->dup_detection_required = sizing->dup_detection_required;
  sstruct_set_parent_pointer(extender, mem_block);

  // Reset all_filters structure
  sns_memset(all_filters, 0, sizeof(sns_all_filters));

  // Provide indices in the same order as in setup_struct_extender
  all_filters->prop_filters = sstruct_get_address_for_item(extender, 1);
  all_filters->service_data_filters = sstruct_get_address_for_item(extender, 2);
  all_filters->manuf_data_filters = sstruct_get_address_for_item(extender, 3);
  all_filters->value_mask_array = sstruct_get_address_for_item(extender, 4);

  event_triggers->trigger_ids = sstruct_get_address_for_item(extender, 5);
  event_triggers->trigger_id_count = 0;

  decode_buffer->prop.prop = sstruct_get_address_for_item(extender, 6);
  decode_buffer->prop.prop_cnt = SNS_TRANSPORT_PPE_PROP_COUNT;
  sns_list_init(&decode_buffer->data.manuf_data_list);
  sns_list_init(&decode_buffer->data.service_data_list);
  decode_buffer->data.max_data_filter_bytes = sizing->max_data_filter_bytes;
  decode_buffer->data.decoded_raw_adv_data = sstruct_get_address_for_item(extender, 7);
  decode_buffer->data.decoded_raw_adv_data_len = sizing->decoded_raw_adv_data_len;
  decode_buffer->data.transport_address_len = sizing->transport_address_len;
  populate_prop_id_decode_buff(decode_buffer);

  all_filters->wakeup_triggers = sstruct_get_address_for_item(extender, 8);

  event_triggers->dup_trigger_ids = sstruct_get_address_for_item(extender, 9);
  event_triggers->dup_trigger_id_count = 0;

  all_filters->prop_filter_count = sizing->prop_filter_count;
  all_filters->service_df_count = sizing->service_df_count;
  all_filters->manuf_df_count = sizing->manuf_df_count;
  all_filters->trigger_count = sizing->trigger_count;
  all_filters->dup_pf_count = sizing->dup_pf_count;
  all_filters->dup_sdf_count = sizing->dup_sdf_count;
  all_filters->dup_mdf_count = sizing->dup_mdf_count;
}

/**
 * @name handle_tppe_config_request
 * @param[i] this : sensor pointer
 * @param[i] req :  request from the client
 * @param[i] cur_instance: Instance for which the request is destined. Can be
 *          NULL, if NULL then it is a new request, if it is valid pointer then
 *          it is a reconfig request.
 * @return  NULL if no action is taken for this request, valid instance pointer
 *          in which triggers were saved.
 *
 * @brief
 *  Called when tppe config is received.
 *  Creates instance if cur_instance is NULL.
 *  Allocates memory for tppe config triggers. If request handling is,
 *  1. Succesful: return valid instance pointer in which triggers were saved.
 *  2. Unsuccesful: return NULL
 **/
sns_sensor_instance *handle_tppe_config_request(sns_sensor *const this, sns_request const *req,
                                                sns_sensor_instance *cur_inst)
{
  sns_sensor_instance *new_inst = cur_inst;
  void *new_mem_block = NULL;
  void *old_mem_block = NULL;
  bool decode_status = false;
  sns_triggers_arg_sizing sizing;
  sns_tppe_inst_state *inst_state;
  size_t tppe_config_size = 0;
  simple_struct_extender extender;
  sns_service_manager *manager = this->cb->get_service_manager(this);
  sns_memory_service *memory_service =
      (sns_memory_service *)manager->get_service(manager, SNS_MEMORY_SERVICE);

  sns_tppe_sensor_state *state = ((sns_tppe_sensor_state *)this->state->state);

  decode_status = decode_tppe_config_sizing(req, &sizing);
  sizing.decoded_raw_adv_data_len = state->raw_adv_data_max_len;
  sizing.transport_address_len =
      sns_transport_layer_get_prop_len(sns_transport_property_device_addr_tag);

  if(!decode_status)
  {
    return NULL;
  }

  setup_struct_extender(&sizing, &extender);
  tppe_config_size = sstruct_get_total_size(&extender);
  if(tppe_config_size > SNS_TPPE_MAX_TRIGGERS_SIZE)
  {
    SNS_PRINTF(ERROR, this, "TPPE Size limit Exceeded %d", tppe_config_size);
    return NULL;
  }

  if(NULL == new_inst)
  {
    new_inst = this->cb->create_instance(this, sizeof(sns_tppe_inst_state));
  }
  if(new_inst)
  {
    inst_state = (sns_tppe_inst_state *)new_inst->state->state;
    new_mem_block = sns_tppe_memory_service_malloc(memory_service, new_inst, tppe_config_size);
    old_mem_block = (cur_inst) ? inst_state->tppe_config_memory_block : NULL;
    if(new_mem_block || 0 == tppe_config_size)
    {
      if(old_mem_block)
      {
        memory_service->api->free(new_inst, old_mem_block);
      }
      setup_instance_state_variables(inst_state, &sizing, &extender, new_mem_block);
      inst_state->crc_cache_max_len = state->reg_config.crc_cache_max_len;
    }
    else
    {
      // Not enough memory, reject request, delete instance iff its created for
      // this request.
      SNS_PRINTF(ERROR, this, "Out of Memory for tppe configuration");
      if(NULL == cur_inst)
      {
        inst_state->tppe_config_memory_block = NULL;
        this->cb->remove_instance(new_inst);
      }
      new_inst = NULL; // reject request
    }
  }
  return new_inst;
}

/**
 * @name validate_request
 * @param this sensor reference
 * @param curr_req: old request if any
 * @param new_req: new request
 *
 * @return true if request is valid, false otherwise
 *
 * @brief
 * first request to any instance should be tppe config
 * if curr_req is null and new req is not config -> invalid
 * first request cannot be flush request
 *
 * @note
 *
 * **/

static bool validate_request(sns_sensor *const this, sns_request const *curr_req,
                             sns_request const *new_req)
{
  bool retval = true;
  if(new_req->message_id != SNS_TRANSPORT_PPE_MSGID_SNS_TRANSPORT_PPE_CONFIG && curr_req == NULL)
  {
    SNS_PRINTF(ERROR, this, "First request should be tppe config %d", new_req->message_id);
    retval = false;
  }
  return retval;
}

/**
 * For SNS_TRANSPORT_PPE_MSGID_SNS_TRANSPORT_PPE_CONFIG
 * decode message  for sizes
 * find out instance state size
 * create instance
 */
sns_sensor_instance *sns_tppe_set_client_request(sns_sensor *const this,
                                                 sns_request const *curr_req,
                                                 sns_request const *new_req, bool remove)
{
  SNS_TPPE_PRINTF(HIGH, this, "set_client_req : current : %i, new : %i, remove : %i", curr_req,
                  new_req, remove);
  sns_sensor_instance *curr_inst =
      sns_sensor_util_find_instance(this, curr_req, this->sensor_api->get_sensor_uid(this));
  sns_sensor_instance *new_inst = curr_inst;
  sns_rc rc = SNS_RC_SUCCESS;

  if(remove)
  {
    if(curr_inst)
    {
      curr_inst->cb->remove_client_request(curr_inst, curr_req);
    }
  }
  else
  {
    if(validate_request(this, curr_req, new_req))
    {

      if(new_req->message_id == SNS_TRANSPORT_PPE_MSGID_SNS_TRANSPORT_PPE_CONFIG)
      {
        new_inst = handle_tppe_config_request(this, new_req, curr_inst);
        if(!new_inst)
        {
          rc = SNS_RC_NOT_SUPPORTED;
        }
      }
      if(new_inst)
      {
        // new_inst will always be active instance both for new and reconfig
        // reqs
        rc = this->instance_api->set_client_config(new_inst, new_req);
        if(SNS_RC_SUCCESS == rc &&
           new_req->message_id == SNS_TRANSPORT_PPE_MSGID_SNS_TRANSPORT_PPE_CONFIG)
        {
          new_inst->cb->add_client_request(new_inst, new_req);
          if(curr_req)
          {
            new_inst->cb->remove_client_request(new_inst, curr_req);
          }
        }
      }
    }
    else
    {
      rc = SNS_RC_NOT_SUPPORTED;
    }
  }

  if(new_inst && NULL == new_inst->cb->get_client_request(
                             new_inst, this->sensor_api->get_sensor_uid(this), true))
  {
    this->cb->remove_instance(new_inst);
  }

  return (rc == SNS_RC_SUCCESS) ? new_inst : NULL;
}

/*===========================================================================
  Sensor API
  ===========================================================================*/

const sns_sensor_api sns_tppe_api = {
    .struct_len = sizeof(sns_sensor_api),
    .init = &sns_tppe_init,
    .deinit = &sns_tppe_deinit,
    .get_sensor_uid = &sns_tppe_get_sensor_uid,
    .set_client_request = &sns_tppe_set_client_request,
    .notify_event = &sns_tppe_notify_event,
};
