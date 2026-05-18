/* Copyright (c) Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#ifndef __SECLIB_DT_DEFINES_H__
#define __SECLIB_DT_DEFINES_H__

/* Defines for seclib.lcp node - start */
#define LCP_DT_PLAIN_DDR_TYPE         0x0
#define LCP_DT_DE_TYPE                0x1
#define LCP_DT_DAE_TYPE               0x2
#define LCP_DT_DARE_TYPE              0x3
#define LCP_DT_PLAIN_DDR_AND_MTE_TYPE 0x4
#define LCP_DT_DE_AND_MTE_TYPE        0x5

/* This tells DSF to leave the LCP region disabled during cold-boot LCP init.
 * The region can be enabled later, when the use case that requires the protection is active.
 */
#define LCP_DT_EARLY_EN_NOT_SET       0x0
/* This tells DSF to enable the LCP region during cold-boot LCP init */
#define LCP_DT_EARLY_EN_SET           0x1
/* Defines for seclib.lcp node - end */

/* Defines for seclib.cpu-feat-en node - start */
#define SECLIB_DT_CPU_FEAT_EN_DEFAULT    (0x0)
#define SECLIB_DT_CPU_FEAT_EN_MTE        (0x1)
#define SECLIB_DT_CPU_FEAT_EN_SME        (0x2)
/* Defines for seclib.cpu-feat-en node - end */

#endif /* __SECLIB_DT_DEFINES_H__ */
