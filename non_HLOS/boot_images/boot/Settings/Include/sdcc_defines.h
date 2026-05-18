/******************************************************************************
 * sdcc_defines.h
 *
 * This file provides SDCC macro definitions for DT
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
2024-03-21   kg      Initial Version
=============================================================================*/

#define SDCC_INVALID_SPEED    0
#define SDCC_SDR12_MODE       1
#define SDCC_DS_MODE          SDCC_SDR12_MODE
#define SDCC_SDR25_MODE       2
#define SDCC_HS_MODE          SDCC_SDR25_MODE
#define SDCC_SDR50_MODE       3
#define SDCC_DDR50_MODE       4
#define SDCC_HS200_MODE       5
#define SDCC_SDR104_MODE      SDCC_HS200_MODE
#define SDCC_HS400_MODE       6

//Maintain in syc with SdccBsp.h
#define SDCC_BSP_CLK_INVALID 0
#define SDCC_BSP_CLK_144_KHZ 144000
#define SDCC_BSP_CLK_400_KHZ 400000
#define SDCC_BSP_CLK_20_MHZ  20000000
#define SDCC_BSP_CLK_24_MHZ  24000000
#define SDCC_BSP_CLK_25_MHZ  25000000
#define SDCC_BSP_CLK_37_MHZ  37500000
#define SDCC_BSP_CLK_50_MHZ  50000000
#define SDCC_BSP_CLK_75_MHZ  75000000
#define SDCC_BSP_CLK_100_MHZ 100000000
#define SDCC_BSP_CLK_148_MHZ 148000000
#define SDCC_BSP_CLK_177_MHZ 177000000
#define SDCC_BSP_CLK_192_MHZ 192000000
#define SDCC_BSP_CLK_200_MHZ 200000000
#define SDCC_BSP_CLK_202_MHZ 202000000
#define SDCC_BSP_CLK_384_MHZ 384000000