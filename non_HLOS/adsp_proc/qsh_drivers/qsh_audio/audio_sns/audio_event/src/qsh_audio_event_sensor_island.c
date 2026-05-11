/*=============================================================================
  @file qsh_audio_event_sensor_island.c

  Base Class of audio event sensor, island code.

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_audio_event_sensor.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

// publish error events to all the clients of this sensor
sns_rc qsh_audio_event_send_error(sns_sensor *const this, sns_std_error error)
{
   qsh_audio_island_exit();
   return qsh_audio_event_send_error_(this, error);
}

sns_rc qsh_audio_event_instance_send_error(sns_sensor_instance *const this,
                                           sns_std_error         error,
                                           sns_sensor_uid const *suid_ptr)
{
   qsh_audio_island_exit();
   return qsh_audio_event_inst_send_error_(this, error, suid_ptr);
}
