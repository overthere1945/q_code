#pragma once
/** ============================================================================
 * @file
 *
 * @brief The API's  to use secure HMAC API.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/
#include <dirent.h>
#include <sys/stat.h>

#include "sns_rc.h"
/*=============================================================================
  functions
  ============================================================================*/

/**
 * @brief Add filename entry to list which need to be secured.
 *
 * @param[in] filename  Name of the file.
 * @param[out] stream   File pointer for writing entries in database.
 *
 * @return
 *  - None.
 *
 */
void sns_secure_entry_to_file_mapping_list(const char *restrict filename,
                                           FILE *stream);

/**
 * @brief Delete entry from the list.
 *
 * @param[in] stream   File pointer for deleting the records from database.
 *
 * @return
 *  - None.
 *
 */
void sns_secure_delete_entry_file_mapping_list(FILE *stream);

/**
 * @brief Secure file.
 *
 * @param[in] filename  Name of the file.
 *
 * @return
 *  - SNS_RC_SUCCESS:   On success.
 *  - SNS_RC_FAILURE:   Otherwise.
 *
 */
sns_rc sns_secure_file(const char *filename);

/**
 * @brief Flush updated secure hmac db list to file system.
 *
 * @return
 *  - None.
 *
 */
void sns_flush_secure_db(void);

/**
 * @brief Check if the data stream is secured or not.
 *
 * @param[in] ptr    Pointer to data buffer.
 * @param[in] len    Length of data buffer.
 * @param[in] stream File pointer for reading the record from database.
 *
 * @return
 *   - True:    If secured.
 *   - False:   Otherwise.
 *
 */
bool sns_secure_is_data_stream_secured(void *ptr, size_t len, FILE *stream);

/**
 * @brief Secure file stream ptr.
 *
 * @param[in] ptr    Pointer to data buffer.
 * @param[in] len    Length of data buffer.
 * @param[in] stream File pointer for writing the record to database.
 *
 * @return
 *  - None.
 *
 */
void sns_secure_make(void const *ptr, size_t len, FILE *stream);

/**
 * @brief Iniitialize secure db file into list.
 *
 * @return
 *  - None.
 *
 */
sns_rc sns_check_secure_database_initialized(void);

/**
 * @brief Delete file entry from local data base.
 *
 * @param[in] filename  Name of the file.
 *
 * @return
 *  - None.
 *
 */
void sns_delete_entry_from_local_db_list(const char *filename);
