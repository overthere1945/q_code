/*===============================================================================================
 * FILE:        wstrlen.c
 *
 * DESCRIPTION: Implementation of API wstrlen.
 *
 *              Copyright (c) 2011 Qualcomm Technologies Incorporated.
 *              All Rights Reserved. QUALCOMM Proprietary and Confidential.
 *===============================================================================================*/
 
/*===============================================================================================
 *
 *                            Edit History
 * $Header: //components/dev/core.qdsp6/8.0/nthompso.core.qdsp6.8.0.singlesrccfg/kernel/libstd/src/wstrlen.c#1 $ 
 * $DateTime: 2022/09/20 15:05:15 $
 *
 *===============================================================================================*/
#include <stringl/stringl.h>

size_t wstrlen(const wchar *s)
{
   size_t i;
   for (i=0; *s; s++, i++);
   return i;
}
