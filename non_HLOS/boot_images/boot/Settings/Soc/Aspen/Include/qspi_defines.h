#ifndef __QSPI_DEFINES_H__
#define __QSPI_DEFINES_H__


/*=============================================================================   
    @file  qspi_defines.h
    @brief interface to device configuration
   
    Copyright (c) Qualcomm Technologies, Inc.
    All Rights Reserved.
    Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/

/*=============================================================================
                              EDIT HISTORY
 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 08/19/25   AY      Added file for aspen
=============================================================================*/

#define TLMM_GPIO_COFG_EX(gpio, func, dir, pull, drive) \
                                      (((gpio) & 0x3FF) << 4  | \
                                       ((func) & 0xF  ) << 0  | \
                                       ((dir)  & 0x1  ) << 14 | \
                                       ((pull) & 0x3  ) << 15 | \
                                       ((drive)& 0xF  ) << 17 )

#define TLMM_GPIO_INPUT             0x0
#define TLMM_GPIO_OUTPUT            0x1

#define TLMM_GPIO_NO_PULL           0x0
#define TLMM_GPIO_PULL_DOWN         0x1
#define TLMM_GPIO_KEEPER            0x2
#define TLMM_GPIO_PULL_UP           0x3

#define TLMM_GPIO_2MA               0x0
#define TLMM_GPIO_4MA               0x1
#define TLMM_GPIO_6MA               0x2
#define TLMM_GPIO_8MA               0x3
#define TLMM_GPIO_10MA              0x4
#define TLMM_GPIO_12MA              0x5
#define TLMM_GPIO_14MA              0x6
#define TLMM_GPIO_16MA              0x7

#define QSPI_CLK    TLMM_GPIO_COFG_EX(83,  4, TLMM_GPIO_INPUT,  TLMM_GPIO_NO_PULL,   TLMM_GPIO_8MA)
#define QSPI_CS_0   TLMM_GPIO_COFG_EX(146, 5, TLMM_GPIO_OUTPUT, TLMM_GPIO_PULL_UP,   TLMM_GPIO_8MA)
#define QSPI_CS_1   TLMM_GPIO_COFG_EX(148, 4, TLMM_GPIO_OUTPUT, TLMM_GPIO_PULL_UP,   TLMM_GPIO_8MA)
#define QSPI_DATA_0 TLMM_GPIO_COFG_EX(80,  3, TLMM_GPIO_INPUT,  TLMM_GPIO_PULL_DOWN, TLMM_GPIO_8MA)
#define QSPI_DATA_1 TLMM_GPIO_COFG_EX(147, 4, TLMM_GPIO_INPUT,  TLMM_GPIO_PULL_DOWN, TLMM_GPIO_8MA)
#define QSPI_DATA_2 TLMM_GPIO_COFG_EX(81,  3, TLMM_GPIO_INPUT,  TLMM_GPIO_PULL_DOWN, TLMM_GPIO_8MA)
#define QSPI_DATA_3 TLMM_GPIO_COFG_EX(82,  4, TLMM_GPIO_INPUT,  TLMM_GPIO_PULL_DOWN, TLMM_GPIO_8MA)

#endif
