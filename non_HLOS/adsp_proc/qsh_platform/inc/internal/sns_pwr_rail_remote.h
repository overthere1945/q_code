#pragma once
/** ============================================================================
 * @file
 *
 * @brief APIs for remote power rail to validate, register and vote.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/
/**
********************************************************************************
                               Includes
********************************************************************************
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sns_pwr_rail_service.h"

/*
********************************************************************************
                               Functions
********************************************************************************
*/

/**
 *  @brief Validates the rail name.
 *
 *  @param[in] name   Remote rail name.
 *
 *  @return
 *   - True  -   If remote rail name is valid.
 *   - False -   Otherwise.
 *
 **/

bool sns_remote_rail_name_is_valid(const char name[]);

/**
 *  @brief Registers the remote rail and returns the handle on
 *  successful registration.
 *
 *  @param[in] name      Remote rail name.
 *
 *  @return
 *   - uint16_t: Handle for remote rail.
 *
 **/
uint16_t sns_remote_rail_sync_register(const char name[]);

/**
 *  @brief Votes the remote rail.
 *
 *  @param[in] handle               Handle of remote rail.
 *  @param[in] rail_to_vote         Vote for the rail.
 *
 *  @return
 *   - True  - 	If voted successfully.
 *   - False -	If voting not succeed.
 *
 **/
bool sns_remote_rail_sync_update(uint16_t handle,
                                 sns_power_rail_state rail_to_vote);
