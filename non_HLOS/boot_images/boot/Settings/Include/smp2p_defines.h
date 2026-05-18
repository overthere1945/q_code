#ifndef SMP2P_DEFINES_H_
#define SMP2P_DEFINES_H_
/*------------------------------------------------------------------------------
   SMP2P_DEFINES.H  - SMP2P Settings definitions
------------------------------------------------------------------------------*/
/*!
  @file
    smp2p_defines.h

  @brief
    This file contains the SMP2P settings symbolic constant definitions.
*/

/*------------------------------------------------------------------------------
     Copyright  2022 Qualcomm Technologies Incorporated.
     All rights reserved.
------------------------------------------------------------------------------*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: $

when         who     what, where, why
--------     ---     ----------------------------------------------------------
02/15/2022   mad     Carved out from smp2p_settings.h for devicetree usage
===========================================================================*/


/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "ipcc_defines.h"

/*===========================================================================
                        DEFINITIONS
===========================================================================*/
/* Feature flags (smp2p_header_version_type.flags, 24 bits) */
#define SMP2P_FFLAGS_SSR_HANDSHAKE      0x000001

#define SMP2P_NULL_SIGNAL (-1)
#define SMP2P_NULL_CLIENT IPCC_C_TOTAL

/* Defined by MPROC Arch */
#define SMP2P_IRQ 2

#endif /* SMP2P_DEFINES_H_ */