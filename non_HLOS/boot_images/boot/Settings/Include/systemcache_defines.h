/*
 * systemcache_defines.h
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries. All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc
 */

/*=============================================================================
 EDIT HISTORY

 when       who       what, where, why
 --------   ---       ---------------------------------------------------------
 03/10/24   Raja      Initial revision
 =============================================================================*/

#ifndef SYSTEMCACHE_DEFINES_H_
#define SYSTEMCACHE_DEFINES_H_

/* System cache clients */
#define LLCC_RESERVE                0
#define LLCC_CPUSS                  1
#define LLCC_VIDSC0                 2
#define LLCC_VIDSC1                 3
#define LLCC_ROTATOR                4
#define LLCC_VOICE                  5
#define LLCC_AUDIO                  6
#define LLCC_MODEMHP_GROW           7
#define LLCC_MODEM                  8
#define LLCC_MODEMHW                9
#define LLCC_COMPUTE                10
#define LLCC_GPUHTW                 11
#define LLCC_GPU                    12
#define LLCC_MMUHWT                 13
#define LLCC_COMPUTE_DMA            15
#define LLCC_DISPLAY                16
#define LLCC_VIDEOFW                17
#define LLCC_CAMERAFW               18
#define LLCC_MSSTCM                 19
#define LLCC_MODEMHP_FIX            20
#define LLCC_MODEM_PAGING           21
#define LLCC_AUDIOHW                22
#define LLCC_NPU                    23
#define LLCC_WLANHW                 24
#define LLCC_PIMEM                  25
#define LLCC_ECC                    26
#define LLCC_DISPLAY_VIG            27
#define LLCC_CVP                    28
#define LLCC_MODEM_VPE              29
#define LLCC_APTCM                  30
#define LLCC_WRITE_CACHE            31
#define LLCC_CVPFW                  32
#define LLCC_CPUSS1                 33
#define LLCC_CAMERA_EXP0            34
#define LLCC_CPU_MTE                35
#define LLCC_CPUHWT                 36
#define LLCC_MODEM_CLADE2           37
#define LLCC_CAMERA_EXP1            38
#define LLCC_COMPUTE_HCP            39
#define LLCC_LCP_DARE               40
#define LLCC_MODEM_CLADE_2_1        41
#define LLCC_MODEM_CLADE_2_2        42
#define LLCC_MODEM_CLADE_2_3        43
#define LLCC_MODEM_CLADE_2_4        44
#define LLCC_AUDIO_ENPU             45
#define LLCC_ISLAND_1               46
#define LLCC_ISLAND_2               47
#define LLCC_ISLAND_3               48
#define LLCC_ISLAND_4               49
#define LLCC_CAMERA_EXP2            50
#define LLCC_CAMERA_EXP3            51
#define LLCC_CAMERA_EXP4            52
#define LLCC_DISPLAY_WB             53
#define LLCC_DISPLAY_1              54
#define LLCC_SAIL                   55
#define LLCC_GDSP                   56
#define LLCC_VIDSC_LEFT             57
#define LLCC_VIDSC_RIGHT            58
#define LLCC_GPU_MV                 59
#define LLCC_EVA_LEFT               60
#define LLCC_EVA_RIGHT              61
#define LLCC_EVA_GAIN               62
#define LLCC_VIDSC_DEPTH            63
#define LLCC_VIDSC_VSP              64
#define LLCC_EVA3DR                 69
#define LLCC_VIDSC_DECODE           70
#define LLCC_CAM_OFE_IP             71
#define LLCC_CAM_IPE_RT_IP          72
#define LLCC_CAM_IPE_SRT_IP         73
#define LLCC_CAM_IPE_RT_RF          74
#define LLCC_CAM_IPE_SRT_RF         75
#define LLCC_GPU_LITTLE             80
#define LLCC_OOBM_NS                81
#define LLCC_OOBM_S                 82
#define LLCC_VIDEO_APV              83
#define LLCC_SAIL_DATA_PRIV         84
#define LLCC_SAIL_META_PRIV         85
#define LLCC_DCP                    86
#define LLCC_COMPUTE_1              87
#define LLCC_CPUSS_OPPORTUNISTIC    88
#define LLCC_CPUSS_MPAM_1           89
#define LLCC_PARTIAL_WRITES         90
#define LLCC_CAM_IPE_STROV          92
#define LLCC_CAM_OFE_STROV          93
#define LLCC_CPUSS_HEU              94
#define LLCC_PCIE_TCU               97
#define LLCC_GPUHTW_LITTLE          98
#define LLCC_MODEM_PAGING_FIXED     100
#define LLCC_VIDSC_LEGACY           256
#define LLCC_CSC_LEGACY             257
#define LLCC_GCX_LEGACY             258
#define LLCC_VIDSC_LAYER0           259
#define LLCC_VIDSC_LAYER1           260
#define LLCC_VIDSC_LAYER2           261
#define LLCC_VIDSC_LAYER3           262
#define LLCC_GPU_LAYERS             263
#define LLCC_CSC_LAYER0             264
#define LLCC_CSC_LAYER1             265
#define LLCC_CSC_LAYER2             266
#define LLCC_CSC_LAYER3             267
#define LLCC_GCX_NEW                280
#define LLCC_TCM_WIFI               281
#define LLCC_TCM_CAMERA             282
#define LLCC_TCM_OEM                283
#define LLCC_GPU_TEMP_DATA          284

/* Caching modes */
#define LLCC_MODE_NORMAL            0x0
#define LLCC_MODE_TCM               0x1
#define LLCC_MODE_NSE               0x2

#define KB_TO_BYTES(n)  (n * 1024)

#endif /* SYSTEMCACHE_DEFINES_H_ */
