#pragma once
/** ============================================================================
 * @file
 *
 * @brief Contains a wrapper for crc calculation function from core services.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/
#include <stddef.h>
#include <stdint.h>

/**
 *  @brief CRC (Cyclic Redundancy Check) calculation function interface.
 *
 *  @param[in] data     Pointer to input data.
 *  @param[in] length   Length of input data in bytes.
 *
 *  @return
 *   - uint32_t:    Calculated CRC (Cyclic Redundancy Check) value.
 *
 */
uint32_t sns_crc_32_calc(uint8_t *data, size_t length);
