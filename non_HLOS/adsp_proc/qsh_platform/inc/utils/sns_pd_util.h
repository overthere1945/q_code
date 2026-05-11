#pragma once
/** =============================================================================
 * @file
 *
 * @brief Process Domain Utility functions provided by Qualcomm for use by
 * Sensors.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

/**
 * @brief Initializes the process domain utility by calling
 * qurt qurt_process_get_id() and assigning the value to a static field
 * variable.
 *
 * @return
 * - None.
 */
void sns_pd_util_init(void);

/**
 * @brief Appends the process domain id with client string into the destination
 * buf. Length of dst_buf == max_len >= length of client_name + 3, to account
 * for pd string appended to client_name and null terminated character.
 *
 * @param [in] dst_buf      Buffer storing client_name and appended process id.
 * @param [in] max_len      Maximum length that can be stored into dst_buf.
 * @param [in] client_name  Client name to have process id appended to.
 *
 */
void sns_pd_util_append(char *dst_buf, int max_len, const char *client_name);
