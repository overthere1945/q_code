#pragma once
/*==============================================================================
  @file sns_ai_oem1_algo_version.h

  AI_OEM1 Sensor Algorithm Version History

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

/*=============================================================================
  Static Data Definitions
  ============================================================================*/

/*=============================================================================
  Version
  =============================================================================*/

// 32-bit version number represented as major[31:16].minor[15:8].rev[7:0]
#define AI_OEM1_MAJOR       1
#define AI_OEM1_MINOR       0
#define AI_OEM1_REV         2
#define SNS_AI_OEM1_VERSION ( ( AI_OEM1_MAJOR << 16 ) | ( AI_OEM1_MINOR << 8 ) | AI_OEM1_REV )

/*=============================================================================
  Version History
  =============================================================================
  Version: 1.0.0   Date: 15/03/2024   POC: deepraj
  Changes:
  - Initial version of AI_OEM1 Sensors Algorithm
  =============================================================================
  Version: 1.0.1   Date: 10/05/2024   POC: deepraj
  Changes:
  - Updates according to CodeColab Review #3400747
  - Interface updates
  =============================================================================
    Version: 1.0.2   Date: 26/03/2025   POC: deepraj
  Changes:
  - Memory Utility Deprecated
  - Sensors alloc API wrapper used for buffer allocation
  =============================================================================*/

