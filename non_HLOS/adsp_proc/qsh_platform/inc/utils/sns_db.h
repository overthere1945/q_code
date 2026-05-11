#pragma once
/** ============================================================================
 * @file
 *
 * @brief Database to save fixed length data in cache with incremental
 * timestamp. The database also supports reading and write back to file system.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

#include "sns_rc.h"

#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * MACRO Definitions
 ******************************************************************************/

/**
 * @brief MIN size of Database cache (in Bytes).
 *
 */
#define SNS_DB_MIN_SIZE 512

/**
 * @brief MAX size of Data of items in Database (in Bytes).
 *
 */
#define SNS_DB_MAX_DATA_SIZE 256

/**
 * @brief MAX size of Database file path.
 *
 */
#define SNS_DB_FILE_PATH_MAX_LEN 64

/**
 * @brief Size of timestamp in Bytes.
 *
 */
#define SNS_DB_TS_SIZE sizeof(sns_db_ts_t)

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/

/**
 * @brief The datatype for timestamp of each item in the Database.
 *
 */
typedef uint32_t sns_db_ts_t;

/**
 * @brief Structure representing the Database configuration.
 *
 */
typedef struct sns_db_config_s
{
  /**
   * @brief file_path Full path to file for storing copy of Database.
   * NOTE: Path string must be NULL terminated.
   *
   */
  char *file_path;

  /**
   * @brief
   * owner The pointer to the sensor/instance with which this Database
   * is attached.
   *
   */
  void *owner;

  /**
   * @brief max_size Max size of Database cache/file (Bytes).
   *
   */
  uint32_t max_size;

  /**
   * @brief data_size The fixed payload size of an item in Database.
   *
   */
  uint16_t data_size;

  /**
   * @brief island Whether ISLAND memory is required for Database cache.
   *
   * @note It is assumed that the utility and the Sensor
   * using the utility use the same island pool and
   * the sensor has the responsibility to vote for island.
   *
   */
  bool island;

} sns_db_config_t;

/**
 * @brief Structure representing an item in the Database.
 *
 */
typedef struct sns_db_item_s
{
  /**
   * @brief timestamp indicates the time when the data was generated.
   * Client can save timestamp in any format utilizing
   * the available bits.
   *
   */
  sns_db_ts_t timestamp;

  /**
   * @brief payload stores the data of length SNS_DB_DATA_SIZE (bytes).
   *
   */
  uint8_t payload[];

} sns_db_item_t;

/**
 * @brief Structure representing a Database query result iterator.
 *
 */
typedef struct sns_db_iter_s
{
  /**
   * @brief Handle to the database list associated with this iterator.
   *
   */
  void *list;

  /**
   * @brief Current position offset of the iterator.
   *
   */
  int32_t current;

} sns_db_iter_t;

/**
 * @brief ENUM with possible return codes for sns_db_add.
 *
 */
typedef enum sns_db_add_rc
{
  SNS_DB_ADD_RC_APPEND = 0,
  SNS_DB_ADD_RC_UPDATE,
  SNS_DB_ADD_RC_INSERT,
  SNS_DB_ADD_RC_FAIL
} sns_db_add_rc;

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/**
 * @brief Function to initialize the Database.
 * This will allocate memory for Database cache and initialize counters.
 * It will then try to read the Database file and load it into cache.
 * If no file is found it will create a new file for future storage.
 *
 * @param[out] handle Opaque handle for the Database.
 * @param[in] config Database configuration parameters.
 *
 * @return
 *  - SNS_RC_SUCCESS       == Success
 *  - SNS_RC_INVALID_TYPE  == Input arguments are invalid,
 *  - SNS_RC_INVALID_VALUE == max_size or data_size is not supported,
 *  - SNS_RC_NOT_AVAILABLE == Database cache memory allocation failure.
 *
 */
sns_rc sns_db_init(void **handle, const sns_db_config_t *config);

/**
 * @brief Function to de-initialize the Database.
 * This will save the cache to file system and then free the cache.
 *
 * @param[inout] handle Opaque handle for the Database.
 *
 * @return
 *  - SNS_RC_SUCCESS       == Success
 *  - SNS_RC_INVALID_TYPE  == Null handle was provided.
 *  - SNS_RC_FAILED        == Unable to sync database to storage.
 *
 */
sns_rc sns_db_deinit(void **handle);

/**
 * @brief Function to save the Database to file system.
 *
 * @param[inout] handle Opaque handle for the Database.
 *
 * @return
 *  - SNS_RC_SUCCESS == Success, any other value == Failure.
 *
 */
sns_rc sns_db_sync(void *handle);

