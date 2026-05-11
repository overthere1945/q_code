#ifndef _ASPS_API_H_
#define _ASPS_API_H_
/**
 * \file asps_api.h
 * \brief
 *  	 This file contains Audio-Sensor Proxy Service Commands and Data Structures
 *
 *    Copyright (c) 2020-2021 Qualcomm Technologies, Inc.
 *    All Rights Reserved.
 *    Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#include "ar_defs.h"

/*------------------------------------------------------------------------------
 *  Usecase ID API includes
 *----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/*------------------------------------------------------------------------------
 *  Module Instance ID definitions
 *----------------------------------------------------------------------------*/

/* ASPS module instance ID.
 * Used by sensor clients to send message to ASPS. */
#ifndef ASPS_MODULE_INSTANCE_ID
#define ASPS_MODULE_INSTANCE_ID  0x00000007
#endif

/*------------------------------------------------------------------------------
 *  API Definitions
 *----------------------------------------------------------------------------*/

#include "spf_begin_pack.h"

/**
    In-band :
         This structure is followed by an in-band payload.
    Out-of-band :
         Out of band payload can be extracted from the mem_map_handle,
         payload_address_lsw and payload_address_msw.
 */
struct asps_cmd_header_t
{
   uint32_t payload_address_lsw;
   /**< Lower 32 bits of the payload address. */

   uint32_t payload_address_msw;
   /**< Upper 32 bits of the payload address.

         The 64-bit number formed by payload_address_lsw and
         payload_address_msw must be aligned to a 32-byte boundary and be in
         contiguous memory.

         @values
         - For a 32-bit shared memory address, this field must be set to 0.
         - For a 36-bit shared memory address, bits 31 to 4 of this field must
           be set to 0. @tablebulletend */

   uint32_t mem_map_handle;
   /**< Unique identifier for a shared memory address.

        @values
        - NULL -- The message is in the payload (in-band).
        - Non-NULL -- The parameter data payload begins at the address
          specified by a pointer to the physical address of the payload in
          shared memory (out-of-band).

        @contcell
        The ADSP returns this memory map handle through
        #ASPS_CMD_SHARED_MEM_MAP_REGIONS.

    */

   uint32_t payload_size;
   /**< Actual size of the variable payload accompanying the message or in
        shared memory. This field is used for parsing both in-band and
        out-of-band data.

        @values > 0 bytes, in multiples of 4 bytes */
}
#include "spf_end_pack.h"
;

typedef struct asps_cmd_header_t asps_cmd_header_t;

/**
   Payload of the register events data structure.
*/

#include "spf_begin_pack.h"

struct asps_cmd_register_module_events_t
{
   uint32_t event_id;
   /**< Valid event ID of the module */

   uint32_t module_instance_id;
   /**< Valid module instance id in the ADSP domain.*/

   uint32_t is_register;
   /**< 1 - to register the event
    *   0 - to de-register the event
    */

   uint32_t event_config_payload_size;
   /**< Size of the event config data.
        @values >= 0 bytes, in multiples of
        4 bytes */
}
#include "spf_end_pack.h"
;
typedef struct asps_cmd_register_module_events_t asps_cmd_register_module_events_t;

/**
  Registers/de-registers one or more event IDs.

  @Payload struct
     asps_cmd_header_t
     asps_cmd_register_module_events_t
     <event config payload if any>
     <8 byte alignment if any, but not mandatory at the end>

  When there are multiple event registration, different
  asps_cmd_register_module_events_t must be 8 byte aligned.

  @gpr_hdr_fields
  Opcode -- ASPS_CMD_REGISTER_MODULE_EVENTS

  @return
  Error code -- #ASPS_BASIC_RSP_RESULT

 */
#define ASPS_CMD_REGISTER_MODULE_EVENTS       0x0100104A

/**
   Payload of the event data structure.
   This structure is followed by event id specific payload
*/
#include "spf_begin_pack.h"

struct asps_event_to_client_t
{
   uint32_t module_instance_id;
   /**< Valid instance ID of module
        @values  */
        
   uint32_t event_id;
   /**< Valid event ID of the module */

   uint32_t event_payload_size;
   /**< Size of the event data based upon the
        module_instance_id/eveny_id combination.
        @values > 0 bytes, in multiples of
        4 bytes */

   uint32_t reserved;
   /**< For alignment purpose, must be set to zero. */
}
#include "spf_end_pack.h"
;

typedef struct asps_event_to_client_t asps_event_to_client_t;

/**
  Event raised by ASPS to the sensors client, if the event is registered
  by the client

  Payload struct --
     asps_event_to_client_t
     <event payload if any>
     <8 byte alignment if any>
     asps_event_to_client_t
     <event payload if any>
     <8 byte alignment if any, but not mandatory at the end>

  @gpr_hdr_fields
  Opcode -- ASPS_EVENT_TO_CLIENT

  @return
  None.
 */
#define ASPS_EVENT_TO_CLIENT                0x0300100E

/**
   Payload of the parameter data structure.
   Immediately following this structure are param_size bytes of
   calibration data. The structure and size depend on the param_id
*/

