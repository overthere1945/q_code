#pragma once

/*=============================================================================
  @file sns_ai_oem1_algo_eai_model.h

  AI_OEM1 EAI models and parameters

  Generated using AIMET(torch-cpu-1.24.0) and eAI(5.6.0)

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/
#include <stdint.h>
#include "eai.h"

/*=============================================================================
  Static Data Definitions
  ============================================================================*/

#define SNS_AI_OEM1_EAI_MODEL_MEMSIZE        (3872)
#define SNS_AI_OEM1_EAI_MODEL_LENOUTPUT      (1)

#define SNS_AI_OEM1_EAI_MAX_INPUTPORT_COUNT  (3)
#define SNS_AI_OEM1_EAI_MAX_OUTPUTPORT_COUNT (1)

typedef enum
{
  SNS_AI_OEM1_EAI_MODEL_OUTPUTLABEL_DOWN = 0,
  SNS_AI_OEM1_EAI_MODEL_OUTPUTLABEL_UP = 1,
  SNS_AI_OEM1_EAI_MODEL_OUTPUTLABEL_UNKNOW = 2,
} sns_ai_oem1_eai_model_outputlabel_t;

/*=============================================================================
  Type Definitions
  ============================================================================*/

typedef enum
{
  EAI_INPUT_PORT = 0,
  EAI_OUTPUT_PORT,
  EAI_MAX_PORT,
} sns_ai_oem1_eai_model_tensor_port_t;

typedef struct sns_ai_oem1_eai_model_quant_info_t
{
  float max;
  float min;
  float scale;
  int32_t offset;
  uint32_t bitwidth;
  uint8_t dtype[3]; // "int" or "flo"
  bool is_symmetric;
} sns_ai_oem1_eai_model_quant_info_t;

typedef struct sns_ai_oem1_algo_eai_context_t
{

  eaih_t eai_handle;
  eai_rt_info_t rt_info;

  eai_memory_info_t model_buffer;
  eai_memory_info_t scratch_buffer;
  eai_memory_info_t persistent_buffer;

  uint32_t io_tensors_count[EAI_MAX_PORT];

  eai_buffer_info_t input_buffers[SNS_AI_OEM1_EAI_MAX_INPUTPORT_COUNT];
  eai_buffer_info_t output_buffers[SNS_AI_OEM1_EAI_MAX_OUTPUTPORT_COUNT];
  eai_batch_info_t io_batch;

  eai_client_perf_config_t perf_config;
  eai_model_meta_info_t eai_model_meta_info;

  uint32_t eai_init_flags;
  EAI_MEM_TYPE buffer_memtype;
  
} sns_ai_oem1_algo_eai_context_t;

/*=============================================================================
  External Reference
  ============================================================================*/
  
extern const uint8_t sns_ai_oem1_eai_model[SNS_AI_OEM1_EAI_MODEL_MEMSIZE];
extern const sns_ai_oem1_eai_model_quant_info_t sns_ai_oem1_eai_model_input_quantization;
extern const sns_ai_oem1_eai_model_quant_info_t sns_ai_oem1_eai_model_output_quantization;
