#ifndef SNS_CIRCULAR_BUFFER_H
#define SNS_CIRCULAR_BUFFER_H
/** ===========================================================================
 * @file
 *
 * @brief Circular buffer utility header file.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/
#include <stdint.h>
#include "sns_matrix.h"

/**
 * @brief Define macro to calculate index into circular buffer.This macro
 * calculates the index into the circular buffer based on the current index
 * and the size of the buffer.
 *
 */
#define BUF_INDEX(index, buf_size) ((index) & (buf_size - 1))

/**
 *  @brief Circular buffer data structure.
 *
 */
typedef struct
{

  matrix_type data;    /*!< Matrix data for storing the values. */
  int32_t end;         /*!< End index of the circular buffer. */
  int32_t window_size; /*!< The number of samples to store in the buffer. */
  int32_t count;       /*!< The number of samples currently stored in the
                        *   buffer.
                        */
} buffer_type;

/**
 *  @brief Circular buffer data structure for 64-bit data.
 *
 */
typedef struct
{
  matrix_type_64 data; /*!< Matrix data uint64 for storing the values. */
  int32_t end;         /*!< End index of the circular buffer. */
  int32_t window_size; /*!< The number of samples to store in the buffer. */
  int32_t count;       /*!< The number of samples currently stored in
                        *   the buffer.
                        */
} buffer_type_64;

/*=============================================================================
  Public Function Declaration
  ===========================================================================*/
/**
 *  @brief Size of memory for the buffer. The buffer may contain more data than
 *  just the window size and columns.
 *
 *  @note Not that it does, but the user doesn't need to know that
 *
 *  @param[in] window_size      The number of samples to store in the buffer.
 *  @param[in] cols             The number of columns in each sample.
 *
 *  @return
 *  - int32_t:  Size in bytes required to allocate memory.
 *
 */
int32_t buffer_mem_req(int32_t window_size, int32_t cols);

/**
 *  @brief Initialize circular buffer structure.
 *
 *  @param[inout] buffer     Pointer to the circular buffer structure.
 *  @param[in] window_size   The number of samples to store in the buffer.
 *  @param[in] cols          The number of columns in each sample.
 *  @param[in] mem           Memory address where the circular buffer resides.
 *
 *  @return
 *    - None.
 *
 */
void buffer_reset(buffer_type *buffer, int32_t window_size, int32_t cols,
                  void *mem);

/**
 *  @brief Inserts the data at the end of index in circular buffer.
 *
 *  @param[inout] buffer     Pointer to the circular buffer structure.
 *  @param[in] new_data      Data to insert into the circular buffer.
 *
 *  @return
 *    - None.
 *
 */
void buffer_insert(buffer_type *buffer, int32_t const *new_data);

/**
 *  @brief Calculates size of memory for window size and columns
 *
 *  @param[in] window_size      The number of samples to store in the buffer.
 *  @param[in] cols             The number of columns in each sample.
 *
 *  @return 
 *    - int32_t:  Size in bytes required to allocate memory.
 *
 */
int32_t buffer_mem_req_64(int32_t window_size, int32_t cols);

/**
 *  @brief Initialize circular buffer structure.
 *
 *  @param[inout] buffer     Pointer to the circular buffer structure.
 *  @param[in] window_size   The number of samples to store in the buffer.
 *  @param[in] cols          The number of columns in each sample.
 *  @param[in] mem           Memory address where the circular buffer resides.
 *
 *  @return
 *     - None.
 *
 */
void buffer_reset_64(buffer_type_64 *buffer, int32_t window_size, int32_t cols,
                     void *mem);

/**
 *  @brief Inserts the data at the end of index in circular buffer.
 *
 *  @param[inout] buffer     Pointer to the circular buffer structure.
 *  @param[in] new_data      Data to insert into the circular buffer.
 *
 *  @return
 *     - None.
 *
 */
void buffer_insert_64(buffer_type_64 *buffer, uint64_t const *new_data);

/**
 *  @brief Calculate the maximum peak to minimum peak spread of the buffer
 *
 *  @param[in] buffer      Input buffer.
 *  @param[out] sprd_data  Pointer to the spread value.
 *
 *  @return
 *     - None.
 *
 */
void buffer_sprd(buffer_type *buffer, int32_t *sprd_data);

