#pragma once

/** ============================================================================
 * @file
 *
 * @brief Infinite Impulse Response(IIR) filter utility header file.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/

#include <stdint.h>
#include "sns_matrix.h"

#define FX_IIR_FILTER_ORDER 2
/*=============================================================================
  Typedefs
  ===========================================================================*/
/**
 * @brief Infinite Impulse Response(IIR) filter configuration structure.
 *
 */
typedef struct
{
  int32_t fc; /*!< LPF cutoff frequency (Q16) */
  int32_t fs; /*!< sample frequency (Q16) */
  int32_t channels; /*!< Number of channels */
} fx_iir_config_s;

/**
 * @brief Infinite Impulse Response(IIR) filter input structure.
 *
 */
typedef struct
{
  int32_t *filter_input; /*!< pointer to an array of length channels */
} fx_iir_input_s;

/**
 * @brief Infinite Impulse Response(IIR) filter output structure.
 *
 */
typedef struct
{
  int32_t *filter_output; /*!< pointer to an array of length channels */
} fx_iir_output_s;

/**
 * @brief Infinite Impulse Response(IIR) filter state structure.
 *
 */
typedef struct
{
  fx_iir_config_s config;               /*!< Filter configuration data. */
  int32_t num[FX_IIR_FILTER_ORDER + 1]; /*!< numerator coefficients. */
  int32_t den[FX_IIR_FILTER_ORDER + 1]; /*!< denominator coefficients. */
  matrix_type input;                    /*!< input matrix for the IIR filter.*/
  matrix_type output;                   /*!< output matrix for the IIR filter.*/
  int32_t settling_samples;             /*!< number of samples required to
                                         *   settle down.
                                         */
  int32_t filter_count;                 /*!< number of samples processed */
} fx_iir_state_s;

/*=============================================================================
  Public Function Declaration
  ===========================================================================*/
/**
 * @brief Computes memory requirement for the IIR filter.
 *
 * @param [in] config_data Pointer to a IIR filter configuration structure.
 *
 * @return
 *   -int32_t:     Memory requirement in bytes.
 *
 */
int32_t fx_iir_lpf_mem_req(fx_iir_config_s *config_data);

/**
 * @brief Initializes the IIR filter with default values.
 *
 * @param [in] config_data Pointer to a IIR filter configuration structure.
 * @param [in] mem         Pointer to allocated memory block.
 *
 *
 * @return
 *  - fx_iir_state_s:   Pointer to IIR filter state structure.
 *
 */
fx_iir_state_s *fx_iir_lpf_state_reset(fx_iir_config_s *config_data, void *mem);

/**
 * @brief Updates the IIR filter.
 *
 * @param [in] fx_iir    Pointer to a IIR filter state structure.
 * @param [in] input     Pointer to a IIR filter input structure.
 * @param [out] output   Pointer to a IIR filter output structure.
 *
 * @return
 *   - None.
 *
 */
void fx_iir_lpf_update(fx_iir_state_s *fx_iir, fx_iir_input_s *input,
                       fx_iir_output_s *output);

/**
 * @brief Computes whether the filter is settled or not.
 *
 * @param [in] fx_iir    Pointer to a IIR filter state structure.
 *
 * @return
 *  - 0:       If settled.
 *  - 1:       Otherwise.
 *
 */
int32_t filter_settled(fx_iir_state_s *fx_iir);
