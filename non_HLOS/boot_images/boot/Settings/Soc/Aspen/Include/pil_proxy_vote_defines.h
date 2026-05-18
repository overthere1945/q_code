#ifndef __PIL_PROXY_VOTE_DEFINES_H__
#define __PIL_PROXY_VOTE_DEFINES_H__


/*=============================================================================   
    @file  pil_defines.h
    @brief interface to sub-system configuration
   
   Copyright (c)  Qualcomm Technologies, Inc. All rights reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc
===============================================================================*/

/*=============================================================================
                              EDIT HISTORY
 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 09/07/21   avm      Added new include file.
=============================================================================*/

/*
 * flag macro
 * */
#define FLAG_NOTHING			0x0
#define	FLAG_VOTE_ONLY			0x1
#define FLAG_UNVOTE_ONLY		0x2
#define	FLAG_VOTE_AND_UNVOTE	(FLAG_VOTE_ONLY|FLAG_UNVOTE_ONLY)

#define FLAG_DTB_IMAGE			0x10

/*
 * Peripheral types
 * */
#define TZ_PIL_AUTH_MODEM_PROC             0    /**< This enum member is deprecated.
                                                     Use #TZ_PIL_AUTH_MODEM_FW_PROC. */
#define TZ_PIL_AUTH_QDSP6_PROC             1    /**< LPASS Hexagon@tm processor. */
#define TZ_PIL_AUTH_SPS_PROC               2    /**< This enum member is deprecated. */
#define TZ_PIL_AUTH_EXEC                   3    /**< QSEE generic executive image. */
#define TZ_PIL_AUTH_MODEM_SW_PROC          4    /**< Modem software processor. */
#define TZ_PIL_AUTH_MODEM_FW_PROC          5    /**< Modem firmware processor. */
#define TZ_PIL_AUTH_WLAN_PROC              6    /**< Riva processor. */
#define TZ_PIL_AUTH_SEC_APP                7    /**< QSEE software secure applications. */
#define TZ_PIL_AUTH_GNSS_PROC              8    /**< GNSS processor. */
#define TZ_PIL_AUTH_VIDEO_PROC             9    /**< Video processor. */
#define TZ_PIL_AUTH_VPU_PROC               10   /**< VPU processor. */
#define TZ_PIL_AUTH_BCSS_PROC              11   /**< BCSS processor. */
#define TZ_PIL_AUTH_SSC_PROC               12   /**< SSC Hexagon@tm processor. */
#define TZ_PIL_AUTH_GPU_UCODE_PROC         13   /**< GPU Microcode. */
#define TZ_PIL_AUTH_SPSS_PROC              14   /**< Secure processor. */
#define TZ_PIL_AUTH_IPA_GSI_PROC           15   /**< IPA GSI Firmware. */
#define TZ_PIL_AUTH_SEC_TEE                16   /**< QSEE software secure TEE. */
#define TZ_PIL_AUTH_SEC_LIB                17   /**< QSEE software secure library. */
#define TZ_PIL_AUTH_CDSP_PROC              18   /**< CDSP processor. */
#define TZ_PIL_AUTH_GSI_QUP_PROC           19   /**< Firmware for the GSI/QUP. */
#define TZ_PIL_AUTH_UEFIFV_PROC            20   /**< UEFI Firmware Volumes. */
#define TZ_PIL_AUTH_ABLFV_PROC             21   /**< ABL (LinuxLoader). */
#define TZ_PIL_AUTH_ACPI                   22   /**< ACPI Tables. */
#define TZ_PIL_AUTH_NPU_PROC               23   /**< NPU Firmware. */
#define TZ_PIL_AUTH_EXT_DSP_PROC           24   /**< External DSP Firmware. */
#define TZ_PIL_AUTH_DAM_APP_PROC           25   /**< Downloadable App Module (non-sec apps used in TX alone). */
#define TZ_PIL_AUTH_CVP_PROC               26   /**< CVP Firmware. */
#define TZ_PIL_AUTH_IPA_UC_PROC            27   /**< IPA uC Firmware. */
#define TZ_PIL_AUTH_GUEST_OS_VM            28   /**  GUEST OS VM  */
#define TZ_PIL_AUTH_EXT_DSP_FWU_PROC       29   /**< External DSP Firmware Update. */
#define TZ_PIL_AUTH_CDSP1_PROC             30   /**< CDSP1 processor. Some target will have 2 different CDSP subsystem, ex: Makena Auto */
#define TZ_PIL_AUTH_QDSP6_LITE_PROC		   31   /**< LPASS (ADSP Lite) Hexagon@tm processor. */
#define TZ_PIL_AUTH_USB4FW_PROC            32   /**< USB FW */
#define TZ_PIL_AUTH_CAMERA_FW_PROC         33   /**< CAMERA FW */
#define TZ_PIL_AUTH_OEM_VM                 34   /**< OEM VM */
#define TZ_PIL_AUTH_CPUSYS_VM              35   /**< CPUSYS VM */
#define TZ_PIL_AUTH_QDSP6_DTB_PROC         36   /**< LPASS DTB (ADSP DTB) Hexagon@tm processor. */
#define TZ_PIL_AUTH_CDSP_DTB_PROC          37   /**< CDSP DTB processor. */
#define TZ_PIL_AUTH_MODEM_SW_DTB_PROC      38   /**< Modem software DTB */
#define TZ_PIL_AUTH_UNSUPPORTED_PROC       39   /**< Processor not supported. */

/*
 * Volatge Level defination for RPM rails
 *
 * Please don't change current exiting enum values for the defiend corners.
 * Newly added corners needs to be defined with new enum value without changing the existing values.
 *
 * Please check with Power/ System driver folks before udpating the header file
 * */
#define RAIL_VOLTAGE_LEVEL_OFF             0x0    // 0
#define RAIL_VOLTAGE_LEVEL_RET             0x10   // 16
#define RAIL_VOLTAGE_LEVEL_MIN_SVS         0x30   // 48
#define RAIL_VOLTAGE_LEVEL_LOW_SVS_D1      0x38   // 56
#define RAIL_VOLTAGE_LEVEL_LOW_SVS         0x40   // 64
#define RAIL_VOLTAGE_LEVEL_LOW_SVS_L1      0x50   // 80
#define RAIL_VOLTAGE_LEVEL_LOW_SVS_L2      0x60   // 96
#define RAIL_VOLTAGE_LEVEL_SVS             0x80   // 128
#define RAIL_VOLTAGE_LEVEL_SVS_L0          0x90   // 144
#define RAIL_VOLTAGE_LEVEL_SVS_L1          0xC0   // 192
#define RAIL_VOLTAGE_LEVEL_SVS_L2          0xE0   // 224
#define RAIL_VOLTAGE_LEVEL_NOM             0x100  // 256
#define RAIL_VOLTAGE_LEVEL_NOM_L1          0x140  // 320
#define RAIL_VOLTAGE_LEVEL_NOM_L2          0x150  // 336
#define RAIL_VOLTAGE_LEVEL_TUR             0x180  // 384
#define RAIL_VOLTAGE_LEVEL_TUR_L0          0x190  // 400
#define RAIL_VOLTAGE_LEVEL_TUR_L1          0x1A0  // 416
#define RAIL_VOLTAGE_LEVEL_TUR_L2          0x1B0  // 432
#define RAIL_VOLTAGE_LEVEL_TUR_L3          0x1C0  // 448
#define RAIL_VOLTAGE_SUPER_TURBO           0x1D0  // 464
#define RAIL_VOLTAGE_SUPER_TURBO_NO_CPR    0x1E0  // 480

/*
 * Master Identifiers
 * Refer icbid.h to configure new master identifiers
 */
