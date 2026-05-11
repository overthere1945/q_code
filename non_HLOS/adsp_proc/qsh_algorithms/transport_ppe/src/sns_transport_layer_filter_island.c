/*=============================================================================
  @file sns_tppe_transport_layer_filter_island.c

  Transport layer filter

  Copyright (c) 2022-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Includes
  ===========================================================================*/
#include "sns_crc_util.h"
#include "sns_mem_util.h"
#include "sns_transport_layer_filter.h"
#include "sns_types.h"
#include <stdbool.h>
#include <string.h>

/*=============================================================================
  Static Function Definitions
  ===========================================================================*/

/* 1. Loop through all property and data filters
 * 2. Reset the matched flags
 */
static void reset_filter_match_flags(sns_all_filters *filters)
{
  size_t total_pf_count = filters->prop_filter_count + filters->dup_pf_count;
  size_t total_mdf_count = filters->manuf_df_count + filters->dup_mdf_count;
  size_t total_sdf_count = filters->service_df_count + filters->dup_sdf_count;
  for(int i = 0; i < total_pf_count; i++)
  {
    filters->prop_filters[i].matched = false;
  }

  for(int i = 0; i < total_mdf_count; i++)
  {
    filters->manuf_data_filters[i].matched = false;
  }

  for(int i = 0; i < total_sdf_count; i++)
  {
    filters->service_data_filters[i].matched = false;
  }
}

/* 1. Start at the current pointer for data_filter provided as function parameter
 * 2. Check if the next 'n' data_filters have the matched flag set to true
 */
static bool check_n_data_match(sns_data_filter *data_filter, int n)
{
  bool match = true;
  for(int i = 0; i < n; i++)
  {
    if(!data_filter[i].matched)
    {
      match = false;
      break;
    }
  }
  return match;
}

/* 1. Start at the current pointer for prop_filter provided as function parameter
 * 2. Check if the next 'n' prop_filters have the matched flag set to true
 */
static bool check_n_prop_match(sns_prop_filter *prop_filter, int n)
{
  bool match = true;
  for(int i = 0; i < n; i++)
  {
    if(!prop_filter[i].matched)
    {
      match = false;
      break;
    }
  }
  return match;
}

static void calculate_crc(sns_transport_decode_buffer *buffer,
                          sns_tppe_event_trigger *event_trigger)
{
  size_t crc_buffer_len =
      buffer->data.decoded_raw_adv_data_len + buffer->data.transport_address_len;
  if(buffer->data.decoded_raw_adv_data)
  {
    event_trigger->crc = sns_crc_32_calc(buffer->data.decoded_raw_adv_data, crc_buffer_len);
  }
}
/*
 * 1. Maintain indices for prop filter and data filter
 *    dup filters will be placed after the wakeup filters in the master array
 *    so use the wakeup filter counts as the offset
 * 2. Loop through all triggers provided by client
 * 3. For every trigger, check the match flags in the next prop filter count and
 *    data filter count maintained in the trigger state
 * 4. If match is found, update trigger id
 * 5. Increment the prop filter and data filter indices by the corresponding
 *    counts
 */
