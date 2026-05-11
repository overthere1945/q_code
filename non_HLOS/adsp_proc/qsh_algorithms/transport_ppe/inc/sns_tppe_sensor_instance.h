#pragma once
/*=============================================================================
  @file sns_tppe_sensor_instance.h

  The TPPE Sensor Instance

  Copyright (c) 2022-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_diag_service.h"
#include "sns_event_service.h"
#include "sns_list.h"
#include "sns_mem_util.h"
#include "sns_pb_util.h"
#include "sns_printf.h"
#include "sns_rc.h"
#include "sns_sensor_instance.h"
#include "sns_service_manager.h"
#include "sns_std_sensor.pb.h"
#include "sns_stream_service.h"
#include "sns_tppe_crc_cache.h"
#include "sns_tppe_sensor.h"
#include "sns_tppe_messages.h"
#include "sns_transport_layer_util.h"

/*============================================================================
  Type Definitions
  ===========================================================================*/

typedef struct sns_tppe_inst_state
{
  sns_tppe_registry_config reg_config;
  sns_transport_decode_buffer decode_buffer;
  sns_all_filters all_filters;
  sns_tppe_event_trigger event_triggers;
  sns_tppe_crc_cache crc_cache;
  sns_sensor_uid batch_suid;
  sns_sensor_uid tl_suid;
  uint32_t crc_cache_max_len;
  sns_transport_layer_api *tl_api;
  sns_data_stream *batch_tl_stream;
  sns_data_stream *tl_stream;
  void *tppe_config_memory_block;
  bool dup_detection_required;
} sns_tppe_inst_state;

/*============================================================================
  Public Function Declarations
  ===========================================================================*/

sns_rc sns_tppe_inst_init(sns_sensor_instance *this, sns_sensor_state const *state);

sns_rc sns_tppe_inst_deinit(sns_sensor_instance *const this);

sns_rc sns_tppe_inst_set_client_config(sns_sensor_instance *const this,
                                       sns_request const *client_request);

void *sns_tppe_memory_service_malloc(sns_memory_service *memory_service,
                                     sns_sensor_instance *const this, size_t size);
