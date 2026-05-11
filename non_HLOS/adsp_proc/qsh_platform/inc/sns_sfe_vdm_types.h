#pragma once
/** ============================================================================
 * @file
 *
 * @brief This file contains structure definitions for VDM Client configuration.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Includes
  ============================================================================*/

#include <inttypes.h>

/*==============================================================================
  Macros
  ============================================================================*/

#define SNS_SFE_VDM_SEQ_NUM_SEQWORDS        250
#define SNS_SFE_VDM_SEQ_NUM_BRANCHES        24
#define SNS_SFE_VDM_SEQ_NUM_NON_SEQMEM      20
#define SNS_SFE_VDM_CLIENT_NUM_TRES         50
#define SNS_SFE_VDM_CLIENT_DMA_NUM_TRES     20
#define SNS_SFE_VDM_CLIENT_SIZE_TRE         4
#define SNS_SFE_VDM_CLIENT_SIZE_SENSOR_DATA 20
#define SNS_SFE_VDM_TRE_INVALID_LOCATIION   0xFFFFFFFF

#define SNS_SFE_VDM_SEQ_BR_MAIN     0x0
#define SNS_SFE_VDM_SHARED_MEM_ADDR 0x00000000

/*==============================================================================
  Types
  ============================================================================*/

/**
 * @brief Structure for shared memory info.
 *
 */
typedef struct
{
  uint32_t
      treTemplate[SNS_SFE_VDM_CLIENT_NUM_TRES * SNS_SFE_VDM_CLIENT_SIZE_TRE];
  uint32_t sensorDataSection[SNS_SFE_VDM_CLIENT_SIZE_SENSOR_DATA];
  uint8_t use_ddr_for_SensorData;
} sns_sfe_shared_mem;
/**
 * @brief Structure for VDM Client CONFIGURATIONS Size.
 *
 */
typedef struct
{
  uint32_t sequenceOpcodeSize;
  uint32_t nonSeqMemSize;
  uint32_t branchAddrSize;
  uint32_t treTemplateSize;
  uint32_t sensorDataSectionSize;
} sns_sfe_vdm_cfgs_sizes;

typedef struct
{
  uint32_t tre_template_location;
  uint32_t offset_value;
} offset_list_type;

/**
 * @brief Structure for firmware version
 *
 */
typedef struct
{
  uint32_t firmware_version_minor;
  uint32_t firmware_version_major;
  uint32_t firmware_version_rev;
} sns_sfe_vdm_version;

typedef struct
{
  const char *format;
  uint8_t num_parameters;

} sns_sfe_vdm_f3_table;

/**
 * @brief Structure for firmware version
 *
 */
typedef struct
{
  uint8_t uses_pram_mgr;
  uint32_t pram_recommended_sz;
  offset_list_type offsets[SNS_SFE_VDM_CLIENT_DMA_NUM_TRES];
  uint8_t slave_address;
  const sns_sfe_vdm_f3_table *f3_table;
  uint8_t f3_table_len;
} sns_sfe_vdm_fw_cfgs;

/**
 * @brief Structure for VDM Client CONFIGURATIONS.
 *
 */
typedef struct
{
  uint32_t sequenceOpcode[SNS_SFE_VDM_SEQ_NUM_SEQWORDS];
  uint32_t nonSeqMem[SNS_SFE_VDM_SEQ_NUM_NON_SEQMEM];

  uint32_t branchAddr[SNS_SFE_VDM_SEQ_NUM_BRANCHES];
  sns_sfe_shared_mem shmm;

  sns_sfe_vdm_cfgs_sizes cfgSize;
  sns_sfe_vdm_fw_cfgs fw_cfgs;
  sns_sfe_vdm_version fw_version;

} sns_sfe_vdm_cfgs;
