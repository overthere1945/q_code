/*=============================================================================
  @file sns_tppe_transport_layer_util.c

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
  Static Function Definitions
  ===========================================================================*/

static bool proto_name_match(sns_transport_layer_params *params_in,
                             sns_transport_layer_params *params_table)
{
  return 0 == strcmp(params_in->proto_name, params_table->proto_name);
}

static sns_transport_layer_api *transport_layer_param_match(sns_transport_layer_params *params_in)
{
  sns_transport_layer_api *transport_api = NULL;
  const struct
  {
    bool (*match_func)(sns_transport_layer_params *, sns_transport_layer_params *);
  } match_func_table[] = {{proto_name_match}};

  for(int api_table_index = 0; api_table_index < ARR_SIZE(sns_tppe_api_map_table);
      api_table_index++)
  {
    bool match = true;

    for(int match_func_index = 0; match_func_index < ARR_SIZE(match_func_table); match_func_index++)
    {
      match &= match_func_table[match_func_index].match_func(
          params_in, &sns_tppe_api_map_table[api_table_index].params);
    }

    if(match)
    {
      transport_api = &sns_tppe_api_map_table[api_table_index].api;
      break;
    }
  }

  return transport_api;
}

size_t sns_transport_layer_get_prop_len(uint8_t prop_id)
{
  size_t prop_len = 0;
  switch(prop_id)
  {
  case sns_transport_property_event_type_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.event_type);
    break;

  case sns_transport_property_device_addr_type_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.device_addr_type);
    break;

  case sns_transport_property_device_addr_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.device_addr);
    break;

  case sns_transport_property_timestamp_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.timestamp);
    break;

  case sns_transport_property_tx_power_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.tx_power);
    break;

  case sns_transport_property_rssi_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.rssi);
    break;

  case sns_transport_property_primary_phy_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.primary_phy);
    break;

  case sns_transport_property_secondary_phy_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.secondary_phy);
    break;

  case sns_transport_property_adv_sid_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.adv_sid);
    break;

  case sns_transport_property_periodic_adv_interval_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.periodic_adv_interval);
    break;

  case sns_transport_property_direct_addr_type_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.direct_addr_type);
    break;

  case sns_transport_property_direct_addr_tag:
    prop_len = sizeof(((sns_transport_property *)NULL)->prop.direct_addr);
    break;
  }
  return prop_len;
}

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

/*
 * 1. Match params against the static api table
 * 2. Use a param match table to match all parameters provided
 * 3. If any of the params don't match, return invalid handle (NULL)
 * 4. If all params match, return the corresponding api handle
 */
sns_transport_layer_api *sns_transport_get_transport_layer_api(sns_transport_layer_params *params)
{
  return transport_layer_param_match(params);
}
