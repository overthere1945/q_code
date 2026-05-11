/**
 * \file qsocket_read_ep_api.h
 * \brief 
 *  	 This file contains CAPI read ep module params
 * 
 * \copyright
 *  Copyright (c) 2021-2023 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
// clang-format off
/*
$Header: //components/dev/qsh.drivers/1.0/dekantha.qsh.drivers.1.0.sensorbuild_3_18/qsh_audio/audio_api/qsocket_read_ep_api.h#1 $
*/
// clang-format on

#ifndef _QSOCKET_RD_EP_API_H_
#define _QSOCKET_RD_EP_API_H_

 /*------------------------------------------------------------------------
 * Include files
 * -----------------------------------------------------------------------*/
#include "ar_defs.h"
#include "media_fmt_api_basic.h"

/**
     @h2xml_title1          {Qsocket Read EP module}
     @h2xml_title_agile_rev {Qsocket Read EP module}
     @h2xml_title_date      {March 16, 2021}
  */

#define QSOCKET_RD_EP_MAX_INPUT_PORTS                  0x1

#define QSOCKET_RD_EP_MAX_OUTPUT_PORTS                 0x0

#define QSOCKET_RD_EP_STACK_SIZE_REQUIREMENT           1024

/** Definition of a timestamp valid flag bitmask. */
#define RD_EP_BIT_MASK_TIMESTAMP_VALID_FLAG    AR_NON_GUID(0x80000000)

/** Definition of a timestamp valid flag shift value. */
#define RD_EP_SHIFT_TIMESTAMP_VALID_FLAG       31

/** Definition of the end of frame bitmask.*/
#define RD_EP_BIT_MASK_END_OF_FRAME_FLAG        AR_NON_GUID(0x40000000)

/** Definition of theend of frameshift value.*/
#define RD_EP_SHIFT_LAST_BUFFER_FLAG           30

/** Definition of the end of history flag bitmask.*/
#define RD_EP_BIT_MASK_END_OF_HIST_FLAG        AR_NON_GUID(0x20000000)

/** Definition of the shift value for end of history flag bitmask.*/
#define RD_EP_SHIFT_TS_CONTINUE_FLAG           29



/**
   @h2xmlx_xmlNumberFormat {int}
*/

/*==============================================================================
   Param ID
==============================================================================*/

/* Parameter id to be used to configure instance id and hist buf sizes.


   Payload struct read_ep_qsocket_cfg_t
 */
#define PARAM_ID_QSOCKET_RD_EP_CFG 0x08001345

/*==============================================================================
   Param structure defintions
==============================================================================*/

/** @h2xmlp_parameter   {"PARAM_ID_QSOCKET_RD_EP_CFG",
                          PARAM_ID_QSOCKET_RD_EP_CFG}
    @h2xmlp_description { This parameter is set by the sensor. This is used to set the sensor-instance-id (destination id to send the audio data in SLPI) and to configure the DAM history buffer size }
    @h2xmlp_toolPolicy {NO_SUPPORT} */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

struct read_ep_qsocket_cfg_t
{
   uint32_t client_id;
   /**< @h2xmle_description {Unique ID of the sensor requesting the audio data. This ID is used by this module to send the raw data to the client sensor.} 
        @h2xmle_default     {0} 
        @h2xmle_range       { 0..0xFFFFFFFF }
        @h2xmle_policy      {Basic} */       

   uint32_t history_buffer_size_us;
   /**< @h2xmle_description {Size of the history buffer in us, this is configured to the Audio DAM module to cache the history data.} 
        @h2xmle_default     {0} 
        @h2xmle_range       { 0..0xFFFFFFFF }
        @h2xmle_policy      {Basic} */      

