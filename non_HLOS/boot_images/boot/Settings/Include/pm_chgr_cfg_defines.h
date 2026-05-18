#ifndef __PM_CHGR_CFG_DEGINES_H__
#define __PM_CHGR_CFG_DEGINES_H__

/*===========================================================================

                 Pmic Charger Config defines Header File

GENERAL DESCRIPTION
  This header file contains declarations and definitions for Pmic Charger Config Device Tree 
  settings.
    
Copyright (c) 2023 by Qualcomm Technologies, Inc.  All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/

#define PM_CHGR_CFG_ENABLE                                                                  1
#define PM_CHGR_CFG_DISABLE                                                                 0

#define PM_CHGR_CFG_UNKNOWN_BATT_BEHAV_SHUTDOWN                                             0
#define PM_CHGR_CFG_UNKNOWN_BATT_BEHAV_BOOT_TO_HLOS                                         1
#define PM_CHGR_CFG_UNKNOWN_BATT_BEHAV_CONSERV_CHG                                          2
#define PM_CHGR_CFG_UNKNOWN_BATT_BEHAV_REGULAR_CHG                                          3

#define PM_CHGR_CFG_DBG_BRD_BATT_BEHAV_LOW_BATT_ICON_DISABLE_PON_TRIGGER                    0
#define PM_CHGR_CFG_DBG_BRD_BATT_BEHAV_LOW_BATT_ICON_STAY_ON                                1
#define PM_CHGR_CFG_DBG_BRD_BATT_BEHAV_BOOT_TO_HLOS                                         2

#define PM_CHGR_CFG_CHGR_LED_CFG_DISABLE                                                    0
#define PM_CHGR_CFG_CHGR_LED_CFG_SOLID                                                      1
#define PM_CHGR_CFG_CHGR_LED_CFG_BLINK                                                      2

#define PM_CHGR_CFG_CHGR_WDOG_DISABLE                                                       0
#define PM_CHGR_CFG_CHGR_WDOG_ENABLE_DUR_CHG_DISABLE_AFTER                                  1
#define PM_CHGR_CFG_CHGR_WDOG_ENABLE_DUR_CHG_ENABLE_AFTER                                   2

#define PM_CHGR_CFG_BATT_ID_PULL_UP_100K                                                    0
#define PM_CHGR_CFG_BATT_ID_PULL_UP_30K                                                     1
#define PM_CHGR_CFG_BATT_ID_PULL_UP_400K                                                    2


#endif