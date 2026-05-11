#pragma once
/*=============================================================================
  @file qsh_audio_island.h

  Header file for the island utility

  Copyright (c) 2021-2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*============================================================================
  INCLUDES
  ============================================================================*/
#include "sns_types.h"

struct npa_client;
typedef struct npa_client * npa_client_handle;

/*============================================================================
  Functions
  ============================================================================*/

/**
 * This function exits the island mode.
 * It will block until the island is exited.
 * It doesn't prevent re-entering to island.
 *
 */
void qsh_audio_island_exit(void);

/**
 * This function sends a npa request to enable island.
 *
 */
void qsh_audio_island_enable_npa_request(void);

/**
 * This function sends a npa request to disable island.
 *
 */
void qsh_audio_island_disable_npa_request(void);

/**
 * This function creates a npa handle.
 */
void qsh_audio_island_npa_create_handle(void);
