/*===============================================================================================
 * FILE:        strcasecmp.c
 *
 * DESCRIPTION: Implementation of standard string API strcasecmp.
 *
 *              Copyright (c) 1999 - 2010 Qualcomm Technologies Incorporated.
 *              All Rights Reserved. QUALCOMM Proprietary and Confidential.
 *===============================================================================================*/
 
/*===============================================================================================
 *
 *                            Edit History
 * $Header: //components/dev/core.qdsp6/8.0/nthompso.core.qdsp6.8.0.singlesrccfg/kernel/libstd/src/strcasecmp.c#1 $ 
 * $DateTime: 2022/09/20 15:05:15 $
 *
 *===============================================================================================*/
#include <limits.h> 
#include <stringl/stringl.h>

int strcasecmp(const char * s1, const char * s2)
{
    return strncasecmp(s1, s2, UINT_MAX);
}
