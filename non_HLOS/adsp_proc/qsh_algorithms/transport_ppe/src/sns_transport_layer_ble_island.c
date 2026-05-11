/*=============================================================================
  @file sns_tppe_transport_layer_ble_island.c

  Transport layer utility for BLE

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Includes
  ===========================================================================*/

#include "pb_decode.h"
#include "qsh_ble_ext.pb.h"
#include "sns_math_ops.h"
#include "sns_mem_util.h"
#include "sns_memory_service.h"
#include "sns_pb_util.h"
#include "sns_tppe_sensor_instance.h"
#include "sns_transport_layer_ble.h"
#include "sns_transport_layer_util.h"
#include "sns_transport_ppe.pb.h"
#include "sns_types.h"

/*=============================================================================
  Macros
  ===========================================================================*/

#define sns_transport_ble_event_init                                                               \
  {                                                                                                \
    {NULL, 0}, {NULL, 0}, {NULL, 0}, qsh_ble_ext_evt_scan_result_event_init_default                \
  }

/*=============================================================================
  Type Definitions
  ===========================================================================*/

typedef struct
{
  pb_buffer_arg ble_device_addr_arg;
  pb_buffer_arg ble_direct_addr_arg;
  pb_buffer_arg ble_raw_data_arg;
  qsh_ble_ext_evt_scan_result_event event;
} sns_transport_ble_event;

typedef struct
{
  sns_sensor_instance *instance;
  sns_transport_decode_buffer *buffer;
  sns_memory_service *memory_service;
} sns_transport_ble_event_decode_arg;

/*=============================================================================
  Static Function Definitions
  ===========================================================================*/

static void get_raw_adv_data(sns_transport_ble_event *ble_event, sns_transport_buf_data *data_buf)
{
  if(data_buf->decoded_raw_adv_data)
  {
    sns_memzero(data_buf->decoded_raw_adv_data, data_buf->decoded_raw_adv_data_len);
    sns_memscpy(data_buf->decoded_raw_adv_data, data_buf->decoded_raw_adv_data_len,
                ble_event->ble_raw_data_arg.buf, ble_event->ble_raw_data_arg.buf_len);
  }
}

static void append_device_address(sns_transport_buf_data *data_buf,
                                  sns_transport_buf_prop *prop_buf)
{
  if(data_buf->decoded_raw_adv_data)
  {
    uint8_t *device_address = data_buf->decoded_raw_adv_data + data_buf->decoded_raw_adv_data_len;
    int prop_idx_device_addr = sns_transport_property_device_addr_tag - 1;

    sns_memscpy(device_address, data_buf->transport_address_len,
                &prop_buf->prop[prop_idx_device_addr].prop.device_addr,
                data_buf->transport_address_len);
  }
}

static void get_prop(sns_transport_ble_event *ble_event, sns_transport_property *prop)
{
  switch(prop->which_prop)
  {
  case sns_transport_property_event_type_tag:
    prop->prop.event_type[0] = ble_event->event.event_type;
    break;

  case sns_transport_property_device_addr_tag:
    sns_memzero(prop->prop.device_addr, sizeof(prop->prop.device_addr));
    sns_memscpy(prop->prop.device_addr, sizeof(prop->prop.device_addr),
                ble_event->ble_device_addr_arg.buf, ble_event->ble_device_addr_arg.buf_len);
    break;

  case sns_transport_property_device_addr_type_tag:
    prop->prop.device_addr_type[0] = ble_event->event.device_address_type;
    break;

  case sns_transport_property_timestamp_tag:
    prop->prop.timestamp = ble_event->event.time_stamp;
    break;

  case sns_transport_property_tx_power_tag:
    prop->prop.tx_power = ble_event->event.tx_power;
    break;

  case sns_transport_property_rssi_tag:
    prop->prop.rssi = ble_event->event.rssi;
    break;

  case sns_transport_property_primary_phy_tag:
    prop->prop.primary_phy[0] = ble_event->event.primary_phy;
    break;

  case sns_transport_property_secondary_phy_tag:
    prop->prop.secondary_phy[0] = ble_event->event.secondary_phy;
    break;

  case sns_transport_property_adv_sid_tag:
    prop->prop.adv_sid[0] = ble_event->event.adv_sid;
    break;

  case sns_transport_property_periodic_adv_interval_tag:
    prop->prop.periodic_adv_interval = ble_event->event.pa_interval;
    break;

  case sns_transport_property_direct_addr_type_tag:
    prop->prop.direct_addr_type[0] = ble_event->event.direct_address_type;
    break;

  case sns_transport_property_direct_addr_tag:
    sns_memzero(prop->prop.direct_addr, sizeof(prop->prop.direct_addr));
    sns_memscpy(prop->prop.direct_addr, sizeof(prop->prop.direct_addr),
                ble_event->ble_direct_addr_arg.buf, ble_event->ble_direct_addr_arg.buf_len);
    break;
  }
}

