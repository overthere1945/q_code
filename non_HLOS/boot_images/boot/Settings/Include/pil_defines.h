#ifndef __PIL_DEFINES_H__
#define __PIL_DEFINES_H__

/*===========================================================================

                 Pil defines Header File

GENERAL DESCRIPTION
  This header file contains declarations and definitions for Uefi Pil 
  settings.
    
Copyright (c) 2023 by Qualcomm Technologies, Inc.  All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/08/23   rbv     Initial revision.
============================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

/*===========================================================================
                    MACROS for Pil Subsystem Cfg Version
===========================================================================*/
#define    YES                              1
#define    NO                               0

#define    PIL_SUBSYS_CFG_VERSION_V1        1
#define    PIL_SUBSYS_CFG_VERSION_V2        2
#define    PIL_SUBSYS_CFG_VERSION_V3        3
#define    PIL_SUBSYS_CFG_VERSION_V4        4
#define    PIL_SUBSYS_CFG_VERSION_V5        5
#define    PIL_SUBSYS_CFG_LATEST_VERSION    PIL_SUBSYS_CFG_VERSION_V5

/*=========================================================================
                 Enums from Source Code
===========================================================================*/
/*PIL_ELF_TYPE Enums from EFIPil.h*/
#define ELF_FV           1
#define ELF_SPLIT        2
#define ELF_SINGLE       3
#define ELF_MERGED       4
#define MAX_PIL_ELF_TYPE 0x7FFFFFFF

//=========================================================================
//=========================================================================

#define EMPTY_GUID 00000000 0000 0000 0000 000000000000

/*=========================================================================
                 Pil Images Loading Address and Size
===========================================================================*/

#define PIL_IMAGE_LOADINFO_BASE 0x1468094C
#define PIL_IMAGE_LOAD_SIZE 200

#endif /*__PIL_DEFINES_H__*/
