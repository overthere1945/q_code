#pragma once
/** ============================================================================
 * @file
 *
 * @brief Interface supporting circular buffers of floating-point sensor
 * samples.
 *
 * @example
 * typedef struct
 * {
 *   SNS_CBUFFER(5, 3, float) buffer;
 * } example;
 *
 * float sample[] = { 0.0f, 0.0f, 9.7f};
 * example eg;
 * float *output;
 * SNS_CBUFFER_INIT(eg.buffer);
 * SNS_CBUFFER_PUSH(eg.buffer, sample_a);
 * output = SNS_CBUFFER_GET_TAIL(eg.buffer, 0);
 * output = SNS_CBUFFER_NEXT(eg.buffer, output);
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "sns_types.h"    /*!< For ARR_SIZE. */
#include "sns_mem_util.h" /*!< For sns_memset, sns_memzero. */

/**
 * @brief Circular buffer to be used as a named variable or structure field.
 * The follow line creates a circular buffer which supports five entries with
 * three axis, whose name is 'buffer'.
 * SNS_CBUFFER(5, 3) buffer;
 *
 * @note 'head' refers to the location where the next entry will be pushed.
 * Hence it also refers to the oldest entry present in the circular buffer.
 *
 * @param[in] length    Length of the circular buffer.
 * @param[in] num_axis  Number of axis per entry in the circular buffer.
 * @param[in] data_type Selected datatype for the items in CBUFFER.
 *
 */
#define SNS_CBUFFER(length, num_axis, data_type)                               \
  struct                                                                       \
  {                                                                            \
    int32_t head;                                                              \
    struct                                                                     \
    {                                                                          \
      data_type data[(num_axis)];                                              \
    } items[(length)];                                                         \
  }

/**
 * @brief Initialize a circular buffer.
 *
 * @param[in] cbuffer Variable declared using SNS_CBUFFER.
 *
 */
#define SNS_CBUFFER_INIT(cbuffer)                                              \
  do                                                                           \
  {                                                                            \
    sns_memzero(&(cbuffer), sizeof(cbuffer));                                  \
    (cbuffer).head = 0;                                                        \
  } while(false)

/**
 * @brief Returns the float array associated with the previous entry
 * (in order of insertion).  E.g. If item refers to the 4th entry pushed,
 * this would return the 3rd entry.
 *
 * @param[in] cbuffer Variable declared using SNS_CBUFFER.
 * @param[in] item    Variable refering to a item in the buffer.
 *
 */
#define SNS_CBUFFER_PREV(cbuffer, item)                                        \
  (uintptr_t)(item) >=                                                         \
          ((uintptr_t)(cbuffer).items + sizeof((cbuffer).items[0].data))       \
      ? (void *)((uintptr_t)(item) - sizeof((cbuffer).items[0].data))          \
      : (cbuffer).items[SNS_CBUFFER_LENGTH(cbuffer) - 1].data

/**
 * @brief Returns the float array associated with the next entry
 * (in order of insertion).  E.g. If item refers to the oldest entry pushed,
 * this will return the second oldest.
 *
 * @param[in] cbuffer Variable declared using SNS_CBUFFER.
 * @param[in] item    Variable refering to a item in the buffer.
 *
 */
#define SNS_CBUFFER_NEXT(cbuffer, item)                                        \
  ((uintptr_t)(item) + sizeof(cbuffer.items[0].data) >=                        \
           ((uintptr_t)(cbuffer).items + sizeof((cbuffer).items))              \
       ? (cbuffer).items[0].data                                               \
       : (void *)((uintptr_t)(item) + sizeof((cbuffer).items[0].data)))

/**
 * @brief Return the lenth of the buffer (as specified in SNS_CBUFFER).
 * @param[in] cbuffer Variable declared using SNS_CBUFFER.
 *
 */
#define SNS_CBUFFER_LENGTH(cbuffer) (ARR_SIZE((cbuffer).items))

/**
 * @brief Return the axis count (as specified in SNS_CBUFFER).
 *
 * @param[in] cbuffer Variable declared using SNS_CBUFFER.
 *
 */
#define SNS_CBUFFER_AXIS_COUNT(cbuffer) (ARR_SIZE((cbuffer).items[0].data))

/**
 * @brief Push a new entry to the head of the circular buffer.
 *
 * @param[in] cbuffer     Variable declared using SNS_CBUFFER.
 * @param[in] entry       Variable with item being added to the buffer.
 * @param[in] entry_size  Variable containing the size of the entry.
 *
 */
#define SNS_CBUFFER_PUSH(cbuffer, entry, entry_size)                           \
  do                                                                           \
  {                                                                            \
    sns_memscpy((cbuffer).items[(cbuffer).head].data,                          \
                sizeof((cbuffer).items[(cbuffer).head].data), entry,           \
                (entry_size));                                                 \
    if(++(cbuffer).head >= SNS_CBUFFER_LENGTH(cbuffer))                        \
      (cbuffer).head = 0;                                                      \
  } while(false)

/**
 * @brief Get an entry from the circular buffer at index.
 * Index is a positive offset from the buffer head and less then the cbuffer
 * length. E.g. index '0' is the oldest entry; index '1' is the second oldest.
 *
 * @param[in] cbuffer     Variable declared using SNS_CBUFFER.
 * @param[in] index       Variable with a positive offset from head.
 *
 */
#define SNS_CBUFFER_GET_TAIL(cbuffer, index)                                   \
  ((cbuffer)                                                                   \
       .items[((index) + (cbuffer).head) < SNS_CBUFFER_LENGTH(cbuffer)         \
                  ? ((index) + (cbuffer).head)                                 \
                  : ((index) + (cbuffer).head - SNS_CBUFFER_LENGTH(cbuffer))]  \
       .data)

/**
 * @brief Get an entry from the circular buffer at index.
 * Index is a positive offset from the buffer tail and less then the cbuffer
 * length. E.g. index '0' is the latest entry; index '1' is the second most
 * recent.
 *
 * @param[in] cbuffer     Variable declared using SNS_CBUFFER.
 * @param[in] index       Variable with a positive offset from tail.
 *
 */
#define SNS_CBUFFER_GET_HEAD(cbuffer, index)                                   \
  ((cbuffer)                                                                   \
       .items[((cbuffer).head - 1 - (index)) >= 0                              \
                  ? ((cbuffer).head - 1 - (index))                             \
                  : ((cbuffer).head - 1 - (index) +                            \
                     SNS_CBUFFER_LENGTH(cbuffer))]                             \
       .data)
