#pragma once
/** ============================================================================
 * @file
 *
 * @brief Functions used by dynamically loaded sensors.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Include Files
  ============================================================================*/
#include <stddef.h>
#include <stdint.h>
#include "sns_sensor.h"

/**
 * @brief Maximum number of clients that can be connected to the dynamic loader.
 *
 */
#define SNS_DL_MAX_REMOTE_CLIENTS 64

struct sns_dl_entry;
struct load_library_config;
struct sns_dl_handle;

/**
 * @brief Definition for note_type to include version note in shared object.
 *
 */
typedef struct sns_note_type
{
  int32_t sizename; /*!< Size of the NOTE section. */
  int32_t sizedesc; /*!< Size of the descriptor(unused). */
  int32_t type;     /*!< Type of section(unused). */
  char name[100];   /*!< Name of NOTE section(version of shared object). */
} sns_note_type;

/**
 * @brief Exported wrapper function around pram_acquire_partition().
 * @param[in] name PRAM name.
 * @param[in] size PRAM size.
 *
 * @return
 *  - None.
 *
 */
void *sns_pram_acquire_partition(char *name, uint32_t *size);

/**
 * @brief Load shared object in sns_dllib_list.
 *
 * @param[in] this Sensor pointer.
 *
 * @return
 *  - None.
 *
 */
bool sns_load_shared_object(sns_sensor *const this);

/**
 * @brief Initializes dynamic loader.
 *
 * @return
 * - SNS_RC_SUCCESS Success.
 * - SNS_RC_FAILED  Failed to initialize.
 *
 */
sns_rc sns_dynamic_loader_init(void);

/**
 * @brief Adds the sensor library specified in dl_entry.
 *
 * @param[in] h Remote handle.
 * @param[in] dl_entry dl entry to add.
 *
 * @return
 * - SNS_RC_SUCCESS Success.
 * - SNS_RC_FAILED  Failed to add.
 *
 */
sns_rc sns_dl_add_sensor_library(uint64_t h,
                                 const struct sns_dl_entry *dl_entry);

/**
 * @brief Adds the generic library specified in load_config.
 *
 * @param[in] h Remote handle.
 * @param[in] load_config Library to add.
 *
 * @return
 * - SNS_RC_SUCCESS Success.
 * - SNS_RC_FAILED  Failed to add.
 *
 */
sns_rc
sns_dl_add_generic_library(uint64_t h,
                           const struct load_library_config *load_config);

/**
 * @brief Removes the library specified in dl_handle.
 *
 * @param[in] h         Remote Handle.
 * @param[in] dl_handle dl handle.
 *
 * @return
 * - SNS_RC_SUCCESS Success.
 * - SNS_RC_FAILED  Failed to remove.
 *
 */
sns_rc sns_dl_remove_library(uint64_t h, const struct sns_dl_handle *dl_handle);

/**
 * @brief Returns a free handle, one not found in active handles.
 *
 * @param[in]  active_handles active remote Handles.
 * @param[out] h              dl handle.
 *
 * @return
 * - SNS_RC_SUCCESS       Success.
 * - SNS_RC_NOT_AVAILABLE Failed to find handle.
 *
 */
sns_rc sns_dl_get_free_client_handle(uint64_t *active_handles, uint64_t *h);

/**
 * @brief Validates whether the given handle is one of the active
 * handles, and optionally release it to free handle pool.
 *
 * @param[in] handle           Remote Handles.
 * @param[in] active_handles   Active remote handle.
 * @param[in] release          Release handle bool.
 *
 * @return
 * - SNS_RC_SUCCESS       Success.
 * - SNS_RC_INVALID_VALUE Failed to find handle.
 *
 */
sns_rc sns_dl_validate_client_handle(uint64_t handle, uint64_t *active_handles,
                                     bool release);

#ifndef SNS_DISABLE_REGISTRY
/**
 * @brief Update dynamic library status.
 *
 * @param[in] library Pointer to the dynamically loaded library.
 * @param[in] is_available  True: if library sensors are available to FW.
 *
 * @return
 *  - None.
 *
 */
void sns_dynamic_sensor_update_lib_status(void const *library,
                                          bool is_available);

/**
 * @brief Intialize island dynamic heap.
 *
 * @return
 *  - None.
 *
 */
void sns_dynl_heap_mgr_init(void);

#else

/**
 * @brief Update dynamic library status.
 *
 * @param[in] library Pointer to the dynamically loaded library.
 * @param[in] is_available  True: if library sensors are available to FW.
 *
 */
#define sns_dynamic_sensor_update_lib_status(library, is_available)

/**
 * @brief Intialize island dynamic heap.
 *
 * @return
 *  - None.
 *
 */
#define sns_dynl_heap_mgr_init()
#endif /* SNS_DISABLE_REGISTRY */