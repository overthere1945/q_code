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

#include "sns_math_util.h"

#include <math.h>
#include <stdint.h>

/*=============================================================================
  Macro Definitions
  ===========================================================================*/

/**
 * @brief Value for the number of axes for a tri axial sensor.
 *
 */
#define TRI_AXIAL_SENSOR_AXES 3

/**
 * @brief SNS_TYPE_MAXVAL finds and returns the max value of the given data type
 *
 * @param[in] x Input data type
 *
 */
#define SNS_TYPE_MAXVAL(x)                                                     \
  _Generic((x), default                                                        \
           : UINT8_MAX, uint8_t                                                \
           : UINT8_MAX, uint16_t                                               \
           : UINT16_MAX, uint32_t                                              \
           : UINT32_MAX, uint64_t                                              \
           : UINT64_MAX, int8_t                                                \
           : INT8_MAX, int16_t                                                 \
           : INT16_MAX, int32_t                                                \
           : INT32_MAX, int64_t                                                \
           : INT64_MAX)

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief 4-dimensional rotation quaternion. Values can be accessed individually
 * or through an array.
 *
 */
typedef struct quaternion
{
  union
  {
    struct
    {
      float w, x, y, z;
    };
    float data[4];
  };
} quaternion;

/**
 * @brief 4-dimensional rotation quaternion.
 *
 */
typedef struct
{
  double w; /*!< Scalar part. */
  double x; /*!< Coefficient of i. */
  double y; /*!< Coefficient of j. */
  double z; /*!< Coefficient of k. */
} quaternion_d;

/*=============================================================================
  Function Definitions
  ===========================================================================*/

/**
 * @brief Creates identity matrix from the array.
 *
 * @param[out] matrix  The matrix to set.
 * @param[in] row      The number of rows.
 * @param[in] col      The number of columns.
 *
 * @return
 *  - None.
 *
 */
static inline void make_identity_matrix3(float *matrix, int row, int col)
{
  for(int i = 0; i < row; i++)
  {
    for(int j = 0; j < col; j++)
    { // Check if row is equal to column
      if(i == j)
        matrix[i + j] = 1;
      else
        matrix[i + j] = 0;
    }
  }
}

/**
 * @brief Calculates dot-product of two 3D vectors.
 *
 * @param[in] a First vector3.
 * @param[in] b Second vector3.
 *
 * @return
 * - float Result of two 3D vectors dot product (a dot b).
 *
 */
static inline float vector3_dot(vector3 a, vector3 b)
{
  return (a.x * b.x + a.y * b.y + a.z * b.z);
}

/**
 * @brief Calculates cross product of two 3D vectors.
 *
 * @param[in] a First vector3.
 * @param[in] b Second vector3.
 *
 * @return
 * - vector3 Result of two 3D vectors cross product (a cross b).
 *
 */
static inline vector3 vector3_cross(vector3 a, vector3 b)
{
  vector3 vec;
  // cross(A,B) = [Ay*Bz-Az*By, Az*Bx-Ax*Bz, Ax*By-Ay*Bx]
  vec.x = a.y * b.z - a.z * b.y;
  vec.y = a.z * b.x - a.x * b.z;
  vec.z = a.x * b.y - a.y * b.x;

  return vec;
}

/**
 * @brief Calculates vector's norm squared.
 *
 * @param[in] v 3x1 vector.
 *
 * @return
 * - float norm-squared of input vector3.
 *
 */
static inline float vector3_norm_sq(vector3 v)
{
  return (v.x * v.x + v.y * v.y + v.z * v.z);
}

/**
 * @brief Calculates norm (magnitude) of a 3x1 vector.
 *
 * @param[in] v vector3.
 *
 * @return
 * - float Vector norm of input vector3.
 *
 */
static inline float vector3_norm(vector3 v)
{
  return sqrtf(vector3_norm_sq(v));
}

/**
 * @brief Calculates norm (magnitude) of a quaternion.
 *
 * @param[in] q quaternion.
 *
 * @return
 * - float norm-squared of input quaternion.
 *
 */