/**
 * @brief Function to add an item into Database along with its timestamp.
 * - An new entry will be created for the item in Database if data base
 * is empty or if its timestamp is not equal to that of any item in
 * Database and greater than the oldest.
 * - Adding an item with timestamp equal to that of an existing item
 * will overwrite the existing item with the new payload.
 *
 * @param[in] handle Opaque handle for the Database.
 * @param[in] timestamp Time when the data vector was generated.
 * @param[in] data The data to be added to the Database.
 * @param[in] length The length of the data to be added to the Database.
 * Length must conform with the Database version constraints.
 *
 * @return
 *  - SNS_DB_ADD_RC_APPEND == Append was performed.
 *  - SNS_DB_ADD_RC_UPDATE == Update was performed.
 *  - SNS_DB_ADD_RC_INSERT == Insert was performed.
 *  - SNS_DB_ADD_RC_FAIL   == Nothing was performed.
 *
 */
sns_db_add_rc sns_db_add(void *handle, sns_db_ts_t timestamp,
                         const uint8_t *data, uint16_t length);

/**
 * @brief Function to fetch items with timestamps falling within range of
 * timestamp_begin and timestamp_end (both inclusive).
 * - Fetch results are only valid until before the next add operation.
 * - Once the fetch results are no longer required, the purge API
 * must be called to free up resources.
 *
 * @param[in] handle   Opaque handle for the Database.
 * @param[out] iter    The iterator to the fetch results which can be used
 * to access items found using iterator APIs.
 * @param[in] begin_ts The timestamp from which to start retrieving items.
 * @param[in] end_ts   The timestamp up to which items are to be retrieved.
 *
 * @return
 *  - SNS_RC_SUCCESS       == Success
 *  - SNS_RC_INVALID_TYPE  == Input arguments are invalid
 *  - SNS_RC_INVALID_STATE == DB state is invalid or corrupted
 *  - SNS_RC_NOT_AVAILABLE == DB has no items or is empty
 *  - SNS_RC_FAILED        == Any other failure.
 *
 */
sns_rc sns_db_fetch(void *handle, sns_db_iter_t **iter, sns_db_ts_t begin_ts,
                    sns_db_ts_t end_ts);

/**
 * @brief Function to purge the memory associated with the fetch results.
 * - This API must be called once the fetch results are no longer
 * required.
 *
 * @param[in]    handle Opaque handle for the Database.
 * @param[inout] iter Pointer to the fetch results iterator
 * received from fetch API.
 *
 * @return
 * -  SNS_RC_SUCCESS == Success, any other value == Failure.
 *
 */
sns_rc sns_db_purge(void *handle, sns_db_iter_t **iter);
/**
 * @brief Function to move the iterator to the head or tail of the list
 * and get the respective item from the fetch results.
 *
 * @param[inout] iter Pointer to the iterator for this database list.
 * @param[in] head Reset the iterator to the first or last element of the list.
 *
 * @return
 *  - sns_db_item_t* NULL == iterator could not be reset to head or tail,
 * Any other value == address of next item.
 *
 */
sns_db_item_t *sns_db_iter_reset(sns_db_iter_t *iter, bool head);

/**
 * @brief Function to move the iterator and get the
 * next (newer) item from the fetch results.
 * The will return failure if iterator is at the end of the list.
 *
 * @param[inout] iter Pointer to the iterator for this database list.
 *
 * @return
 * - sns_db_item_t* NULL == iterator is at end of list,
 * Any other value == address of next item.
 *
 */
sns_db_item_t *sns_db_iter_advance(sns_db_iter_t *iter);

/**
 * @brief Function to move the iterator and get the
 * previous (older) item from the fetch results.
 * The will return failure if iterator is at the start of the list.
 *
 * @param[inout] iter Pointer to the iterator for this database list.
 *
 * @return
 * - sns_db_item_t* NULL == iterator at beginning of list,
 * Any other value == address of previous item.
 */
sns_db_item_t *sns_db_iter_return(sns_db_iter_t *iter);

/**
 * @brief Function to get item at the iterators
 * current offset from the fetch results.
 *
 * @param[in] iter Pointer to the iterator for this database list.
 *
 * @return
 * - sns_db_item_t* NULL == Failure,
 * Any other value == address of requested item.
 *
 */
sns_db_item_t *sns_db_iter_curr(sns_db_iter_t *iter);

/**
 * @brief Function to get the number of items in the fetch results.
 *
 * @param[in] iter Pointer to the iterator for this database list.
 *
 * @return
 *  - int32_t Number of items that this iterator iterates over.
 * Error = -1.
 *
 */
int32_t sns_db_iter_count(sns_db_iter_t *iter);
