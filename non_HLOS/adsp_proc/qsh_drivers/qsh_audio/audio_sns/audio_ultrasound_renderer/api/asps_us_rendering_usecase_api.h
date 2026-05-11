#ifndef _ASPS_US_RENDERER_USECASE_API_H_
#define _ASPS_US_RENDERER_USECASE_API_H_
/**
 * \file asps_us_renderer_useacase_api.h
 * \brief
 *  	 This file contains US Renderer Usecase ID specific APIs.
 *
 *    Copyright (c) 2024 Qualcomm Technologies, Inc.
 *    All Rights Reserved.
 *    Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#include "ar_defs.h"

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/


/*------------------------------------------------------------------------------
 *  Usecase ID definition
 *----------------------------------------------------------------------------*/
/* ASPS_USECASE_ID_ULTRASOUND_RENDERING will render the tone configured by client
 * in a specified pattern. This is only the Rendering Path of ultrasound usecases.
 * This can be used in conjunction with ADS usecase to capture data for detection.
 * In the case of ASPS_USECASE_ID_UPD, rendering and detection happen in Audio PD
 * and only the detection events are recieved by the sensor and the sensor client. */
#define ASPS_USECASE_ID_ULTRASOUND_RENDERING 0x0B00100B
/**
   Ultrasound Renderer sepcific payload. This payload contains the
   speaker configuration requested by the sensor.
   This structure is preceeded by param_id_asps_sensor_usecase_register_t structure. */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct asps_ultrasound_rendering_usecase_register_payload_t
{
	uint32_t sample_rate;
	/* sampling rate of pcm data at speaker */

	uint32_t bit_width;
	/* bit width of pcm data
	 * currently supports only 16 bit */

	uint32_t num_channels;
	/* number of channels takes values from {0..32} */

#if 0
	uint8_t channel_type[0];
	/* channel type for each channel in num_channels
	 * variable array of size num_channels elements
	 * takes values from {0..63} */
#endif
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

typedef struct asps_ultrasound_rendering_usecase_register_payload_t asps_ultrasound_rendering_usecase_register_payload_t;

/** Ultrasound Renderer usecase specific information */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct asps_ultrasound_rendering_usecase_register_ack_payload_t
{
	uint32_t module_iid_us_tone_renderer;
	/* miid for sending set params to the audio renderer
	 * which will be received as ack for US gen proxy
	 * usecase registration */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

typedef struct asps_ultrasound_rendering_usecase_register_ack_payload_t asps_ultrasound_rendering_usecase_register_ack_payload_t;

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* _ASPS_US_RENDERER_USECASE_API_H_ */