static void populate_dup_trigger_ids(sns_all_filters *filters,
                                     sns_tppe_event_trigger *event_trigger)
{
  int prop_filter_idx = filters->prop_filter_count;
  int manuf_df_idx = filters->manuf_df_count;
  int service_df_idx = filters->service_df_count;
  bool trigger_satisfied = false;

  for(int i = 0; i < filters->trigger_count; i++)
  {
    sns_single_trigger *trigger = &filters->wakeup_triggers[i];
    if(trigger->dup_pf_count &&
       check_n_prop_match(&filters->prop_filters[prop_filter_idx], trigger->dup_pf_count))
    {
      trigger_satisfied = true;
    }
    if(trigger->dup_mdf_count &&
       check_n_data_match(&filters->manuf_data_filters[manuf_df_idx], trigger->dup_mdf_count))
    {
      trigger_satisfied = true;
    }
    if(trigger->dup_sdf_count &&
       check_n_data_match(&filters->service_data_filters[service_df_idx], trigger->dup_sdf_count))
    {
      trigger_satisfied = true;
    }
    if(trigger_satisfied)
    {
      event_trigger->dup_trigger_ids[event_trigger->dup_trigger_id_count] = trigger->trigger_id;
      event_trigger->dup_trigger_id_count++;
      trigger_satisfied = false;
    }
    prop_filter_idx += trigger->dup_pf_count;
    manuf_df_idx += trigger->dup_mdf_count;
    service_df_idx += trigger->dup_sdf_count;
  }
}
/*
 * 1. Maintain indices for prop filter and data filter
 * 2. Loop through all triggers provided by client
 * 3. For every trigger, check the match flags in the next prop filter count and
 *    data filter count maintained in the trigger state
 * 4. If match is found, update trigger id
 * 5. Increment the prop filter and data filter indices by the corresponding
 *    counts
 */
static void populate_trigger_ids(sns_all_filters *filters, sns_tppe_event_trigger *event_trigger)
{
  int prop_filter_idx = 0;
  int manuf_df_idx = 0;
  int service_df_idx = 0;

  for(int i = 0; i < filters->trigger_count; i++)
  {
    sns_single_trigger *trigger = &filters->wakeup_triggers[i];
    bool prop_match =
        check_n_prop_match(&filters->prop_filters[prop_filter_idx], trigger->prop_filter_count);
    bool manuf_data_match =
        check_n_data_match(&filters->manuf_data_filters[manuf_df_idx], trigger->manuf_df_count);
    bool service_data_match = check_n_data_match(&filters->service_data_filters[service_df_idx],
                                                 trigger->service_df_count);
    if(prop_match && manuf_data_match && service_data_match)
    {
      event_trigger->trigger_ids[event_trigger->trigger_id_count] = trigger->trigger_id;
      event_trigger->trigger_id_count++;
    }
    prop_filter_idx += trigger->prop_filter_count;
    manuf_df_idx += trigger->manuf_df_count;
    service_df_idx += trigger->service_df_count;
  }
}

static bool match_data_bytes(sns_data_filter *req_data_filter,
                             sns_transport_data_list_item *event_data_filter)
{
  bool match = true;
  uint8_t xor_value = 0;
  uint8_t *req_data_value = req_data_filter->value;
  uint8_t *req_data_mask = req_data_filter->mask;
  uint8_t *event_data_value = event_data_filter->buffer;

  for(int byte_idx = 0; byte_idx < req_data_filter->bytes_size; byte_idx++)
  {
    xor_value = (event_data_value[byte_idx] ^ req_data_value[byte_idx]) & req_data_mask[byte_idx];
    match &= (0 == xor_value);
    if(!match)
    {
      break;
    }
  }
  return match;
}

static void filter_data_for_list(sns_data_filter *req_data_filters, size_t req_data_filters_count,
                                 sns_list *event_data_filters)
{
  for(int req_data_idx = 0; req_data_idx < req_data_filters_count; req_data_idx++)
  {
    sns_list_iter iter;
    sns_transport_data_list_item *event_data_filter_item = NULL;
    for(event_data_filter_item =
            (sns_transport_data_list_item *)sns_list_iter_init(&iter, event_data_filters, true);
        NULL != event_data_filter_item;
        event_data_filter_item = (sns_transport_data_list_item *)sns_list_iter_advance(&iter))
    {
      if((0 == sns_memcmp(&event_data_filter_item->id, &req_data_filters[req_data_idx].id,
                          sizeof(sns_transport_ppe_id))) &&
         match_data_bytes(&req_data_filters[req_data_idx], event_data_filter_item))
      {
        req_data_filters[req_data_idx].matched = true;
      }
    }
  }
}

