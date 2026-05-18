#ifndef __PM_DEFINES_H__
#define __PM_DEFINES_H__

/*===========================================================================

                 Boot Loader Error Handler Header File

GENERAL DESCRIPTION
  This header file contains declarations and definitions for boot sw settings.
    
Copyright (c) Qualcomm Technologies, Inc. All rights reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/13/20   alal    Initial revision.

============================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
/*=========================================================================
                Generic PMIC MACROS
===========================================================================*/

/*************WARNING********************************************

***Below section needs to match with pm_dt.h ****

*********WARNING************************************************/

//LDO
#define PM_LDO_SET_ENABLE        0
#define PM_LDO_SET_VOLT          1
#define PM_LDO_SET_MODE          2
#define PM_LDO_SET_PD_CTRL       3
#define PM_LDO_SET_PIN_CTRL      4
#define PM_LDO_SET_OCP_BROADCAST 5
#define PM_LDO_SET_AHC           6
#define PM_LDO_LPM_OCP_RESP	     7
#define PM_LDO_OCP_RESP_CFG      8
#define PM_LDO_SET_ULS           9

//SMPS
#define PM_SMPS_SET_ENABLE        20
#define PM_SMPS_SET_VOLT          21
#define PM_SMPS_SET_MODE          22
#define PM_SMPS_SET_PD_CTRL       23
#define PM_SMPS_SET_PIN_CTRL      24
#define PM_SMPS_SET_OCP_BROADCAST 25
#define PM_SMPS_SET_AHC           26
#define PM_SMPS_SET_AHC_HR        27
#define PM_SMPS_SET_ULS           28

//BOB
#define PM_BOB_SET_ENABLE         40
#define PM_BOB_SET_VOLT           41
#define PM_BOB_SET_MODE           42

//GPIO  
#define PM_GPIO_SET_ENABLE          60
#define PM_GPIO_SET_CFG_MODE        61
#define PM_GPIO_SET_OUTPUT_LVL      62
#define PM_GPIO_SET_VOLT_SRC        63
#define PM_GPIO_SET_OUT_BUFF_CONFIG 64
#define PM_GPIO_SET_OUT_DRV_STR     65
#define PM_GPIO_SET_OUT_SRC_CFG     66
#define PM_GPIO_SET_PULL_SEL        67  

//CLK
#define PM_CLK_ENABLE         80
#define PM_CLK_DRV_STR        81
#define PM_CLK_XO_MODE        82
#define PM_CLK_XO_TRIM        83

//MISC  
#define PM_SET_DELAY                 100
#define PM_MASTER_SPMI_CLK_BUF_CFG   101
#define PM_MASTER2_SPMI_CLK_BUF_CFG  102
#define PM_SPMI_CLK_BUF_CFG          103
#define PM_SPMI2_CLK_BUF_CFG         104
#define PM_PRINT_REG                 105



//EPM
#define PM_EPM_EN            130
#define PM_EPM_MODE          131
#define PM_EPM_CONFIG        132
#define PM_EPM_DEFAULT_MODE  133
/************WARNING END************/


/*=========================================================================
                SPMI/eSPMI BUS MACROS
===========================================================================*/
#define PM_SPMI_BUS_0 (0)
#define PM_SPMI_BUS_1 (1)
#define PM_SPMI_BUS_2 (2)
#define PM_SPMI_BUS_3 (3)

#define PM_ESPMI_BUSID_0  PM_SPMI_BUS_0
#define PM_ESPMI_BUSID_1  PM_SPMI_BUS_1
#define PM_ESPMI_BUSID_2  PM_SPMI_BUS_2
#define PM_ESPMI_BUSID_3  PM_SPMI_BUS_3

#define PM_ENABLE  (0x1)
#define PM_DISABLE (0x0)
#define PM_TRUE   (1)
#define PM_FALSE  (0)

#define PM_OFF (0x0)
#define PM_ON  (0x1)