/**
 * @brief Calculate sum of the buffer provided.
 *
 * @param[in] buffer    Input buffer
 * @param[out] sum_data Pointer to output variable containing summation.
 *
 * @return
 *    - None.
 *
 */
void buffer_sum(buffer_type *buffer, int32_t *sum_data);

/**
 * @brief Calculate sum of the buffer provided.
 *
 * @param[in] buffer       Input buffer.
 * @param[out] sum_sq_data Sum of square of input buffer.
 * @param[in] q_format     FX_QFACTOR or SPI_QFACTOR.
 *
 * @return
 *    - None.
 *
 */
void buffer_sum_sq(buffer_type *buffer, int32_t *sum_sq_data, int32_t q_format);

/**
 * @brief Check if the buffer is full.
 *
 * @param[in] buffer      Input buffer.
 *
 * @return
 *  - True:   If buffer is full.
 *  - False:  Otherwise.
 *
 */
int32_t buffer_full(buffer_type *buffer);

/**
 *  @brief Function to get number of samples in the buffer.
 *
 *  @param[in] buffer      Input buffer.
 *
 *  @return
 *    - uint8_t: Number of samples in the buffer.
 *
 */
uint8_t buffer_num_samples(buffer_type *buffer);

/**
 * @brief Calculate sum and sum of square root of the buffer provided.
 *
 * @param[in] buffer       Input buffer.
 * @param[out] sum_sq_data Sum of square of input buffer.
 * @param[out] sum_data    Sum of input buffer.
 * @param[in] q_format     FX_QFACTOR or SPI_QFACTOR..
 *
 * @return
 *    - None.
 *
 */
void buffer_sum_and_sum_sq(buffer_type *buffer, int32_t *sum_data,
                           int32_t *sum_sq_data, int32_t q_format);

/**
 * @brief Round up to the nearest power of 2.
 *
 * @param[in] window_size  Value to round up.
 *
 * @return
 *  - int32_t:  Rounded up value.
 *
 * @note we need at least one empty spot in the buffer.
 *
 */

int32_t buffer_size(int32_t window_size);

/**
 * @brief Get number of data row in the buffer from a given starting index.
 *
 * @param[in] buffer       Input buffer.
 * @param[in] start_index  Start Index.
 * @param[in] end_index    End Index
 *
 *  @return
 *    -int32_t: Number of rows in the buffer.
 *
 */
int32_t buffer_partial_compute_num_data_rows(buffer_type const *buffer,
                                             int32_t start_index,
                                             int32_t end_index);

/**
 * @brief Get partial sum of the buffer from start index to end index
 *
 * @param[in] buffer       Input buffer.
 * @param[in] start_index  Start Index.
 * @param[in] end_index    End Index.
 * @param[out] sum_data    Output pointer to hold sum of the buffer.
 *
 *  @return
 *     - None.
 *
 */
void buffer_partial_sum(buffer_type const *buffer, int32_t start_index,
                        int32_t end_index, int32_t *sum_data);

/**
 * @brief Get partial sum of square of the buffer from start index to end index
 *
 * @param[in] buffer        Input buffer.
 * @param[in] start_index   Start Index.
 * @param[in] end_index     End Index.
 * @param[out] sum_sq_data  Output pointer to hold sum of square of the buffer.
 *
 *  @return
 *     - None.
 *
 */
void buffer_partial_sum_sq(buffer_type const *buffer, int32_t start_index,
                           int32_t end_index, int32_t *sum_sq_data);

/**
 * @brief Get partial max of the buffer from start index to end index.
 *
 * @param[in] buffer        Input buffer.
 * @param[in] start_index   Start index.
 * @param[in] stop_index    Stop index.
 * @param[in] col           Column index.
 * @param[out] max_data     Output pointer to hold max of the buffer.
 * @param[out] min_data     Output pointer to hold min of the buffer.
 * @param[out] sprd_data    Output pointer to hold spread of the buffer.
 *
 *  @return
 *     - None.
 *
 */

void buffer_partial_min_max_sprd_one_column(
    buffer_type const *buffer, int32_t start_index, int32_t stop_index,
    int32_t col, int32_t *min_data, int32_t *max_data, int32_t *sprd_data);

#endif /* SNS_CIRCULAR_BUFFER_H */
