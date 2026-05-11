#pragma once
/** ============================================================================
 * @file
 *
 * @brief Calibration Utility functions for use by Sensor Developers.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_math_util.h"

/*==============================================================================
  Function Definitions
  ============================================================================*/

/**
 * @brief Calculates calibrated value of a sample for tri-axial sensors
 * based on following equation.
 *
 * @details Sc = C * (Sr - B) where,
 * - Sc = calibrated sample (3x1 vector (x, y, z)).
 * - Sr = raw sample (3x1 vector (x, y, z)).
 * - C  = compensation matrix (3x3) B = bias (3x1 vector (x, y, z)).
 *
 * @param[in] sample_raw          vector3 of the raw sample.
 * @param[in] bias                vector3 of the bias.
 * @param[in] compensation_matrix matrix3 containing the compensation values.
 *
 * @return
 * - vector3 - 3x1 column vector calibrated sample.
 *
 */
static inline vector3
sns_apply_calibration_correction_3(vector3 sample_raw, vector3 bias,
                                   matrix3 compensation_matrix)
{
  return matrix3_mul_vector3(compensation_matrix,
                             vector3_sub(sample_raw, bias));
}
