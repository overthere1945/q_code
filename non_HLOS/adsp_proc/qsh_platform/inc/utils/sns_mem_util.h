#pragma once
/** =============================================================================
 * @file sns_mem_util.h
 *
 * @brief Utility functions associated with memory and memory management.
 * All utilities in this file can be used in island mode.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *  ===========================================================================
 */

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "stdlib.h"

/*=============================================================================
  Public Function Declarations
  ===========================================================================*/

/**
 * @brief Function to set a given value into a memory buffer of given size.
 * 
 * @param[inout] ptr   Memory buffer pointer that need to be set with the given 
                       value. 
 * @param[in] value    Value that need to be set into the memory buffer.
 * @param[in] size     Size of the memory buffer in Bytes.
 *
 * @return
 *  None
 */
void *sns_memset(void *ptr, int value, size_t size);

/**
 * @brief Function to set a zero into a memory buffer of given size.
 * 
 * @param[inout] ptr  Memory buffer pointer that need to be set to zero.
 * @param[in] size    Size of the memory buffer.
 *
 * @return
 *  None
 */
void *sns_memzero(void *ptr, size_t size);

/**
 * @brief Function to compare two memory buffers of given size.
 *
 * @param[in] ptr1 Poitner of the first memory buffer.
 * @param[in] ptr2 Poitner of the second memory buffer.
 * @param[in] size Size of the region to compare, which should be less than or
 *                 equal to the size of the smallest memory buffer.
 *
 * @return int 
 * This function return an interger value, which is less than, equal
 * to or greater than zero. if the first n bytes of ptr1 are found,
 * respectively, to be less than, to match, or be greater than the first n bytes
 * of ptr2.
 *
 */
int sns_memcmp(const void *ptr1, const void *ptr2, size_t size);

/**
 * @brief Fucntoin to copy data from one memory location to another.
 *
 * @param[out]  dest        Destination memory pointer, where data will be
 *                          copied to.
 * @param[in]   dest_size   Destination memory size.
 * @param[in]   source      Source memory pointer, from where data will be
 *                          copied.
 * @param[in]   source_size Source memory size.
 *
 * @return size_t Size of the data get copied.
 */
size_t sns_memscpy(void *dest, size_t dest_size, const void *source,
                   size_t source_size);

/**
 * @brief Function to copy data from overlapping memory locations.
 *
 * @param[out]  dst        Destination memory pointer, where data will be
 *                         copied to.
 * @param[in]   dst_size   Destination memory size.
 * @param[in]   src        Source memory pointer, from where data will be
 *                         copied.
 * @param[in]   src_size   Source memory size.
 *
 * @return size_t Size of the data get copied.
 */
size_t sns_memsmove(void *dst, size_t dst_size, const void *src,
                    size_t src_size);

/**
 * @brief Function intended for saver, consistant and less error prone usage of
 * strcpy() and strncpy(). This function copy a string from source to
 * destination memory buffer, ensuring that destination is null terminated. and
 * not overrun.
 *
 * @param[out]  dst   Destination memory pointer.
 * @param[in]   src   Source memory pointer.
 * @param[in]   size  Size of the memory buffer.
 *
 * @return size_t Size of the data get copied.
 */
size_t sns_strlcpy(char *dst, const char *src, size_t size);

/**
 * @brief Function intended for saver, consistant and less error prone usage of
 * strcat() and strncat(). This function concatenate a sourcestring to the end
 * of destination string, ensuring that destination is null terminated. and not
 * overrun.
 *
 * @param[in,out] dst   Resultant string pointer after cancatenation.
 * @param[in] src       String that will be appended at the end of destination
 *                      string.
 * @param[in] size      Size of the resultant string.
 *
 * @return size_t Size of the resultant string.
 */
size_t sns_strlcat(char *dst, const char *src, size_t size);
