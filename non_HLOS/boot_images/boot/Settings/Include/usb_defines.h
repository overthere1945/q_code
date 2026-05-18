/** @file usb_defines.h

**/
//============================================================================
/**
  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries. All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
//============================================================================

/*=============================================================================
                              EDIT HISTORY

 when         who        what, where, why
 --------   -------      ----------------------------------------------------------
 12/22/25   vgondipa     Init Check-in
=============================================================================*/

#ifndef __USB_SW_DEFINES_H__
#define __USB_SW_DEFINES_H__

#define FEATURE_QC_USB_WOL_EN 1

#define FREQ_MHZ(f)     (f * 1000 * 1000)
#define FREQ_KHZ(f)     (f * 1000)
#define QUSB_DCI_PMI_INDEX                    (7)   /*For PMIC_D*/

/*
 * USB 3.0 Clock definitions
 */
#define USB3_MASTER_CLK_MIN_FREQ_HZ           FREQ_MHZ(120)
#define USB20_MASTER_CLK_MIN_FREQ_HZ 	      FREQ_MHZ(120)
#define USB3_AXI_CLK_MIN_FREQ_HZ              FREQ_MHZ(120)
#define USB3_PHY_AUX_CLK_MIN_FREQ_HZ          FREQ_KHZ(19200)
// Mock CLK needs to be set at 19.2 Mhz
#define USB3_MOCK_CLK_MIN_FREQ_HZ             FREQ_KHZ(19200)
// Vote for no frequency
#define USB3_NOC_CLK_MIN_FREQ_HZ              FREQ_MHZ(0)

#endif //__USB_SW_DEFINES_H__