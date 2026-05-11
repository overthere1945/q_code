/*=============================================================================
  @file sns_tppe_request_handler.c

  Contains functions to test tppe config message

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/
#include "sns_tppe_request_handler.h"

#include "sns_math_ops.h"
#include "sns_mem_util.h"

static void copy_prop_filter(const sns_transport_ppe_filter *filter,
                             sns_tppe_config_decode_arg *decode_arg, size_t index)
{
  sns_transport_ppe_prop_filter *prop_filter = &decode_arg->prop_filters[index].prop_filter;
  sns_memscpy(prop_filter, sizeof(sns_transport_ppe_prop_filter), &filter->prop_filter,
              sizeof(sns_transport_ppe_prop_filter));
}

static void add_always_true_pf(sns_tppe_config_decode_arg *decode_arg, size_t index)
{
  sns_transport_ppe_prop_filter *prop_filter = &decode_arg->prop_filters[index].prop_filter;
  sns_transport_ppe_prop_filter filter = sns_transport_ppe_prop_filter_init_default;
  filter.prop.which_prop = 0; // for always true this is the prop type
  sns_memscpy(prop_filter, sizeof(sns_transport_ppe_prop_filter), &filter,
              sizeof(sns_transport_ppe_prop_filter));
}

static void copy_service_data_filter(const sns_transport_ppe_filter *filter,
                                     sns_tppe_config_decode_arg *decode_arg, size_t index)
{

  size_t write_index = decode_arg->bytes_array_write_index;
  sns_data_filter *df = &decode_arg->service_data_filters[index];
  pb_buffer_arg *value_arg = (pb_buffer_arg *)filter->service_data_filter.value.arg;
  pb_buffer_arg *mask_arg = (pb_buffer_arg *)filter->service_data_filter.mask.arg;

  df->bytes_size = value_arg->buf_len;
  df->id.id_low = filter->service_data_filter.id.id_low;
  df->id.id_high = filter->service_data_filter.id.id_high;
  df->mask = &decode_arg->value_mask_array[write_index];
  df->value = &decode_arg->value_mask_array[write_index + df->bytes_size];

  sns_memscpy(df->mask, df->bytes_size, mask_arg->buf, df->bytes_size);
  sns_memscpy(df->value, df->bytes_size, value_arg->buf, df->bytes_size);

  decode_arg->bytes_array_write_index += (df->bytes_size + df->bytes_size);
}

static void copy_manuf_data_filter(const sns_transport_ppe_filter *filter,
                                   sns_tppe_config_decode_arg *decode_arg, size_t index)
{
  size_t write_index = decode_arg->bytes_array_write_index;
  sns_data_filter *df = &decode_arg->manuf_data_filters[index];
  pb_buffer_arg *value_arg = (pb_buffer_arg *)filter->manuf_data_filter.value.arg;
  pb_buffer_arg *mask_arg = (pb_buffer_arg *)filter->manuf_data_filter.mask.arg;

  df->bytes_size = value_arg->buf_len;
  df->id.id_low = filter->manuf_data_filter.id.id_low;
  df->id.id_high = filter->manuf_data_filter.id.id_high;
  df->mask = &decode_arg->value_mask_array[write_index];
  df->value = &decode_arg->value_mask_array[write_index + df->bytes_size];

  sns_memscpy(df->mask, df->bytes_size, mask_arg->buf, df->bytes_size);
  sns_memscpy(df->value, df->bytes_size, value_arg->buf, df->bytes_size);

  decode_arg->bytes_array_write_index += (df->bytes_size + df->bytes_size);
}

static bool decode_dup_detect_filters(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
  UNUSED_VAR(field);
  bool status = true;
  sns_tppe_config_decode_arg *decode_arg = (sns_tppe_config_decode_arg *)*arg;
  sns_single_trigger *cur_trigger = &decode_arg->wakeup_triggers[decode_arg->trigger_index];
  sns_transport_ppe_filter filter = sns_transport_ppe_filter_init_default;
  pb_buffer_arg sdf_value_arg = {.buf = NULL, .buf_len = -1};
  pb_buffer_arg sdf_mask_arg = {.buf = NULL, .buf_len = -1};
  pb_buffer_arg mdf_value_arg = {.buf = NULL, .buf_len = -1};
  pb_buffer_arg mdf_mask_arg = {.buf = NULL, .buf_len = -1};
  filter.service_data_filter.mask.funcs.decode = pb_decode_string_cb;
  filter.service_data_filter.mask.arg = &sdf_value_arg;
  filter.service_data_filter.value.funcs.decode = pb_decode_string_cb;
  filter.service_data_filter.value.arg = &sdf_mask_arg;
  filter.manuf_data_filter.mask.funcs.decode = pb_decode_string_cb;
  filter.manuf_data_filter.mask.arg = &mdf_value_arg;
  filter.manuf_data_filter.value.funcs.decode = pb_decode_string_cb;
  filter.manuf_data_filter.value.arg = &mdf_mask_arg;
  if(-1 != stream->bytes_left)
  {
    status = pb_decode_noinit(stream, sns_transport_ppe_filter_fields, &filter);
    if(status)
    {
      if(filter.has_prop_filter)
      {
        copy_prop_filter(&filter, decode_arg, decode_arg->dup_pf_index);
        decode_arg->dup_pf_index++;
        cur_trigger->dup_pf_count++;
      }
      else if(filter.has_service_data_filter)
      {
        copy_service_data_filter(&filter, decode_arg, decode_arg->dup_sdf_index);
        decode_arg->dup_sdf_index++;
        cur_trigger->dup_sdf_count++;
      }
      else if(filter.has_manuf_data_filter)
      {
        copy_manuf_data_filter(&filter, decode_arg, decode_arg->dup_mdf_index);
        decode_arg->dup_mdf_index++;
        cur_trigger->dup_mdf_count++;
      }
    }
  }
  return status;
}

static bool decode_filters(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
  UNUSED_VAR(field);
  bool status = true;
  sns_tppe_config_decode_arg *tppe_decode_arg = (sns_tppe_config_decode_arg *)*arg;
  sns_single_trigger *cur_trigger =
      &tppe_decode_arg->wakeup_triggers[tppe_decode_arg->trigger_index];
  sns_transport_ppe_filter filter = sns_transport_ppe_filter_init_default;
  pb_buffer_arg sdf_value_arg = {.buf = NULL, .buf_len = -1};
  pb_buffer_arg sdf_mask_arg = {.buf = NULL, .buf_len = -1};
  pb_buffer_arg mdf_value_arg = {.buf = NULL, .buf_len = -1};
  pb_buffer_arg mdf_mask_arg = {.buf = NULL, .buf_len = -1};
  filter.service_data_filter.mask.funcs.decode = pb_decode_string_cb;
  filter.service_data_filter.mask.arg = &sdf_value_arg;
  filter.service_data_filter.value.funcs.decode = pb_decode_string_cb;
  filter.service_data_filter.value.arg = &sdf_mask_arg;
  filter.manuf_data_filter.mask.funcs.decode = pb_decode_string_cb;
  filter.manuf_data_filter.mask.arg = &mdf_value_arg;
  filter.manuf_data_filter.value.funcs.decode = pb_decode_string_cb;
  filter.manuf_data_filter.value.arg = &mdf_mask_arg;
  if(-1 != stream->bytes_left)
  {
    status = pb_decode_noinit(stream, sns_transport_ppe_filter_fields, &filter);
    if(status)
    {
      if(filter.has_prop_filter)
      {
        copy_prop_filter(&filter, tppe_decode_arg, tppe_decode_arg->pf_index);
        tppe_decode_arg->pf_index++;
        cur_trigger->prop_filter_count++;
      }
      else if(filter.has_service_data_filter)
      {
        copy_service_data_filter(&filter, tppe_decode_arg, tppe_decode_arg->sdf_index);
        tppe_decode_arg->sdf_index++;
        cur_trigger->service_df_count++;
      }
      else if(filter.has_manuf_data_filter)
      {
        copy_manuf_data_filter(&filter, tppe_decode_arg, tppe_decode_arg->mdf_index);
        tppe_decode_arg->mdf_index++;
        cur_trigger->manuf_df_count++;
      }
    }
  }
  return status;
}

static bool decode_triggers(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
  UNUSED_VAR(field);
  bool status = true;
  sns_tppe_config_decode_arg *tppe_decode_arg = (sns_tppe_config_decode_arg *)*arg;
  sns_transport_ppe_trigger trigger = sns_transport_ppe_trigger_init_default;
  sns_single_trigger *cur_trigger =
      &tppe_decode_arg->wakeup_triggers[tppe_decode_arg->trigger_index];
  trigger.wakeup_filters.funcs.decode = decode_filters;
  trigger.wakeup_filters.arg = tppe_decode_arg;
  trigger.dup_detection.dup_detect_filters.funcs.decode = decode_dup_detect_filters;
  trigger.dup_detection.dup_detect_filters.arg = tppe_decode_arg;

  if(-1 != stream->bytes_left)
  {
    status = pb_decode_noinit(stream, sns_transport_ppe_trigger_fields, &trigger);
    if(status)
    {
      cur_trigger->trigger_id = trigger.trigger_id;
      cur_trigger->dup_detect_period = UINT32_MAX; // infinite time
      tppe_decode_arg->trigger_index++;
      if(trigger.has_dup_detection)
      {
        // add always true filters in these cases
        if((cur_trigger->prop_filter_count + cur_trigger->service_df_count +
            cur_trigger->manuf_df_count) == 0)
        {

          // 0 wakeup filters
          add_always_true_pf(tppe_decode_arg, tppe_decode_arg->pf_index);
          tppe_decode_arg->pf_index++;
          cur_trigger->prop_filter_count++;
        }
        if((cur_trigger->dup_pf_count + cur_trigger->dup_sdf_count + cur_trigger->dup_mdf_count) ==
           0)
        {
          // 0 dup filters
          add_always_true_pf(tppe_decode_arg, tppe_decode_arg->dup_pf_index);
          tppe_decode_arg->dup_pf_index++;
          cur_trigger->dup_pf_count++;
        }
        if(trigger.dup_detection.has_dup_detect_period)
        {
          cur_trigger->dup_detect_period = trigger.dup_detection.dup_detect_period;
        }
      }
    }
  }
  return status;
}

bool decode_tppe_config(sns_request const *req, sns_std_request *std_request,
                        sns_all_filters const *const all_filters)
{
  bool status = false;
  pb_istream_t istream = pb_istream_from_buffer(req->request, req->request_len);
  sns_transport_ppe_config tppe_config = sns_transport_ppe_config_init_default;

  pb_simple_cb_arg arg = {.decoded_struct = &tppe_config,
                          .fields = sns_transport_ppe_config_fields};

  std_request->payload.funcs.decode = &pb_decode_simple_cb;
  std_request->payload.arg = &arg;

  sns_tppe_config_decode_arg decode_arg;
  sns_memset(&decode_arg, 0, sizeof(sns_tppe_config_decode_arg));

  decode_arg.dup_pf_index = all_filters->prop_filter_count;
  decode_arg.dup_sdf_index = all_filters->service_df_count;
  decode_arg.dup_mdf_index = all_filters->manuf_df_count;
  decode_arg.prop_filters = all_filters->prop_filters;
  decode_arg.service_data_filters = all_filters->service_data_filters;
  decode_arg.manuf_data_filters = all_filters->manuf_data_filters;
  decode_arg.wakeup_triggers = all_filters->wakeup_triggers;
  decode_arg.value_mask_array = all_filters->value_mask_array;

  tppe_config.triggers.funcs.decode = decode_triggers;
  tppe_config.triggers.arg = &decode_arg;

  status = pb_decode_noinit(&istream, sns_std_request_fields, std_request);
  return status;
}

/**
For overall filters
  Atleast one filter should be populated
For property filters
  check if which_prop field is not zero
For data filters
  check if len of value and mask is same
**/
static bool validate_filter(sns_transport_ppe_filter *filter)
{
  pb_buffer_arg *value_arg = NULL;
  pb_buffer_arg *mask_arg = NULL;
  if(!(filter->has_manuf_data_filter || filter->has_service_data_filter || filter->has_prop_filter))
  {
    // Atleast one filter should be present
    return false;
  }
  if(filter->has_prop_filter && filter->prop_filter.prop.which_prop == 0)
  {
    // property filter should have valid prop
    return false;
  }
  if(filter->has_manuf_data_filter)
  {
    value_arg = (pb_buffer_arg *)filter->manuf_data_filter.value.arg;
    mask_arg = (pb_buffer_arg *)filter->manuf_data_filter.mask.arg;
  }
  if(filter->has_service_data_filter)
  {
    value_arg = (pb_buffer_arg *)filter->service_data_filter.value.arg;
    mask_arg = (pb_buffer_arg *)filter->service_data_filter.mask.arg;
  }
  if(value_arg && mask_arg && (value_arg->buf_len != mask_arg->buf_len))
  {
    // Datafilter value & mask lengths must be same
    return false;
  }
  return true;
}

