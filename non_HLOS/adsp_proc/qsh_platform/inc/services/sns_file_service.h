#pragma once
/** =============================================================================
 * @file
 *
 * @brief Service for sensor file service management.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include <stdio.h>
#include <unistd.h>
#include "sns_service.h"

/**
 * @brief The Sensor File Service.
 * Will be obtained from sns_service_manager::get_service.
 *
 */
typedef struct sns_file_service
{
  sns_service service;              /*!< Service information. */
  struct sns_file_service_api *api; /*!< Public api provided by the framework to
                                     *   be used by the sensor.
                                     */
} sns_file_service;

/**
 * @brief public state used by the Framework for the sensor file service.
 *
 */
typedef struct sns_file_service_api
{
  uint32_t struct_len; /*!< Length of sns_file_service_api structure. */

  /**
   * @brief Opens the file whose name is the string pointed to by filename and
   * assosiates a stream with it.
   *
   * @param[in] filename  Absolute path of file to open.
   * @param[in] mode      What open mode to use.
   *
   * @return
   *  - FILE* The associated file stream.
   *
   */
  FILE *(*fopen)(const char *restrict filename, const char *restrict mode);

  /**
   * @brief Attempts to write the buffer pointed to by ptr whose size is
   * specified by size upto n elements specifed by count, to the file_handle.
   *
   * @param[in] ptr    Pointer to the buffer.
   * @param[in] size   Size of the data type.
   * @param[in] count  Number of elements to be written.
   * @param[in] stream The file stream to which to write.
   *
   * @return
   *  - size_t The number of elements successfully written.
   *
   */
  size_t (*fwrite)(void const *ptr, size_t size, size_t count, FILE *stream);

  /**
   * @brief Attempts to read size n elements specified by count from the buffer
   * ptr, each size bytes long, from the file_handle.
   *
   * @param[in] ptr     Pointer to the buffer.
   * @param[in] size    Size of the data type.
   * @param[in] count   Number of elements to be read.
   * @param[in] stream  The file stream from which to read.
   *
   * @return
   *  - size_t  The number of elements successfully read.
   *
   */
  size_t (*fread)(void *ptr, size_t size, size_t count, FILE *stream);

  /**
   * @brief Closes the file pointed by stream.
   *
   * @param[in] stream The file stream.
   *
   * @return
   *  - 0     if successful
   *  - EOF   otherwise.
   *
   */
  int (*fclose)(FILE *stream);

  /**
   * @brief Gets the size of the file.
   *
   * @param[in] path Absolute path of file.
   * @param[in] size Destination for the file size.
   *
   * @return
   *  - SNS_RC_SUCCESS   Success.
   *  - Any other value  Failure.
   *
   */
  int (*fsize)(const char *path, off_t *size);

} sns_file_service_api;
