/**============================================================================
  @file qsh_audio_speaker_diarization_sensor_instance_island.c

  @brief The SDZ Sensor Instance implementation

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qurt_sclk.h"
#include "qurt_timer.h"
#include "qsh_audio_speaker_diarization_sensor_instance.h"
#include "pb_decode.h"
#include "pb_encode.h"

/*=============================================================================
  Function Definitions
  ===========================================================================*/

static bool pb_encode_sdz_event(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
   pb_buffer_arg       *arr              = (pb_buffer_arg *)*arg;
   sdz_output_status_t *status_ptr       = (sdz_output_status_t *)arr->buf;
   sdz_speaker_info_t  *speaker_info_ptr = (sdz_speaker_info_t *)(status_ptr + 1);

   for (int i = 0; i < status_ptr->num_speakers; i++)
   {
      if (!pb_encode_tag_for_field(stream, field))
      {
         return false;
      }
      if (!pb_encode_submessage(stream, qsh_audio_speaker_diarization_speaker_info_fields, speaker_info_ptr))
      {
         return false;
      }
      speaker_info_ptr++;
   }

   return true;
}

// publish received events
void qsh_audio_speaker_diarization_inst_publish_event(sns_sensor_instance *const this,
                                                      event_id_sdz_output_event_t *event_ptr)
{
   qsh_audio_speaker_diarization_inst_state *state_ptr = (qsh_audio_speaker_diarization_inst_state *)this->state->state;
   sdz_output_status_t                      *status_ptr = (sdz_output_status_t *)(event_ptr + 1);

   if (!state_ptr->sdz_enable)
   {
      return;
   }

   // raise separate events for each output received from SPF
   for (int i = 0; i < event_ptr->num_outputs; i++)
   {
      qsh_audio_speaker_diarization_event event = qsh_audio_speaker_diarization_event_init_default;

      event.overlap_detected = status_ptr->overlap_detected;
      pb_buffer_arg data     = (pb_buffer_arg){ .buf = status_ptr, .buf_len = status_ptr->num_speakers };

      event.speaker_info.funcs.encode = &pb_encode_sdz_event;
      event.speaker_info.arg          = &data;

      size_t encoded_len = 0;

      pb_get_encoded_size(&encoded_len, qsh_audio_speaker_diarization_event_fields, &event);

      sns_sensor_event *event_out_ptr =
         state_ptr->event_service->api->alloc_event(state_ptr->event_service, this, encoded_len);
      if (event_out_ptr != NULL)
      {
         pb_ostream_t ostream = pb_ostream_from_buffer((pb_byte_t *)event_out_ptr->event, event_out_ptr->event_len);

         if (pb_encode(&ostream, qsh_audio_speaker_diarization_event_fields, &event))
         {
            event_out_ptr->message_id = QSH_AUDIO_SPEAKER_DIARIZATION_MSGID_QSH_AUDIO_SPEAKER_DIARIZATION_EVENT;
            event_out_ptr->event_len  = ostream.bytes_written;
            state_ptr->event_service->api->publish_event(state_ptr->event_service, this, event_out_ptr, NULL);
         }
      }

      status_ptr =
         (sdz_output_status_t *)((uint8_t *)(status_ptr + 1) + (status_ptr->num_speakers * sizeof(sdz_speaker_info_t)));
   }
}

/*===========================================================================
  Public API Definitions
  ===========================================================================*/
/**
 * Process incoming events to the Sensor Instance.
 */
static sns_rc qsh_audio_speaker_diarization_inst_notify_event(sns_sensor_instance *const this)
{
   UNUSED_VAR(this);
   sns_rc rc = SNS_RC_SUCCESS;
   return rc;
}

const sns_sensor_instance_api qsh_audio_speaker_diarization_sensor_instance_api = {
   .struct_len        = sizeof(sns_sensor_instance_api),
   .init              = &qsh_audio_speaker_diarization_inst_init,
   .deinit            = &qsh_audio_speaker_diarization_inst_deinit,
   .set_client_config = &qsh_audio_speaker_diarization_inst_set_client_config,
   .notify_event      = &qsh_audio_speaker_diarization_inst_notify_event
};
