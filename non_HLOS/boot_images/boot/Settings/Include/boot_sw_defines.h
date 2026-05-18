#ifndef __BOOT_SW_DEFINES_H__
#define __BOOT_SW_DEFINES_H__

/*===========================================================================

                 Boot Loader Error Handler Header File

GENERAL DESCRIPTION
  This header file contains declarations and definitions for boot sw settings.
    
Copyright 2020-2022 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/10/22   rama    Added macros for check_usb_option
01/22/21   rama    Added ENTER_EDL
11/22/20   rama    Added macros for BOOT_VIBRATION property.
11/13/20   rama    Initial revision.

============================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
/*=========================================================================
                Generic MACROS
===========================================================================*/

#define ENABLE  (0x1)
#define DISABLE (0x0)

#define SHUT_DOWN  (0x0)
#define HARD_RESET (0x1)
#define WARM_RESET (0x2)
#define ENTER_EDL  (0x3)
/*=========================================================================
                MACROS for FDL_TIMEOUT_ACTION property
===========================================================================*/
/*
Description: This SW featur config allows user to configure device behavior 
in case of fedl timeout cookie set by PBL.
*/

//version
#define BOOT_FEDL_TIMEOUT_PROP_VERSION (0x1)

//Valid values that can be assigned to sw/boot/fedl_timeout/action property
#define BOOT_FEDL_TIMEOUT_ACTION_SHUTDOWN   SHUT_DOWN
#define BOOT_FEDL_TIMEOUT_ACTION_HARD_RESET HARD_RESET

//Valid values that can be assigned to sw/boot/force_download/check_usb_option property
#define CHECK_USB_D_PLUS_GROUND             (0x0)
#define CHECK_USB_D_PLUS_HIGH_WITH_TIMEOUT  (0x1)
#define CHECK_USB_REGULAR_CABLE             (0x2)
//=========================================================================
//=========================================================================


#endif /*__BOOT_SW_DEFINES_H__*/
