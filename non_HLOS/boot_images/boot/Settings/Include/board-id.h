#ifndef __BOARD_ID_DEFINES_H__
#define __BOARD_ID_DEFINES_H__

/*=============================================================================
@file  board-id.h
@brief defines the default values for board-id node.

Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
All rights reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/

#define NORD_CHIP_FAMILY       0x009C
#define KAANAPALI_CHIP_FAMILY  0x00A0
#define GLYMUR_CHIP_FAMILY     0x00A1
#define BALSAM_CHIP_FAMILY     0x00A3
#define ASPEN_CHIP_FAMILY      0x00A4
#define MOLOKAI_CHIP_FAMILY    0x00A7
#define MAHUA_CHIP_FAMILY      0x00A8
#define BONSAI_CHIP_FAMILY     0x00A9 //ADDED Dummy for compilation support
#define SECA_CHIP_FAMILY       0x00AF
#define MASKED_VAL_ZERO        0x0000
#define MAJOR_VERSION_01       0x0001
#define MINOR_VERSION_00       0x0000

#define BOARD_TYPE_QRD    0x0B
#define BOARD_TYPE_ATP    0x21
#define BOARD_TYPE_HDK    0x1F
#define BOARD_TYPE_WDP    0x24
#define BOARD_TYPE_IDP    0x22
#define DEFAULT_SUB_TYPE  0x00
#define SUB_TYPE_2        0x02
#endif
