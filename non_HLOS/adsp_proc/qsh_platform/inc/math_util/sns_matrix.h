#ifndef SNS_MATRIX_H
#define SNS_MATRIX_H
/**============================================================================
 * @file
 *
 * @brief Matrix utility header file.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *==========================================================================*/

#pragma once
#include <stdint.h>
/**
 * @brief Get an element from the matrix.
 *
 * @param[in] circular_buffer Circular buffer containing the matrix.
 * @param[in] row             Row index of the desired element.
 * @param[in] col             Column index of the desired element.
 *
 * @return Element at the specified location.
 *
 */
#define MATRIX_ELEM(circular_buffer, row, col)                                 \
  (*((circular_buffer)->matrix + row * (circular_buffer)->cols + col))

/**
 * @brief Get a row from the matrix.
 *
 * @param[in] circular_buffer Circular buffer containing the matrix.
 * @param[in] row             Row index of the desired element.
 *
 * @return Pointer to the start of the requested row.
 *
 */
#define MATRIX_ROW(circular_buffer, row)                                       \
  ((circular_buffer)->matrix + row * (circular_buffer)->cols)

/**
 * @brief Matrix structure.
 *
 */
typedef struct
{
  int32_t *matrix; /*!< Data of the matrix.*/
  int32_t rows;    /*!< Number of rows in the matrix.*/
  int32_t cols;    /*!< Number of column in the matrix.*/
} matrix_type;

/**
 * @brief Matrix structure - 64 bit
 *
 */
typedef struct
{
  uint64_t *matrix; /*!< Data of the matrix.*/
  int32_t rows;    /*!< Number of rows in the matrix.*/
  int32_t cols;    /*!< Number of column in the matrix.*/
} matrix_type_64;

/**
 * @brief Calculates size of memory for a rows x cols matrix.
 *
 * @param[in] rows  Rows of the matrix.
 * @param[in] cols  Columns of the matrix.
 *
 * @return
 *   - int32_t:      Size (in bytes) of a rows x cols matrix.
 *
 */
int32_t matrix_mem_req(int32_t rows, int32_t cols);

/**
 * @brief Initializes a matrix.
 *
 * @param[in] m     The matrix.
 * @param[in] rows  Rows of the matrix.
 * @param[in] cols  Columns of the matrix.
 * @param[in] mem   Pointer to memory for matrix.
 *
 * @return
 *    -None.
 *
 */
void matrix_reset(matrix_type *m, int32_t rows, int32_t cols, void *mem);

/**
 * @brief Sets a matrix's values to zero.
 *
 * @param[in] m     The matrix.
 *
 * @return
 *    - None.
 *
 */
void matrix_zero(matrix_type *m);

/**
 * @brief Calculates size of memory for a rows x cols matrix.
 *
 * @param[in] rows  Rows of the matrix.
 * @param[in] cols  Columns of the matrix.
 *
 * @return
 *   -int32_t:   Size (in bytes) of a rows x cols matrix.
 *
 */
int32_t matrix_mem_req_64(int32_t rows, int32_t cols);

/**
 * @brief Initializes a matrix.
 *
 * @param[in] m     The matrix.
 * @param[in] rows  Rows of the matrix.
 * @param[in] cols  Columns of the matrix.
 * @param[in] mem   Pointer to memory for matrix.
 *
 * @return
 *    - None.
 *
 */
void matrix_reset_64(matrix_type_64 *m, int32_t rows, int32_t cols, void *mem);

/**
 * @brief Sets a matrix's values to zero.
 *
 * @param[in] m     The matrix.
 *
 * @return
 *    - None.
 *
 */
void matrix_zero_64(matrix_type_64 *m);

/**
 * @brief Multiplies two matrices together.
 *
 * @param[inout] result  The resultant, A * B.
 * @param[in] A          First matrix.
 * @param[in] B          Second matrix.
 *
 * @return
 *    - None.
 *
 * @note
 *   "result" must be allocated before calling this function.
 */
void matrix_multiply(matrix_type *result, matrix_type *A, matrix_type *B);

/**
 * @brief Copies a matrix.
 *
 * @param[inout] to    The destination.
 * @param[in] from     The source.
 *
 * @return
 *    - None.
 *
 * @note
 *   "to" must be allocated before calling..
 */
void matrix_copy(matrix_type *to, matrix_type *from);

/**
 * @brief Calculates the norm of a vector.
 *
 * @param[inout] m Matrix (vector) to normalize.
 *
 * @return
 *   -int32_t:  The sqrt of the sum of squares in the vector.
 *
 */
int32_t vector_norm(matrix_type *m);

/**
 * @brief Normalizes a vector.
 *
 * @param[inout] m Matrix (vector) to normalize.
 *
 * @return
 *    - None.
 */
void vector_normalize(matrix_type *m);

#endif /* SNS_MATRIX_H */
