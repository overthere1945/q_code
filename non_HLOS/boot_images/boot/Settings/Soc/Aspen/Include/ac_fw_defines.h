#ifndef __AC_FW_DEFINES_H__
#define __AC_FW_DEFINES_H__


/*=============================================================================   
    @file  ac_fw_config_defines.h
    @brief interface to access control & firmware loading configuration
   
    Copyright (c) Qualcomm Technologies, Inc.
	All rights reserved.
    Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/

/*=============================================================================
                              EDIT HISTORY
 when       who         what, where, why
 --------   --------    -------------------------------------------------------
 10/24/24   anupvish    Added new header file to include AC FW defines
=============================================================================*/

#define NUM_SE_SUPPORTED		1   

#define QUP3_1_SE0_BASE_ADDR	0x00A80000		
#define QUP3_1_SE1_BASE_ADDR	0x00A84000
#define QUP3_1_SE2_BASE_ADDR	0x00A88000
#define QUP3_1_SE3_BASE_ADDR	0x00A8C000
#define QUP3_1_SE4_BASE_ADDR	0x00A90000
#define QUP3_1_SE5_BASE_ADDR	0x00A94000
#define QUP3_1_SE6_BASE_ADDR	0x00A98000
#define QUP3_1_SE7_BASE_ADDR	0x00A9C000
#define QUP3_1_SE8_BASE_ADDR	0x00AA0000
#define QUP3_1_SE9_BASE_ADDR	0x00AA4000
#define QUP3_1_SE10_BASE_ADDR	0x00AA8000

#define QUP3_1_COMMON_BASE_ADDR	0x00AC0000

/* SE region size for all the QUPs */
#define QUP_SE_REGION_SIZE 	0x1000 //from QUPAC_regions.csv file
	
#define QUP3_1_GPII10_ADDR	0xa38000 //from QUPAC_regions.csv file

#define QUP3_GPII_REGION_SIZE	0x4000 //from QUPAC_regions.csv file

#define HAL_VMIDMT_QUPV3_1		25

#define AC_CSID_DCP           	7	/* SMMU target configuration sheet: DCP row, CSID colom */
#define AC_CSID_NONE            0xFFFFFFFF

#define AC_VM_DCP_ELF           64	/* TZ.XF.5.0/ssg.tz/1.0/securemsm/accesscontrol/api/ACCommon.h */
#define AC_VM_NONE				0 

#define AC_SID_NONE				0

#define SE_PROTOCOL_SPI      	0x1
#define SE_PROTOCOL_UART     	0x2
#define SE_PROTOCOL_I2C      	0x3
#define SE_PROTOCOL_I3C      	0x4
#define SE_PROTOCOL_QSPI_HID 	0x8
#define SE_PROTOCOL_QSPI     	0x9
#define SE_PROTOCOL_UFCS     	0xD

#endif