static void get_all_props(sns_transport_ble_event *ble_event, sns_transport_buf_prop *prop_buf)
{
  for(int prop_idx = 0; prop_idx < prop_buf->prop_cnt; prop_idx++)
  {
    get_prop(ble_event, &prop_buf->prop[prop_idx]);
  }
}

static void add_data_item_to_list(sns_transport_data_list_item *list_item, sns_list *list)
{
  sns_list_iter iter;
  sns_list_item_init(&list_item->list_entry, list_item);
  sns_list_iter_init(&iter, list, false);
  sns_list_iter_insert(&iter, &list_item->list_entry, true);
}

static void populate_service_data(sns_transport_data_list_item *list_item,
                                  qsh_ble_ext_service_data *service_data, pb_buffer_arg *data_arg,
                                  size_t data_buffer_len)
{

  list_item->id.id_low = service_data->uuid.uuid_low;
  list_item->id.id_high = service_data->uuid.uuid_high;
  list_item->buffer_length = data_buffer_len;
  list_item->buffer = (uint8_t *)list_item + sizeof(sns_transport_data_list_item);
  sns_memscpy(list_item->buffer, list_item->buffer_length, data_arg->buf, data_arg->buf_len);
}

static bool decode_service_data(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
  UNUSED_VAR(field);
  bool status = true;
  sns_transport_ble_event_decode_arg *decode_arg = (sns_transport_ble_event_decode_arg *)*arg;

  sns_transport_decode_buffer *buffer = decode_arg->buffer;
  sns_sensor_instance *instance = decode_arg->instance;
  sns_memory_service *memory_service = decode_arg->memory_service;

  if(stream->bytes_left > 0)
  {
    size_t data_buffer_len = 0;
    sns_transport_data_list_item *list_item = NULL;
    qsh_ble_ext_service_data service_data = qsh_ble_ext_service_data_init_default;
    pb_buffer_arg arg = {.buf = NULL, .buf_len = 0};

    service_data.data.funcs.decode = pb_decode_string_cb;
    service_data.data.arg = &arg;

    status = pb_decode_noinit(stream, qsh_ble_ext_service_data_fields, &service_data);

    data_buffer_len = SNS_MIN(buffer->data.max_data_filter_bytes, arg.buf_len);

    list_item = sns_tppe_memory_service_malloc(
        memory_service, instance, sizeof(sns_transport_data_list_item) + data_buffer_len);

    if(NULL != list_item)
    {
      populate_service_data(list_item, &service_data, &arg, data_buffer_len);
      add_data_item_to_list(list_item, &buffer->data.service_data_list);
    }
  }
  return status;
}

