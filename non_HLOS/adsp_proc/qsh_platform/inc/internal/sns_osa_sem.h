#pragma once
/** ============================================================================
 *  @file
 *
 *  @brief OS Abstraction layer for semaphores used in Sensors.
 *
 *  @copyright Copyright (c) Qualcomm Technologies, Inc. and / or its
 *  subsidiaries. All Rights Reserved. Confidential and Proprietary - Qualcomm
 *  Technologies, Inc.
 *  ===========================================================================
 */
/*
********************************************************************************
                               Includes
********************************************************************************
*/
#include <stdint.h>
#include "sns_osa_attr.h"
#include "sns_rc.h"

/*
********************************************************************************
                               Typedefs
********************************************************************************
*/
/**
 * @brief OS dependent semaphore structure
 *
 */
typedef struct sns_osa_sem sns_osa_sem;

/**
 * @brief OS dependent semaphore attributes
 *
 */
typedef union
{
  char __size[__SIZEOF_ATTR_SEM]; /*!< Size of the structure. */
  long int __alignment;
} sns_osa_sem_attr;

/*
********************************************************************************
                               Functions
********************************************************************************
*/
/**
 *  @brief Creates a semaphore attribute structure initialized with defaults.
 *
 *  @param [inout] attrib         The attribute structure.
 *
 *  @return
 *    - None.
 *
 */
void sns_osa_sem_attr_init(sns_osa_sem_attr *attrib) __attribute__((nonnull));

/**
 *  @brief Sets the semaphore initial value.
 *
 *  @param [inout] attrib      The attribute structure.
 *  @param [in] value          Initial value.
 *
 *  @return
 *     - None.
 *
 */
void sns_osa_sem_attr_set_value(sns_osa_sem_attr *attrib, int32_t value)
    __attribute__((nonnull));

/**
 *  @brief Specifies the memory type where the semaphore will be located.
 *
 *  @param [inout] attrib         The attribute structure.
 *  @param [in]  mem_type         The memory type.
 *
 *  @return
 *   - SNS_RC_SUCCESS:           Memory partition attribute set.
 *   - SNS_RC_NOT_SUPPORTED:     Memory partition attribute is not supported.
 *
 */
sns_rc sns_osa_sem_attr_set_memory_partition(sns_osa_sem_attr *attrib,
                                             sns_osa_mem_type mem_type)
    __attribute__((nonnull));

/**
 *  @brief  Creates a new semaphore.
 *
 *  @param [in] attrib           Semaphore attribute.
 *  @param [out] sem             Destination for the newly created semaphore.
 *
 *  @return
 *   - SNS_RC_SUCCESS:             New semaphore successfully created.
 *   - SNS_RC_NOT_AVAILABLE:       No memory available in the specified region.
 *
 */
sns_rc sns_osa_sem_create(const sns_osa_sem_attr *attrib, sns_osa_sem **sem)
    __attribute__((nonnull));

/**
 *  @brief  Deletes the given semaphore.
 *
 *  @param [inout] sem          The semaphore.
 *
 *  @return
 *     - None.
 *
 */
void sns_osa_sem_delete(sns_osa_sem **sem) __attribute__((nonnull));

/**
 *  @brief  Waits on the given semaphore.
 *
 *  @note
 *  Blocks until semaphore value is greater than or equal to 0.
 *
 *  @param [in] sem        The semaphore.
 *
 *  @return
 *    - None.
 *
 */
void sns_osa_sem_wait(sns_osa_sem *sem) __attribute__((nonnull));

/**
 *  @brief  Increments value of the given semaphore and wakes any threads
 *  waiting on this semaphore.
 *
 *  @param [in] sem           The semaphore.
 *
 *  @return
 *    - None.
 *
 */
void sns_osa_sem_post(sns_osa_sem *sem) __attribute__((nonnull));

/**
 *  @brief  Gets the semaphore count value
 *
 *  @param [in] sem                The semaphore
 *
 *  @return
 *  - int32_t:    Semaphore count value.
 *  -  -1:        If given semaphore is invalid
 *
 */
int32_t sns_osa_sem_get_value(sns_osa_sem *sem) __attribute__((nonnull));
