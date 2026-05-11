#pragma once
/*=============================================================================
  @file sns_tppe_messages.h

  The TPPE proto messages structures, used while decoding the messages
  and encoding events

  Copyright (c) 2022-23 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_pb_util.h"
#include "sns_rc.h"
#include "sns_request.h"
#include "sns_sensor_event.h"
#include "sns_list.h"
#include "sns_std.pb.h"
#include "sns_tppe_messages.h"
#include "sns_transport_ppe.pb.h"
#include "sns_types.h"
#include <stddef.h>
#include <stdint.h>

/*=============================================================================
  Macro definitions
  ===========================================================================*/
#define ALWAYS_TRUE_PROP_ID 0

/*=============================================================================
  Structure definitions
  ===========================================================================*/
typedef struct sns_prop_filter
{
  bool matched;
  sns_transport_ppe_prop_filter prop_filter;
} sns_prop_filter;

typedef struct sns_data_filter
{
  sns_transport_ppe_id id;
  bool matched;
  uint8_t *mask;
  uint8_t *value;
  size_t bytes_size;
} sns_data_filter;

typedef struct sns_single_trigger
{
  // start time of a duplicate period for this trigger
  sns_time dup_detect_period_start_ts;
  size_t prop_filter_count;
  size_t service_df_count;
  size_t manuf_df_count;
  size_t dup_pf_count;
  size_t dup_sdf_count;
  size_t dup_mdf_count;
  uint32_t trigger_id;
  uint32_t dup_detect_period;
} sns_single_trigger;

typedef struct sns_all_filters
{
  // Counts are updated before decode and not modified during decode.
  size_t prop_filter_count;
  size_t service_df_count;
  size_t manuf_df_count;
  size_t dup_pf_count;
  size_t dup_sdf_count;
  size_t dup_mdf_count;
  size_t trigger_count;
  sns_prop_filter *prop_filters;
  sns_data_filter *service_data_filters;
  sns_data_filter *manuf_data_filters;
  sns_single_trigger *wakeup_triggers;
  uint8_t *value_mask_array;
} sns_all_filters;

// most of these variables are useless after decode, thus creating a stucture
// to be created on stack and discarded after decode
typedef struct sns_tppe_config_decode_arg
{
  size_t trigger_index;
  size_t pf_index;
  size_t sdf_index;
  size_t mdf_index;
  size_t dup_pf_index;
  size_t dup_sdf_index;
  size_t dup_mdf_index;
  size_t bytes_array_write_index;
  sns_prop_filter *prop_filters;
  sns_data_filter *service_data_filters;
  sns_data_filter *manuf_data_filters;
  sns_single_trigger *wakeup_triggers;
  uint8_t *value_mask_array;

} sns_tppe_config_decode_arg;

typedef struct
{
  sns_time event_ts;
  uint32_t *trigger_ids;
  uint32_t *dup_trigger_ids;
  size_t trigger_id_count;
  size_t dup_trigger_id_count;
  uint32_t crc;
} sns_tppe_event_trigger;

typedef struct
{
  sns_list_item list_entry;
  sns_transport_ppe_id id;
  uint8_t *buffer;
  size_t buffer_length;
} sns_transport_data_list_item;

typedef struct
{

  // Singly linked lists of type sns_transport_data_list_item
  sns_list service_data_list;
  sns_list manuf_data_list;

  // decoded raw adv bytes
  uint8_t *decoded_raw_adv_data;
  size_t decoded_raw_adv_data_len;
  size_t transport_address_len;

  size_t max_data_filter_bytes;

} sns_transport_buf_data;

typedef struct
{
  sns_transport_property *prop;
  uint8_t prop_cnt;
} sns_transport_buf_prop;

typedef struct
{
  // array of decoded properties
  sns_transport_buf_prop prop;

  // list of decoded data bytes
  sns_transport_buf_data data;
} sns_transport_decode_buffer;

typedef struct sns_triggers_arg_sizing
{
  size_t prop_filter_count;
  size_t service_df_count;
  size_t manuf_df_count;
  size_t dup_pf_count;
  size_t dup_sdf_count;
  size_t dup_mdf_count;
  size_t trigger_count;
  size_t decoded_raw_adv_data_len;
  size_t transport_address_len;
  size_t service_df_bytes_count;
  size_t manuf_df_bytes_count;
  size_t max_data_filter_bytes;
  bool dup_detection_required;
} sns_triggers_arg_sizing;
