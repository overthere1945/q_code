#pragma once
/*===============-==============================================================
  @file sns_com_port_profile.h

  @brief APIs to profile latency of com port APIs

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/

#include <stdint.h>
#include "sns_printf_int.h"
#include "sns_profile.h"

/*=============================================================================
  Type Definitions
  ============================================================================*/
typedef void *sns_sync_com_port_handle;
typedef enum function_list
{
  PROFILE_I2C_TRANSFER = 0,
  PROFILE_SPI_TRANSFER = 1,
  PROFILE_FUNC_LAST,
} function_list;

/*=============================================================================
  Public function declaration
  ============================================================================*/
void sns_com_port_profile_init(sns_sync_com_port_handle *port_handle);
void sns_com_port_profile_deinit(sns_sync_com_port_handle *port_handle);
void sns_com_port_latency_dump(sns_sync_com_port_handle *port_handle);
void sns_com_port_profile(sns_sync_com_port_handle *port_handle,
                          sns_profile_action action,
                          function_list function_to_);