/**
 * Called every time a new filter is to be decoded
 * Increment filter count here
 * **/
static bool decode_filters_sizing(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
  UNUSED_VAR(field);
  sns_triggers_arg_sizing *sizing = (sns_triggers_arg_sizing *)*arg;
  bool status = false;
  sns_transport_ppe_filter filter = sns_transport_ppe_filter_init_default;
  pb_buffer_arg sdf_value_arg = {.buf = NULL, .buf_len = -1};
  pb_buffer_arg sdf_mask_arg = {.buf = NULL, .buf_len = -1};
  pb_buffer_arg mdf_value_arg = {.buf = NULL, .buf_len = -1};
  pb_buffer_arg mdf_mask_arg = {.buf = NULL, .buf_len = -1};
  filter.service_data_filter.mask.funcs.decode = pb_decode_string_cb;
  filter.service_data_filter.mask.arg = &sdf_value_arg;
  filter.service_data_filter.value.funcs.decode = pb_decode_string_cb;
  filter.service_data_filter.value.arg = &sdf_mask_arg;
  filter.manuf_data_filter.mask.funcs.decode = pb_decode_string_cb;
  filter.manuf_data_filter.mask.arg = &mdf_value_arg;
  filter.manuf_data_filter.value.funcs.decode = pb_decode_string_cb;
  filter.manuf_data_filter.value.arg = &mdf_mask_arg;
  if(-1 != stream->bytes_left)
  {
    status = pb_decode_noinit(stream, sns_transport_ppe_filter_fields, &filter);
    status = status && validate_filter(&filter);
    if(status)
    {
      if(filter.has_prop_filter)
      {
        sizing->prop_filter_count++;
      }
      else if(filter.has_service_data_filter)
      {
        sizing->service_df_count++;
        sizing->service_df_bytes_count += sdf_value_arg.buf_len;
        sizing->max_data_filter_bytes =
            SNS_MAX(sizing->max_data_filter_bytes, sdf_value_arg.buf_len);
      }
      else if(filter.has_manuf_data_filter)
      {
        sizing->manuf_df_count++;
        sizing->manuf_df_bytes_count += mdf_value_arg.buf_len;
        sizing->max_data_filter_bytes =
            SNS_MAX(sizing->max_data_filter_bytes, mdf_value_arg.buf_len);
      }
    }
  }
  return status;
}
/**
 * A tppe trigger should have atleast 1 wakeup filter
 * if dup_detection is enabled then wakeup filter can be zero
 * return false otherwise
 **/
