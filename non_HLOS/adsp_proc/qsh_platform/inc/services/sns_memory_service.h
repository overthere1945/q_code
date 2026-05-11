#pragma once
/** =============================================================================
 * @file
 *
 * @brief Service for sensor memory management
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdint.h>
#include "sns_rc.h"
#include "sns_service.h"
#include "sns_sensor_instance.h"

/*=============================================================================
  Macro constants
  ===========================================================================*/

/** ======================== Version History ===================================
 *  @brief Version  comments
 *  1        Initial version
 *  2        Added support for API: "register_island_memory_vote_cb()"
 *  3        New API support for allocating memory in island
 *  4        Garbage collection implemented
 *  5        Added new API to query memory pool information.
 *
 */
#define SNS_MEMORY_SERVICE_VERSION (5)

/**
 * @brief Maximum character length of the memory pool name.
 *
 */
#define SNS_MAX_MEM_POOL_NAME_LEN (255)

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief Island memory pool vote callback function prototype.
 *
 * @param [in] sensor     Sensor pointer.
 * @param [in] enable     - True: add vote for sensor island memory pool.
 *                        - False: remove vote for sensor island memory pool.
 *
 * @return
 *  - SNS_RC_SUCCESS   Success.
 *  - Any other value  Failure.
 *
 */
typedef sns_rc (*sns_island_memory_vote_cb)(sns_sensor *const sensor,
                                            bool enable);

/**
 * @brief The Sensor Memory Manager.
 * Will be obtained from sns_service_manager::get_service.
 *
 */
typedef struct sns_memory_service
{
  sns_service service;                      /*!<  Service information. */
  const struct sns_memory_service_api *api; /*!<  Public api provided by
                                             *    the framework to be used by
                                             *    the sensor.*/
} sns_memory_service;

/**
 * @brief Sensor Memory Service APIs.
 *
 */
typedef struct sns_memory_service_api
{
  uint32_t struct_len; /*!< Length of sns_memory_service_api structure. */

  /**
   * @brief Allocates memory from big image memory and returns a pointer to it.
   *
   * @param [in] instance Sensor Instance.
   * @param [in] size     Number of bytes required.
   *
   * @return
   * - void*  Pointer to the memory if successful.
   * - NULL   If alloc failed.
   *
   */
  void *(*malloc)(struct sns_sensor_instance *instance, size_t size);

  /**
   * @brief Frees a block of memory.
   *
   * @param [in] instance Sensor Instance.
   * @param [in] ptr      Pointer to memory block to be freed.
   *
   * @return
   *  - None.
   *
   */
  void (*free)(struct sns_sensor_instance *instance, void *ptr);

  /**
   * @brief Register callback function to vote for sensor island memory pool.
   * Sensor framework calls this registered callback function for an
   * island sensor in following scenarios,
   * 1) Before delivering first request to the sensor.
   * 2) After removal of all requests on a sensor.
   * Sensor can deregister the callback function by passing
   * "sns_island_memory_vote_cb" parameter as NULL.
   *
   * @param [in] Sensor:   Sensor pointer.
   * @param [in] cb:       Sensor callback function of type
   *                       "sns_island_memory_vote_cb".
   *
   * @return
   *  - None.
   *
   */
  void (*register_island_memory_vote_cb)(sns_sensor *const sensor,
                                         sns_island_memory_vote_cb cb);

  /**
   * @brief Allocates memory from island memory.
   *
   * @param [in] instance Sensor Instance.
   * @param [in] size     Number of bytes required.
   *
   * @return
   *  - void* Pointer to the memory if successful.
   *  - NULL  if alloc failed.
   *
   */
  void *(*malloc_island)(struct sns_sensor_instance *instance, size_t size);

  /**
   * @brief Returns name of the memory pool where sensor is allocated.
   *
   * @param [in]     sensor      Sensor pointer.
   * @param [inout]  dst_buffer  Destination character buffer pointer.
   * @param [in]     dst_size    Size of the destination buffer in bytes.
   *
   * @return
   *  - SNS_RC_SUCCESS       Success.
   *  - SNS_RC_INVALID_VALUE Either sensor/dst_buffer is null or not enough
   *                         memory in the pool.
   *  - SNS_RC_NOT_SUPPORTED Unsupported memory segment.
   *
   */
  sns_rc (*get_memory_pool_name)(struct sns_sensor *sensor, char *dst_buffer,
                                 size_t dst_size);

  /**
   * @brief Allocates memory from big image memory and returns a pointer to it.
   *
   * @param [in] sensor   Sensor pointer.
   * @param [in] size     Number of bytes required.
   *
   * @return
   * - void*  Pointer to the memory if successful.
   * - NULL   If alloc failed.
   *
   */
  void *(*sensor_malloc)(struct sns_sensor *sensor, size_t size);

  /**
   * @brief Frees a block of memory.
   *
   * @param [in] sensor   Sensor pointer.
   * @param [in] ptr      Pointer to memory block to be freed.
   *
   * @return
   *  - None.
   *
   */
  void (*sensor_free)(struct sns_sensor *sensor, void *ptr);

  /**
   * @brief Allocates memory from island memory when sensor is coming from island.
   *        otherwise, memory will be allocated in the big image.
   *
   * @param [in] sensor   Sensor pointer.
   * @param [in] size     Number of bytes required.
   * @param [in] heap_id  Heap to allocate from .
   *
   * @return
   *  - void* Pointer to the memory if successful.
   *  - NULL  if alloc failed.
   *
   */
  void *(*sensor_malloc_island)(struct sns_sensor *sensor, size_t size);

} sns_memory_service_api;
