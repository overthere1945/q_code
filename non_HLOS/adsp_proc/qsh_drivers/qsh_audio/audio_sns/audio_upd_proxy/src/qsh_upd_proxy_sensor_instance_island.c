/**============================================================================
  @file qsh_upd_proxy_sensor_instance_island.c

  @brief The UPD PROXY Sensor Instance implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qurt_sclk.h"
#include "qurt_timer.h"
#include "qsh_upd_proxy_sensor_instance.h"
#include "pb_decode.h"
#include "pb_encode.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

// publish received events
void qsh_upd_proxy_inst_publish_event(sns_sensor_instance *const this, event_id_upd_detection_event_t *event_ptr)
{
   sns_proximity_event event = sns_proximity_event_init_default;
   event.status              = SNS_STD_SENSOR_SAMPLE_STATUS_ACCURACY_HIGH;
   if (event_ptr->proximity_event_type == US_DETECT_NEAR)
   {
      event.proximity_event_type = SNS_PROXIMITY_EVENT_TYPE_NEAR;
   }
   else if (event_ptr->proximity_event_type == US_DETECT_FAR)
   {
      event.proximity_event_type = SNS_PROXIMITY_EVENT_TYPE_FAR;
   }

   sns_time event_timestamp =
      ((sns_time)event_ptr->detection_timestamp_msw << 32) + (sns_time)event_ptr->detection_timestamp_lsw;

   // send event to the clients
   pb_send_event(this,
                 sns_proximity_event_fields,
                 &event,
                 QURT_TIMER_TIMETICK_FROM_US(event_timestamp),
                 SNS_PROXIMITY_MSGID_SNS_PROXIMITY_EVENT,
                 NULL);
}

/*===========================================================================
  Public API Definitions
  ===========================================================================*/
/**
 * Process incoming events to the Sensor Instance.
 */
static sns_rc qsh_upd_proxy_inst_notify_event(sns_sensor_instance *const this)
{
   UNUSED_VAR(this);
   sns_rc rc = SNS_RC_SUCCESS;
   return rc;
}

const sns_sensor_instance_api qsh_upd_proxy_sensor_instance_api = { .struct_len = sizeof(sns_sensor_instance_api),
                                                              .init       = &qsh_upd_proxy_inst_init,
                                                              .deinit     = &qsh_upd_proxy_inst_deinit,
                                                              .set_client_config =
                                                                 &qsh_upd_proxy_inst_set_client_config,
                                                              .notify_event = &qsh_upd_proxy_inst_notify_event };
