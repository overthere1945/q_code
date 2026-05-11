#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Extend a C structure.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/
#include <iso646.h>
#include <stdlib.h>
#include <stdint.h>
#include "sns_rc.h"

/**
 * @brief This utility provides a mechanism for appending multiple
 * buffers to the end of a C structure.
 *
 * @code
 * For example consider a two C structures:
 * struct A
 * {
 *   int32_t x;
 *   int32_t y;
 *   sns_cstruct_extn extn;
 * }
 *
 * struct B
 * {
 *   char buffer[];
 * }
 *
 * Multiple instances of struct B can be appended to the end of
 * struct A. Here is an example to append 2 instances of B:
 *
 * size_t alloc_size = sns_cstruct_extn_compute_total_size(
 *    sizeof(struct A), 2, 2 * ALIGN_8(buffer1_size) +
 *    ALIGN_8(buffer2_size));
 *
 * struct A *example = malloc(alloc_size);
 * sns_cstruct_extn_init(&example->extn, example,
 *   sizeof(*example), 2);
 *
 * uint32_t b1_idx =
 *   sns_cstruct_extn_setup_buffer(&example->extn,
 *   buffer1_size);
 * uint32_t b2_idx =
 *   sns_cstruct_extn_setup_buffer(&example->extn,
 *   buffer2_size);
 *
 * void *b1 =  sns_cstruct_extn_get_buffer(&example->extn,
 *   b1_idx);
 * void *b2 =  sns_cstruct_extn_get_buffer(&example->extn,
 *   b2_idx);
 * @endcode
 */

/**
 * @brief  The C structure extender.
 *
 */
typedef struct
{
  void *original_struct; /*!< Pointer to the original structure */
  size_t original_size;  /*!< Size of the original structure */

  size_t total_size; /*!< Total size of all the buffers, metadata
                      *   and the original structure
                      */
  uint8_t cnt;       /*!< Number of buffers appended to the end */
} sns_cstruct_extn;

/**
 * @brief Compute the total size of the C structure and all the buffers.
 *
 * @param[in] original_size    Size of the original C structure.
 * @param[in] cnt              Number of buffers being appended.
 * @param[in] sum_buffer_sizes The sum of the buffer sizes for all the buffers
 *                             and the extra padding bytes added to each buffer
 *                             for alignment.
 *
 * @return
 *    - size_t:    Combined size.
 *
 */
size_t sns_cstruct_extn_compute_total_size(size_t original_size, uint8_t cnt,
                                           size_t sum_buffer_sizes);

/**
 * @brief Initialize the C structure extensions.
 *
 * @param[in] extn                 The C structure extender ptr.
 * @param[in] original_struct      Pointer to the original structure.
 * @param[in] original_struct_size Size of original struct.
 * @param[in] cnt                  Number of buffers being appended.
 *
 * @return
 *  - SNS_RC_SUCCESS: Initialization successful.
 *  - SNS_RC_FAILED:  Otherwise.
 *
 */
sns_rc sns_cstruct_extn_init(sns_cstruct_extn *extn, void *original_struct,
                             size_t original_struct_size, uint8_t cnt);

/**
 * @brief Set up each buffer.
 *
 * @param[in] extn           The C structure extender ptr.
 * @param[in] size           Size of the buffer being added.
 *
 * @return
 *  - UINT8_MAX:    If buffer not setup correctly Index of buffer to
 *                  be used with sns_cstruct_extn_get_buffer.
 *  - 0:            Otherwise.
 *
 */
uint8_t sns_cstruct_extn_setup_buffer(sns_cstruct_extn *extn, size_t size);

/**
 * @brief Get an appended buffer from its index.
 *
 * @param[in] extn    The C structure extender ptr.
 * @param[in] idx     Index of the buffer obtained from
 *                    sns_cstruct_extn_setup_buffer.
 *
 * @return
 *  - Pointer to the appended buffer if successful.
 *  - NULL: Otherwise.
 *
 */
void *sns_cstruct_extn_get_buffer(sns_cstruct_extn *extn, uint8_t idx);
