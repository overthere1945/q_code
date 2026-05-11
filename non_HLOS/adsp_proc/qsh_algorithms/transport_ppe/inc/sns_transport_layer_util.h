#pragma once
/*=============================================================================
  @file sns_transport_layer_util.h

  Transport layer utility

  Copyright (c) 2022-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Includes
  ===========================================================================*/

#include "sns_memory_service.h"
#include "sns_rc.h"
#include "sns_sensor_event.h"
#include "sns_tppe_messages.h"
#include <stddef.h>
#include <stdint.h>

/*=============================================================================
  Macros
  ===========================================================================*/

#define SNS_TPPE_API_MAP_TABLE_SIZE (1)

/*=============================================================================
  Type Definitions
  ===========================================================================*/

typedef struct
{
  char *proto_name;
} sns_transport_layer_params;

typedef struct
{
  /**
   * Decode event
   *
   * @param[i] this     : Sensor instance pointer
   * @param[i] event    : Encoded event containing transport data
   * @param[io] buffer  : Buffer to be populated with decoded properties and data.
   *                      Note that this buffer already has the unique property ids
   *                      and max data length populated
   * @param[i] memory_service : Memory service pointer
   *
   * @return sns_rc : success if event is decoded successfully,
   *                  failure otherwise
   */
  sns_rc (*decode_event)(struct sns_sensor_instance *this, sns_sensor_event *event,
                         sns_transport_decode_buffer *buffer, sns_memory_service *memory_service);

  /**
   * Get property length
   *
   * @param[i] prop_id : Property id
   *
   * @return size_t : Expected length for the given property id
   */
  size_t (*get_prop_len)(uint8_t prop_id);

  /**
   * Compare property based on the threshold type
   *
   * @param[i] prop_event : Property populated in the event
   * @param[i] prop_req   : Property populated in the request
   * @param[i] threshold  : Threshold type used for comparison
   *
   * @return bool : true if the threshold is satisfied
   *                false if the threshold is not satisfied or threshold type
   *                      is not supported for the given property id
   */
  bool (*compare_prop)(sns_transport_property *prop_event, sns_transport_property *prop_req,
                       sns_transport_ppe_threshold threshold);

} sns_transport_layer_api;

typedef struct
{
  sns_transport_layer_params params;
  sns_transport_layer_api api;
} sns_transport_layer_api_map;

/*=============================================================================
  Extern variables
  ===========================================================================*/

extern sns_transport_layer_api_map sns_tppe_api_map_table[SNS_TPPE_API_MAP_TABLE_SIZE];

/*=============================================================================
  Public Function Declarations
  ===========================================================================*/

/**
 * Free any memory allocated dynamically while decoding an event
 *
 * @param[i] this   : Sensor instance pointer
 * @param[i] buffer : Buffer containing decoded event data
 * @param[i] memory_service : Memory service pointer
 * @return none
 */
void sns_transport_layer_free_data_items(sns_sensor_instance *this,
                                         sns_transport_decode_buffer *buffer,
                                         sns_memory_service *memory_service);

/**
 * Get property length for a given prop id
 *
 * @param[i] prop_id : Property id
 *
 * @return size_t : return property length if prop_id is recognized, 0 otherwise
 */
size_t sns_transport_layer_get_prop_len(uint8_t prop_id);

/**
 * Compare request and event properties based on the given threshold type
 *
 * @param[i] prop_event : Property populated in the event
 * @param[i] prop_req   : Property populated in the request
 * @param[i] threshold  : Threshold type used for comparison
 *
 * @return bool : true if the threshold is satisfied
 *                false if the threshold is not satisfied or threshold type
 *                      is not supported for the given property id
 */
bool sns_transport_layer_compare_prop(sns_transport_property *prop_event,
                                      sns_transport_property *prop_req,
                                      sns_transport_ppe_threshold threshold);

/**
 * Get a handle to the transport layer API based on the provided unique transport
 * layer parameters
 *
 * @param[i] params : Parameters to uniquely identify the transport layer
 *
 * @return sns_transport_layer_api* : valid pointer if an API is found for the
 *                                    given parameters, NULL otherwise
 */
sns_transport_layer_api *sns_transport_get_transport_layer_api(sns_transport_layer_params *params);
