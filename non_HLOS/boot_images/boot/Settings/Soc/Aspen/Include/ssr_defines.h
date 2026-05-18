#ifndef __SSR_DEFINES_H__
#define __SSR_DEFINES_H__


/*=============================================================================   
    @file  ssr_config.h
    @brief interface to device configuration
   
    Copyright (c) Qualcomm Technologies, Inc. All rights reserved.
    Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/

/*=============================================================================
                              EDIT HISTORY
 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 06/10/21   avm      Added new include file.
=============================================================================*/

// Interrupt Configuration
#define INTR_CONFIG_LEVEL_TRIGGER    0
#define INTR_CONFIG_EDGE_TRIGGER     1

// List of hosts supported in SMEM
//#define SMEM_MODEM     1
//#define SMEM_ADSP      2
//#define SMEM_SSC       3
//#define SMEM_CDSP      5
//#define SMEM_SOCCP          19

// List of SFRType 
#define SMEM_SSR_REASON_MSS0         421
#define SMEM_SSR_REASON_LPASS0       423
#define SMEM_SSR_REASON_DSPS0        424
#define SMEM_SSR_REASON_SWM0         678
#define SMEM_SSR_REASON_AWM0         679



/**	
	copied from subsystem_control_v02.h
 */
#define SSCTL_QMI_SERVICE_INSTANCE_MDM_V02  0x10 /**<  MDM, added this instance for HLOS perspective to use appropriate SSCTL 
          service instance to communicate if in a Fusion environment, it must use MDM; 
          if standalone, use MPSS or whatever is appropriate  */
#define SSCTL_QMI_SERVICE_INSTANCE_APSS_V02  0x11 /**<  APSS  */
#define SSCTL_QMI_SERVICE_INSTANCE_MPSS_V02  0x12 /**<  MPSS  */
#define SSCTL_QMI_SERVICE_INSTANCE_WCNSS_V02  0x13 /**<  WCNSS */
#define SSCTL_QMI_SERVICE_INSTANCE_ADSP_V02  0x14 /**<  ADSP */
#define SSCTL_QMI_SERVICE_INSTANCE_QSC_V02  0x15 /**<  QSC service instance number cannot be written and is
       beyond our control because of QMUX  */
#define SSCTL_QMI_SERVICE_INSTANCE_SLPI_V02  0x16 /**<  SLPI */
#define SSCTL_QMI_SERVICE_INSTANCE_CDSP_V02  0x17 /**<  CDSP */
#define SSCTL_QMI_SERVICE_INSTANCE_NPU_V02  0x18 /**<  NPU */
#define SSCTL_QMI_SERVICE_INSTANCE_WPSS_V02  0x19 /**<  WPSS */


#endif
