#pragma once
/** ============================================================================
 *  @file
 *
 *  @brief This file has API declaration of diag msg wrapper functions.
 *
 *  @copyright Copyright (c) Qualcomm Technologies, Inc. and / or its
 *  subsidiaries. All Rights Reserved. Confidential and Proprietary - Qualcomm
 *  Technologies, Inc.
 *  ===========================================================================
 */

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief This function check if the mask is valid or not.
 *
 * @param[in] ss_id     Subsystem ID (SSID):
 *                      Unique identifier given to each subsystem within the
 *                      target.
 * @param[in] ss_mask   Sub-system mask.
 *
 * @return
 *  - True:    Success.
 *  - False:   Otherwise.
 */

bool sns_msg_status(uint16_t ss_id, uint32_t ss_mask);