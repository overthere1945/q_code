#ifndef __NVME_DT_DEFINES_H
#define __NVME_DT_DEFINES_H
/******************************************************************************
 * nvme_dt_defines.h
 *
 * This file provides NVMe macro definitions for DT
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
2024-12-12   pz      Initial Version
=============================================================================*/


/* NVME power on pin is controlled by TLMM GPIO or PMIC GPIO */
#define TLMM_GPIO      0
#define PMIC_GPIO      1

/* Pmic Gpio is not yet supported in devprg, write the Spmi directly as WA */
#define GPIO14_MODE_CTL_OFFSET            0x9540
#define GPIO14_DIG_OUT_SOURCE_CTL_OFFSET  0x9544
#define GPIO14_EN_CTL_OFFSET              0x9546
#define GPIO14_MODE_CTL_VALUE             0x1
#define GPIO14_DIG_OUT_SOURCE_CTL_VALUE   0x80
#define GPIO14_EN_CTL_VALUE               0x80

#endif
