#ifndef SNS_FIXED_POINT_H
#define SNS_FIXED_POINT_H

/** ============================================================================
 * @file
 *
 * @brief Fixed point utility header file.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/

#include <stdint.h>
#ifdef QDSP6
#include <hexagon_protos.h>
#endif /* QDSP6 */

#ifdef _WIN32
// @brief suppress warning C4293 on visual studio platform
#pragma warning(disable : 4293)
#endif /* _WIN32 */

/*=============================================================================
  Typedef and Macros
  ===========================================================================*/
typedef int32_t q16_t;

/** @brief compute the absolute value of a 32-bit integer */
#define FX_ABS(x) ((x < 0) ? (-(x)) : (x))
/** @brief compute the minimum of two values */
#define FX_MIN(x, y)      (((x) < (y)) ? (x) : (y))
/** @brief compute the maximum of two values */
#define FX_MAX(x, y)      (((x) > (y)) ? (x) : (y))
/** @brief bound a value between two limits */
#define FX_BOUND(y, x, z) (FX_MIN(FX_MAX((x), (y)), (z)))
/** @brief negate a value based on a flag */
#define FX_NEG(x, flag)   ((flag) ? (-(x)) : (x))
/** @brief compute the fixed-point representation of 1 */
#define FX_ONE(q)         ((int32_t)1 << q)
/** @brief compute the fixed-point representation of 0.5 */
#define FX_HALF(q)        (FX_ONE(q) >> 1)
/** @brief extract the sign bit of a 32-bit integer */
#define FX_SIGN32(x)      (((x) >> 31) & 1)

/** @brief convert a floating-point number to fixed-point */
#define FX_FLTTOFIX(f, q)                                                      \
  ((int32_t)((f) * (1 << (q)) + (((f) > (0)) ? (0.5f) : (-0.5f))))
/** @brief convert a fixed-point number to double */
#define FX_FIXTOFLT(i, q) (((double)(i)) / ((double)(1 << (q))))
/** @brief convert a fixed-point number to float (single precision) */
#define FX_FIXTOFLT_SP(i, q)                                                   \
  (((float)(i)) / ((float)(1 << (q)))) /*!< Single Precision */
/** @brief compute the floor of a fixed-point number */
#define FX_FLOOR(a, q) ((a) & (0xFFFFFFFF << (q)))
/** @brief round a fixed-point number */
#define FX_ROUND(a, q) (FX_FLOOR((a) + FX_HALF(q), q))
/** @brief compute the ceiling of a fixed-point number */
#define FX_CEIL(a, q)  (FX_FLOOR((a) + (FX_ONE(q) - 1), q))

/** @brief convert a fixed-point number from one format to another */
#define FX_CONV(a, q1, q2)                                                     \
  (((q2) > (q1)) ? (a) << ((q2) - (q1)) : (a) >> ((q1) - (q2)))
/** @brief add two fixed-point numbers */
#define FX_ADD(a, b, q1, q2, q3)                                               \
  (FX_CONV((a), (q1), (q3)) + FX_CONV((b), (q2), (q3)))
/** @brief subtract two fixed-point numbers */
#define FX_SUB(a, b, q1, q2, q3)                                               \
  (FX_CONV((a), (q1), (q3)) - FX_CONV((b), (q2), (q3)))
/** @brief multiply two fixed-point numbers */
#define FX_MUL(a, b, q1, q2, q3)                                               \
  ((int32_t)(FX_CONV((((int64_t)(int32_t)(a)) * ((int64_t)(int32_t)(b))),      \
                     ((q1) + (q2)), (q3))))
/** @brief divide two fixed-point numbers */
#define FX_DIV(a, b, q1, q2, q3)                                               \
  ((int32_t)(FX_CONV(((int64_t)(int32_t)(a)), (q1), (q2) + (q3)) /             \
             ((int64_t)(int32_t)(b))))

/**
 * @defgroup Q16Macros Q16 Fixed-Point Macros
 * @{
 */
/** @brief Q16 fixed-point factor */
#define FX_QFACTOR (16)
/** @brief maximum value for Q16 fixed-point */
#define FX_MAX_Q16 ((int32_t)0x7FFFFFFF)
/** @brief minimum value for Q16 fixed-point */
#define FX_MIN_Q16 ((int32_t)0x80000000)
/** @} */