/*
 * 1. Event : Decoded buffer has lists of decoded manuf data and service data
 * 2. Request : All triggers has lists for decoded manuf data and service data
 * 3. For both manuf and servie data, loop through all the items in the request
 *    and events list perform a match
 * 4. Perform a match for the items as follows :
 *    a. If the data_id does not match, the data items are not matched
 *    b. Use the mask and value in request to compare against the corresponding
 *       value in the event
 *       i. If the bits match, data items are matched
 *       ii. If the bits don't match, data items are not matched
 * 5. Bit matching is done as follows : mask & (value ^ decoded_data_buff) for
 *    all bytes
 * 6. Set the match value in the data_req
 */
static void filter_data(sns_transport_layer_api *api, sns_transport_decode_buffer *buffer,
                        sns_all_filters *filters)
{
  UNUSED_VAR(api);

  size_t total_mdf_count = filters->manuf_df_count + filters->dup_mdf_count;
  size_t total_sdf_count = filters->service_df_count + filters->dup_sdf_count;
  filter_data_for_list(filters->manuf_data_filters, total_mdf_count, &buffer->data.manuf_data_list);
  filter_data_for_list(filters->service_data_filters, total_sdf_count,
                       &buffer->data.service_data_list);
}

/*
 * 1. Loop through all the properties in the decode buffer
 * 2. Loop through all properties in the trigger buffer
 * 3. Prop type is matched as follows
 *    a. If prop type does not match, properties are not matched
 *    b. Use threshold type to compare values
 *      i. If threshold is not satisfied, properties are not matched
 *      ii. If threshold is satisfied, properties are matched
 * 4. Set the match value in prop_req accordingly
 */
static void filter_prop(sns_transport_layer_api *api, sns_transport_decode_buffer *buffer,
                        sns_all_filters *filters)
{
  sns_transport_buf_prop *prop_buf = &buffer->prop;
  size_t total_pf_count = filters->prop_filter_count + filters->dup_pf_count;
  for(int buf_prop_idx = 0; buf_prop_idx < prop_buf->prop_cnt; buf_prop_idx++)
  {
    sns_transport_property *prop_event = &prop_buf->prop[buf_prop_idx];
    for(int trig_prop_idx = 0; trig_prop_idx < total_pf_count; trig_prop_idx++)
    {
      sns_transport_property *prop_req = &filters->prop_filters[trig_prop_idx].prop_filter.prop;
      sns_transport_ppe_threshold threshold =
          filters->prop_filters[trig_prop_idx].prop_filter.threshold;

      if(prop_event->which_prop == prop_req->which_prop &&
         api->compare_prop(prop_event, prop_req, threshold))
      {
        filters->prop_filters[trig_prop_idx].matched = true;
      }
      else if(prop_req->which_prop == ALWAYS_TRUE_PROP_ID)
      {
        filters->prop_filters[trig_prop_idx].matched = true;
      }
    }
  }
}

/*
 * 1. Use the previous trigger id count to reset memory for trigger id array
 * 2. Set the trigger id count to 0
 */
static void reset_event_triggers(sns_tppe_event_trigger *event_trigger)
{
  sns_memzero(event_trigger->trigger_ids, event_trigger->trigger_id_count * sizeof(uint32_t));
  event_trigger->trigger_id_count = 0;
  sns_memzero(event_trigger->dup_trigger_ids,
              event_trigger->dup_trigger_id_count * sizeof(uint32_t));
  event_trigger->dup_trigger_id_count = 0;
}

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

/*
 * 1. reset trigger ids populated from previous event
 * 2. filter properties
 * 3. filter data
 * 4. populate trigger ids
 */
void sns_transport_filter(sns_transport_layer_api *api, sns_transport_decode_buffer *buffer,
                          sns_all_filters *filters, sns_tppe_event_trigger *event_trigger)
{
  reset_event_triggers(event_trigger);
  reset_filter_match_flags(filters);
  filter_prop(api, buffer, filters);
  filter_data(api, buffer, filters);
  populate_trigger_ids(filters, event_trigger);
  populate_dup_trigger_ids(filters, event_trigger);
  calculate_crc(buffer, event_trigger);
}