static void populate_manuf_data(sns_transport_data_list_item *list_item,
                                qsh_ble_ext_manufacturer_data *manuf_data, pb_buffer_arg *data_arg,
                                size_t data_buffer_len)
{
  list_item->id.id_low = manuf_data->manufacturer_id;
  list_item->buffer_length = data_buffer_len;
  list_item->buffer = (uint8_t *)list_item + sizeof(sns_transport_data_list_item);
  sns_memscpy(list_item->buffer, list_item->buffer_length, data_arg->buf, data_arg->buf_len);
}

static bool decode_manuf_data(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
  UNUSED_VAR(field);
  bool status = true;
  sns_transport_ble_event_decode_arg *decode_arg = (sns_transport_ble_event_decode_arg *)*arg;

  sns_transport_decode_buffer *buffer = decode_arg->buffer;
  sns_sensor_instance *instance = decode_arg->instance;
  sns_memory_service *memory_service = decode_arg->memory_service;

  if(stream->bytes_left > 0)
  {
    size_t data_buffer_len = 0;
    sns_transport_data_list_item *list_item = NULL;
    qsh_ble_ext_manufacturer_data manuf_data = qsh_ble_ext_manufacturer_data_init_default;
    pb_buffer_arg arg = {.buf = NULL, .buf_len = 0};

    manuf_data.data.funcs.decode = pb_decode_string_cb;
    manuf_data.data.arg = &arg;

    status = pb_decode_noinit(stream, qsh_ble_ext_manufacturer_data_fields, &manuf_data);

    data_buffer_len = SNS_MIN(buffer->data.max_data_filter_bytes, arg.buf_len);
    list_item = sns_tppe_memory_service_malloc(
        memory_service, instance, sizeof(sns_transport_data_list_item) + data_buffer_len);
    if(NULL != list_item)
    {
      populate_manuf_data(list_item, &manuf_data, &arg, data_buffer_len);
      add_data_item_to_list(list_item, &buffer->data.manuf_data_list);
    }
  }
  return status;
}

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

sns_rc sns_transport_layer_ble_decode_event(sns_sensor_instance *instance, sns_sensor_event *event,
                                            sns_transport_decode_buffer *buffer,
                                            sns_memory_service *memory_service)
{
  sns_rc rc = SNS_RC_SUCCESS;
  if(QSH_BLE_EXT_MSGID_QSH_BLE_EXT_EVT_SCAN_RESULT_EVENT == event->message_id)
  {
    pb_istream_t stream = pb_istream_from_buffer((pb_byte_t *)event->event, event->event_len);
    sns_transport_ble_event ble_event = sns_transport_ble_event_init;
    sns_transport_ble_event_decode_arg decode_arg = {
        .instance = instance, .buffer = buffer, .memory_service = memory_service};

    ble_event.event.scan_record.service_data_s.funcs.decode = decode_service_data;
    ble_event.event.scan_record.service_data_s.arg = &decode_arg;

    ble_event.event.scan_record.manufacturer_data_s.funcs.decode = decode_manuf_data;
    ble_event.event.scan_record.manufacturer_data_s.arg = &decode_arg;

    ble_event.event.scan_record.raw_adv_data.funcs.decode = pb_decode_string_cb;
    ble_event.event.scan_record.raw_adv_data.arg = &ble_event.ble_raw_data_arg;

    ble_event.event.device_address.funcs.decode = pb_decode_string_cb;
    ble_event.event.device_address.arg = &ble_event.ble_device_addr_arg;

    ble_event.event.direct_address.funcs.decode = pb_decode_string_cb;
    ble_event.event.direct_address.arg = &ble_event.ble_direct_addr_arg;

    if(pb_decode_noinit(&stream, qsh_ble_ext_evt_scan_result_event_fields, &ble_event.event))
    {
      get_all_props(&ble_event, &buffer->prop);
      get_raw_adv_data(&ble_event, &buffer->data);
      append_device_address(&buffer->data, &buffer->prop);
    }
    else
    {
      rc = SNS_RC_FAILED;
    }
  }
  else
  {
    rc = SNS_RC_NOT_SUPPORTED;
  }

  return rc;
}
