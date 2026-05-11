#ifndef __ADC_H__
#define __ADC_H__
/*============================================================================
  @file adc.h

  Function and data structure declarations for ADC.


                Copyright (c) Qualcomm Technologies, Inc.
                All Rights Reserved.
                Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/
/* $Header: //components/rel/core.qdsp6/8.2.3/api/common/pmic/adc/adc.h#1 $ */

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "AdcInputs.h"
#include <stdint.h>
#include <stdbool.h>

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
/**
* @brief ADC return codes
*/
typedef enum
{
   ADC_STATUS_SUCCESS = 0,
   ADC_STATUS_ERROR,
   ADC_STATUS_INVALID_PARAM,
   ADC_STATUS_CHANNEL_NOT_SUPPORTED
} adc_status_type;

/**
* @brief ADC result structure
*/
typedef struct
{
   int32_t physical;     /**< The result in physical units. The units are defined in AdcInputs.h */
   uint32_t microvolts;  /**< The result in microvolts */
   uint32_t code;        /**< The raw ADC code from hardware */
} adc_result_type;

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/
/**
*  @brief Reads the desired ADC channel
*
*  @details This function reads an ADC channel and returns a structure with ADC results
*           in several units. The physical units for the channel are defined in
*           AdcInputs.h. For example, a temperature channel may have physical units
*           of degrees Celsius.
*
*           ADC channels are defined in AdcInputs.h. For all the supported channels for
*           a particular target, please see its ADC BSP files. If a channel is not
*           present in the BSP file it cannot be read.
*
*  @attention This function must be called after 'adc' is initialzied by RCINIT.
*
*  @param[in]  input       The ADC channel (from AdcInputs.h)
*  @param[out] adc_result  The ADC result
*
*  @return
*  ADC_STATUS_SUCCESS               -- Call succeeded \n
*  ADC_STATUS_ERROR                 -- Call failed \n
*  ADC_STATUS_INVALID_PARAM         -- Invalid parameters were specified \n
*  ADC_STATUS_CHANNEL_NOT_SUPPORTED -- The channel is not supported \n
*/
adc_status_type adc_read(const char *input, adc_result_type *adc_result);

#endif /* #ifndef __ADC_H__ */

