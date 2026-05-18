#ifndef __UFS_DT_DEFINES_H
#define __UFS_DT_DEFINES_H
/******************************************************************************
 * ufs_dt_defines.h
 *
 * This file provides UFS macro definitions for DT
 *
 * Copyright (c) Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 *****************************************************************************/
/*=============================================================================

                        EDIT HISTORY FOR MODULE


when         who     what, where, why
----------   ---     --------------------------------------------------- 
2025-10-14   gml     Added HS Gear macros
2024-03-05   sa      Added bitmask for log level
2024-02-15   gml     Initial Version
=============================================================================*/


/* Flags for controlling PHY init entries in ufs_mphy_init_item
 
   Bits 0-1: rate_modes
   Bits 2-3: gear_modes
   Bits 4-14: reserved
   Bit  15: stop- no more entries
*/

#define UFS_RATE_A  0x1
#define UFS_RATE_B  0x2
#define UFS_PHY_STOP_CONDITION 0x8000

#define UFS_RATE_SHFT 0x0
#define UFS_RATE_MASK 0x00000003

#define UFS_GEAR_SHFT 0x2
#define UFS_GEAR_MASK 0x0000000c

#define UFS_HS_G4   0x4
#define UFS_HS_G5   0x5

#define CONDITIONAL_FLAG(rate, gear) ((rate << UFS_RATE_SHFT) | (gear << UFS_GEAR_SHFT))

/*
These define should correspond to the defines in ufs_bsp.h
*/
#define UFS_LOG_ERROR      1
#define UFS_LOG_INFO       2
#define UFS_LOG_SENSITIVE  4

/*Vendor device IDs defines.*/
#define UFS_VENDOR_MICRON      0x12C
#define UFS_VENDOR_SAMSUNG     0x1CE
#define UFS_VENDOR_SKHYNIX     0x1AD
#define UFS_VENDOR_TOSHIBA     0x198
#define UFS_VENDOR_WDC         0x145

#endif
