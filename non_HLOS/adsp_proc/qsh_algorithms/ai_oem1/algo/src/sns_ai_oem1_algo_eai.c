/*=============================================================================
  @file sns_ai_oem1_algo_eai.c

  AI_OEM1 algo EAI APIs
  
  This file contains APIs to load the eAI model, initialize the eAI runtime 
  instance with the model, and set/apply scratch and persistent buffers for 
  the runtime instance via the "eai_handle" obtained at init.

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*=============================================================================
  Include Files
  ============================================================================*/

#include "sns_ai_oem1_algo.h"

/*=============================================================================
  Public Function Definition
  ============================================================================*/

void sns_ai_oem1_eai_model_load(
    sns_ai_oem1_algo_eai_context_t * eai_context, 
    const char * eai_model, 
    size_t eai_model_size )
{

  eai_context->model_buffer.memory_size = eai_model_size;
  eai_context->model_buffer.memory_type = eai_context->buffer_memtype;
  sns_memscpy( eai_context->model_buffer.addr,
               eai_model_size,
               eai_model,
               eai_model_size );

}

/* ---------------------------------------------------------------------------*/

bool sns_ai_oem1_eai_model_init(
    sns_ai_oem1_algo_eai_context_t * context )
{

  EAI_RESULT eai_ret = EAI_SUCCESS;
  bool rc = true;

  eai_ports_info_t ports_info;
  sns_ai_oem1_eai_model_tensor_port_t tensor_port;

  // eAI: Init model and get handle
  eai_ret = eai_init( &( context->eai_handle ),
                      &( context->model_buffer ),
                      context->eai_init_flags );

  
  if( eai_ret == EAI_SUCCESS )
  {
    // eAI: Get Scratch memory footprint
    eai_ret = eai_get_property( context->eai_handle,
                                EAI_PROP_SCRATCH_MEM,
                                &context->scratch_buffer );
  }
  
  if( eai_ret == EAI_SUCCESS )
  {
    // eAI: Get Persistent memory footprint
    eai_ret = eai_get_property( context->eai_handle,
                                EAI_PROP_PERSISTENT_MEM,
                                &context->persistent_buffer );
  }
  
  if( eai_ret == EAI_SUCCESS )
  {
    // eAI: Get input and output counts
    for( tensor_port = EAI_INPUT_PORT; tensor_port < EAI_MAX_PORT; ++tensor_port )
    {
      ports_info.input_or_output = tensor_port;
      eai_ret = eai_get_property( context->eai_handle,
                                  EAI_PROP_PORTS_NUM,
                                  &ports_info );
      if( eai_ret == EAI_SUCCESS )
      {
        context->io_tensors_count[tensor_port] = ports_info.size;
      }
    }
  }

  if( eai_ret != EAI_SUCCESS )
  {
    rc = false;
  }
  return rc;
}

/* ---------------------------------------------------------------------------*/

bool sns_ai_oem1_eai_model_setbuffers(
    sns_ai_oem1_algo_eai_context_t * context, 
    void * restrict scratch_buf, 
    void * restrict persistent_buf )
{

  EAI_RESULT eai_ret = EAI_SUCCESS;
  uint32_t i;
  bool rc = true;

  sns_ai_oem1_eai_model_tensor_port_t tensor_port;
  eai_tensor_info_t tensor_temp;
  eai_buffer_info_t* buffer_ptr;

  // eAI: Set scratch buffer
  context->scratch_buffer.addr = scratch_buf;
  context->scratch_buffer.memory_type = context->buffer_memtype;
  eai_ret = eai_set_property( context->eai_handle,
                              EAI_PROP_SCRATCH_MEM,
                              &( context->scratch_buffer ) );

  if( eai_ret == EAI_SUCCESS )
  {
    // eAI: Set persistent buffer
    context->persistent_buffer.addr = persistent_buf;
    context->persistent_buffer.memory_type = context->buffer_memtype;
    eai_ret = eai_set_property( context->eai_handle,
                                EAI_PROP_PERSISTENT_MEM,
                                &( context->persistent_buffer ) );
  }
					  
  if( eai_ret == EAI_SUCCESS )
  {
    // eAI: Set Performance Config
    eai_ret = eai_set_property( context->eai_handle,
                                EAI_PROP_CLIENT_PERF_CFG,
                                &context->perf_config );
  }
  
  if( eai_ret == EAI_SUCCESS )
  {
    // eAI: Apply
    eai_ret = eai_apply( context->eai_handle );
  }
  
  if( eai_ret == EAI_SUCCESS )
  {
    // eAI: Set io buffers, tensors and batch
    for( tensor_port = EAI_INPUT_PORT; tensor_port < EAI_MAX_PORT; ++tensor_port )
    {
      switch( tensor_port )
      {
        case EAI_INPUT_PORT:
        {
          context->io_batch.num_inputs = context->io_tensors_count[tensor_port];
          context->io_batch.inputs = context->input_buffers;

          buffer_ptr = context->io_batch.inputs;
          break;
        }
        case EAI_OUTPUT_PORT:
        {
          context->io_batch.num_outputs = context->io_tensors_count[tensor_port];
          context->io_batch.outputs = context->output_buffers;

          buffer_ptr = context->io_batch.outputs;
          break;
        }
        default:
        {
          buffer_ptr = NULL;
          break;
        }
      }

      for( i = 0; i < context->io_tensors_count[tensor_port]; i++ )
      {
        tensor_temp.input_or_output = tensor_port;
        tensor_temp.index = i;

        eai_ret = eai_get_property( context->eai_handle,
                                    EAI_PROP_TENSOR_INFO,
                                    &( tensor_temp ) );

		if( eai_ret == EAI_SUCCESS )
        {
          if( tensor_temp.address != NULL ) // normally, this address is pointing location in eAI handle,
          {
            buffer_ptr[i].index = i;
            buffer_ptr[i].addr = (uint8_t*)( tensor_temp.address );
            buffer_ptr[i].buffer_size = tensor_temp.tensor_size;
            buffer_ptr[i].element_type = tensor_temp.element_type;
            buffer_ptr[i].memory_type = context->buffer_memtype;
          }
          else
          {
            rc = false;
          }
        }
      }
    }
  }

  if( eai_ret != EAI_SUCCESS )
  {
    rc = false;
  }

  return rc;
}

/* ---------------------------------------------------------------------------*/