/** @brief a floating-point number to Q16 fixed-point */
#define FX_FLTTOFIX_Q16(d) (FX_FLTTOFIX(d, FX_QFACTOR))
/** @brief convert a Q16 fixed-point number to double */
#define FX_FIXTOFLT_Q16(a) (FX_FIXTOFLT(a, FX_QFACTOR))
/** @brief convert a Q16 fixed-point number to float (single precision) */
#define FX_FIXTOFLT_Q16_SP(a)                                                  \
  (FX_FIXTOFLT_SP(a, FX_QFACTOR)) // Single Precision
/** @brief compute the floor of a Q16 fixed-point number */
#define FX_FLOOR_Q16(a) (FX_FLOOR(a, FX_QFACTOR))
/** @brief round a Q16 fixed-point number */
#define FX_ROUND_Q16(a) (FX_ROUND(a, FX_QFACTOR))
/** @brief compute the ceiling of a Q16 fixed-point number */
#define FX_CEIL_Q16(a)  (FX_CEIL(a, FX_QFACTOR))

/** @brief convert a fixed-point number to Q16 format */
#define FX_CONV_Q16(a, q1) (FX_CONV(a, q1, FX_QFACTOR))
/** @brief multiply two Q16 fixed-point numbers */
#define FX_MUL_Q16(a, b)                                                       \
  ((int32_t)FX_MUL(a, b, FX_QFACTOR, FX_QFACTOR, FX_QFACTOR))
/** @brief divide two Q16 fixed-point numbers */
#define FX_DIV_Q16(a, b)                                                       \
  ((int32_t)FX_DIV(a, b, FX_QFACTOR, FX_QFACTOR, FX_QFACTOR))

/**
 * @defgroup Q16Constants Q16 Fixed-Point Constants
 * @{
 */
/** @brief Q16 fixed-point constant for π/4 */
#define FX_PIOVER4_Q16 (FX_FLTTOFIX_Q16(0.25f * PI))
/** @brief Q16 fixed-point constant for π/2 */
#define FX_PIOVER2_Q16 (FX_FLTTOFIX_Q16(0.5f * PI))
/** @brief Q16 fixed-point constant for π */
#define FX_PI_Q16 (FX_FLTTOFIX_Q16(1.0f * PI))
/** @brief Q16 fixed-point constant for 3π/2 */
#define FX_3PIOVER2_Q16 (FX_FLTTOFIX_Q16(1.5f * PI))
/** @brief Q16 fixed-point constant for 2π */
#define FX_2PI_Q16 (FX_FLTTOFIX_Q16(2.0f * PI))
/** @brief Q16 fixed-point constant for 1 */
#define FX_ONE_Q16 (FX_ONE(FX_QFACTOR))
/** @brief Q16 fixed-point constant for 0.5 */
#define FX_HALF_Q16 (FX_HALF(FX_QFACTOR))
/** @} */

#ifndef PI
#define PI (3.14159265358979323846f)
#endif

#define FX_G_Q16 (FX_FLTTOFIX_Q16(G))

/*=============================================================================
  Public Function Declaration
  ===========================================================================*/
/**
 * @brief  Computes the square root.
 *
 * @param [in] val Q16 value of which to compute the square root.
 *
 * @return
 *  - uint32_t:   Q16 representation of the square root.
 *
 */
uint32_t sqrtFxQ16(uint32_t val);

/**
 * @brief  Computes sine, cosine and tangent for an angle in radians.
 *
 * @param [in] theta   Angle in radians.
 * @param [out] sin    Sin output.
 * @param [out] cos    Cos output.
 * @param [out] tan    Tan output.
 *
 * @return
 *    - None.
 *
 */
void sinCosTanFxQ16(int32_t theta, int32_t *sin, int32_t *cos, int32_t *tan);

/**
 * @brief  Converts tanagent to Q16.
 *
 * @param [in] num Numerator.
 * @param [in] den Denominator.
 *
 * @return
 *   - int32_t:      Arc tangent2 of num/dem.
 *
 */
int32_t arcTan2FxQ16(int32_t num, int32_t den);

/**
 * @brief  Converts Cosine to Q16.
 *
 * @param [in] cosTheta   Angle in radians.
 *
 * @return
 *    - int32_t:      Arc cos.
 *
 */
int32_t arcCosFxQ16(int32_t cosTheta);

/**
 * @brief  Converts Sine to Q16.
 *
 * @param [in] sinTheta   Angle in radians.
 *
 * @return
 *    -int32_t:       Arc sine.
 *
 */
int32_t arcSinFxQ16(int32_t sinTheta);

#endif /* SNS_FIXED_POINT_H */
