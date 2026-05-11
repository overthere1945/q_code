#pragma once

/** ============================================================================
 * @file
 *
 * @brief
 * - sfpFFT()     : Floating-point implementation of radix-4 FFT.
 * - gentwiddle() : Generate interleave & bit-reverse order twiddle factor
 *                  array for radix-4 floating-point implementation of FFT.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <math.h>
#include <stdint.h>
#include <complex.h>

/*=============================================================================
  Type Definitions
  ===========================================================================*/

typedef __complex__ float cfloat;

/*=============================================================================
  Public Function Declaration
  ===========================================================================*/

/**
 * @brief  Floating-point implementation of radix-4 FFT.
 *
 * @param [in]    Input    Pointer to input array.
 * @param [in]    points   N point FFT twiddle.
 * @param [inout] twiddles Pointer to twiddle factor array.
 * @param [inout] Output   Pointer to output array.
 *
 * @return
 *  - None.
 *
 */

void sfpFFT(cfloat *Input, int points, cfloat *twiddles, cfloat *Output);

/**
 * @brief  Generate interleave & bit-reverse order twiddle factor array for
 *         radix-4 floating-point implementation of FFT
 *
 * @param [out]   twiddle    Pointer to twiddle factor array.
 * @param [inout] NP         N point FFT twiddle.
 * @param [inout] log2NP     LOG2( NP ).
 * @param [inout] x          Pointer to scratch array of size NP/2.
 * @param [inout] y          Pointer to scratch array of size NP/4.
 *
 * @return
 *  - None.
 *
 */
void gentwiddle(cfloat *twiddle, uint32_t NP, uint32_t log2NP, cfloat *x,
                cfloat *y);
