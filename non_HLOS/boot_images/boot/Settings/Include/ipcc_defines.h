#ifndef IPCC_DEFINES_H
#define IPCC_DEFINES_H

/*!
 * @file ipcc_defines.h
 * This file defines symbolic constants that are part of public API to the IPCC
 * driver.
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

/* IPCC protocols */
#define IPCC_P_MPROC      0
#define IPCC_P_COMPUTEL0  1
#define IPCC_P_COMPUTEL1  2
#define IPCC_P_PCIEMSI    3
#define IPCC_P_PERIPH     IPCC_P_PCIEMSI
#define IPCC_P_FENCE      4
#define IPCC_P_TOTAL      5

/* IPCC clients */
#define IPCC_C_AOP        0
#define IPCC_C_TZ         1
#define IPCC_C_APSS_S     IPCC_C_TZ
#define IPCC_C_MPSS       2
#define IPCC_C_LPASS      3
#define IPCC_C_ADSP0      IPCC_C_LPASS
#define IPCC_C_SLPI       4
#define IPCC_C_SDC        5
#define IPCC_C_NSP0       6
#define IPCC_C_CDSP       IPCC_C_NSP0
#define IPCC_C_NPU        7
#define IPCC_C_APPS       8
#define IPCC_C_APSS_NS0   IPCC_C_APPS
#define IPCC_C_GPU        9
#define IPCC_C_GPU0       IPCC_C_GPU
#define IPCC_C_CVP        10
#define IPCC_C_CAM        11
#define IPCC_C_ICP0       IPCC_C_CAM
#define IPCC_C_VPU        12
#define IPCC_C_PCIE0      13
#define IPCC_C_PCIE1      14
#define IPCC_C_PCIE2      15
#define IPCC_C_SPSS       16
#define IPCC_C_SMSS       17
#define IPCC_C_NSP1       18
#define IPCC_C_PCIE3      19
#define IPCC_C_PCIE4      20
#define IPCC_C_PCIE5      21
#define IPCC_C_PCIE6      22
#define IPCC_C_TMESS      23
#define IPCC_C_WPSS       24
#define IPCC_C_DPU        25
#define IPCC_C_DPU0       IPCC_C_DPU
#define IPCC_C_IPA        26
#define IPCC_C_SAIL0      27
#define IPCC_C_SAIL1      28
#define IPCC_C_SAIL2      29
#define IPCC_C_SAIL3      30
#define IPCC_C_GPDSP0     31
#define IPCC_C_ADSP1      IPCC_C_GPDSP0
#define IPCC_C_GPDSP1     32
#define IPCC_C_ADSP2      IPCC_C_GPDSP1
#define IPCC_C_APSS_NS1   33
#define IPCC_C_APSS_NS2   34
#define IPCC_C_APSS_NS3   35
#define IPCC_C_APSS_NS4   36
#define IPCC_C_APSS_NS5   37
#define IPCC_C_APSS_NS6   38
#define IPCC_C_APSS_NS7   39
#define IPCC_C_TENX       40
#define IPCC_C_ORAN       41
#define IPCC_C_MVMSS      42
#define IPCC_C_DPU1       43
#define IPCC_C_PCIE7      44
#define IPCC_C_DBG        45
#define IPCC_C_DUMMY      IPCC_C_DBG
#define IPCC_C_SOCCP      46
#define IPCC_C_ICP1       47
#define IPCC_C_NSP2       48
#define IPCC_C_NSP3       49
#define IPCC_C_SAIL4      50
#define IPCC_C_SAIL5      51
#define IPCC_C_CPUCP      52
#define IPCC_C_A78CSS     53
#define IPCC_C_GPU1       54
#define IPCC_C_OOBSS      55
#define IPCC_C_OOBSS_S    56
#define IPCC_C_DCP        57
#define IPCC_C_PDP0       58
#define IPCC_C_PDP1       59
#define IPCC_C_PDP2       60
#define IPCC_C_PDP3       61
#define IPCC_C_M55_WM     62
#define IPCC_C_M55_AM     63
#define IPCC_C_IFE0       128
#define IPCC_C_CAM_ENG0   IPCC_C_IFE0
#define IPCC_C_IFE1       129
#define IPCC_C_CAM_ENG1   IPCC_C_IFE1
#define IPCC_C_IFE2       130
#define IPCC_C_CAM_ENG2   IPCC_C_IFE2
#define IPCC_C_IFE3       131
#define IPCC_C_CAM_ENG3   IPCC_C_IFE3
#define IPCC_C_IFE4       132
#define IPCC_C_CAM_ENG4   IPCC_C_IFE4
#define IPCC_C_IFE5       133
#define IPCC_C_CAM_ENG5   IPCC_C_IFE5
#define IPCC_C_IFE6       134
#define IPCC_C_CAM_ENG6   IPCC_C_IFE6
#define IPCC_C_IFE7       135
#define IPCC_C_CAM_ENG7   IPCC_C_IFE7
#define IPCC_C_IFE8       136
#define IPCC_C_CAM_ENG8   IPCC_C_IFE8
#define IPCC_C_IFE9       137
#define IPCC_C_CAM_ENG9   IPCC_C_IFE9
#define IPCC_C_IFE10      138
#define IPCC_C_CAM_ENG10  IPCC_C_IFE10
#define IPCC_C_IFE11      139
#define IPCC_C_CAM_ENG11  IPCC_C_IFE11
#define IPCC_C_TOTAL      140

#endif /* IPCC_DEFINES_H */
