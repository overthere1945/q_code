/*=============================================================================
  @file

  @brief Platform specific file service API headers

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_fw_file_service_internal.h"

/*=============================================================================
  Macros
  ===========================================================================*/
#define SNS_FILE_OSA_MAX_RETRY_CNT                                             \
  10 /*!<  Max retry count before reporting failure  */
#define SNS_FILE_OSA_WAIT_NS                                                   \
  10 * 1000000                   /*!<  Busy wait time in nano seconds  */
#define ENOLINK               67 /*!<  ENOLINK error code */
#define SNS_FILE_OSA_PATH_MAX 256

/*=============================================================================
  Structures
  ===========================================================================*/

typedef struct
{
  sns_file_service_internal service; /*!< File Service internal structure*/
  sns_file_service_internal_api api; /*!< File Service internal API */
  sns_service_status_cb single_cb;   /*!< Single callback function*/
  sns_service_status_func_arg cb_arg; /*!< Callback argument */

} sns_fw_file_service_internal;

/*=============================================================================
  Functions
  ===========================================================================*/

/**
 * @brief The API to register call back with file service
 * @note: This API should be called only once after boot up by the framework.
 *
 * @par Parameters
 *      None.
 *
 * @return
 * - None.
 *
 */
void sns_fs_daemon_register(void);

/**
 * @brief The API to initialize file service
 *
 * @par Parameters
 *      None.
 *
 * @return
 * - SNS_RC_SUCCESS        Success
 * - SNS_RC_FAILED         Failed
 *
 */
sns_rc sns_fs_init(void);

/**
 * @brief API to get handle for file service
 *
 * @par Parameters
 * - filename - file name to associate the file stream to
 * - mode - Mode of the file to be opened
 *
 * @return
 * - FILE        File stream pointer
 *
 */
FILE *sns_fopen(const char *restrict filename, const char *restrict mode);

/**
 * @brief API to write stream of bytes into a file
 *
 * @par Parameters
 * - ptr -    pointer to the first object in the array to be written
 * - size -   size of each object
 * - count -  the number of the objects to be written
 * - stream - pointer to the output stream
 *
 * @return
 * - size_t  number of objects written successfully
 *
 */
size_t sns_fwrite(void const *ptr, size_t size, size_t count, FILE *stream);

/**
 * @brief API to read stream of bytes from a file
 *
 * @par Parameters
 * - ptr -    pointer to the output buffer
 * - size -   size of each object
 * - count -  the number of the objects to be read
 * - stream - pointer to the input stream
 *
 * @return
 * - size_t  number of objects read successfully
 *
 */
size_t sns_fread(void *ptr, size_t size, size_t count, FILE *stream);

/**
 * @brief API to close file stream
 *
 * @par Parameters
 * - stream - the file stream to close
 *
 * @return
 * - 0        on Success
 * - Error codes otherwise
 *
 */
int sns_fclose(FILE *stream);

/**
 * @brief API to Set the file position indicator for the file stream
 *
 * @par Parameters
 * - stream - the file stream to close
 * - offset - number of bytes to shift the position relative to origin
 * - whence - Position to which offset is added
 * @return
 * - 0        on Success
 * - Error codes otherwise
 *
 */
int sns_fseek(FILE *stream, long long offset, int whence);

/**
 * @brief API to write stream of bytes into a file
 *
 * @par Parameters
 * - file - File name to be written to
 * - temp_dir - Directory where file will be created
 * - buffer - pointer to the first object in the array to be written
 * - len -     Number of bytes of data to be written to file stream
 *
 * @return
 * - 0 - on Success
 * --1 - on Failure
 *
 */
int sns_fwritesafe(const char *restrict file, const char *restrict temp_dir,
                   void const *buffer, size_t len);

/**
 * @brief API to get size of a file
 *
 * @par Parameters
 * - path - Path to file
 * - size - Output parameter where size of the input file will gets filled
 *
 * @return
 * -  0 - on Success
 * - -1 - on Failure
 *
 */
int sns_fsize(const char *path, off_t *size);

/**
 * @brief API to get information about a file
 *
 * @par Parameters
 * - path - Path to file
 * - size - Output parameter where size of the input file will gets filled
 *
 * @return
 * -  0 - on Success
 * - -1 - on Failure
 *
 */
int sns_stat(const char *path, struct stat *buffer);

/**
 * @brief API to unlink a file
 *
 * @par Parameters
 * - path - Path to file
 *
 * @return
 * -  0 - on Success
 * - -1 - on Failure
 *
 */
int sns_unlink(const char *path);

/**
 * @brief Get the absolute path to the file
 *
 * @par Parameters
 * - filename[i] - filename with or without the absolute path.
 * - filepath[o] - absolute path to the file
 * - filepath_size[i] - filepath size
 *
 * @return
 * -  filepath is prefixed with default path if filename is
 *    without the absolute path.
 *
 */
void sns_fs_get_filepath(const char *restrict filename, char *filepath,
                         size_t filepath_size);
