/*=============================================================================   
    @file  qntm_tz_memmap_defines.h
    @brief interface to device configuration
   
    Copyright (c) Qualcomm Technologies, Inc. All Rights Reserved.
    Confidential and Proprietary - Qualcomm Technologies, Inc
===============================================================================*/

/*=============================================================================
                              EDIT HISTORY
 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
02/29/24   sd      Added new include file.
=============================================================================*/

// IMEM Configuration
#define SHARED_IMEM_OFFSET_ADDR     0x0
#define IMEM_HYP_OFFSET_ADDR        0x00000B20
#define IMEM_TZ_DIAG_OFFSET_ADDR    0x00000720
#define SYSTEM_IMEM_TZ_OFFSET_ADDR  0xF000
#define SYSTEM_IMEM_TZ_SIZE_ADDR    0x19000
#define NUM_OF_CLUSTERS             2
#define NUM_OF_CPUS_PER_CLUSTER     4
#define TZBSP_CPU_COUNT				5

// App Region Configuration
#define SCL_TZ_DDR_OFFSET_ADDR      0x68900000
#define SCL_TZ_DDR_SIZE_ADDR        0x500000
#define TZ_APPS_REGION_SIZE_ADDR    0xC800000
