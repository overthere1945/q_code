/*
 * Copyright (c)  Qualcomm Technologies, Inc. All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
 
#ifndef REGULATOR_DT_H
#define REGULATOR_DT_H

/* Note: these are the redefined values from voltage_level.h
         Make sure it is always inlined with it.
*/
#define REGULATOR_LEVEL_OFF                  0x0
#define REGULATOR_LEVEL_RETENTION            0x10
#define REGULATOR_LEVEL_TURBO                0x1E0
#define REGULATOR_LEVEL_NOM                  0x100
#define REGULATOR_LEVEL_MIN_SVS              0x30
#define REGULATOR_LEVEL_LOW_SVS_D3           0x32
#define REGULATOR_LEVEL_LOW_SVS_D2           0x34
#define REGULATOR_LEVEL_LOW_SVS_D1           0x38
#define REGULATOR_LEVEL_LOW_SVS_D0           0x3C
#define REGULATOR_LEVEL_LOW_SVS              0x40
#define REGULATOR_LEVEL_LOW_SVS_P1           0x48
#define REGULATOR_LEVEL_LOW_SVS_L1           0x50
#define REGULATOR_LEVEL_LOW_SVS_L2           0x60
#define REGULATOR_LEVEL_SVS                  0x80
#define REGULATOR_LEVEL_SVS_L0               0x90
#define REGULATOR_LEVEL_SVS_L1               0xC0
#define REGULATOR_LEVEL_SVS_L2               0xE0
#define REGULATOR_LEVEL_NOM_L0               0x120
#define REGULATOR_LEVEL_NOM_L1               0x140
#define REGULATOR_LEVEL_NOM_L2               0x150
#define REGULATOR_LEVEL_TUR                  0x180
#define REGULATOR_LEVEL_TUR_L0               0x190
#define REGULATOR_LEVEL_TUR_L1               0x1A0
#define REGULATOR_LEVEL_TUR_L2               0x1B0
#define REGULATOR_LEVEL_TUR_L3               0x1C0
#define REGULATOR_LEVEL_TUR_L4               0x1C4
#define REGULATOR_LEVEL_TUR_L5               0x1C8
#define REGULATOR_LEVEL_SUPER_TURBO          0x1D0
#define REGULATOR_LEVEL_SUPER_TURBO_NO_CPR   0x1E0

/* Note: these are the redefined values from rpmh_common.h
         Make sure it is always inlined with it.
*/

#define RSC_DRV_TZ              0x0
#define RSC_DRV_HYP             0x1
#define RSC_DRV_HLOS            0x2
#define RSC_DRV_L3              0x3
#define RSC_DRV_SECPROC         0x4
#define RSC_DRV_AUDIO           0x5
#define RSC_DRV_SENSORS         0x6
#define RSC_DRV_AOP             0x7
#define RSC_DRV_DEBUG           0x8
#define RSC_DRV_GPU             0x9
#define RSC_DRV_DISPLAY         0xA
#define RSC_DRV_COMPUTE_DSP     0xB
#define RSC_DRV_TME_SW          0xC
#define RSC_DRV_TME_HW          0xD
#define RSC_DRV_MODEM_SW        0xE
#define RSC_DRV_MODEM_HW        0xF
#define RSC_DRV_WLAN_RF         0x10
#define RSC_DRV_WLAN_BB         0x11
#define RSC_DRV_DDR_AUX         0x12
#define RSC_DRV_ARC_CPRF        0x13
#define RSC_DRV_ARC_DEP         0x14
#define RSC_DRV_BCM             0x15
#define RSC_DRV_RESERVED        0x16
#define RSC_DRV_MAX             0x17
#define RSC_DRV_VIRTUAL_DRVS    0x3FFFFF00
#define RSC_DRV_VIRTUAL_SENSORS 0x3FFFFF01
#define RSC_DRV_VIRTUAL_MAX     0x3FFFFF02

/* Note: these are the redefined values from npa_remote_resource.h
         Make sure it is always inlined with it.
*/
#define MULTI_PD_REGULATOR         (int)0xFFFFFFFF

/* Note: these are the redefined values from cpr_defs.h
         Make sure it is always inlined with it.
*/

#define CPR_RAIL_MSS_LDO           0x500
#define CPR_RAIL_TURING_LDO        0x501
#define CPR_RAIL_NAV_LDO           0x502
#define CPR_RAIL_WMSS_CX_1         0x600
#define CPR_RAIL_WMSS_CX_2         0x601
#define CPR_RAIL_WMSS_CX_3         0x602
#define CPR_RAIL_WMSS_MX_1         0x800
#define CPR_RAIL_WMSS_MX_2         0x801


#endif //REGULATOR_DT_H
