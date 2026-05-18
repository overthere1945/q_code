/**
 * @file pcie_defines.h
 *
 * This header file defines macros for use in PCIe Device Tree.
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#ifndef __PCIE_DEFINES_H__
#define __PCIE_DEFINES_H__

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define PCIE_LINK_SPEED_1  1
#define PCIE_LINK_SPEED_2  2
#define PCIE_LINK_SPEED_3  3
#define PCIE_LINK_SPEED_4  4
#define PCIE_LINK_SPEED_5  5

#define PCIE_LINK_WIDTH_1   1
#define PCIE_LINK_WIDTH_2   2
#define PCIE_LINK_WIDTH_4   4
#define PCIE_LINK_WIDTH_8   8
#define PCIE_LINK_WIDTH_16  16

#define PCIE_MAX_PAYLOAD_SIZE_128   0
#define PCIE_MAX_PAYLOAD_SIZE_256   1
#define PCIE_MAX_PAYLOAD_SIZE_512   2
#define PCIE_MAX_PAYLOAD_SIZE_1024  3
#define PCIE_MAX_PAYLOAD_SIZE_2048  4
#define PCIE_MAX_PAYLOAD_SIZE_4096  5

#define PCIE_POWER_STATE_D0  0
#define PCIE_POWER_STATE_D3  3

#endif
