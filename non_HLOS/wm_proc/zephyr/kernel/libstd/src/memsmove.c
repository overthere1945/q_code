/*===============================================================================================
 * FILE:        libstd_std_scanul.c
 *
 * DESCRIPTION: Implementation of a secure API memsmove - Size bounded memory move.
 *
 *              Copyright (c) 2012, 2013,2018 Qualcomm Technologies, Inc.
 *              All Rights Reserved. QUALCOMM Proprietary and Confidential.
 *===============================================================================================*/
 
/*===============================================================================================
 *
 *                            Edit History
 * $Header: //components/dev/core.qdsp6/8.0/nthompso.core.qdsp6.8.0.singlesrccfg/kernel/libstd/src/memsmove.c#1 $ 
 * $DateTime: 2022/09/20 15:05:15 $
 *
 *===============================================================================================*/
 
#include <stringl/stringl.h>

size_t memsmove(
          void        *dst,
          size_t      dst_size,
          const void  *src,
          size_t      src_size
          )
{
  size_t  copy_size = (dst_size <= src_size)? dst_size : src_size;

  memmove(dst, src, copy_size);

  return copy_size;
}

