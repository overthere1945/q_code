/*===============================================================================================
 * FILE:        secure_memset.c
 *
 * DESCRIPTION: Implementation of standard string API secure_memset.
 *
 *              Copyright (c) 2013,2018 Qualcomm Technologies, Inc.
 *              All Rights Reserved. Qualcomm Confidential and Proprietary.
 *===============================================================================================*/
 
/*===============================================================================================
 *
 *                            Edit History
 * $Header: //components/dev/core.qdsp6/8.0/nthompso.core.qdsp6.8.0.singlesrccfg/kernel/libstd/src/secure_memset.c#1 $ 
 * $DateTime: 2022/09/20 15:05:15 $
 *
 *===============================================================================================*/

/* Example implementation of a secure_memset() function
   This file MUST always be separate from all other files
*/

#include <stdint.h>
#define __STDC_WANT_LIB_EXT1__ 1
#include <stringl/stringl.h>

/* This function MUST NEVER be included in a file with any other functions    */
/* Security relies on the fact that the linker cannot optimize function calls */
/* between separate files and so cannot strip this functionality from the     */
/* execution flow.                                                            */

void* secure_memset(void* ptr, int value, size_t len)
{
    (void)memset(ptr, value, len);
    return ptr;
}