#define PMIC_A (0)
#define PMIC_B (1)
#define PMIC_C (2)
#define PMIC_D (3)
#define PMIC_E (4)
#define PMIC_F (5)
#define PMIC_G (6)
#define PMIC_H (7)
#define PMIC_I (8)
#define PMIC_J (9)
#define PMIC_K (10)
#define PMIC_L (11)
#define PMIC_M (12)
#define PMIC_N (13)
#define PMIC_O (14)

#define PMIC_A_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_A)
#define PMIC_B_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_B)
#define PMIC_C_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_C)
#define PMIC_D_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_D)
#define PMIC_E_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_E)
#define PMIC_F_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_F)
#define PMIC_G_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_G)
#define PMIC_H_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_H)
#define PMIC_I_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_I)
#define PMIC_J_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_J)
#define PMIC_K_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_K)
#define PMIC_L_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_L)
#define PMIC_M_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_M)
#define PMIC_N_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_N)
#define PMIC_O_E0 ((PM_ESPMI_BUSID_0 << 16) | PMIC_O)

#define PMIC_A_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_A)
#define PMIC_B_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_B)
#define PMIC_C_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_C)
#define PMIC_D_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_D)
#define PMIC_E_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_E)
#define PMIC_F_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_F)
#define PMIC_G_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_G)
#define PMIC_H_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_H)
#define PMIC_I_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_I)
#define PMIC_J_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_J)
#define PMIC_K_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_K)
#define PMIC_L_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_L)
#define PMIC_M_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_M)
#define PMIC_N_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_N)
#define PMIC_O_E1 ((PM_ESPMI_BUSID_1 << 16) | PMIC_O)

#define PMIC_A_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_A)
#define PMIC_B_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_B)
#define PMIC_C_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_C)
#define PMIC_D_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_D)
#define PMIC_E_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_E)
#define PMIC_F_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_F)
#define PMIC_G_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_G)
#define PMIC_H_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_H)
#define PMIC_I_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_I)
#define PMIC_J_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_J)
#define PMIC_K_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_K)
#define PMIC_L_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_L)
#define PMIC_M_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_M)
#define PMIC_N_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_N)
#define PMIC_O_E2 ((PM_ESPMI_BUSID_2 << 16) | PMIC_O)

#define PMIC_A_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_A)
#define PMIC_B_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_B)
#define PMIC_C_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_C)
#define PMIC_D_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_D)
#define PMIC_E_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_E)
#define PMIC_F_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_F)
#define PMIC_G_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_G)
#define PMIC_H_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_H)
#define PMIC_I_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_I)
#define PMIC_J_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_J)
#define PMIC_K_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_K)
#define PMIC_L_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_L)
#define PMIC_M_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_M)
#define PMIC_N_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_N)
#define PMIC_O_E3 ((PM_ESPMI_BUSID_3 << 16) | PMIC_O)

#define PMIC_INVALID 0xFF

#define PM_BUSID_0 (0)
#define PM_BUSID_1 (1)

#define PM_WARM_RESET  (0)
#define PM_HARD_RESET  (1)
#define PM_SHUTDOWN    (2)

#define PM_DELAY(x) PM_SET_DELAY 0 0 0 x

/*=========================================================================
                PMIC Clock MACROS
===========================================================================*/
#define  PM_CLK_SLEEP      (0)
#define  PM_CLK_XO         (1)
#define  PM_CLK_1          (2)
#define  PM_CLK_2          (3)
#define  PM_CLK_3          (4)
#define  PM_CLK_4          (5)
#define  PM_CLK_5          (6)
#define  PM_CLK_6          (7)
#define  PM_CLK_7          (8)
#define  PM_CLK_8          (9)
#define  PM_CLK_9          (10)
#define  PM_CLK_10         (11)
#define  PM_CLK_11         (12)
#define  PM_CLK_12         (13)
#define  PM_CLK_13         (14)
#define  PM_CLK_DIST       (15)
#define  PM_CLK_MAX_INDEX  (16)