#include "spf_begin_pack.h"

struct asps_module_param_data_t
{
   uint32_t module_instance_id;
   /**< Valid instance ID of module
        @values  */

   uint32_t param_id;
   /**< Valid ID of the parameter. */

   uint32_t param_size;
   /**< Size of the parameter data.
        @values >= 0 bytes, in multiples of
        4 bytes */

   uint32_t reserved;
   /**< For alignment purpose, must be set to zero. */
}
#include "spf_end_pack.h"
;

typedef struct asps_module_param_data_t asps_module_param_data_t;

/**
  Configures one or more parameter IDs.

  @gpr_hdr_fields
  Opcode -- ASPS_CMD_SET_CFG

  @Payload struct

    asps_cmd_header_t
    asps_module_param_data_t
    <>Param ID payload<>
    <8 byte alignment if any>
    asps_module_param_data_t
    <>Param ID payload <>
    <8 byte alignment if any, but not mandatory at the end>

  @return
  Error code -- #ASPS_BASIC_RSP_RESULT

 */
#define ASPS_CMD_SET_CFG                  0x0100104B

/**
  This parameter is used for querying one or more
  configuration/calibration parameter ID.

  @gpr_hdr_fields
  Opcode -- ASPS_CMD_GET_CFG

  @Payload struct

    asps_cmd_header_t
    asps_module_param_data_t
    <>Param ID empty payload<>
    <8 byte alignment if any>
    asps_module_param_data_t
    <>Param ID empty payload <>
    <8 byte alignment if any, but not mandatory at the end>

  @return
  Response code -- #ASPS_CMD_RSP_GET_CFG

 */
#define ASPS_CMD_GET_CFG 0x01001049

/**
 * payload for #ASPS_CMD_RSP_GET_CFG
 */
#include "spf_begin_pack.h"

struct asps_cmd_rsp_get_cfg_t
{
   uint32_t status;
}
#include "spf_end_pack.h"
;

typedef struct asps_cmd_rsp_get_cfg_t asps_cmd_rsp_get_cfg_t;

/**
  The response code returned as an acknowledgment for the
  #ASPS_CMD_GET_CFG command.

  The response payload starts with status field which indicates
  the overall status of the #ASPS_CMD_GET_CFG command.

  Immediately following the status field is variable length
  payload consisting the parameter data for the param IDs
  which are queried as part of the #ASPS_CMD_GET_CFG command.
  Each PID data uses the header structure
  #asps_module_param_data_t followed by the actual param ID data.

  Client is responsible for specifying sufficiently large
  payload size to accommodate configuration/calibration data
  corresponding to each parameter ID data being queried.

  Status field is populated with success code (#AR_EOK) if all
  the parameter ID's being queried returns success code.

  Status field is populated with failure code (#AR_EFAILED) if
  at least one param ID results failure error code.

  @gpr_hdr_fields
  Opcode -- ASPS_CMD_RSP_GET_CFG

  @Payload struct

    asps_cmd_rsp_get_cfg_t
    asps_module_param_data_t
    <>Param ID  payload<>
    <8 byte alignment if any>
    asps_module_param_data_t
    <>Param ID  payload <>
    <8 byte alignment if any, but not mandatory at the end>

  @return
  None

 */
#define ASPS_CMD_RSP_GET_CFG 0x0200100C

#include "spf_begin_pack.h"

struct asps_basic_cmd_rsp_t
{
   uint32_t opcode; /**< Command operation code that completed. */
   uint32_t status; /**< Completion status. */
}
#include "spf_end_pack.h"
;

typedef struct asps_basic_cmd_rsp_t asps_basic_cmd_rsp_t;

/**
  The basic response code returned as an acknowledgment for the
  asps command.

  @gpr_hdr_fields
  Opcode -- ASPS_CMD_BASIC_RSP

  @Payload struct

    asps_basic_cmd_rsp_t

  @return
  None

 */
#define ASPS_CMD_BASIC_RSP 0x0200100D

/*------------------------------------------------------------------------------
 *  PARAM_ID_ASPS_GET_SUPPORTED_CONTEXT_IDS
 *----------------------------------------------------------------------------*/

/**
  The parameter used to get the list of supported context ids from ASPS.
  This param is used through cmd ASPS_CMD_GET_CFG and response is received
  through cmd APM_CMD_RSP_GET_CFG.

  @Payload struct for APM_CMD_GET_CFG
     asps_cmd_header_t
     asps_module_param_data_t

  @Payload struct for response APM_CMD_RSP_GET_CFG
     asps_cmd_rsp_get_cfg_t
     asps_module_param_data_t
     param_id_asps_get_supported_context_ids_t
     <array of supported context ids?

  @gpr_hdr_fields
  Opcode -- APM_CMD_GET_CFG

  @return
  Opcode -- #APM_CMD_RSP_GET_CFG

 */
#define PARAM_ID_ASPS_GET_SUPPORTED_CONTEXT_IDS                 0x08001341

