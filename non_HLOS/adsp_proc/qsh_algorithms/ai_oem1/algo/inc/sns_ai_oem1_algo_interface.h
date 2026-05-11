#pragma once
/*==============================================================================
  @file sns_ai_oem1_algo_interface.h

  AI_OEM1 Sensor Algorithm Interface

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sns_ai_oem1_algo_input.h"

/*=============================================================================
  Static Data Definitions
  ============================================================================*/

/*=============================================================================
  Type Definitions
  ============================================================================*/

typedef enum sns_ai_oem1_mem_location
{

  SNS_AI_OEM1_MEM_LOCATION_DDR = 1, // Main memory, only available in non-island mode
  SNS_AI_OEM1_MEM_LOCATION_LLC = 2, // Last level cache
  SNS_AI_OEM1_MEM_LOCATION_TCM = 3, // Tightly coupled memory for hardware

} sns_ai_oem1_mem_location;

typedef enum sns_ai_oem1_mem_type
{

  SNS_AI_OEM1_MEM_TYPE_ALGO_STATE_SIZE    = 1,
  SNS_AI_OEM1_MEM_TYPE_MODEL_SIZE         = 2,
  SNS_AI_OEM1_MEM_TYPE_MODEL_ALIGNMENT    = 3,

} sns_ai_oem1_mem_type;

typedef enum sns_ai_oem1_model_mem_type
{

  SNS_AI_OEM1_MODEL_MEM_TYPE_SCRATCH_SIZE    = 1,
  SNS_AI_OEM1_MODEL_MEM_TYPE_PERSISTENT_SIZE = 2,

} sns_ai_oem1_model_mem_type;

typedef enum sns_ai_oem1_device_state
{

  SNS_AI_OEM1_DEVICE_STATE_FACING_DOWN   = 0,  //Device Orientation is facing down
  SNS_AI_OEM1_DEVICE_STATE_FACING_UP     = 1,  //Device Orientation is facing up
  SNS_AI_OEM1_DEVICE_STATE_UNKNOWN       = 2,  //Device Orientation is unknown

} sns_ai_oem1_device_state;

/**
 *  Opaque handle for ai_oem1 algorithm
 */
typedef struct sns_ai_oem1_algo_handle sns_ai_oem1_algo_handle;

/*=============================================================================
  Public Function Declaration
  ============================================================================*/

/*!
  -----------------------------------------------------------------------------
  sns_ai_oem1_algo_get_mem_req()
  -----------------------------------------------------------------------------

  @brief  The API to query AI_OEM1 algorithm memory sizes.

  @param [i] mem_type   : AI_OEM1 Memory type ( sns_ai_oem1_mem_type )


  @return
  size_t       size : Memory size in bytes, failure if returns 0
  -----------------------------------------------------------------------------
*/

size_t sns_ai_oem1_algo_get_mem_req( sns_ai_oem1_mem_type mem_type );

/*!
  -----------------------------------------------------------------------------
  sns_ai_oem1_algo_get_model_mem_req()
  -----------------------------------------------------------------------------

  @brief  The API to query AI_OEM1 algorithm eAI model memory sizes.

  @param [i] algo_handle : Pointer to the algorithm state
  @param [i] mem_type   : AI_OEM1 Memory type ( sns_ai_oem1_mem_type )


  @return
  size_t         size : Memory size in bytes, failure if returns 0
  -----------------------------------------------------------------------------
*/

size_t sns_ai_oem1_algo_get_model_mem_req( sns_ai_oem1_algo_handle* algo_handle,
                                           sns_ai_oem1_model_mem_type mem_type );

/*!
  -----------------------------------------------------------------------------
  sns_ai_oem1_algo_model_init()
  -----------------------------------------------------------------------------

  @brief  The API to initialize AI_OEM1 model

  @param [i/o] algo_handle     : Pointer to the algorithm state
  @param [i/o] model_buf       : Pointer to the Pre allocated Model buffer,
                                 should be aligned with SNS_AI_OEM1_MEM_TYPE_MODEL_ALIGNMENT
  @param [i]   model_loc       : Memory location of model_buf and all other related
                                 buffers will be passed through sns_ai_oem1_algo_state_init
  @return
  true      When operation successful
  false     When operation fails
  -----------------------------------------------------------------------------
*/
bool sns_ai_oem1_algo_model_init( sns_ai_oem1_algo_handle* algo_handle,
                                  void* restrict model_buf,
                                  sns_ai_oem1_mem_location model_loc );

/*!
  -----------------------------------------------------------------------------
  sns_ai_oem1_algo_state_init()
  -----------------------------------------------------------------------------

  @brief  The API to initialize AI_OEM1 algorithm

  @param [i/o] algo_handle         : Pointer to the algorithm state
  @param [i/o] scratch_buf         : Pointer to the Pre allocated Scratch buffer
  @param [i/o] persistent_buf      : Pointer to the Pre allocated Persistent buffer

  @return
  true      When operation successful
  false     When operation fails
  -----------------------------------------------------------------------------
*/
bool sns_ai_oem1_algo_state_init( sns_ai_oem1_algo_handle* algo_handle,
                                  void* restrict scratch_buf,
                                  void* restrict persistent_buf );

/*!
  -----------------------------------------------------------------------------
  sns_ai_oem1_algo_deinit()
  -----------------------------------------------------------------------------

  @brief  The API to de-initialize AI_OEM1 algorithm state
  Note: algo_state and model_buffer must be
        freed after calling this function

  @param [i/o] algo_state          : Pointer to the algorithm state

  @return
  true      When operation successful
  false     When operation fails
  -----------------------------------------------------------------------------
*/

bool sns_ai_oem1_algo_deinit( sns_ai_oem1_algo_handle* algo_handle );

/*!
  -----------------------------------------------------------------------------
  sns_ai_oem1_algo_run()
  -----------------------------------------------------------------------------

  @brief  AI_OEM1 algorithm processing.

  @param [i/o] algo_handle            : Pointer to the algorithm state
  @param [i]   input                  : AI_OEM1 algo input
                                        ( accel )
  @param [o]   output                 : AI_OEM1 output state. Sensor should not report event if
                                        output state is "SNS_AI_OEM1_DEVICE_STATE_UNKNOWN".
  @param [o]   is_orientation_changed : Value is true when orientation change is detected.
                                        Sensor shall report event to the client.
                                        Value is false When orientation not changed.
  @return
  int16_t   Error code for run. If returns 0, execution is successful.
  -----------------------------------------------------------------------------
*/

int16_t sns_ai_oem1_algo_run( sns_ai_oem1_algo_handle* algo_handle,
                              sns_ai_oem1_algo_input* input,
                              sns_ai_oem1_device_state* output,
                              bool* is_orientation_changed );

/*!---------------------------------------------------------------------------*/