#define  PM_CLK_RF_1       (PM_CLK_1)
#define  PM_CLK_RF_2       (PM_CLK_2)
#define  PM_CLK_RF_3       (PM_CLK_3)
#define  PM_CLK_RF_4       (PM_CLK_4)
#define  PM_CLK_RF_5       (PM_CLK_5)

#define  PM_CLK_BB_1       (PM_CLK_6)
#define  PM_CLK_BB_2       (PM_CLK_7)
#define  PM_CLK_BB_3       (PM_CLK_8)
#define  PM_CLK_BB_4       (PM_CLK_9)
#define  PM_CLK_BB_5       (PM_CLK_10)

#define  PM_CLK_DIV_1      (PM_CLK_11)
#define  PM_CLK_DIV_2      (PM_CLK_12)
#define  PM_CLK_DIV_3      (PM_CLK_13)

#define  PM_CLK_LN_BB      (PM_CLK_6)

#define  PM_CLK_LN_6       (PM_CLK_6)
#define  PM_CLK_LN_7       (PM_CLK_7)
#define  PM_CLK_LN_8       (PM_CLK_8)
#define  PM_CLK_LN_9       (PM_CLK_9)

#define  PM_ALL_CLKS       (PM_CLK_MAX_INDEX)

/*=========================================================================
                PON MACROS
===========================================================================*/
#define PM_PON_RESET_SOURCE_KPDPWR            (0)
#define PM_PON_RESET_SOURCE_RESIN             (1)
#define PM_PON_RESET_SOURCE_RESIN_AND_KPDPWR  (2)
#define PM_PON_RESET_SOURCE_RESIN_OR_KPDPWR   (3)


#define PON_EVENT_LOG_LEVEL_MIN (0)
#define PON_EVENT_LOG_LEVEL_VERBOSE (1)
#define PON_EVENT_LOG_LEVEL_RAWDATA (2)
#define PON_EVENT_LOG_LEVEL_MAX (0x7F)

/*=========================================================================
                RAILS MACROS
===========================================================================*/
#define PM_LDO_1  (0)
#define PM_LDO_2  (1)
#define PM_LDO_3  (2)
#define PM_LDO_4  (3)
#define PM_LDO_5  (4)
#define PM_LDO_6  (5)
#define PM_LDO_7  (6)
#define PM_LDO_8  (7)
#define PM_LDO_9  (8)
#define PM_LDO_10 (9)
#define PM_LDO_11 (10)
#define PM_LDO_12 (11)
#define PM_LDO_13 (12)
#define PM_LDO_14 (13)
#define PM_LDO_15 (14)
#define PM_LDO_16 (15)
#define PM_LDO_17 (16)
#define PM_LDO_18 (17)
#define PM_LDO_19 (18)
#define PM_LDO_20 (19)
#define PM_LDO_21 (20)
#define PM_LDO_22 (21)
#define PM_LDO_23 (22)
#define PM_LDO_24 (23)
#define PM_LDO_25 (24)
#define PM_LDO_26 (25)
#define PM_LDO_27 (26)
#define PM_LDO_28 (27)
#define PM_LDO_29 (28)
#define PM_LDO_30 (29)
#define PM_LDO_31 (30)
#define PM_LDO_32 (31)
#define PM_LDO_33 (32)

#define PM_SMPS_1  (0)
#define PM_SMPS_2  (1)
#define PM_SMPS_3  (2)
#define PM_SMPS_4  (3)
#define PM_SMPS_5  (4)
#define PM_SMPS_6  (5)
#define PM_SMPS_7  (6)
#define PM_SMPS_8  (7)
#define PM_SMPS_9  (8)
#define PM_SMPS_10 (9)
#define PM_SMPS_11 (10)
#define PM_SMPS_12 (11)
#define PM_SMPS_13 (12)
#define PM_SMPS_14 (13)
#define PM_SMPS_15 (14)

#define PM_BOB_1 (0)
#define PM_BOB_2 (1)
#define PM_BOB_3 (2)

#define PM_SW_MODE_LPM        (0)
#define PM_SW_MODE_BYPASS     (1)
#define PM_SW_MODE_AUTO       (2)
#define PM_SW_MODE_NPM        (3)
#define PM_SW_MODE_RETENTION  (4)