   uint32_t frame_size_us;
   /**< @h2xmle_description {used to configure uqsi packet size} 
        @h2xmle_default     {0} 
        @h2xmle_range       { 0..0xFFFFFFFF }
        @h2xmle_policy      {Basic} */       
        

                 
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"

;

/* Structure type def for above payload. */
typedef struct read_ep_qsocket_cfg_t read_ep_qsocket_cfg_t;


/* Parameter id to be used to enable or disable dataflow.

   Payload struct read_ep_qsocket_data_flow_t
 */
#define PARAM_ID_QSOCKET_RD_EP_ENABLE_DATA_FLOW 0x08001346

/*==============================================================================
   Param structure defintions
==============================================================================*/

/** @h2xmlp_parameter   {"PARAM_ID_QSOCKET_RD_EP_ENABLE_DATA_FLOW",
                          PARAM_ID_QSOCKET_RD_EP_ENABLE_DATA_FLOW}
    @h2xmlp_description { Parameter to start or stop the data flow. }
    @h2xmlp_toolPolicy {NO_SUPPORT} */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

struct read_ep_qsocket_data_flow_t
{
   uint32_t enable;
   /**< @h2xmle_description {Enable or disable the data-flow to the sensor.
                             \n1: Enable data flow
                             \n0: Disable data flow
                             } 
        @h2xmle_default     {0} 
        @h2xmle_range       { 0..1 }
        @h2xmle_policy      {Basic} */       
       

                 
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/* Structure type def for above payload. */
typedef struct read_ep_qsocket_data_flow_t read_ep_qsocket_data_flow_t;



/* Parameter id to be used to drain the history data alone.
 * Audio DAM gate is opened and requested to drain all the history data.
 * Once history data is drained, gate is closed.

   No payload
 */
#define PARAM_ID_QSOCKET_RD_EP_DRAIN_HISTORY_DATA 0x080013B6

/* Parameter id to be used to configure additional configurations.
 * Currently supports configuration of batch period to be used when
 * data events are flowing to the sensor.


   Payload struct read_ep_qsocket_extn_cfg_t
 */
#define PARAM_ID_QSOCKET_RD_EP_EXTN_CFG 0x08001520

/*==============================================================================
   Param structure defintions
==============================================================================*/

/** @h2xmlp_parameter   {"PARAM_ID_QSOCKET_RD_EP_EXTN_CFG",
                          PARAM_ID_QSOCKET_RD_EP_EXTN_CFG}
    @h2xmlp_description { This parameter is set by the sensor. This is used to configure batch_size to be used during batching }
    @h2xmlp_toolPolicy {NO_SUPPORT} */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

struct read_ep_qsocket_extn_cfg_t
{
   uint32_t version;
   /**< @h2xmle_description {Version of this payload (currently 1). In 
							 subsequent versions, fields might be added } 
        @h2xmle_default     {1} 
        @h2xmle_range       { 0..0xFFFFFFFF }
        @h2xmle_policy      {Basic} */
        
   uint32_t batch_size_us;
   /**< @h2xmle_description {used to configure sensor batch size} 
        @h2xmle_default     {0} 
        @h2xmle_range       { 0..0xFFFFFFFF }
        @h2xmle_policy      {Basic} */

                 
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"

;

/* Structure type def for above payload. */
typedef struct read_ep_qsocket_extn_cfg_t read_ep_qsocket_extn_cfg_t;

/**
 * Media format sent from this module to the client.
 *
 * Payload media_format_t
 */
#define DATA_EVENT_ID_QSOCKET_RD_EP_MEDIA_FORMAT   0x06001004


/** Flags that are passed with every  buffer and must be filled by the
    module for every output buffer. These flags apply only to the buffer with
    which they are associated.
*/

typedef union qsocket_rd_ep_buffer_flags_t qsocket_rd_ep_buffer_flags_t;

/** @h2xmlp_subStruct
    @h2xmlp_description  {Flags that are passed with every  buffer and must be filled by the
                          module for every output buffer. These flags apply only to the buffer with
                          which they are associated. }
    @h2xmlp_toolPolicy   {Calibration} */
union qsocket_rd_ep_buffer_flags_t
{
   /** Defines the flags.
   */
   struct
   {
      uint32_t is_timestamp_valid : 1;
      /**< Specifies whether the timestamp associated with the buffer is valid.

           @valuesbul
           - 0 -- Not valid
           - 1 -- Valid @tablebulletend */

      uint32_t end_of_frame : 1;
      /**< Specifies whether the buffer has an end of frame.
	       This is associated with the last sample in the buffer.

           @valuesbul
           - 0 -- end_of_frame is not marked
           - 1 -- end_of_frame is marked.

           end_of_frame is also set for discontinuities (timestamp discontinuity, EOS).
           */

      /**< Other bitfields must be set to zero. */
   };