static bool validate_trigger_sizes(sns_triggers_arg_sizing *wakeup_sizing, bool has_dup_detection)
{
  if(!has_dup_detection)
  {
    return ((wakeup_sizing->prop_filter_count + wakeup_sizing->service_df_count +
             wakeup_sizing->manuf_df_count) > 0);
  }
  return true;
}
/**
 * Called every time a new trigger is to be decoded
 * Increment trigger count here
 * figure out sizes of dup_filters and wakeup_filters separately
 * update the counts accordingly
 * **/
static bool decode_triggers_sizing(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
  UNUSED_VAR(field);
  bool status = true;
  sns_triggers_arg_sizing *sizing = (sns_triggers_arg_sizing *)*arg;
  sns_triggers_arg_sizing wakeup_sizing;
  sns_triggers_arg_sizing dup_sizing;
  sns_memset(&wakeup_sizing, 0, sizeof(sns_triggers_arg_sizing));
  sns_memset(&dup_sizing, 0, sizeof(sns_triggers_arg_sizing));

  sns_transport_ppe_trigger trigger = sns_transport_ppe_trigger_init_default;
  trigger.wakeup_filters.funcs.decode = decode_filters_sizing;
  trigger.wakeup_filters.arg = &wakeup_sizing;
  trigger.dup_detection.dup_detect_filters.funcs.decode = decode_filters_sizing;
  trigger.dup_detection.dup_detect_filters.arg = &dup_sizing;

  if(stream->bytes_left > 0)
  {
    status = pb_decode_noinit(stream, sns_transport_ppe_trigger_fields, &trigger);
  }
  if(!validate_trigger_sizes(&wakeup_sizing, trigger.has_dup_detection))
  {
    return false;
  }

  ++sizing->trigger_count;
  sizing->prop_filter_count += wakeup_sizing.prop_filter_count;
  sizing->service_df_count += wakeup_sizing.service_df_count;
  sizing->manuf_df_count += wakeup_sizing.manuf_df_count;
  sizing->dup_pf_count += dup_sizing.prop_filter_count;
  sizing->dup_sdf_count += dup_sizing.service_df_count;
  sizing->dup_mdf_count += dup_sizing.manuf_df_count;
  sizing->service_df_bytes_count +=
      wakeup_sizing.service_df_bytes_count + dup_sizing.service_df_bytes_count;
  sizing->manuf_df_bytes_count +=
      wakeup_sizing.manuf_df_bytes_count + dup_sizing.manuf_df_bytes_count;
  size_t temp = SNS_MAX(wakeup_sizing.max_data_filter_bytes, dup_sizing.max_data_filter_bytes);
  sizing->max_data_filter_bytes = SNS_MAX(sizing->max_data_filter_bytes, temp);
  sizing->dup_detection_required = trigger.has_dup_detection || sizing->dup_detection_required;
  if(trigger.has_dup_detection)
  {
    // if dup detection is enabled
    // add an always true filter to wakeup filters if 0 wakeup filters are set
    if((wakeup_sizing.prop_filter_count + wakeup_sizing.service_df_count +
        wakeup_sizing.manuf_df_count) == 0)
    {
      sizing->prop_filter_count++;
    }
    // add always true filter to dup detection filters if 0 dup filters are set
    if((dup_sizing.prop_filter_count + dup_sizing.service_df_count + dup_sizing.manuf_df_count) ==
       0)
    {
      sizing->dup_pf_count++;
    }
  }
  return status;
}

bool decode_tppe_config_sizing(const sns_request *req, sns_triggers_arg_sizing *sizing)
{
  bool status = false;
  sns_memset(sizing, 0, sizeof(sns_triggers_arg_sizing));
  pb_istream_t istream = pb_istream_from_buffer(req->request, req->request_len);
  sns_std_request std_request = sns_std_request_init_default;
  sns_transport_ppe_config tppe_config = sns_transport_ppe_config_init_default;

  pb_simple_cb_arg arg = {.decoded_struct = &tppe_config,
                          .fields = sns_transport_ppe_config_fields};

  std_request.payload.funcs.decode = &pb_decode_simple_cb;
  std_request.payload.arg = &arg;

  tppe_config.triggers.funcs.decode = decode_triggers_sizing;
  tppe_config.triggers.arg = sizing;

  status = pb_decode_noinit(&istream, sns_std_request_fields, &std_request);
  return status;
}