/**
   Payload of the get config response of the PARAM_ID_ASPS_GET_SUPPORTED_CONTEXT_IDS data structure. 
   This structure is preceeded by  asps_module_param_data_t structure.
*/
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct param_id_asps_get_supported_context_ids_t
{
   uint32_t num_supported_contexts;
   /**< Number of supported context ids */

   uint32_t supported_context_ids[0];
   /**< Variable length array of supported context ids. 
        Length of the array is "num_supported_context" */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

typedef struct param_id_asps_get_supported_context_ids_t param_id_asps_get_supported_context_ids_t;

/*------------------------------------------------------------------------------
 *  PARAM_ID_ASPS_SENSOR_USECASE_REGISTER
 *----------------------------------------------------------------------------*/

/**
  The param is used to register a given usecase from sensors.
  This param is through ASPS_CMD_SET_CFG.

  The usecase specific config is required only if is_register = true. else
  passing the usecase_id is enough to de-register the usecase.

  sizeof of the param payload = sizeof(param_id_asps_sensor_usecase_register_t)
                                 + <usecase id specific payload size>;

  @Payload struct
     asps_cmd_header_t
     asps_module_param_data_t
     param_id_asps_sensor_usecase_register_t
     <usecase id specifc config id any>
     <8 byte alignment if any, but not mandatory at the end>

  @gpr_hdr_fields
  Opcode -- ASPS_CMD_SET_CFG

  @return
  Error code -- #ASPS_CMD_BASIC_RSP

 */
#define PARAM_ID_ASPS_SENSOR_USECASE_REGISTER                     0x0800133F

/**
   Payload of the  PARAM_ID_ASPS_SENSOR_USECASE_REGISTER data structure. This
   structure is preceeded by  asps_module_param_data_t structure.
*/
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct param_id_asps_sensor_usecase_register_t
{
   uint32_t is_register;
   /**< Indicates if sensor is trying to register/de-register 
      
        value > 0 : register
        value == 0: deregister. */

   uint32_t usecase_id;
   /**< Indicates the usecase which a given sensor instance is trying to 
        register/de-regsiter.

        Supported usecase ids:
          1. ASPS_USECASE_ID_ACD
          2. ASPS_USECASE_ID_UPD 
          3. ASPS_USECASE_ID_PCM_DATA
          etc. */
	
   uint32_t payload_size;
   /**< Size of the "usecase_id" specific payload if any. This payload is 
        required only if is_register = true.
        
        Payload structures for usecases:
            1. ASPS_USECASE_ID_ACD - asps_acd_usecase_register_payload_t 
            2. ASPS_USECASE_ID_UPD - No payload 
            3. ASPS_USECASE_ID_PCM_DATA - asps_pcm_data_usecase_register_payload_t*/

   uint32_t payload[0];
   /**< Points to usecase_id specific paylod if any*/
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;
typedef struct param_id_asps_sensor_usecase_register_t param_id_asps_sensor_usecase_register_t;



/*------------------------------------------------------------------------------
 *  EVENT_ID_ASPS_SENSOR_USECASE_INFO
 *----------------------------------------------------------------------------*/

/**
  Event raised by ASPS to update usecase specific info to Sensors.
  
  The event has a variable payload which is specific to usecase id. For example,
  ACD usecase will contain list of context ids for a given module instance id.

  @Payload struct
     asps_event_to_client_t
     event_id_asps_sensor_usecase_info_t
     <usecase specific payload if any>
     <8 byte alignment if any, but not mandatory at the end>

  @gpr_hdr_fields
  Opcode -- ASPS_MODULE_EVENT_TO_CLIENT

  @return
 */
#define EVENT_ID_ASPS_SENSOR_USECASE_INFO                    0x08001340

/**
   Payload of the  EVENT_ID_ASPS_SENSOR_USECASE_INFO data structure. This
   structure is preceeded by asps_module_event_t structure and followed by 
   usecase specific payload if any.
*/
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct event_id_asps_sensor_usecase_info_t
{
   uint32_t usecase_id;
   /**< Indicates the usecase which a given sensor instance is trying to 
        register.

        Supported usecase ids:
          1. ASPS_USECASE_ID_ACD
          2. ASPS_USECASE_ID_UPD 
          3. ASPS_USECASE_ID_PCM_DATA */
	
   uint32_t payload_size;
   /**< Size of the "usecase_id" specific payload if any.        
         
        Payload structures for usecases:
         1. ASPS_USECASE_ID_ACD - 
            < asps_acd_usecase_register_ack_payload_t > - payload of ACD MIID 1
            < asps_acd_usecase_register_ack_payload_t > - payload of ACD MIID 2

         2. ASPS_USECASE_ID_UPD - asps_upd_usecase_register_ack_payload_t
         
         3. ASPS_USECASE_ID_PCM_DATA - asps_audio_data_usecase_register_ack_payload_t 
      
      >*/

   uint32_t payload[0];
   /**< Points to usecase_id specific paylod if any*/
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;
typedef struct event_id_asps_sensor_usecase_info_t event_id_asps_sensor_usecase_info_t;

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif //_ASPS_API_H_
