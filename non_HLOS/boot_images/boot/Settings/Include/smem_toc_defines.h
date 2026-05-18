#ifndef SMEM_TOC_DEFINES_H_
#define SMEM_TOC_DEFINES_H_

/*------------------------------------------------------------------------------
   SMEM_TOC_DEFINES.H  
------------------------------------------------------------------------------*/
/*!
  @file
    smem_toc_defines.h

  @brief
    This file contains the symbolic constants for SMEM table of contents
*/

/*------------------------------------------------------------------------------
 Copyright (c) 2021 Qualcomm Technologies, Inc. All Rights Reserved.
 Qualcomm Technologies, Inc. Confidential and Proprietary.
------------------------------------------------------------------------------*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.boot/6.5.6/Settings/Include/smem_toc_defines.h#1 $

when         who     what, where, why
--------     ---     ----------------------------------------------------------
09/24/2021   mad     Carved out from smem_toc.h, for device-tree-based settings
===============================================================================*/

/*===========================================================================
                        DEFINITIONS
===========================================================================*/

/** Value placed in ToC identifier field */
#define SMEM_TOC_IDENTIFIER      0x434F5424 /* $TOC */

/** TOC version supported */
#define SMEM_TOC_VERSION         0x00000001
/** Define the size of the Table of Contents partition.  Like all partitions,
 * it must be a 4kB multiple. */
#define SMEM_TOC_SIZE            (4 * 1024)

/** Max number of exclusions per ToC entry */
#define SMEM_TOC_MAX_EXCLUSIONS  4

/** Flag definitions for the entries */
#define SMEM_TOC_ENTRY_FLAGS_ENABLE_READ_PROTECTION   ( 0x00000001 )
#define SMEM_TOC_ENTRY_FLAGS_ENABLE_WRITE_PROTECTION  ( 0x00000002 )
#define SMEM_TOC_ENTRY_FLAGS_ENABLE_RW_PROTECTION \
          ( SMEM_TOC_ENTRY_FLAGS_ENABLE_READ_PROTECTION | \
            SMEM_TOC_ENTRY_FLAGS_ENABLE_WRITE_PROTECTION )

#define SMEM_TOC_ENTRY_FLAGS_IGNORE_PARTITION         ( 0x00000004 )


#endif //SMEM_TOC_DEFINES_H_