   uint32_t word;
   /**< Entire 32-bit word for easy access to read or write the entire word in
        one shot. */
};


/**
 *  DATA buffer sent from this module to the client.
 *
 *  Payload 
 *		data_event_id_qsocket_rd_sh_mem_ep_buffer_done_t
 */
 
    
#define DATA_EVENT_ID_QSOCKET_RD_EP_BUFFER_DONE		0x06001005


/** @h2xmlp_parameter   {"DATA_EVENT_ID_QSOCKET_RD_EP_BUFFER_DONE",
                          DATA_EVENT_ID_QSOCKET_RD_EP_BUFFER_DONE}
    @h2xmlp_description { *  DATA buffer sent from this module to the client. }
    @h2xmlp_toolPolicy {NO_SUPPORT} */
    
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct data_event_id_qsocket_rd_ep_buffer_done_t
{
   uint32_t                 timestamp_lsw;
   /**< @h2xmle_description {Lower 32 bits of the 64-bit session time in microseconds of the first sample in the data buffer..} 
   @h2xmle_default     {0} 
   @h2xmle_range       { 0..0xFFFFFFFF }
   @h2xmle_policy      {Basic} */ 

   uint32_t                 timestamp_msw;
   /**< @h2xmle_description {Upper 32 bits of the 64-bit session time in microseconds of the first sample in the data buffer..} 
   @h2xmle_default     {0} 
   @h2xmle_range       { 0..0xFFFFFFFF }
   @h2xmle_policy      {Basic} */ 

   qsocket_rd_ep_buffer_flags_t flags;
   /**< @h2xmle_description {Bit field of flags of type qsocket_rd_ep_buffer_flags_t } 
   @h2xmle_default     {0}   */
   
   uint32_t                 data_size;
   /**< @h2xmle_description { Total size of the data in bytes} 
   @h2xmle_default     {0} 
   @h2xmle_range       { 0..0xFFFFFFFF }
   @h2xmle_policy      {Basic} */ 
   
#ifdef __H2XML__
   int8_t      buffer[data_size];
   /**< data in bytes. */
#endif
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

typedef struct data_event_id_qsocket_rd_ep_buffer_done_t data_event_id_qsocket_rd_ep_buffer_done_t;

/*------------------------------------------------------------------------------
   Module
------------------------------------------------------------------------------*/

/*
 * ID of the read ep Module
*/
#define MODULE_ID_QSOCKET_RD_EP 0x070010D5

/**
    @h2xmlm_module         {"MODULE_ID_QSOCKET_RD_EP", MODULE_ID_QSOCKET_RD_EP}
    @h2xmlm_displayName    {"Qsocket Read EP EndPoint"}
    @h2xmlm_modSearchKeys  {software, sink}
    @h2xmlm_description    {This module is for transferring data over (u)Qsocket. Main use case intended for is data transfer from Audio domain to sensor domain.}
    @h2xmlm_dataMaxInputPorts    {QSOCKET_RD_EP_MAX_INPUT_PORTS}
    @h2xmlm_dataInputPorts       {IN = 2}
    @h2xmlm_dataMaxOutputPorts   {QSOCKET_RD_EP_MAX_OUTPUT_PORTS}
    @h2xmlm_supportedContTypes  { APM_CONTAINER_TYPE_GC}
    @h2xmlm_stackSize            { QSOCKET_RD_EP_STACK_SIZE_REQUIREMENT }
    @h2xmlm_toolPolicy     {Calibration}
    @h2xmlm_toolPolicy     {Calibration}
    @h2xmlm_ctrlDynamicPortIntent   { "DAM-DE Control" = INTENT_ID_AUDIO_DAM_DETECTION_ENGINE_CTRL,
                                      maxPorts= 1 }
    @h2xmlm_ctrlDynamicPortIntent{"AAD State Control"=INTENT_ID_AAD_STATE_CONTROL, maxPorts= 1 }
    @{                     <-- Start of the Module -->

	@h2xml_Select          {"read_ep_qsocket_cfg_t"}
    @h2xmlm_InsertParameter

	@h2xml_Select          {"read_ep_qsocket_extn_cfg_t"}
    @h2xmlm_InsertParameter

	@h2xml_Select          {"read_ep_qsocket_data_flow_t"}
    @h2xmlm_InsertParameter 

	@h2xml_Select          {"data_event_id_qsocket_rd_ep_buffer_done_t"}
    @h2xmlm_InsertParameter

		@h2xml_Select          {"qsocket_rd_ep_buffer_flags_t"}
		@h2xmlm_InsertParameter
    
    @}                     <-- End of the Module -->
*/

#endif // _QSOCKET_RD_EP_API_H_
