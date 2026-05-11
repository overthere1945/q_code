/*=============================================================================
  @file sns_ai_oem1_algo_quantization_island.c

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_ai_oem1_algo.h"

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

void sns_ai_oem1_aimet_quantization_apply(
    void * output, 
    void * input, 
    size_t length, 
    size_t pad, 
    sns_ai_oem1_eai_model_quant_info_t quant_info )
{

  uint32_t i;
  float data_in, data_in_min, data_in_max, data_in_scale;

  data_in_min = -(float)( 1 << ( quant_info.bitwidth - 1 ) );
  data_in_max = (float)( 1 << ( quant_info.bitwidth - 1 ) ) - 1;

  if( quant_info.is_symmetric == true )
  {
    data_in_scale = 2.0f *
                    ( SNS_AI_OEM1_MAX( fabsf( quant_info.max ), fabsf( quant_info.min ) ) );
  }
  else
  {
    data_in_scale = ( ( SNS_AI_OEM1_MAX( quant_info.max, 0 ) ) -
                      ( SNS_AI_OEM1_MIN( quant_info.min, 0 ) ) );
  }

  data_in_scale = data_in_scale / ( data_in_max - data_in_min ); 

  for( i = 0; i < length + pad; ++i )
  {
    if( i < length )
    {
      data_in = ( (float*)input )[i];
    }
    else
    {
      data_in = 0;
    }
    data_in /= data_in_scale;

    data_in = roundf( data_in );

    data_in = ( data_in > data_in_max ) ? data_in_max : data_in;
    data_in = ( data_in < data_in_min ) ? data_in_min : data_in;

    switch( quant_info.bitwidth )
    {
      case 8:
      {
        ( (int8_t*)output )[i] = (int8_t)data_in;
        break;
      }
      case 16:
      {
        ( (int16_t*)output )[i] = (int16_t)data_in;
        break;
      }
      case 32:
      {
        ( (int32_t*)output )[i] = (int32_t)data_in;
        break;
      }
      case 64:
      {
        ( (int64_t*)output )[i] = (int64_t)data_in;
        break;
      }
      default:
      {
        ( (int8_t*)output )[i] = (int8_t)data_in;
      }
    }
  }

}

void sns_ai_oem1_aimet_quantization_reverse(
    void * output, 
    void * input, 
    size_t length, 
    sns_ai_oem1_eai_model_quant_info_t quant_info )
{

  uint32_t i;
  float data_in, data_in_min, data_in_max, data_in_scale;

  data_in_min = -(float)( 1 << ( quant_info.bitwidth - 1 ) );
  data_in_max = (float)( 1 << ( quant_info.bitwidth - 1 ) ) - 1;

  if( quant_info.is_symmetric == true )
  {
    data_in_scale = 2.0f *
                    ( SNS_AI_OEM1_MAX( fabsf( quant_info.max ), fabsf( quant_info.min ) ) );
  }
  else
  {
    data_in_scale = ( ( SNS_AI_OEM1_MAX( quant_info.max, 0 ) ) -
                      ( SNS_AI_OEM1_MIN( quant_info.min, 0 ) ) );
  }

  data_in_scale = data_in_scale / ( data_in_max - data_in_min );

  for( i = 0; i < length; ++i )
  {
    switch( quant_info.bitwidth )
    {
      case 8:
      {
        data_in = (float)( (int8_t*)input )[i];
        break;
      }
      case 16:
      {
        data_in = (float)( (int16_t*)input )[i];
        break;
      }
      case 32:
      {
        data_in = (float)( (int32_t*)input )[i];
        break;
      }
      case 64:
      {
        data_in = (float)( (int64_t*)input )[i];
        break;
      }
      default:
      {
        data_in = (float)( (int8_t*)input )[i];
      }
    }
    data_in *= data_in_scale;

    ( (float*)output )[i] = data_in;
  }
}

