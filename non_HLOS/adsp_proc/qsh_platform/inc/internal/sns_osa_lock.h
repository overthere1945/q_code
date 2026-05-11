#pragma once
/** ============================================================================
 *  @file
 *
 *  @brief OS Abstraction layer for locking mechanism used in Sensors.
 *
 *  @copyright Copyright (c) Qualcomm Technologies, Inc. and / or its
 *  subsidiaries. All Rights Reserved. Confidential and Proprietary - Qualcomm
 *  Technologies, Inc.
 *  ===========================================================================
 */
/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdint.h>
#include "sns_osa_attr.h"
#include "sns_rc.h"

/*=============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * @brief OS dependent locking mechanism.
 *
 */
typedef union
{
  char __size[__SIZEOF_LOCK];
  long int __alignment;
} sns_osa_lock __attribute__((aligned(8)));

/**
 * @brief OS dependent lock attributes.
 *
 */
typedef union
{
  char __size[__SIZEOF_ATTR_LOCK];
  long int __alignment;
} sns_osa_lock_attr;

/*=============================================================================
  Public Functions
  ===========================================================================*/

/**
 *  @brief Initializes the given lock attribute structure with defaults.
 *
 *  @param [in,out] attrib        The attribute structure.
 *
 *  @return
 *   - SNS_RC_SUCCESS:            Attribute structure initialized.
 *   - SNS_RC_INVALID_VALUE:      Input parameter is invalid.
 *
 */
sns_rc sns_osa_lock_attr_init(sns_osa_lock_attr *attrib)
    __attribute__((nonnull));

/**
 *
 *  @brief Specifies the memory type where the lock will be located.
 *
 *  @param [inout] attrib      The attribute structure.
 *  @param [in]  mem_type      The memory type.
 *
 *  @return
 *   - SNS_RC_SUCCESS:            Memory partition attribute set.
 *   - SNS_RC_NOT_SUPPORTED:      Memory partition attribute is not supported.
 *   - SNS_RC_INVALID_VALUE:      One or more input parameters are invalid.
 *
 */
sns_rc sns_osa_lock_attr_set_memory_partition(sns_osa_lock_attr *attrib,
                                              sns_osa_mem_type mem_type)
    __attribute__((nonnull));

/**
 *
 *  @brief  Allocates memory for a new lock and initializes it.
 *
 *  @param [in] attrib          The initialized attribute structure.
 *  @param [out] lock           Destination for the newly created lock.
 *
 *  @return
 *   - SNS_RC_SUCCESS:              New lock successfully created.
 *   - SNS_RC_NOT_AVAILABLE:        No memory.
 *   - SNS_RC_INVALID_VALUE:        One or more input pointers are NULL.
 *
 */
sns_rc sns_osa_lock_create(const sns_osa_lock_attr *attrib, sns_osa_lock **lock)
    __attribute__((nonnull));

/**
 *  @brief  Initializes the given lock.
 *
 *  @param [in] attrib           The initialized attribute structure.
 *  @param [inout] lock          The lock to initialize.
 *
 *  @retval SNS_RC_SUCCESS:       New lock successfully initialized.
 *
 */
sns_rc sns_osa_lock_init(const sns_osa_lock_attr *attrib, sns_osa_lock *lock)
    __attribute__((nonnull));

/**
 *  @brief  Deinit the given lock and free the memory.
 *
 *  @param [in] lock              The lock to deinitialize.
 *
 *  @return
 *   - SNS_RC_SUCCESS:              The given lock deleted.
 *   - SNS_RC_INVALID_VALUE:        Not a valid lock.
 *
 */
sns_rc sns_osa_lock_delete(sns_osa_lock *lock);

/**
 *  @brief  Deinit the given lock
 *
 *  @param [in] lock        The lock to deinitialize.
 *
 *  @return
 *   - SNS_RC_SUCCESS:              The given lock deleted
 *   - SNS_RC_INVALID_VALUE:        Not a valid lock
 *
 */
sns_rc sns_osa_lock_deinit(sns_osa_lock *lock);

/**
 *  @brief  Acquires the given lock
 *
 *  @param [in] lock              The lock.
 *
 *  @return
 *   - SNS_RC_SUCCESS:              The given lock acquired
 *   - SNS_RC_INVALID_VALUE:        Not a valid lock
 *
 *  @note Blocks until lock is acquired
 *
 */
sns_rc sns_osa_lock_acquire(sns_osa_lock *lock);

/**
 *  @brief  Tries to acquires the given lock.
 *
 *  @note Returns immediately if lock cannot be acquired.
 *
 *  @param [in] lock              The lock.
 *
 *  @return
 *   - SNS_RC_SUCCESS:              The given lock acquired.
 *   - SNS_RC_NOT_AVAILABLE:        Unable to acquire lock.
 *   - SNS_RC_INVALID_VALUE:        Not a valid lock.
 *
 */
sns_rc sns_osa_lock_try_acquire(sns_osa_lock *lock);

/**
 *  @brief  Releases the given lock.
 *
 *  @param [in] lock              The lock.
 *
 *  @return
 *   - SNS_RC_SUCCESS:              The given lock released.
 *   - SNS_RC_INVALID_VALUE:        Not a valid lock.
 *
 */
sns_rc sns_osa_lock_release(sns_osa_lock *lock);
