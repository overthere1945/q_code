/*=============================================================================
  @file sns_tppe_transport_layer_util_island.c

  Transport layer utility

  Copyright (c) 2022-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Includes
  ===========================================================================*/

#include "sns_mem_util.h"
#include "sns_memory_service.h"
#include "sns_transport_layer_ble.h"
#include "sns_transport_layer_util.h"
#include "sns_types.h"
#include <stdbool.h>
#include <string.h>

/*=============================================================================
  Public Data Definitions
  ===========================================================================*/

sns_transport_layer_api_map sns_tppe_api_map_table[SNS_TPPE_API_MAP_TABLE_SIZE] = {
    {{SNS_TRANSPORT_LAYER_BLE_PROTO_NAME},
     {sns_transport_layer_ble_decode_event, sns_transport_layer_get_prop_len,
      sns_transport_layer_compare_prop}}};

/*=============================================================================
  Static Function Definitions
  ===========================================================================*/

static bool compare_prop_equal(sns_transport_property *prop_event, sns_transport_property *prop_req)
{
  bool match = false;
  switch(prop_event->which_prop)
  {
  case sns_transport_property_event_type_tag:
    match = prop_event->prop.event_type[0] == prop_req->prop.event_type[0];
    break;

  case sns_transport_property_device_addr_type_tag:
    match = prop_event->prop.device_addr_type[0] == prop_req->prop.device_addr_type[0];
    break;

  case sns_transport_property_device_addr_tag:
    match = (0 == sns_memcmp(prop_event->prop.device_addr, prop_req->prop.device_addr,
                             sizeof(prop_event->prop.device_addr)));
    break;

  case sns_transport_property_timestamp_tag:
    match = prop_event->prop.timestamp == prop_req->prop.timestamp;
    break;

  case sns_transport_property_tx_power_tag:
    match = prop_event->prop.tx_power == prop_req->prop.tx_power;
    break;

  case sns_transport_property_rssi_tag:
    match = prop_event->prop.rssi == prop_req->prop.rssi;
    break;

  case sns_transport_property_primary_phy_tag:
    match = prop_event->prop.primary_phy[0] == prop_req->prop.primary_phy[0];
    break;

  case sns_transport_property_secondary_phy_tag:
    match = prop_event->prop.secondary_phy[0] == prop_req->prop.secondary_phy[0];
    break;

  case sns_transport_property_adv_sid_tag:
    match = prop_event->prop.adv_sid[0] == prop_req->prop.adv_sid[0];
    break;

  case sns_transport_property_periodic_adv_interval_tag:
    match = prop_event->prop.periodic_adv_interval == prop_req->prop.periodic_adv_interval;
    break;

  case sns_transport_property_direct_addr_type_tag:
    match = prop_event->prop.direct_addr_type[0] == prop_req->prop.direct_addr_type[0];
    break;

  case sns_transport_property_direct_addr_tag:
    match = (0 == sns_memcmp(prop_event->prop.direct_addr, prop_req->prop.direct_addr,
                             sizeof(prop_event->prop.direct_addr)));
    break;
  }
  return match;
}

static bool compare_prop_max(sns_transport_property *prop_event, sns_transport_property *prop_req)
{
  bool match = false;
  switch(prop_event->which_prop)
  {
  case sns_transport_property_timestamp_tag:
    match = prop_event->prop.timestamp <= prop_req->prop.timestamp;
    break;

  case sns_transport_property_tx_power_tag:
    match = prop_event->prop.tx_power <= prop_req->prop.tx_power;
    break;

  case sns_transport_property_rssi_tag:
    match = prop_event->prop.rssi <= prop_req->prop.rssi;
    break;

  case sns_transport_property_periodic_adv_interval_tag:
    match = prop_event->prop.periodic_adv_interval <= prop_req->prop.periodic_adv_interval;
    break;
  }
  return match;
}

static bool compare_prop_min(sns_transport_property *prop_event, sns_transport_property *prop_req)
{
  bool match = false;
  switch(prop_event->which_prop)
  {
  case sns_transport_property_timestamp_tag:
    match = prop_event->prop.timestamp >= prop_req->prop.timestamp;
    break;

  case sns_transport_property_tx_power_tag:
    match = prop_event->prop.tx_power >= prop_req->prop.tx_power;
    break;

  case sns_transport_property_rssi_tag:
    match = prop_event->prop.rssi >= prop_req->prop.rssi;
    break;

  case sns_transport_property_periodic_adv_interval_tag:
    match = prop_event->prop.periodic_adv_interval >= prop_req->prop.periodic_adv_interval;
    break;
  }
  return match;
}

static void remove_all_data_items_from_list(sns_sensor_instance *instance, sns_list *list,
                                            sns_memory_service *memory_service)
{
  sns_list_iter iter;
  sns_list_iter_init(&iter, list, true);
  while(sns_list_iter_curr(&iter))
  {
    sns_transport_data_list_item *list_item =
        (sns_transport_data_list_item *)sns_list_iter_remove(&iter);
    memory_service->api->free(instance, list_item);
  }
}

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

void sns_transport_layer_free_data_items(sns_sensor_instance *this,
                                         sns_transport_decode_buffer *buffer,
                                         sns_memory_service *memory_service)
{
  remove_all_data_items_from_list(this, &buffer->data.service_data_list, memory_service);
  remove_all_data_items_from_list(this, &buffer->data.manuf_data_list, memory_service);
}

bool sns_transport_layer_compare_prop(sns_transport_property *prop_event,
                                      sns_transport_property *prop_req,
                                      sns_transport_ppe_threshold threshold)
{
  bool match = false;
  switch(threshold)
  {
  case SNS_TRANSPORT_PPE_THRESHOLD_MIN:
    match = compare_prop_min(prop_event, prop_req);
    break;
  case SNS_TRANSPORT_PPE_THRESHOLD_MAX:
    match = compare_prop_max(prop_event, prop_req);
    break;
  case SNS_TRANSPORT_PPE_THRESHOLD_EQUAL:
    match = compare_prop_equal(prop_event, prop_req);
    break;
  case SNS_TRANSPORT_PPE_THRESHOLD_UNINITIALIZED:
    match = false;
    break;
  }
  return match;
}
