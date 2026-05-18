#ifndef __QNTM_DEFINES_COMMON_H__
#define __QNTM_DEFINES_COMMON_H__


/*=============================================================================   
    @file  qntm_defines.h
    @brief interface to device configuration
   
    Copyright (c) Qualcomm Technologies, Inc.  All Rights Reserved.
    Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/

/*=============================================================================
                              EDIT HISTORY
 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
02/29/24   sd      Added new include file.
=============================================================================*/
#define QUANTUM_SUBSYS_NONE	    	0
#define	QUANTUM_UEFI			    1
#define	QUANTUM_ADSP			    2
#define	QUANTUM_SLPI			    3
#define	QUANTUM_CDSP			    4
#define	QUANTUM_MPSS			    5
#define	QUANTUM_NPU				    6
#define	QUANTUM_SDC				    7
#define	QUANTUM_CDSP1			    8
#define	QUANTUM_WPSS			    9
#define QUANTUM_GPDSP0			   10
#define QUANTUM_GPDSP1			   11
#define QUANTUM_SOCCP			   12
#define QUANTUM_ADSP1			   13
#define QUANTUM_ADSP2			   14
#define QUANTUM_DCP         	   15
#define QUANTUM_CDSP2			   16
#define QUANTUM_CDSP3			   17
#define QUANTUM_OOBTEE			   18
#define QUANTUM_OOBNS			   19
#define QUANTUM_AWM 			   20
#define	QUANTUM_SWM  			   21
#define QUANTUM_ADSP_COMPUTELITE   22
#define QUANTUM_ADSP1_COMPUTELITE  23
#define QUANTUM_ADSP2_COMPUTELITE  24
#define QUANTUM_SUBSYS_MAX  	   25

#define SHARE_NO_SUPPORT      1
#define	SHARE_BY_SID          2
#define SHARE_BY_HYP_ASSIGN   3
#define AC_VM_MSS_MSA         15
#define AC_VM_WLAN_DSP        44

#define IORT_WORLD_ID_NON_SECURE 0x0
#define IORT_WORLD_ID_SECURE     0x1
#define IORT_WORLD_ID_SHIFT      31

#define IORT_DYNAMIC_MAPPING_NO          0x0
#define IORT_DYNAMIC_MAPPING_YES_DEFAULT 0x0
#define IORT_DYNAMIC_MAPPING_YES         0x1
#define IORT_DYNAMIC_MAPPING_SHIFT       30

#define IORT_TRANSLATION_TYPE_S2_ONLY      0x0
#define IORT_TRANSLATION_TYPE_S2CR_BYPASS  0x1
#define IORT_TRANSLATION_TYPE_SINGLE_STAGE 0x1
#define IORT_TRANSLATION_TYPE_NESTED       0x3
#define IORT_TRANSLATION_TYPE_SHIFT        24

#define IORT_VMID_SHIFT 16


#endif
