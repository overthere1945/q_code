#ifndef __BAM_CMN_DEFINES_H__
#define __BAM_CMN_DEFINES_H__


/*=============================================================================   
    @file  bam_cmn_defines.h
    @brief defines the default values for BAM common config.
   
    Copyright (c) 2021-2022 Qualcomm Technologies, Incorporated.
                        All rights reserved.
    Qualcomm Technologies, Confidential and Proprietary.
===============================================================================*/

#define BAM_BASE_ADDRESS               0x01DC4000
#define BAM_CNFG_BITS_VAL              0xFFFFF004
#define BAM_TGT_CMD_UL_CHCK            0X100
#define BAM_MAX_MMAP                   0x40000
#define BAM_TGT_CFG_LAST               0x0
#define BAM_TGT_CFG_FORCEINIT          0x80 /**< Initialize the BAM even if EE index is not EE0.Used in SBL boot*/
#endif