#define ICBID_MASTER_APPSS_PROC               0
#define ICBID_MASTER_MSS_PROC                 1
#define ICBID_MASTER_MDSP_Q6                  ICBID_MASTER_MSS_PROC
#define ICBID_MASTER_MNOC_BIMC                2
#define ICBID_MASTER_MNOC_HF_MEM_NOC          ICBID_MASTER_MNOC_BIMC
#define ICBID_MASTER_SNOC_BIMC                3
#define ICBID_MASTER_SNOC_BIMC_0              ICBID_MASTER_SNOC_BIMC
#define ICBID_MASTER_SNOC_GC_MEM_NOC          ICBID_MASTER_SNOC_BIMC
#define ICBID_MASTER_CNOC_MNOC_MMSS_CFG       4
#define ICBID_MASTER_CNOC_MNOC_CFG            5
#define ICBID_MASTER_GFX3D                    6
#define ICBID_MASTER_JPEG                     7
#define ICBID_MASTER_MDP                      8
#define ICBID_MASTER_MDP0                     ICBID_MASTER_MDP
#define ICBID_MASTER_MDPS                     ICBID_MASTER_MDP
#define ICBID_MASTER_VIDEO                    9
#define ICBID_MASTER_VIDEO_P0                 ICBID_MASTER_VIDEO
#define ICBID_MASTER_VIDEO_P1                 10
#define ICBID_MASTER_VFE                      11
#define ICBID_MASTER_VFE0                     ICBID_MASTER_VFE
#define ICBID_MASTER_CNOC_ONOC_CFG            12
#define ICBID_MASTER_JPEG_OCMEM               13
#define ICBID_MASTER_MDP_OCMEM                14
#define ICBID_MASTER_VIDEO_P0_OCMEM           15
#define ICBID_MASTER_VIDEO_P1_OCMEM           16
#define ICBID_MASTER_VFE_OCMEM                17
#define ICBID_MASTER_LPASS_AHB                18
#define ICBID_MASTER_QDSS_BAM                 19
#define ICBID_MASTER_SNOC_CFG                 20
#define ICBID_MASTER_BIMC_SNOC                21
#define ICBID_MASTER_BIMC_SNOC_0              ICBID_MASTER_BIMC_SNOC
#define ICBID_MASTER_MEM_NOC_SNOC             ICBID_MASTER_BIMC_SNOC
#define ICBID_MASTER_GEM_NOC_SNOC             ICBID_MASTER_BIMC_SNOC
#define ICBID_MASTER_CNOC_SNOC                22
#define ICBID_MASTER_CRYPTO                   23
#define ICBID_MASTER_CRYPTO_CORE0             ICBID_MASTER_CRYPTO
#define ICBID_MASTER_CRYPTO_CORE1             24
#define ICBID_MASTER_LPASS_PROC               25
#define ICBID_MASTER_MSS                      26
#define ICBID_MASTER_MSS_NAV                  27
#define ICBID_MASTER_OCMEM_DMA                28
#define ICBID_MASTER_PNOC_SNOC                29
#define ICBID_MASTER_WCSS                     30
#define ICBID_MASTER_QDSS_ETR                 31
#define ICBID_MASTER_QDSS_ETR_0               ICBID_MASTER_QDSS_ETR
#define ICBID_MASTER_USB3                     32
#define ICBID_MASTER_USB3_0                   ICBID_MASTER_USB3
#define ICBID_MASTER_SDCC_1                   33
#define ICBID_MASTER_SDCC_3                   34
#define ICBID_MASTER_SDCC_2                   35
#define ICBID_MASTER_SDCC_4                   36
#define ICBID_MASTER_TSIF                     37
#define ICBID_MASTER_BAM_DMA                  38
#define ICBID_MASTER_BLSP_2                   39
#define ICBID_MASTER_QUP_2                    ICBID_MASTER_BLSP_2
#define ICBID_MASTER_USB_HSIC                 40
#define ICBID_MASTER_BLSP_1                   41
#define ICBID_MASTER_QUP_1                    ICBID_MASTER_BLSP_1
#define ICBID_MASTER_USB_HS                   42
#define ICBID_MASTER_USB_HS1                  ICBID_MASTER_USB_HS
#define ICBID_MASTER_PNOC_CFG                 43
#define ICBID_MASTER_SNOC_PNOC                44
#define ICBID_MASTER_RPM_INST                 45
#define ICBID_MASTER_RPM_DATA                 46
#define ICBID_MASTER_RPM_SYS                  47
#define ICBID_MASTER_DEHR                     48
#define ICBID_MASTER_QDSS_DAP                 49
#define ICBID_MASTER_SPDM                     50
#define ICBID_MASTER_TIC                      51
#define ICBID_MASTER_SNOC_CNOC                52
#define ICBID_MASTER_GFX3D_OCMEM              53
#define ICBID_MASTER_GFX3D_GMEM               ICBID_MASTER_GFX3D_OCMEM
#define ICBID_MASTER_OVIRT_SNOC               54
#define ICBID_MASTER_SNOC_OVIRT               55
#define ICBID_MASTER_SNOC_GVIRT               ICBID_MASTER_SNOC_OVIRT
#define ICBID_MASTER_ONOC_OVIRT               56
#define ICBID_MASTER_USB_HS2                  57
#define ICBID_MASTER_QPIC                     58
#define ICBID_MASTER_IPA                      59
#define ICBID_MASTER_DSI                      60
#define ICBID_MASTER_MDP1                     61
#define ICBID_MASTER_MDPE                     ICBID_MASTER_MDP1
#define ICBID_MASTER_VPU_PROC                 62
#define ICBID_MASTER_VPU                      63
#define ICBID_MASTER_VPU0                     ICBID_MASTER_VPU
#define ICBID_MASTER_CRYPTO_CORE2             64
#define ICBID_MASTER_PCIE_0                   65
#define ICBID_MASTER_PCIE_1                   66
#define ICBID_MASTER_SATA                     67
#define ICBID_MASTER_UFS                      68
#define ICBID_MASTER_UFS_CARD                 ICBID_MASTER_UFS
#define ICBID_MASTER_USB3_1                   69
#define ICBID_MASTER_VIDEO_OCMEM              70
#define ICBID_MASTER_VPU1                     71
#define ICBID_MASTER_VCAP                     72
#define ICBID_MASTER_EMAC                     73
#define ICBID_MASTER_BCAST                    74
#define ICBID_MASTER_MMSS_PROC                75
#define ICBID_MASTER_SNOC_BIMC_1              76
#define ICBID_MASTER_SNOC_SF_MEM_NOC          ICBID_MASTER_SNOC_BIMC_1
#define ICBID_MASTER_SNOC_PCNOC               77
#define ICBID_MASTER_AUDIO                    78
#define ICBID_MASTER_MM_INT_0                 79
#define ICBID_MASTER_MM_INT_1                 80
#define ICBID_MASTER_MM_INT_2                 81
#define ICBID_MASTER_MM_INT_BIMC              82
#define ICBID_MASTER_MSS_INT                  83
#define ICBID_MASTER_PCNOC_CFG                84
#define ICBID_MASTER_PCNOC_INT_0              85
#define ICBID_MASTER_PCNOC_INT_1              86
#define ICBID_MASTER_PCNOC_M_0                87
#define ICBID_MASTER_PCNOC_M_1                88
#define ICBID_MASTER_PCNOC_S_0                89
#define ICBID_MASTER_PCNOC_S_1                90
#define ICBID_MASTER_PCNOC_S_2                91
#define ICBID_MASTER_PCNOC_S_3                92
#define ICBID_MASTER_PCNOC_S_4                93
#define ICBID_MASTER_PCNOC_S_6                94
#define ICBID_MASTER_PCNOC_S_7                95
#define ICBID_MASTER_PCNOC_S_8                96
#define ICBID_MASTER_PCNOC_S_9                97
#define ICBID_MASTER_QDSS_INT                 98
#define ICBID_MASTER_SNOC_INT_0               99
#define ICBID_MASTER_SNOC_INT_1               100
#define ICBID_MASTER_SNOC_INT_BIMC            101
#define ICBID_MASTER_TCU_0                    102
#define ICBID_MASTER_TCU_1                    103
#define ICBID_MASTER_BIMC_INT_0               104
#define ICBID_MASTER_BIMC_INT_1               105
#define ICBID_MASTER_CAMERA                   106
#define ICBID_MASTER_RICA                     107
#define ICBID_MASTER_SNOC_BIMC_2              108
#define ICBID_MASTER_BIMC_SNOC_1              109
#define ICBID_MASTER_A0NOC_SNOC               110
#define ICBID_MASTER_ANOC_SNOC                ICBID_MASTER_A0NOC_SNOC
#define ICBID_MASTER_A1NOC_SNOC               111
#define ICBID_MASTER_A2NOC_SNOC               112
#define ICBID_MASTER_PIMEM                    113
#define ICBID_MASTER_SNOC_VMEM                114
#define ICBID_MASTER_CPP                      115
#define ICBID_MASTER_CNOC_A1NOC               116
#define ICBID_MASTER_PNOC_A1NOC               117
#define ICBID_MASTER_HMSS                     118
#define ICBID_MASTER_PCIE_2                   119
#define ICBID_MASTER_ROTATOR                  120
#define ICBID_MASTER_VENUS_VMEM               121
#define ICBID_MASTER_DCC                      122
#define ICBID_MASTER_MCDMA                    123
#define ICBID_MASTER_PCNOC_INT_2              124
#define ICBID_MASTER_PCNOC_INT_3              125
#define ICBID_MASTER_PCNOC_INT_4              126
#define ICBID_MASTER_PCNOC_INT_5              127
#define ICBID_MASTER_PCNOC_INT_6              128
#define ICBID_MASTER_PCNOC_S_5                129
#define ICBID_MASTER_SENSORS_AHB              130
#define ICBID_MASTER_SENSORS_PROC             131
#define ICBID_MASTER_QSPI                     132
#define ICBID_MASTER_QSPI_0                   ICBID_MASTER_QSPI
#define ICBID_MASTER_VFE1                     133
#define ICBID_MASTER_SNOC_INT_2               134
#define ICBID_MASTER_SMMNOC_BIMC              135
#define ICBID_MASTER_CRVIRT_A1NOC             136
#define ICBID_MASTER_XM_USB_HS1               137
#define ICBID_MASTER_XI_USB_HS1               138
#define ICBID_MASTER_PCNOC_BIMC_1             139
#define ICBID_MASTER_BIMC_PCNOC               140
#define ICBID_MASTER_XI_HSIC                  141
#define ICBID_MASTER_SGMII                    142
#define ICBID_MASTER_SPMI_FETCHER             143
#define ICBID_MASTER_GNOC_BIMC                144
#define ICBID_MASTER_GNOC_MEM_NOC             ICBID_MASTER_GNOC_BIMC
#define ICBID_MASTER_CRVIRT_A2NOC             145
#define ICBID_MASTER_CNOC_A2NOC               146
#define ICBID_MASTER_WLAN                     147
#define ICBID_MASTER_MSS_CE                   148
#define ICBID_MASTER_A1NOC_CFG                149
#define ICBID_MASTER_A2NOC_CFG                150
#define ICBID_MASTER_GNOC_CFG                 151
#define ICBID_MASTER_GNOC_SNOC                152
#define ICBID_MASTER_CDSP_HCP_TCM             153
#define ICBID_MASTER_CDSP_HCP                 154
#define ICBID_MASTER_CDSP_DMA_TCM             155
#define ICBID_MASTER_CDSP_DMA                 156
#define ICBID_MASTER_AOSS                     157
#define ICBID_MASTER_CDSP_NOC_CFG             158
#define ICBID_MASTER_MEM_NOC_CFG              159
#define ICBID_MASTER_GEM_NOC_CFG              ICBID_MASTER_MEM_NOC_CFG
#define ICBID_MASTER_VAPSS_NOC_CFG            160
#define ICBID_MASTER_SHRM                     161
#define ICBID_MASTER_CNOC_DC_NOC              162
#define ICBID_MASTER_CDC_SNOC_GC              163
#define ICBID_MASTER_CDC_SNOC_SF              164
#define ICBID_MASTER_CDSP                     165
#define ICBID_MASTER_LPASS_ANOC               166
#define ICBID_MASTER_LPASS_GEM_NOC            ICBID_MASTER_LPASS_ANOC
#define ICBID_MASTER_MEM_NOC_GNOC             167
#define ICBID_MASTER_MEM_NOC_PCIE_SNOC        168
#define ICBID_MASTER_GEM_NOC_PCIE_SNOC        ICBID_MASTER_MEM_NOC_PCIE_SNOC
#define ICBID_MASTER_MNOC_SF_MEM_NOC          169
#define ICBID_MASTER_ANOC_PCIE_SNOC           170
#define ICBID_MASTER_ANOC_PCIE_GEM_NOC        ICBID_MASTER_ANOC_PCIE_SNOC
#define ICBID_MASTER_CAMNOC_HF                171
#define ICBID_MASTER_CAMNOC_HF0               ICBID_MASTER_CAMNOC_HF
#define ICBID_MASTER_CAMNOC_SF                172
#define ICBID_MASTER_CDSP_PROC                173
#define ICBID_MASTER_SP                       174
#define ICBID_MASTER_VAPSS_CDSP_0             175
#define ICBID_MASTER_VAPSS_CDSP_1             176
#define ICBID_MASTER_VIDEO_PROC               177
#define ICBID_MASTER_UFS_MEM                  178
#define ICBID_MASTER_GIC                      179
#define ICBID_MASTER_LLCC                     180
#define ICBID_MASTER_ANOC_IPA                 181
#define ICBID_MASTER_IPA_PCIE                 182
#define ICBID_MASTER_IPA_CORE                 183
#define ICBID_MASTER_CAMNOC_HF1               184
#define ICBID_MASTER_CAMNOC_HF0_UNCOMP        185
#define ICBID_MASTER_CAMNOC_HF1_UNCOMP        186
#define ICBID_MASTER_CAMNOC_SF_UNCOMP         187
#define ICBID_MASTER_LPASS_CORE               188
#define ICBID_MASTER_Q6                       189
#define ICBID_MASTER_SENSORSS_PROC            ICBID_MASTER_Q6
#define ICBID_MASTER_QUPV3                    190
#define ICBID_MASTER_SENSORSS_QUPV3           ICBID_MASTER_QUPV3
#define ICBID_MASTER_LPASS_QUPV3              ICBID_MASTER_QUPV3
#define ICBID_MASTER_SDC                      191
#define ICBID_MASTER_SNOC_LPASS               192
#define ICBID_MASTER_EMMC                     193
#define ICBID_MASTER_QUP_0                    194
#define ICBID_MASTER_NPU                      195
#define ICBID_MASTER_GPU_TCU                  196
#define ICBID_MASTER_SYS_TCU                  197
#define ICBID_MASTER_COMPUTE_NOC              198
#define ICBID_MASTER_ECC                      199
#define ICBID_MASTER_CNOC_SSC_NOC             200
#define ICBID_MASTER_LPASS_LEC                201
#define ICBID_MASTER_QSPI_1                   202
#define ICBID_MASTER_UFS_GEN4                 203
#define ICBID_MASTER_USB3_2                   204
#define ICBID_MASTER_PCIE_3                   205
#define ICBID_MASTER_QMIP_3_CFG               206
#define ICBID_MASTER_LPASS_SDR                207
#define ICBID_MASTER_EMAC_EVB                 208
#define ICBID_MASTER_PCIE                     209
#define ICBID_MASTER_USB2                     210
#define ICBID_MASTER_QUP_CORE_1               211
#define ICBID_MASTER_QUP_CORE_2               212
#define ICBID_MASTER_NPU_PROC                 213
#define ICBID_MASTER_CNOC_LPASS_AG_NOC        214
#define ICBID_MASTER_CAMNOC_ICP               215
#define ICBID_MASTER_NPU_CDP                  216
#define ICBID_MASTER_NPU_NOC_CFG              217
#define ICBID_MASTER_NPU_SYS                  218
#define ICBID_MASTER_CDSP_NOC_A2NOC_COMP_NOC  219
#define ICBID_MASTER_CAMNOC_NRT               220
#define ICBID_MASTER_CAMNOC_RT                221
#define ICBID_MASTER_QUP_CORE_0               222
#define ICBID_MASTER_GEM_NOC_CNOC             223
#define ICBID_MASTER_HCPDMA                   224
#define ICBID_MASTER_CAMNOC_HF2               225
#define ICBID_MASTER_PKA_CORE                 226
#define ICBID_MASTER_HWKM_CORE                227
#define ICBID_MASTER_LPASS_AML                228
#define ICBID_MASTER_LPASS_AML_RD             ICBID_MASTER_LPASS_AML
#define ICBID_MASTER_PCIE_TCU                 229
#define ICBID_MASTER_USB_NOC_SNOC             230
#define ICBID_MASTER_SMSS                     231
#define ICBID_MASTER_SMSS_AHB                 232
#define ICBID_MASTER_MDP_CORE1_0              233
#define ICBID_MASTER_MDP_CORE1_1              234
#define ICBID_MASTER_ROTATOR_1                235
#define ICBID_MASTER_EMAC_1                   236
#define ICBID_MASTER_PCIE_2A                  237
#define ICBID_MASTER_PCIE_2B                  238
#define ICBID_MASTER_PCIE_3A                  239
#define ICBID_MASTER_PCIE_3B                  240
#define ICBID_MASTER_PCIE_4                   241
#define ICBID_MASTER_USB3_MP                  242
#define ICBID_MASTER_USB4_0                   243
#define ICBID_MASTER_USB4_1                   244
#define ICBID_MASTER_HCPDMA_B                 245
#define ICBID_MASTER_CDSP_PROC_B              246
#define ICBID_MASTER_COMPUTE_NOC_1            247
#define ICBID_MASTER_CDSPB_NOC_CFG            248
#define ICBID_MASTER_SHRM_2                   249
#define ICBID_MASTER_AONCAM                   250
#define ICBID_MASTER_PCIE_ANOC_CFG            251
#define ICBID_MASTER_VIDEO_CV_PROC            252
#define ICBID_MASTER_VIDEO_V_PROC             253
#define ICBID_MASTER_DDRSS_EVEN               254
#define ICBID_MASTER_DDRSS_ODD                255
#define ICBID_MASTER_TME                      256
#define ICBID_MASTER_QDSS_ETR_1               257
#define ICBID_MASTER_GIC_AHB                  258
#define ICBID_MASTER_QPIC_CORE                259
#define ICBID_MASTER_WLAN_Q6                  260
#define ICBID_MASTER_CNOC2_CNOC3              261
#define ICBID_MASTER_CNOC3_CNOC2              262
#define ICBID_MASTER_LPASS_AML_WD             263
#define ICBID_MASTER_DDR_PERF_MODE            264
#define ICBID_MASTER_CNOC_CFG                 265
#define ICBID_MASTER_DC_NOC_LLCLPI_NOC        266
#define ICBID_MASTER_LPASS_ENPU_AUDIO_RD      267
#define ICBID_MASTER_LPASS_ENPU_CAMERA_RD     268
#define ICBID_MASTER_LPASS_ENPU_AUDIO_WR      269
#define ICBID_MASTER_LPASS_ENPU_CAMERA_WR     270
#define ICBID_MASTER_LPASS_AUDIO_CORE         271
#define ICBID_MASTER_LPASS_PROC_AHBM          272
#define ICBID_MASTER_VA_CORE                  273
#define ICBID_MASTER_LPASS_AG_NOC_LPICX_NOC   274
#define ICBID_MASTER_LPASS_LPINOC             275
#define ICBID_MASTER_LPASS_LPINOC_CFG         276
#define ICBID_MASTER_LPIAON_NOC               277
#define ICBID_MASTER_LPIAON_NOC_LLCLPI_NOC    278
#define ICBID_MASTER_CDSP_TBF                 279
#define ICBID_MASTER_Q6_NOC_GEMNOC            280
#define ICBID_MASTER_Q6NOC_CFG                281
#define ICBID_MASTER_CNOC_MNOC_HF_CFG         282
#define ICBID_MASTER_CNOC_MNOC_SF_CFG         283
#define ICBID_MASTER_DDRSS                    284
#define ICBID_MASTER_COUNT                    285
#define ICBID_MASTER_NONE                     ICBID_MASTER_COUNT