static inline float quaternion_norm(quaternion q)
{
  return sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

/**
 * @brief Return a matrix3 given its rows as vectors.
 *
 * @param[in] r0 row 0.
 * @param[in] r1 row 1.
 * @param[in] r2 row 2.
 *
 * @return
 * - matrix3 Returns a matrix3 given its rows as vectors.
 *
 */
static inline matrix3 make_matrix3_by_rows(vector3 r0, vector3 r1, vector3 r2)
{
  matrix3 mat;

  mat.e00 = r0.x;
  mat.e01 = r0.y;
  mat.e02 = r0.z;
  mat.e10 = r1.x;
  mat.e11 = r1.y;
  mat.e12 = r1.z;
  mat.e20 = r2.x;
  mat.e21 = r2.y;
  mat.e22 = r2.z;

  return mat;
}

/**
 * @brief Returns a matrix given its columns as vectors.
 *
 * @param[in] c0 column 0.
 * @param[in] c1 column 1.
 * @param[in] c2 column 2.
 *
 * @return
 * - matric3 Returns a matrix3 given its columns as vectors.
 *
 */
static inline matrix3 make_matrix3_by_cols(vector3 c0, vector3 c1, vector3 c2)
{
  matrix3 mat;

  mat.e00 = c0.x;
  mat.e01 = c1.x;
  mat.e02 = c2.x;
  mat.e10 = c0.y;
  mat.e11 = c1.y;
  mat.e12 = c2.y;
  mat.e20 = c0.z;
  mat.e21 = c1.z;
  mat.e22 = c2.z;

  return mat;
}

/**
 * @brief Creates a matrix3 object from raw data.
 *
 * @param[in] data Pointer to a float array of size 9.
 *
 * @return
 * - matrix3 Result matrix3 created from input array.
 *
 */
static inline matrix3 make_matrix3_from_array(const float *data)
{
  matrix3 m;
  for(int i = 0; i < 9; i++)
  {
    m.data[i] = data[i];
  }
  return m;
}

/**
 * @brief Muliply a 3x3 matrix transposed to a 3x1 vector.
 *
 * @param[in] m  3x3 Matrix.
 * @param[in] v  3x1 vector.
 *
 * @return
 * - vector3 Result of Muliplication of a 3x3 matrix transposed to a 3x1
 * vector.
 *
 */
static inline vector3 matrix3_transposed_mul_vector3(matrix3 m, vector3 v)
{
  vector3 mv;

  for(int i = 0; i < 3; i++)
  {
    mv.data[i] = m.data[0 * 3 + i] * v.data[0] + m.data[1 * 3 + i] * v.data[1] +
                 m.data[2 * 3 + i] * v.data[2];
  }
  return mv;
}

/**
 * @brief Convert a 3x3 rotation matrix to a quaternion.
 *
 * @param[in] m Rotation matrix.
 *
 * @return
 * - quaternion Converted 3x3 rotational matrix to a quaternion.
 *
 */
static inline quaternion matrix3_to_quaternion(matrix3 m)
{
  quaternion q;
  float denom;
  float diag_sum = 1.0 + m.e00 + m.e11 + m.e22;
  if(diag_sum > 0.01)
  {
    denom = 2 * sqrtf(diag_sum);
    q.w = denom / 4;
    q.x = (m.e21 - m.e12) / denom;
    q.y = (m.e02 - m.e20) / denom;
    q.z = (m.e10 - m.e01) / denom;
  }
  else if(m.e00 > m.e11 && m.e00 > m.e22)
  {
    denom = 2 * sqrtf(1 + m.e00 - m.e11 - m.e22);
    q.w = (m.e21 - m.e12) / denom;
    q.x = denom / 4;
    q.y = (m.e10 + m.e01) / denom;
    q.z = (m.e02 + m.e20) / denom;
  }
  else if(m.e11 > m.e22)
  {
    denom = 2 * sqrtf(1 + m.e11 - m.e00 - m.e22);
    q.w = (m.e02 - m.e20) / denom;
    q.x = (m.e10 + m.e01) / denom;
    q.y = denom / 4;
    q.z = (m.e21 + m.e12) / denom;
  }
  else
  {
    denom = 2 * sqrtf(1.0 + m.e22 - m.e00 - m.e11);
    q.w = (m.e10 - m.e01) / denom;
    q.x = (m.e02 + m.e20) / denom;
    q.y = (m.e21 + m.e12) / denom;
    q.z = denom / 4;
  }
  /* any round trip conversion must (will) yield original rotation matrix,
   for example fmv_get_rot_m(rotation_matrix_to_quaternion(R)) must equal R. */
  q.x = -q.x;
  q.y = -q.y;
  q.z = -q.z;

  /* make sure QW is always non-negative. That will guarantee that
   our 4 element rotation vector is always corretly interpreted by Android
   apps which have to use 3 element rotation vector thus have to assume
   non-negative QW. */
  if(q.w < 0)
  {
    q.x = -q.x;
    q.y = -q.y;
    q.z = -q.z;
    q.w = -q.w;
  }
  return q;
}

/**
 * @brief Converts a quaternion to 3x3 rotation matrix.
 *
 * @param[in] q Quaternion.
 *
 * @return
 * - matrix3 Rotation matrix.
 *
 */

static inline matrix3 quaternion_to_matrix3(quaternion q)
{
  matrix3 m;
  m.e00 = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
  m.e01 = 2 * (q.x * q.y + q.w * q.z);
  m.e02 = 2 * (q.x * q.z - q.w * q.y);
  m.e10 = 2 * (q.x * q.y - q.w * q.z);
  m.e11 = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
  m.e12 = 2 * (q.y * q.z + q.w * q.x);
  m.e20 = 2 * (q.x * q.z + q.w * q.y);
  m.e21 = 2 * (q.y * q.z - q.w * q.x);
  m.e22 = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
  return m;
}

/*=========================================================================
  FUNCTION:  vector_cross
  =======================================================================*/
/**
 * @brief Compute the cross product of 2 vectors.*
 *
 * @param[in]  v1 vector1.
 * @param[in]  v2 vector2.
 * @param[out] v3 vector3.
 *
 * @return
 * - None.
 *
 */

static inline void vector_cross_d(const double *restrict v1,
                                  const double *restrict v2,
                                  double *restrict v3)
{
  // cross(A,B) = [Ay*Bz-Az*By, Az*Bx-Ax*Bz, Ax*By-Ay*Bx]
  v3[0] = v1[1] * v2[2] - v1[2] * v2[1];
  v3[1] = v1[2] * v2[0] - v1[0] * v2[2];
  v3[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

/*=========================================================================
  FUNCTION:  vector_norm
  =======================================================================*/
/**
 * @brief Compute the norm of a vector.
 *
 * @param[in] v Vector.
 *
 * @return
 * - double Norm of the vector.
 *
 */
static inline double vector_norm_d(double *v)
{
  return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/*=========================================================================
  FUNCTION:  convert_rotation_vector
  =======================================================================*/
/**
 * @brief Convert rotation vector (quaternion) from NED(SAE) to ENU(Android),
 *  or vice versa.
 *  - Swap q->x and q->y.
 *  - Reverse q->z sign.
 *  - Copy q->w as is.
 *
 * @param[in]  q1 Source vector.
 * @param[out] q2 Destination vector.
 *
 * @return
 * - None.
 *
 */
/*=======================================================================*/
static inline void convert_rotation_vector_sae_enu_d(const quaternion_d *q1,
                                                     quaternion_d *q2)
{
  q2->x = q1->y;
  q2->y = q1->x;
  q2->z = -q1->z;
  q2->w = q1->w;
}

/*=========================================================================
  FUNCTION:  rotation_matrix_to_quaternion
  =======================================================================*/
/**
 * @brief Convert 3x3 six axis rotation matrix to quaternion.
 *
 * @param[in] m Rotation matrix.
 * @param[out] q Quaternion.
 *
 * @return
 * - None.
 *
 */
/*=======================================================================*/
static inline void
rotation_matrix_to_quaternion_d(double m[][TRI_AXIAL_SENSOR_AXES],
                                quaternion_d *q)
{
  double diag_sum;
  double denom;

  /* @details
    Matlab script from FDD v1.7
      diag_sum = 1.0 + Matrix(1,1) + Matrix(2,2) + Matrix(3,3);
    if diag_sum > 0.01
        denom = 2.0 * sqrt(diag_sum);
        Quaternion_WXYZ = [...
                denom / 4.0,...
                (Matrix(3,2) - Matrix(2,3)) / denom,...
                (Matrix(1,3) - Matrix(3,1)) / denom,...
                (Matrix(2,1) - Matrix(1,2)) / denom];
        debug_out_case = 1;
    elseif Matrix(1,1) > Matrix(2,2) && Matrix(1,1) > Matrix(3,3)
        denom = 2.0 * sqrt(1.0 + Matrix(1,1) - Matrix(2,2) - Matrix(3,3));
        Quaternion_WXYZ = [...
                (Matrix(3,2) - Matrix(2,3)) / denom,...
                denom / 4.0,...
                (Matrix(2,1) + Matrix(1,2)) / denom,...
                (Matrix(1,3) + Matrix(3,1)) / denom];
        debug_out_case = 2;
    elseif Matrix(2,2) > Matrix(3,3)
        denom = 2.0 * sqrt(1 + Matrix(2,2) - Matrix(1,1) - Matrix(3,3));
        Quaternion_WXYZ = [...
                (Matrix(1,3) - Matrix(3,1)) / denom,...
                (Matrix(2,1) + Matrix(1,2)) / denom,...
                denom / 4.0,...
                (Matrix(3,2) + Matrix(2,3)) / denom];
        debug_out_case = 3;
    else
         denom = 2.0 * sqrt(1 + Matrix(3,3) - Matrix(1,1) - Matrix(2,2));
         Quaternion_WXYZ = [...
                (Matrix(2,1) - Matrix(1,2)) / denom,...
                (Matrix(1,3) + Matrix(3,1))  / denom,...
                (Matrix(3,2) + Matrix(2,3)) / denom,...
                denom / 4.0];
         debug_out_case = 4;
      end
  */

  diag_sum = (double)1.0 + m[0][0] + m[1][1] + m[2][2];
  if(diag_sum > 0.01)
  {
    denom = 2 * (double)sqrt(diag_sum);
    q->w = denom / 4;
    q->x = (m[2][1] - m[1][2]) / denom;
    q->y = (m[0][2] - m[2][0]) / denom;
    q->z = (m[1][0] - m[0][1]) / denom;
  }
  else if(m[0][0] > m[1][1] && m[0][0] > m[2][2])
  {
    denom = 2 * (double)sqrt(1 + m[0][0] - m[1][1] - m[2][2]);
    q->w = (m[2][1] - m[1][2]) / denom;
    q->x = (double)(denom / 4.0);
    q->y = (m[1][0] + m[0][1]) / denom;
    q->z = (m[0][2] + m[2][0]) / denom;
  }
  else if(m[1][1] > m[2][2])
  {
    denom = 2 * (double)sqrt(1 + m[1][1] - m[0][0] - m[2][2]);
    q->w = (m[0][2] - m[2][0]) / denom;
    q->x = (m[1][0] + m[0][1]) / denom;
    q->y = denom / 4;
    q->z = (m[2][1] + m[1][2]) / denom;
  }
  else
  {
    denom = 2 * (double)sqrt(1.0 + m[2][2] - m[0][0] - m[1][1]);
    q->w = (m[1][0] - m[0][1]) / denom;
    q->x = (m[0][2] + m[2][0]) / denom;
    q->y = (m[2][1] + m[1][2]) / denom;
    q->z = denom / 4;
  }

  // Now any round trip conversion must (will) yield original rotation matrix,
  // for example fmv_get_rot_m(rotation_matrix_to_quaternion(R)) must equal R.
  q->x = -q->x;
  q->y = -q->y;
  q->z = -q->z;

  // make sure QW is always non-negative. That will guarantee that
  // our 4 element rotation vector is always corretly interpreted by Android
  // apps which have to use 3 element rotation vector thus have to assume
  // non-negative QW.
  if(q->w < 0)
  {
    q->x = -q->x;
    q->y = -q->y;
    q->z = -q->z;
    q->w = -q->w;
  }
}
