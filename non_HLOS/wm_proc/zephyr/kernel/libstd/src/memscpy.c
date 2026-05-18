/*===============================================================================================
 * FILE:        libstd_std_scanul.c
 *
 * DESCRIPTION: Implementation of a secure API memscpy - Size bounded memory copy. 
 *
 *              Copyright (c) 2012, 2013,2018 Qualcomm Technologies, Inc.
 *              All Rights Reserved. QUALCOMM Proprietary and Confidential.
 *===============================================================================================*/
 
/*===============================================================================================
 *
 *                            Edit History
 * $Header: //components/dev/core.qdsp6/8.0/nthompso.core.qdsp6.8.0.singlesrccfg/kernel/libstd/src/memscpy.c#1 $ 
 * $DateTime: 2022/09/20 15:05:15 $
 *
 *===============================================================================================*/

#include <stringl/stringl.h>

size_t  memscpy(
          void        *dst,
          size_t      dst_size,
          const void  *src,
          size_t      src_size
          )
{
  size_t  copy_size = (dst_size <= src_size)? dst_size : src_size;

  memcpy(dst, src, copy_size);

  return copy_size;
}
