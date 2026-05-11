#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Mathematical Utility functions for use by Sensor Developers.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <math.h>

/*=============================================================================
  Macro Definitions
  ===========================================================================*/
#ifndef G

/**
 * @brief Value for gravity (m/s^2).
 *
 */
#define G 9.80665f
#endif

#ifndef PI

/**
 * @brief Value for PI.
 *
 */
#define PI 3.14159f
#endif

/**
 * @brief SNS_MAX finds and returns the max of two values provided.
 *
 * @param[in] a First input of comparison.
 * @param[in] b Second input of comparison.
 *
 */
#define SNS_MAX(a, b)                                                          \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

/**
 * @brief SNS_MIN finds and returns the min of two values provided.
 *
 * @param[in] a First input of comparison.
 * @param[in] b Second input of comparison.
 *
 */
#define SNS_MIN(a, b)                                                          \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief 3x1 column vector.
 *
 */
typedef struct vector3
{
  union
  {
    struct
    {
      float x, y, z;
    };
    float data[3];
  };
} vector3;

/**
 * @brief 3x3 square matrix (row major).
 *
 */
typedef struct matrix3
{
  union
  {
    struct
    {
      float e00, e01, e02;
      float e10, e11, e12;
      float e20, e21, e22;
    };
    float data[9];
  };
} matrix3;

/**
 * @brief Creates vector3 from three numbers.
 *
 * @param[in] x First number.
 * @param[in] y Second number.
 * @param[in] z Third number.
 *
 * @return
 * - vector3 Created by input three parameters.
 *
 */
static inline vector3 make_vector3(float x, float y, float z)
{
  vector3 vec;

  vec.x = x;
  vec.y = y;
  vec.z = z;

  return vec;
}

/**
 * @brief Creates vector3 from an array.
 *
 * @param[in] data Input array pointer with three numbers that need to be
 * converted into vector3.
 *
 * @return
 * - vector3 Created by input array.
 *
 */
static inline vector3 make_vector3_from_array(const float *data)
{
  vector3 vec;

  vec.x = data[0];
  vec.y = data[1];
  vec.z = data[2];

  return vec;
}

/**
 * @brief Adds one 3D vector to another.
 *
 * @param[in] a First vector3 that need to be added.
 * @param[in] b Second vector3 that need to be added.
 *
 * @return
 * - vector3 Result of two vector3 addition (a + b).
 *
 */
static inline vector3 vector3_add(vector3 a, vector3 b)
{
  return make_vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

/**
 * @brief Subtracts one 3D vector from another.
 *
 * @param[in] a Vector3 from which another vector3 gets subtracted.
 * @param[in] b Vector3 that get subtracted.
 *
 * @return
 * - vector3 Result of two vector3 subtraction (a - b).
 *
 */
static inline vector3 vector3_sub(vector3 a, vector3 b)
{
  return make_vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

/**
 * @brief Multiply two 3x3 matrices.
 *
 * @param[in] a First matrix.
 * @param[in] b Second matrix.
 *
 * @return
 * - matrix3 Result of matric multiplication.
 *
 */
static inline matrix3 matrix3_multiply(matrix3 a, matrix3 b)
{
  matrix3 ab;
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      float sum = 0.0;
      for(int k = 0; k <= 2; k++)
      {
        sum += a.data[i * 3 + k] * b.data[k * 3 + j];
      }
      ab.data[i * 3 + j] = sum;
    }
  }
  return ab;
}

/**
 * @brief Muliply a 3x3 matrix to a 3x1 vector.
 *
 * @param[in] m  3x3 Matrix.
 * @param[in] v  3x1 vector.
 *
 * @return
 * - vector3 Result of 3x3 Matrix multiplication with 3x1 vector.
 *
 */
static inline vector3 matrix3_mul_vector3(matrix3 m, vector3 v)
{
  vector3 mv;

  for(int i = 0; i < 3; i++)
  {
    mv.data[i] = m.data[i * 3 + 0] * v.data[0] + m.data[i * 3 + 1] * v.data[1] +
                 m.data[i * 3 + 2] * v.data[2];
  }
  return mv;
}