/*
 * Slave Identifiers
 * Refer icbid.h to configure new slave identifiers
 */
#define ICBID_SLAVE_EBI1                            0
#define ICBID_SLAVE_DDR                             ICBID_SLAVE_EBI1
#define ICBID_SLAVE_APPSS_L2                        1
#define ICBID_SLAVE_BIMC_SNOC                       2
#define ICBID_SLAVE_BIMC_SNOC_0                     ICBID_SLAVE_BIMC_SNOC
#define ICBID_SLAVE_MEM_NOC_SNOC                    ICBID_SLAVE_BIMC_SNOC
#define ICBID_SLAVE_GEM_NOC_SNOC                    ICBID_SLAVE_BIMC_SNOC
#define ICBID_SLAVE_CAMERA_CFG                      3
#define ICBID_SLAVE_DISPLAY_CFG                     4
#define ICBID_SLAVE_OCMEM_CFG                       5
#define ICBID_SLAVE_CPR_CFG                         6
#define ICBID_SLAVE_CPR_XPU_CFG                     7
#define ICBID_SLAVE_MISC_CFG                        8
#define ICBID_SLAVE_MISC_XPU_CFG                    9
#define ICBID_SLAVE_VENUS_CFG                       10
#define ICBID_SLAVE_GFX3D_CFG                       11
#define ICBID_SLAVE_MMSS_CLK_CFG                    12
#define ICBID_SLAVE_MMSS_CLK_XPU_CFG                13
#define ICBID_SLAVE_MNOC_MPU_CFG                    14
#define ICBID_SLAVE_ONOC_MPU_CFG                    15
#define ICBID_SLAVE_MNOC_BIMC                       16
#define ICBID_SLAVE_MNOC_HF_MEM_NOC                 ICBID_SLAVE_MNOC_BIMC
#define ICBID_SLAVE_SERVICE_MNOC                    17
#define ICBID_SLAVE_OCMEM                           18
#define ICBID_SLAVE_GMEM                            ICBID_SLAVE_OCMEM
#define ICBID_SLAVE_SERVICE_ONOC                    19
#define ICBID_SLAVE_APPSS                           20
#define ICBID_SLAVE_LPASS                           21
#define ICBID_SLAVE_USB3                            22
#define ICBID_SLAVE_USB3_0                          ICBID_SLAVE_USB3
#define ICBID_SLAVE_WCSS                            23
#define ICBID_SLAVE_SNOC_BIMC                       24
#define ICBID_SLAVE_SNOC_BIMC_0                     ICBID_SLAVE_SNOC_BIMC
#define ICBID_SLAVE_SNOC_MEM_NOC_GC                 ICBID_SLAVE_SNOC_BIMC
#define ICBID_SLAVE_SNOC_GEM_NOC_GC                 ICBID_SLAVE_SNOC_BIMC
#define ICBID_SLAVE_SNOC_CNOC                       25
#define ICBID_SLAVE_IMEM                            26
#define ICBID_SLAVE_OCIMEM                          ICBID_SLAVE_IMEM
#define ICBID_SLAVE_SNOC_OVIRT                      27
#define ICBID_SLAVE_SNOC_GVIRT                      ICBID_SLAVE_SNOC_OVIRT
#define ICBID_SLAVE_SNOC_PNOC                       28
#define ICBID_SLAVE_SNOC_PCNOC                      ICBID_SLAVE_SNOC_PNOC
#define ICBID_SLAVE_SERVICE_SNOC                    29
#define ICBID_SLAVE_QDSS_STM                        30
#define ICBID_SLAVE_SDCC_1                          31
#define ICBID_SLAVE_SDC1                            ICBID_SLAVE_SDCC_1
#define ICBID_SLAVE_SDCC_3                          32
#define ICBID_SLAVE_SDCC_2                          33
#define ICBID_SLAVE_SDCC_4                          34
#define ICBID_SLAVE_TSIF                            35
#define ICBID_SLAVE_BAM_DMA                         36
#define ICBID_SLAVE_BLSP_2                          37
#define ICBID_SLAVE_QUP_2                           ICBID_SLAVE_BLSP_2
#define ICBID_SLAVE_USB_HSIC                        38
#define ICBID_SLAVE_BLSP_1                          39
#define ICBID_SLAVE_QUP_1                           ICBID_SLAVE_BLSP_1
#define ICBID_SLAVE_USB_HS                          40
#define ICBID_SLAVE_USB_HS1                         ICBID_SLAVE_USB_HS
#define ICBID_SLAVE_PDM                             41
#define ICBID_SLAVE_PERIPH_APU_CFG                  42
#define ICBID_SLAVE_PNOC_MPU_CFG                    43
#define ICBID_SLAVE_PRNG                            44
#define ICBID_SLAVE_HWKM                            ICBID_SLAVE_PRNG
#define ICBID_SLAVE_PNOC_SNOC                       45
#define ICBID_SLAVE_PCNOC_SNOC                      ICBID_SLAVE_PNOC_SNOC
#define ICBID_SLAVE_SERVICE_PNOC                    46
#define ICBID_SLAVE_CLK_CTL                         47
#define ICBID_SLAVE_CNOC_MSS                        48
#define ICBID_SLAVE_PCNOC_MSS                       ICBID_SLAVE_CNOC_MSS
#define ICBID_SLAVE_SECURITY                        49
#define ICBID_SLAVE_TCSR                            50
#define ICBID_SLAVE_TLMM                            51
#define ICBID_SLAVE_CRYPTO_0_CFG                    52
#define ICBID_SLAVE_CRYPTO_1_CFG                    53
#define ICBID_SLAVE_IMEM_CFG                        54
#define ICBID_SLAVE_MESSAGE_RAM                     55
#define ICBID_SLAVE_BIMC_CFG                        56
#define ICBID_SLAVE_BOOT_ROM                        57
#define ICBID_SLAVE_CNOC_MNOC_MMSS_CFG              58
#define ICBID_SLAVE_PMIC_ARB                        59
#define ICBID_SLAVE_SPDM_WRAPPER                    60
#define ICBID_SLAVE_DEHR_CFG                        61
#define ICBID_SLAVE_MPM                             62
#define ICBID_SLAVE_QDSS_CFG                        63
#define ICBID_SLAVE_RBCPR_CFG                       64
#define ICBID_SLAVE_RBCPR_CX_CFG                    ICBID_SLAVE_RBCPR_CFG
#define ICBID_SLAVE_RBCPR_QDSS_APU_CFG              65
#define ICBID_SLAVE_CNOC_MNOC_CFG                   66
#define ICBID_SLAVE_SNOC_MPU_CFG                    67
#define ICBID_SLAVE_CNOC_ONOC_CFG                   68
#define ICBID_SLAVE_PNOC_CFG                        69
#define ICBID_SLAVE_SNOC_CFG                        70
#define ICBID_SLAVE_EBI1_DLL_CFG                    71
#define ICBID_SLAVE_PHY_APU_CFG                     72
#define ICBID_SLAVE_EBI1_PHY_CFG                    73
#define ICBID_SLAVE_RPM                             74
#define ICBID_SLAVE_CNOC_SNOC                       75
#define ICBID_SLAVE_SERVICE_CNOC                    76
#define ICBID_SLAVE_OVIRT_SNOC                      77
#define ICBID_SLAVE_OVIRT_OCMEM                     78
#define ICBID_SLAVE_USB_HS2                         79
#define ICBID_SLAVE_QPIC                            80
#define ICBID_SLAVE_IPS_CFG                         81
#define ICBID_SLAVE_DSI_CFG                         82
#define ICBID_SLAVE_USB3_1                          83
#define ICBID_SLAVE_PCIE_0                          84
#define ICBID_SLAVE_PCIE_1                          85
#define ICBID_SLAVE_PSS_SMMU_CFG                    86
#define ICBID_SLAVE_CRYPTO_2_CFG                    87
#define ICBID_SLAVE_PCIE_0_CFG                      88
#define ICBID_SLAVE_PCIE_1_CFG                      89
#define ICBID_SLAVE_SATA_CFG                        90
#define ICBID_SLAVE_SPSS_GENI_IR                    91
#define ICBID_SLAVE_UFS_CFG                         92
#define ICBID_SLAVE_UFS_CARD_CFG                    ICBID_SLAVE_UFS_CFG
#define ICBID_SLAVE_AVSYNC_CFG                      93
#define ICBID_SLAVE_VPU_CFG                         94
#define ICBID_SLAVE_USB_PHY_CFG                     95
#define ICBID_SLAVE_RBCPR_MX_CFG                    96
#define ICBID_SLAVE_RBCPR_MXA_CFG                   ICBID_SLAVE_RBCPR_MX_CFG
#define ICBID_SLAVE_PCIE_PARF                       97
#define ICBID_SLAVE_VCAP_CFG                        98
#define ICBID_SLAVE_EMAC_CFG                        99
#define ICBID_SLAVE_BCAST_CFG                       100
#define ICBID_SLAVE_KLM_CFG                         101
#define ICBID_SLAVE_DISPLAY_PWM                     102
#define ICBID_SLAVE_GENI                            103
#define ICBID_SLAVE_SNOC_BIMC_1                     104
#define ICBID_SLAVE_SNOC_MEM_NOC_SF                 ICBID_SLAVE_SNOC_BIMC_1
#define ICBID_SLAVE_SNOC_GEM_NOC_SF                 ICBID_SLAVE_SNOC_BIMC_1
#define ICBID_SLAVE_AUDIO                           105
#define ICBID_SLAVE_CATS_0                          106
#define ICBID_SLAVE_CATS_1                          107
#define ICBID_SLAVE_MM_INT_0                        108
#define ICBID_SLAVE_MM_INT_1                        109
#define ICBID_SLAVE_MM_INT_2                        110
#define ICBID_SLAVE_MM_INT_BIMC                     111
#define ICBID_SLAVE_MMU_MODEM_XPU_CFG               112
#define ICBID_SLAVE_MSS_INT                         113
#define ICBID_SLAVE_PCNOC_INT_0                     114
#define ICBID_SLAVE_PCNOC_INT_1                     115
#define ICBID_SLAVE_PCNOC_M_0                       116
#define ICBID_SLAVE_PCNOC_M_1                       117
#define ICBID_SLAVE_PCNOC_S_0                       118
#define ICBID_SLAVE_PCNOC_S_1                       119
#define ICBID_SLAVE_PCNOC_S_2                       120
#define ICBID_SLAVE_PCNOC_S_3                       121
#define ICBID_SLAVE_PCNOC_S_4                       122
#define ICBID_SLAVE_PCNOC_S_6                       123
#define ICBID_SLAVE_PCNOC_S_7                       124
#define ICBID_SLAVE_PCNOC_S_8                       125
#define ICBID_SLAVE_PCNOC_S_9                       126
#define ICBID_SLAVE_PRNG_XPU_CFG                    127
#define ICBID_SLAVE_QDSS_INT                        128
#define ICBID_SLAVE_RPM_XPU_CFG                     129
#define ICBID_SLAVE_SNOC_INT_0                      130
#define ICBID_SLAVE_SNOC_INT_1                      131
#define ICBID_SLAVE_SNOC_INT_BIMC                   132
#define ICBID_SLAVE_TCU                             133
#define ICBID_SLAVE_BIMC_INT_0                      134
#define ICBID_SLAVE_BIMC_INT_1                      135
#define ICBID_SLAVE_RICA_CFG                        136
#define ICBID_SLAVE_SNOC_BIMC_2                     137
#define ICBID_SLAVE_BIMC_SNOC_1                     138
#define ICBID_SLAVE_PNOC_A1NOC                      139
#define ICBID_SLAVE_SNOC_VMEM                       140
#define ICBID_SLAVE_A0NOC_SNOC                      141
#define ICBID_SLAVE_ANOC_SNOC                       ICBID_SLAVE_A0NOC_SNOC
#define ICBID_SLAVE_A1NOC_SNOC                      142
#define ICBID_SLAVE_A2NOC_SNOC                      143
#define ICBID_SLAVE_A0NOC_CFG                       144
#define ICBID_SLAVE_ANOC_MPU_CFG                    ICBID_SLAVE_A0NOC_CFG
#define ICBID_SLAVE_A0NOC_MPU_CFG                   145
#define ICBID_SLAVE_A0NOC_SMMU_CFG                  146
#define ICBID_SLAVE_A1NOC_CFG                       147
#define ICBID_SLAVE_A1NOC_MPU_CFG                   148
#define ICBID_SLAVE_A1NOC_SMMU_CFG                  149
#define ICBID_SLAVE_A2NOC_CFG                       150
#define ICBID_SLAVE_A2NOC_MPU_CFG                   151
#define ICBID_SLAVE_A2NOC_SMMU_CFG                  152
#define ICBID_SLAVE_AHB2PHY                         153
#define ICBID_SLAVE_CAMERA_THROTTLE_CFG             154
#define ICBID_SLAVE_CAMERA_RT_THROTTLE_CFG          ICBID_SLAVE_CAMERA_THROTTLE_CFG
#define ICBID_SLAVE_DCC_CFG                         155
#define ICBID_SLAVE_DISPLAY_THROTTLE_CFG            156
#define ICBID_SLAVE_DSA_CFG                         157
#define ICBID_SLAVE_DSA_MPU_CFG                     158
#define ICBID_SLAVE_SSC_MPU_CFG                     159
#define ICBID_SLAVE_HMSS_L3                         160
#define ICBID_SLAVE_LPASS_SMMU_CFG                  161
#define ICBID_SLAVE_MMAGIC_CFG                      162
#define ICBID_SLAVE_PCIE20_AHB2PHY                  163
#define ICBID_SLAVE_PCIE_2                          164
#define ICBID_SLAVE_PCIE_2_CFG                      165
#define ICBID_SLAVE_PIMEM                           166
#define ICBID_SLAVE_PIMEM_CFG                       167
#define ICBID_SLAVE_QDSS_RBCPR_APU_CFG              168
#define ICBID_SLAVE_RBCPR_CX                        169
#define ICBID_SLAVE_RBCPR_MX                        170
#define ICBID_SLAVE_SMMU_CPP_CFG                    171
#define ICBID_SLAVE_SMMU_JPEG_CFG                   172
#define ICBID_SLAVE_SMMU_MDP_CFG                    173
#define ICBID_SLAVE_SMMU_ROTATOR_CFG                174
#define ICBID_SLAVE_SMMU_VENUS_CFG                  175
#define ICBID_SLAVE_SMMU_VFE_CFG                    176
#define ICBID_SLAVE_SSC_CFG                         177
#define ICBID_SLAVE_VENUS_THROTTLE_CFG              178
#define ICBID_SLAVE_VENUS_CVP_THROTTLE_CFG          ICBID_SLAVE_VENUS_THROTTLE_CFG
#define ICBID_SLAVE_VMEM                            179
#define ICBID_SLAVE_VMEM_CFG                        180
#define ICBID_SLAVE_QDSS_MPU_CFG                    181
#define ICBID_SLAVE_USB3_PHY_CFG                    182
#define ICBID_SLAVE_IPA_CFG                         183
#define ICBID_SLAVE_PCNOC_INT_2                     184
#define ICBID_SLAVE_PCNOC_INT_3                     185
#define ICBID_SLAVE_PCNOC_INT_4                     186
#define ICBID_SLAVE_PCNOC_INT_5                     187
#define ICBID_SLAVE_PCNOC_INT_6                     188
#define ICBID_SLAVE_PCNOC_S_5                       189
#define ICBID_SLAVE_QSPI                            190
#define ICBID_SLAVE_QSPI_0                          ICBID_SLAVE_QSPI
#define ICBID_SLAVE_A1NOC_MS_MPU_CFG                191
#define ICBID_SLAVE_A2NOC_MS_MPU_CFG                192
#define ICBID_SLAVE_MODEM_Q6_SMMU_CFG               193
#define ICBID_SLAVE_MSS_MPU_CFG                     194
#define ICBID_SLAVE_MSS_PROC_MS_MPU_CFG             195
#define ICBID_SLAVE_SKL                             196
#define ICBID_SLAVE_SNOC_INT_2                      197
#define ICBID_SLAVE_SMMNOC_BIMC                     198
#define ICBID_SLAVE_CRVIRT_A1NOC                    199
#define ICBID_SLAVE_SGMII                           200
#define ICBID_SLAVE_QHS4_APPS                       201
#define ICBID_SLAVE_BIMC_PCNOC                      202
#define ICBID_SLAVE_PCNOC_BIMC_1                    203
#define ICBID_SLAVE_SPMI_FETCHER                    204
#define ICBID_SLAVE_MMSS_SMMU_CFG                   205
#define ICBID_SLAVE_WLAN                            206
#define ICBID_SLAVE_CRVIRT_A2NOC                    207
#define ICBID_SLAVE_CNOC_A2NOC                      208
#define ICBID_SLAVE_GLM                             209
#define ICBID_SLAVE_GNOC_BIMC                       210
#define ICBID_SLAVE_GNOC_MEM_NOC                    ICBID_SLAVE_GNOC_BIMC
#define ICBID_SLAVE_GNOC_SNOC                       211
#define ICBID_SLAVE_QM_CFG                          212
#define ICBID_SLAVE_TLMM_EAST                       213
#define ICBID_SLAVE_TLMM_NORTH                      214
#define ICBID_SLAVE_TLMM_WEST                       215
#define ICBID_SLAVE_TLMM_SOUTH                      216
#define ICBID_SLAVE_SPSS_CFG                        217
#define ICBID_SLAVE_AOP                             218
#define ICBID_SLAVE_AOSS                            219
#define ICBID_SLAVE_CDSP_CFG                        220
#define ICBID_SLAVE_CNOC_DDRSS                      221
#define ICBID_SLAVE_DDRSS_CFG                       222
#define ICBID_SLAVE_DDRSS_XPU_CFG                   223
#define ICBID_SLAVE_LLCC_CFG                        224
#define ICBID_SLAVE_MC_CFG                          225
#define ICBID_SLAVE_MCCC                            226
#define ICBID_SLAVE_MCCC_MASTER                     227
#define ICBID_SLAVE_MEM_NOC_CFG                     228
#define ICBID_SLAVE_GEM_NOC_CFG                     ICBID_SLAVE_MEM_NOC_CFG
#define ICBID_SLAVE_SOUTH_PHY_CFG                   229
#define ICBID_SLAVE_QM_MPU_CFG                      230
#define ICBID_SLAVE_QMIP_0_CFG                      231
#define ICBID_SLAVE_QMIP_1_CFG                      232
#define ICBID_SLAVE_QMIP_2_CFG                      233
#define ICBID_SLAVE_SHRM_CFG                        234
#define ICBID_SLAVE_SHRM_MEM                        235
#define ICBID_SLAVE_UFS_MEM_CFG                     236
#define ICBID_SLAVE_UFS_MEM_0_CFG                   ICBID_SLAVE_UFS_MEM_CFG
#define ICBID_SLAVE_VSENSE_CTRL_CFG                 237
#define ICBID_SLAVE_MNOC_SF_MEM_NOC                 238
#define ICBID_SLAVE_MEM_NOC_GNOC                    239
#define ICBID_SLAVE_CDC_SNOC_GC                     240
#define ICBID_SLAVE_CDC_SNOC_SF                     241
#define ICBID_SLAVE_CDC_TCU                         242
#define ICBID_SLAVE_LLCC                            243
#define ICBID_SLAVE_LPASS_SNOC                      244
#define ICBID_SLAVE_LPASS_GEM_NOC                   ICBID_SLAVE_LPASS_SNOC
#define ICBID_SLAVE_MEM_NOC_PCIE_SNOC               245
#define ICBID_SLAVE_GEM_NOC_PCIE_CNOC               ICBID_SLAVE_MEM_NOC_PCIE_SNOC
#define ICBID_SLAVE_ANOC_PCIE_SNOC                  246
#define ICBID_SLAVE_ANOC_PCIE_GEM_NOC               ICBID_SLAVE_ANOC_PCIE_SNOC
#define ICBID_SLAVE_CDSP_MEM_NOC                    247
#define ICBID_SLAVE_CDSP_GEM_NOC                    ICBID_SLAVE_CDSP_MEM_NOC
#define ICBID_SLAVE_BOOT_IMEM                       248
#define ICBID_SLAVE_CDSP_TCM                        249
#define ICBID_SLAVE_CDSP_HCP_VAPSS                  250
#define ICBID_SLAVE_CDSP_DMA_VAPSS                  251
#define ICBID_SLAVE_SERVICE_A1NOC                   252
#define ICBID_SLAVE_SERVICE_A2NOC                   253
#define ICBID_SLAVE_SERVICE_CDSP_NOC                254
#define ICBID_SLAVE_SERVICE_VAPSS_NOC               255
#define ICBID_SLAVE_SERVICE_DC_NOC                  256
#define ICBID_SLAVE_SERVICE_GNOC                    257
#define ICBID_SLAVE_SERVICE_MEM_NOC                 258
#define ICBID_SLAVE_SERVICE_GEM_NOC                 ICBID_SLAVE_SERVICE_MEM_NOC
#define ICBID_SLAVE_ANOC_IPA                        259
#define ICBID_SLAVE_ECC_CFG                         260
#define ICBID_SLAVE_MCDMA_MS_MPU_CFG                261
#define ICBID_SLAVE_PCNOC_QHS6                      262
#define ICBID_SLAVE_SNOC_QHS0                       263
#define ICBID_SLAVE_IPA_CORE                        264
#define ICBID_SLAVE_CAMNOC_UNCOMP                   265
#define ICBID_SLAVE_ANOC_PCIE_A1NOC_SNOC            266
#define ICBID_SLAVE_CCD                             267
#define ICBID_SLAVE_SENSORSS_CCD                    ICBID_SLAVE_CCD
#define ICBID_SLAVE_LPASS_CORE                      268
#define ICBID_SLAVE_MCC                             269
#define ICBID_SLAVE_SENSORSS_MCC                    ICBID_SLAVE_MCC
#define ICBID_SLAVE_PRAM                            270
#define ICBID_SLAVE_SENSORSS_PRAM                   ICBID_SLAVE_PRAM
#define ICBID_SLAVE_Q6                              271
#define ICBID_SLAVE_SENSORSS_PROC                   ICBID_SLAVE_Q6
#define ICBID_SLAVE_LPASS_PROC                      ICBID_SLAVE_Q6
#define ICBID_SLAVE_Q6_MPU_CFG                      272
#define ICBID_SLAVE_SENSORSS_Q6_MPU_CFG             ICBID_SLAVE_Q6_MPU_CFG
#define ICBID_SLAVE_QTIMER                          273
#define ICBID_SLAVE_SENSORSS_QTIMER                 ICBID_SLAVE_QTIMER
#define ICBID_SLAVE_QUPV3                           274
#define ICBID_SLAVE_SENSORSS_QUPV3                  ICBID_SLAVE_QUPV3
#define ICBID_SLAVE_RSCC                            275
#define ICBID_SLAVE_SENSORSS_RSCC                   ICBID_SLAVE_RSCC
#define ICBID_SLAVE_SDC                             276
#define ICBID_SLAVE_SDC_MPU_CFG                     277
#define ICBID_SLAVE_VOLTAGE_SENSOR_CX               278
#define ICBID_SLAVE_SENSORSS_VOLTAGE_SENSOR_CX      ICBID_SLAVE_VOLTAGE_SENSOR_CX
#define ICBID_SLAVE_VOLTAGE_SENSOR_MX               279
#define ICBID_SLAVE_SENSORSS_VOLTAGE_SENSOR_MX      ICBID_SLAVE_VOLTAGE_SENSOR_MX
#define ICBID_SLAVE_LPASS_ANOC                      280
#define ICBID_SLAVE_SERVIC_SSC_NOC                  281
#define ICBID_SLAVE_SERVICE_SSC_NOC                 ICBID_SLAVE_SERVIC_SSC_NOC
#define ICBID_SLAVE_SERVIC_SSC_LPASS_CORE           282
#define ICBID_SLAVE_SERVICE_SSC_LPASS_CORE          ICBID_SLAVE_SERVIC_SSC_LPASS_CORE
#define ICBID_SLAVE_DEBUG                           283
#define ICBID_SLAVE_SENSORSS_DEBUG                  ICBID_SLAVE_DEBUG
#define ICBID_SLAVE_AHB2PHY_SOUTH                   284
#define ICBID_SLAVE_CAMERA_NRT_THROTTLE_CFG         285
#define ICBID_SLAVE_RBCPR_MMCX_CFG                  286
#define ICBID_SLAVE_NPU_CFG                         287
#define ICBID_SLAVE_NORTH_PHY_CFG                   288
#define ICBID_SLAVE_VENUS_VCODEC_THROTTLE_CFG       289
#define ICBID_SLAVE_SHRM_MPU_CFG                    290
#define ICBID_SLAVE_ECC                             291
#define ICBID_SLAVE_SSC_NOC_ANOC                    292
#define ICBID_SLAVE_QUP_0                           293
#define ICBID_SLAVE_EMMC_CFG                        294
#define ICBID_SLAVE_SERVICE_GEM_NOC_1               295
#define ICBID_SLAVE_PCIE_3                          296
#define ICBID_SLAVE_PCIE_3_CFG                      297
#define ICBID_SLAVE_AHB2PHY_CENTER                  298
#define ICBID_SLAVE_AHB2PHY_EAST                    299
#define ICBID_SLAVE_AHB2PHY_WEST                    300
#define ICBID_SLAVE_QSPI_1                          301
#define ICBID_SLAVE_UFS_MEM_1_CFG                   302
#define ICBID_SLAVE_USB3_2                          303
#define ICBID_SLAVE_DC_NOC_GEMNOC                   304
#define ICBID_SLAVE_ANOC_IPA_MS_MPU_CFG             305
#define ICBID_SLAVE_ANOC_MS_MPU_CFG                 306
#define ICBID_SLAVE_SPMI_VGI_COEX                   307
#define ICBID_SLAVE_PCNOC_QHS4                      308
#define ICBID_SLAVE_EMAC_AVB_CFG                    309
#define ICBID_SLAVE_PCIE_CFG                        310
#define ICBID_SLAVE_USB2                            311
#define ICBID_SLAVE_QUP_CORE_1                      312
#define ICBID_SLAVE_QUP_CORE_2                      313
#define ICBID_SLAVE_SERVICE_GEM_NOC_2               314
#define ICBID_SLAVE_LPASS_MPU_CFG                   315
#define ICBID_SLAVE_LPASS_CORE_CFG                  316
#define ICBID_SLAVE_LPASS_LPI_CFG                   317
#define ICBID_SLAVE_LPASS_TOP_CFG                   318
#define ICBID_SLAVE_SERVICE_LPASS_AG_NOC            319
#define ICBID_SLAVE_AHB2PHY_NORTH                   320
#define ICBID_SLAVE_ANOC_PCIE_GEM_NOC_1             321
#define ICBID_SLAVE_CX_RDPM                         322
#define ICBID_SLAVE_IPC_ROUTER_CFG                  323
#define ICBID_SLAVE_ISENSE_CFG                      324
#define ICBID_SLAVE_NPU_CAL_DP0                     325
#define ICBID_SLAVE_NPU_CAL_DP1                     326
#define ICBID_SLAVE_NPU_COMPUTE_NOC                 327
#define ICBID_SLAVE_NPU_CP                          328
#define ICBID_SLAVE_NPU_DPM                         329
#define ICBID_SLAVE_NPU_INT_DMA_BWMON_CFG           330
#define ICBID_SLAVE_NPU_LLM_CFG                     331
#define ICBID_SLAVE_NPU_TCM                         332
#define ICBID_SLAVE_SERVICE_NPU_NOC                 333
#define ICBID_SLAVE_CDSP_NOC_A2NOC_COMP_NOC         334
#define ICBID_SLAVE_DISPLAY_RT_THROTTLE_CFG         335
#define ICBID_SLAVE_QUP_CORE_0                      336
#define ICBID_SLAVE_DDRSS_TG0                       337
#define ICBID_SLAVE_DDRSS_TG1                       338
#define ICBID_SLAVE_DPCC                            339
#define ICBID_SLAVE_LAGG_CFG                        340
#define ICBID_SLAVE_NPU_DMA_BWMON_CFG               341
#define ICBID_SLAVE_NPU_PROC_BWMON_CFG              342
#define ICBID_SLAVE_PCIE_THROTTLE_CFG               343
#define ICBID_SLAVE_SNOC_THROTTLE_CFG               344
#define ICBID_SLAVE_SNOOP_BWMON                     345
#define ICBID_SLAVE_BOOT_IMEM_MPU_CFG               346
#define ICBID_SLAVE_GEM_NOC_CNOC                    347
#define ICBID_SLAVE_LPASS_TCM                       348
#define ICBID_SLAVE_MCDMA_THROTTLE_CFG              349
#define ICBID_SLAVE_MSS_PROC_THROTTLE_CFG           350
#define ICBID_SLAVE_MX_RDPM                         351
#define ICBID_SLAVE_NSP_HCP_THROTTLE_CFG            352
#define ICBID_SLAVE_PKA_WRAPPER_CFG                 353
#define ICBID_SLAVE_PMU_WRAPPER_CFG                 354
#define ICBID_SLAVE_SERVICES_LPASS_AML_NOC          355
#define ICBID_SLAVE_SERVICE_NSP_NOC                 356
#define ICBID_SLAVE_AHB2PHY_0                       357
#define ICBID_SLAVE_AHB2PHY_1                       358
#define ICBID_SLAVE_AHB2PHY_2                       359
#define ICBID_SLAVE_PKA_CORE                        360
#define ICBID_SLAVE_HWKM_CORE                       361
#define ICBID_SLAVE_CDSP1_CFG                       362
#define ICBID_SLAVE_CPR_NSPCX                       363
#define ICBID_SLAVE_DISPLAY1_CFG                    364
#define ICBID_SLAVE_DISPLAY1_THROTTLE_CFG           365
#define ICBID_SLAVE_DISPLAY1_RT_THROTTLE_CFG        366
#define ICBID_SLAVE_EMAC1_CFG                       367
#define ICBID_SLAVE_GIC_MS_MPU_CFG                  368
#define ICBID_SLAVE_GPU_TCU_THROTTLE_CFG            369
#define ICBID_SLAVE_LPASS_THROTTLE_CFG              370
#define ICBID_SLAVE_MXC_RDPM                        371
#define ICBID_SLAVE_PCIE_2A_CFG                     372
#define ICBID_SLAVE_PCIE_2B_CFG                     373
#define ICBID_SLAVE_PCIE_3A_CFG                     374
#define ICBID_SLAVE_PCIE_3B_CFG                     375
#define ICBID_SLAVE_PCIE_4_CFG                      376
#define ICBID_SLAVE_PCIE_RSC_CFG                    377
#define ICBID_SLAVE_PCIE_TCU_THROTTLE_CFG           378
#define ICBID_SLAVE_SMMUV3_CFG                      379
#define ICBID_SLAVE_SMSS_CFG                        380
#define ICBID_SLAVE_SMSS                            381
#define ICBID_SLAVE_USB3_MP                         382
#define ICBID_SLAVE_USB4_0                          383
#define ICBID_SLAVE_USB4_1                          384
#define ICBID_SLAVE_VSENSE_CTRL_R_CFG               385
#define ICBID_SLAVE_USB_NOC_SNOC                    386
#define ICBID_SLAVE_NSP_XFR                         387
#define ICBID_SLAVE_DDRSS_DPM                       388
#define ICBID_SLAVE_DDRSS_TG2                       389
#define ICBID_SLAVE_DDRSS_TG3                       390
#define ICBID_SLAVE_ANOC_PCIE_BRIDGE_CFG            391
#define ICBID_SLAVE_SNOC_SF_BRIDGE_CFG              392
#define ICBID_SLAVE_CDSPB_MEM_NOC                   393
#define ICBID_SLAVE_NSPB_XFR                        394
#define ICBID_SLAVE_RBCPR_MXC_CFG                   395
#define ICBID_SLAVE_PCIE_2A                         396
#define ICBID_SLAVE_PCIE_2B                         397
#define ICBID_SLAVE_PCIE_3A                         398
#define ICBID_SLAVE_PCIE_3B                         399
#define ICBID_SLAVE_PCIE_4                          400
#define ICBID_SLAVE_SERVICE_NSPB_NOC                401
#define ICBID_SLAVE_QMIP_3_CFG                      402
#define ICBID_SLAVE_CDSPB_NOC_CFG                   403
#define ICBID_SLAVE_GEMNOC_MS_MPU_CFG               404
#define ICBID_SLAVE_ANOC_THROTTLE_CFG               405
#define ICBID_SLAVE_AONCAM_MPU_CFG                  406
#define ICBID_SLAVE_PCIE_MS_MPU_CFG                 407
#define ICBID_SLAVE_GPU_THROTTLE_CFG                408
#define ICBID_SLAVE_SENSORSS_ICB                    409
#define ICBID_SLAVE_MCC_MPU_CFG                     410
#define ICBID_SLAVE_SENSORSS_QUPV3_MS_MPU_CFG       411
#define ICBID_SLAVE_SHRM_2_CFG                      412
#define ICBID_SLAVE_SHRM_2_MEM                      413
#define ICBID_SLAVE_SENSORSS_AON_CAM_CFG            414
#define ICBID_SLAVE_LPASS_AON_CAM_CFG               ICBID_SLAVE_SENSORSS_AON_CAM_CFG
#define ICBID_SLAVE_TME_CFG                         415
#define ICBID_SLAVE_VENUS_CV_CPU_THROTTLE_CFG       416
#define ICBID_SLAVE_VENUS_V_CPU_THROTTLE_CFG        417
#define ICBID_SLAVE_PCIE_ANOC_CFG                   418
#define ICBID_SLAVE_DDRSS_AHB2PHY_0                 419
#define ICBID_SLAVE_DDRSS_AHB2PHY_1                 420
#define ICBID_SLAVE_DDRSS_AHB2PHY_2                 421
#define ICBID_SLAVE_DDRSS_AHB2PHY_3                 422
#define ICBID_SLAVE_DDRSS_EVEN                      423
#define ICBID_SLAVE_DDRSS_ODD                       424
#define ICBID_SLAVE_DDRSS_LLCC_0                    425
#define ICBID_SLAVE_DDRSS_LLCC_1                    426
#define ICBID_SLAVE_DDRSS_LLCC_2                    427
#define ICBID_SLAVE_DDRSS_LLCC_3                    428
#define ICBID_SLAVE_DDRSS_MC_0                      429
#define ICBID_SLAVE_DDRSS_MC_1                      430
#define ICBID_SLAVE_DDRSS_MC_2                      431
#define ICBID_SLAVE_DDRSS_MC_3                      432
#define ICBID_SLAVE_DDRSS_MCCC_0                    433
#define ICBID_SLAVE_DDRSS_MCCC_1                    434
#define ICBID_SLAVE_DDRSS_MCCC_2                    435
#define ICBID_SLAVE_DDRSS_MCCC_3                    436
#define ICBID_SLAVE_SENSORSS_Q6AXI                  437
#define ICBID_SLAVE_SERVICE_DC_NOC_EVEN             438
#define ICBID_SLAVE_SERVICE_DC_NOC_ODD              439
#define ICBID_SLAVE_SERVICE_PCIE_ANOC               440
#define ICBID_SLAVE_QPIC_CORE                       441
#define ICBID_SLAVE_WLAN_Q6_CFG                     442
#define ICBID_SLAVE_WLAN_Q6_THROTTLE_CFG            443
#define ICBID_SLAVE_CNOC2_CNOC3                     444
#define ICBID_SLAVE_CNOC3_CNOC2                     445
#define ICBID_SLAVE_DDR_PERF_MODE                   446
#define ICBID_SLAVE_I2C                             447
#define ICBID_SLAVE_CNOC_CFG                        448
#define ICBID_SLAVE_DDRSS_REGS                      449
#define ICBID_SLAVE_SERVICE_CNOC_CFG                450
#define ICBID_SLAVE_LLCC_BACKDOOR                   451
#define ICBID_SLAVE_ANOC2_THROTTLE_CFG              452
#define ICBID_SLAVE_LB_0_CFG                        453
#define ICBID_SLAVE_LB_1_CFG                        454
#define ICBID_SLAVE_LB_2_CFG                        455
#define ICBID_SLAVE_LB_3_CFG                        456
#define ICBID_SLAVE_LPASS_MS_MPU_CFG                457
#define ICBID_SLAVE_LPASS_AON_CFG                   458
#define ICBID_SLAVE_LPASS_LPI_CC                    459
#define ICBID_SLAVE_LPASS_LPI_PDC                   460
#define ICBID_SLAVE_LPASS_LPI_VA                    461
#define ICBID_SLAVE_ENPU_0_CFG                      462
#define ICBID_SLAVE_ENPU_1_CFG                      463
#define ICBID_SLAVE_LPASS_HS_MPU_CFG                464
#define ICBID_SLAVE_LPASS_LS_MPU_CFG                465
#define ICBID_SLAVE_CNOC_MPU_CFG                    466
#define ICBID_SLAVE_LPIAON_NOC_LLCLPI_NOC           467
#define ICBID_SLAVE_LPASS_LPIAON_NOC_CFG            468
#define ICBID_SLAVE_LPIAON_NOC_LPASS_AG_NOC         469
#define ICBID_SLAVE_LPASS_AG_NOC_LPASS_LPICX_NOC    470
#define ICBID_SLAVE_LPICX_NOC_LPIAON_NOC            471
#define ICBID_SLAVE_LLCLPI_NOC_CFG                  472
#define ICBID_SLAVE_LPASS_QTB_CFG                   473
#define ICBID_SLAVE_NSP_QTB_CFG                     474
#define ICBID_SLAVE_SERVICE_CHIPCX_LPASS_LPIAON_NOC 475
#define ICBID_SLAVE_SERVICE_LLCLPI_NOC              476
#define ICBID_SLAVE_SERVICE_LLCLPI_NOC_CHIPCX       477
#define ICBID_SLAVE_SERVICE_LPICX_LPASS_LPIAON_NOC  478
#define ICBID_SLAVE_SERVICE_LPINOC_CHIPCX           479
#define ICBID_SLAVE_SERVICE_LPINOC_LPICX            480
#define ICBID_SLAVE_CNOC_MNOC_HF_CFG                481
#define ICBID_SLAVE_CNOC_MNOC_SF_CFG                482
#define ICBID_SLAVE_Q6NOC_GEMNOC                    483
#define ICBID_SLAVE_SERVICE_DC_NOC_CFG              484
#define ICBID_SLAVE_SERVICE_Q6NOC                   485
#define ICBID_SLAVE_SERVICE_MNOC_HF                 486
#define ICBID_SLAVE_SERVICE_MNOC_SF                 487
#define ICBID_SLAVE_COUNT                           488
#define ICBID_SLAVE_NONE                            ICBID_SLAVE_COUNT


/*
 * Arbitration Request Types
 * Refer icbid.h to configure new arbitration request types
 */
#define ICBARB_REQUEST_TYPE_1              0x1  /**< "Bursty" master */
#define ICBARB_REQUEST_TYPE_2              0x2  /**< CPU-style master */
#define ICBARB_REQUEST_TYPE_3              0x4  /**< Ib/Ab pair */

#endif