/*==================================================================
                GPIO MACROS
====================================================================*/
#define PM_GPIO_1  (0)
#define PM_GPIO_2  (1)
#define PM_GPIO_3  (2)
#define PM_GPIO_4  (3)
#define PM_GPIO_5  (4)
#define PM_GPIO_6  (5)
#define PM_GPIO_7  (6)
#define PM_GPIO_8  (7)
#define PM_GPIO_9  (8)
#define PM_GPIO_10 (9)
#define PM_GPIO_11 (10)
#define PM_GPIO_12 (11)
#define PM_GPIO_13 (12)
#define PM_GPIO_14 (13)
#define PM_GPIO_15 (14)
#define PM_GPIO_16 (15)
#define PM_GPIO_17 (16)
#define PM_GPIO_18 (17)
#define PM_GPIO_19 (18)
#define PM_GPIO_20 (19)
#define PM_GPIO_21 (20)
#define PM_GPIO_22 (21)
#define PM_GPIO_23 (22)
#define PM_GPIO_24 (23)
#define PM_GPIO_25 (24)
#define PM_GPIO_26 (25)
#define PM_GPIO_27 (26)
#define PM_GPIO_28 (27)
#define PM_GPIO_29 (28)
#define PM_GPIO_30 (29)
#define PM_GPIO_31 (30)
#define PM_GPIO_32 (31)
#define PM_GPIO_33 (32)
#define PM_GPIO_34 (33)
#define PM_GPIO_35 (34)
#define PM_GPIO_36 (35)
#define PM_GPIO_37 (36)
#define PM_GPIO_38 (37)
#define PM_GPIO_39 (38)
#define PM_GPIO_40 (39)
#define PM_GPIO_41 (40)
#define PM_GPIO_42 (41)
#define PM_GPIO_43 (42)
#define PM_GPIO_44 (43)

/*==================================================================
                GPIO I/O Macros
====================================================================*/
#define PM_GPIO_DIG_IN        (0)
#define PM_GPIO_DIG_OUT       (1)
#define PM_GPIO_DIG_IN_OUT    (2)
#define PM_GPIO_ANA_PASS_THRU (3)
#define PM_GPIO_TYPE_INVALID  (4)

/*==================================================================
                GPIO Voltage Source Macros
====================================================================*/
#define PM_GPIO_VIN0        (0)
#define PM_GPIO_VIN1        (1)
#define PM_GPIO_VIN2        (2)
#define PM_GPIO_VIN_INVALID (3)

/*==================================================================
                GPIO Input/Output level Status Macros
====================================================================*/
#define PM_GPIO_LEVEL_LOW      (0)
#define PM_GPIO_LEVEL_HIGH     (1)
#define PM_GPIO_LEVEL_HIGH_Z   (2)
#define PM_GPIO_LEVEL_INVALID  (3)


/*==================================================================
                GPIO output buffer config Macros
====================================================================*/
#define PM_GPIO_OUT_BUF_CFG_CMOS               (0)
#define PM_GPIO_OUT_BUF_CFG_OPEN_DRAIN_NMOS    (1)
#define PM_GPIO_OUT_BUF_CFG_OPEN_DRAIN_PMOS    (2)
#define PM_GPIO_OUT_BUF_CFG_INVALID            (3)

/*==================================================================
                GPIO Out Drive Strength Macros
====================================================================*/
#define PM_GPIO_OUT_DRV_STR_LOW       (0)
#define PM_GPIO_OUT_DRV_STR_MEDIUM    (1)
#define PM_GPIO_OUT_DRV_STR_HIGH      (2)
#define PM_GPIO_OUT_DRV_STR_INVALID   (3)


