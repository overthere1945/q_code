/*=============================================================================
  @file sns_oem1_sensor.c

  The oem1 virtual Sensor implementation

  Copyright (c) 2017,2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_mem_util.h"
#include "sns_oem1_sensor.h"
#include "sns_oem1_sensor_instance.h"
#include "sns_stream_service.h"
#include "sns_service_manager.h"
#include "sns_event_service.h"
#include "sns_attribute_service.h"
#include "sns_service.h"
#include "sns_sensor_util.h"
#include "sns_oem1.pb.h"
#include "sns_types.h"
#include "sns_suid.pb.h"
#include "sns_pb_util.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "sns_attribute_util.h"
#include "sns_printf.h"

// API used by registry
#include "sns_registry.pb.h"

/*=============================================================================
  Global Definitions
  ===========================================================================*/
const sns_sensor_uid oem1_suid = OEM1_SUID;
