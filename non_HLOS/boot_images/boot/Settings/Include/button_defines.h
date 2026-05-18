#ifndef __BUTTON_DEFINES_H__
#define __BUTTON_DEFINES_H__

/*===========================================================================
                Buttons config DT Header File

GENERAL DESCRIPTION
  This header file contains declarations and definitions for buttons gpio and
  platform key map settings.
    
Copyright (c) Qualcomm Technologies, Inc. All rights reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when          who     what, where, why
----------    ---     ----------------------------------------------------------
10/08/2022    gan    Initial revision.
02/06/2023	  samakash Button enable
============================================================================*/

// GPIO Types
#define TLMM_GPIO       0
#define PMIC_GPIO       1

// Button Indexs
#define VOL_UP_BTN  0
#define HOME_BTN    1
#define CAMERA_BTN  2
#define VOL_DN_BTN  4

// Platform Key map Button Types
#define RESERVED    0x0
#define PWR         0x1
#define VOL_UP      0x2
#define VOL_DOWN    0x3

// source Pull Up
#define BUTTON_GPIO_PULL_UP_30uA          0x0
#define BUTTON_GPIO_NO_PULL               0x0
#define BUTTON_GPIO_PULL_DOWN		      0x1
#define BUTTON_GPIO_PULL_UP		0x3

// Voltage Source
#define BUTTON_GPIO_VIN0                  0x0
#define BUTTON_GPIO_VIN1                  0x1
#define BUTTON_GPIO_INPUT                 0x0

// Out Buffer Strength
#define BUTTON_GPIO_OUT_DRV_STR_LOW       0x0
#define BUTTON_GPIO_2MA                   0x0
#define BUTTON_GPIO_6MA                   0x3

// Source
#define BUTTON_GPIO_SRC_GND               0x0

//GPIOs
#define TLMM_GPIO_69                      69
#define TLMM_GPIO_82                      82
#define TLMM_GPIO_48                      48
#define TLMM_GPIO_101                     101
#define TLMM_GPIO_136                     136

#endif /*__BUTTON_DEFINES_H__*/