/*==================================================================
                GPIO Current Source Pulls Macros
====================================================================*/
#define PM_GPIO_I_SRC_PULL_UP_30uA                  (0)
#define PM_GPIO_I_SRC_PULL_UP_1_5uA                 (1)
#define PM_GPIO_I_SRC_PULL_UP_31_5uA                (2)
#define PM_GPIO_I_SRC_PULL_UP_1_5uA_PLUS_30uA_BOOST (3)
#define PM_GPIO_I_SRC_PULL_DOWN_10uA                (4)
#define PM_GPIO_I_SRC_PULL_NO_PULL                  (5)
#define PM_GPIO_I_SRC_PULL_INVALID                  (6)


/*==================================================================
                GPIO Source Macros
====================================================================*/
#define PM_GPIO_SRC_GND               (0)
#define PM_GPIO_SRC_PAIRED_GPIO       (1)
#define PM_GPIO_SRC_SPECIAL_FUNCTION1 (2)
#define PM_GPIO_SRC_SPECIAL_FUNCTION2 (3)
#define PM_GPIO_SRC_SPECIAL_FUNCTION3 (4)
#define PM_GPIO_SRC_SPECIAL_FUNCTION4 (5)
#define PM_GPIO_SRC_DTEST1            (6)
#define PM_GPIO_SRC_DTEST2            (7)
#define PM_GPIO_SRC_DTEST3            (8)
#define PM_GPIO_SRC_DTEST4            (9)
#define PM_GPIO_SRC_INVALID           (10)

/*==================================================================
                GPIO D-test buffer enable/disable Macros
====================================================================*/
#define PM_GPIO_DTEST_DISABLE (0)
#define PM_GPIO_DTEST_ENABLE  (1)
#define PM_GPIO_DTEST_INVALID (2)

/*==================================================================
                GPIO External pin configuration Macros
====================================================================*/
#define PM_GPIO_EXT_PIN_DISABLE             (0)     
#define PM_GPIO_EXT_PIN_ENABLE              (1)
#define PM_GPIO_EXT_PIN_CONFIG_TYPE_INVALID (2)
 
/*==================================================================
                GPIO External pin configuration Macros
====================================================================*/
#define PM_GPIO_PULL_UP_30UA                (0)
#define PM_GPIO_PULL_UP_1P5UA               (1)
#define PM_GPIO_PULL_UP_31P5UA              (2)
#define PM_GPIO_PULL_UP_1P5_AND_30UA_BOOST  (3)
#define PM_GPIO_PULL_DOWN_10UA              (4)
#define PM_GPIO_PULL_NO_PULL                (5)
#define PM_GPIO_PULL_INVALID                (6)
							  
/*=========================================================================
                DISPLAY MACROS
===========================================================================*/
#define MAP_A (0)
#define MAP_B (1)
#define MAP_C (2)
#define MAP_D (3)
#define MAP_E (4)
#define MAP_F (5)
#define MAP_G (6)
#define MAP_H (7)
#define MAP_I (8)

/*=========================================================================
                BMD MACROS
===========================================================================*/
#define PM_BMD_SRC_ID (0)
#define PM_BMD_SRC_THERM (1)

/*=========================================================================
                Charger USB peripheral MACROS
===========================================================================*/
#define PM_INPUT_PRIORITY_WLS (0)
#define PM_INPUT_PRIORITY_USB (1)

#define PM_EXT_RSNS_SCALE_1x (0)
#define PM_EXT_RSNS_SCALE_2x (1)
#define PM_EXT_RSNS_SCALE_2P5x (2)

#define PM_CHG_2S_BATT_TYPE (0)
#define PM_CHG_1S_BATT_TYPE (1)
#define PM_CHG_2P_BATT_TYPE (2)

/*=========================================================================
                LOCKBIT MACROS
===========================================================================*/
#define PM_IMG_NONE    				(0)
#define PM_IMG_LOADER  				(1)
#define PM_IMG_CORE    				(2)
#define PM_IMG_RAMDUMP		             	(3)
#define PM_IMG_INVALID 				(4)
#define PM_IMG_DEVPROG    			(5)

#define LOCKED_NONE   (0)
#define LOCKED_OPEN   (2)
#define LOCKED_CLOSED (3)

/*=========================================================================
                MGPI_PVC MACROS
===========================================================================*/
#define APPS0_PORT               (0)  
#define APPS1_PORT               (1)  
#define MGPI_PVC_PORT            (5)  
#define SPMI_ACCESS_PRIORITY_LOW (0) 

/*=========================================================================
                ACCESS CTRL MACROS
===========================================================================*/
#define REMOVE           (0)
#define APPEND           (1)
#define APPEND_WITH_IRQ  (2)

/*=========================================================================
                MISC MACROS
===========================================================================*/
#define PM_THERM_PULL_UP_10K (0)
#define PM_THERM_PULL_UP_30K (1)
#define PM_THERM_PULL_UP_100K (2)
#define PM_THERM_PULL_UP_400K (3)

/*=========================================================================
                SDAM MACROS
===========================================================================*/
#define PM_SDAM_1 (0)


#define WR_MAX_CNT (0x7156)

/*=========================================================================
                EPM MACROS
===========================================================================*/
//EPM
#define PM_EPM_MODE_RCM  1
#define PM_EPM_MODE_ACAT 0

#define PM_EPM_EN_SET(enable)                      PM_EPM_EN 0 0 0 enable /*enable can be TRUE or FALSE*/
#define PM_EPM_MODE_SET(mode)                      PM_EPM_MODE 0 0 0 mode /*mode can be PM_EPM_MODE_RCM or PM_EPM_MODE_ACAT*/
#define PM_EPM_CONFIG_SET(enable, gang, sid, ppid) PM_EPM_CONFIG enable gang sid ppid
#define PM_EPM_DEFAULT_MODE_SET(enable)            PM_EPM_DEFAULT_MODE 0 0 0 enable

/*=========================================================================
                PSI OEM MACROS
===========================================================================*/
#define PM_SBL_WRITE (0)
#define PM_SBL_DELAY (1)
#define PM_SBL_NOP   (4)

#define  EQUAL                     (0)
#define  GREATER                   (1)
#define  GREATER_OR_EQUAL          (2)
#define  LESS                      (3)
#define  LESS_OR_EQUAL             (4)
#define  NOT_EQUAL                 (5)
#define  REV_ID_OPERATION_INVALID  (6)

/*=========================================================================
                RGB LED CONFIG MACROS
===========================================================================*/
#define PM_RGB_DIM_LEVEL_LOW  (0x080)
#define PM_RGB_DIM_LEVEL_MID  (0x100)
#define PM_RGB_DIM_LEVEL_HIGH (0x180)
#define PM_RGB_DIM_LEVEL_MAX  (0x1FF)

#define RED_BIT_POS           (18)
#define GREEN_BIT_POS         (9)
#define BLUE_BIT_POS          (0)

#define PM_RGB_DIM_LEVEL_SET(red, green, blue) (((red & PM_RGB_DIM_LEVEL_MAX) << RED_BIT_POS) | \
                                              ((green & PM_RGB_DIM_LEVEL_MAX) << GREEN_BIT_POS) | \
                                              ((blue & PM_RGB_DIM_LEVEL_MAX) << BLUE_BIT_POS))

/*=========================================================================
                 PRM MACROS
===========================================================================*/
#define PM_PRM_RPMH  (0)
#define PM_PRM_LOCAL (1)


#define  PM_PRM_RSRC_LDO      (0)
#define  PM_PRM_RSRC_SMPS     (1)
#define  PM_PRM_RSRC_VS       (2)
#define  PM_PRM_RSRC_BOB      (3)
#define  PM_PRM_RSRC_CLK      (4)
#define  PM_PRM_RSRC_TARGET   (5)


#define   PM_PRM_MODE_BYPASS      (2)
#define   PM_PRM_MODE_RETENTION   (3)
#define   PM_PRM_MODE_LPM         (4)
#define   PM_PRM_MODE_OPM         (5)
#define   PM_PRM_MODE_NPM         (7)
#define   PM_PRM_MODE_NONE        (255)

#define   PM_PRM_DISABLE  (0)
#define   PM_PRM_ENABLE   (1)


#endif /*__PM_DEFINES_H__*/
