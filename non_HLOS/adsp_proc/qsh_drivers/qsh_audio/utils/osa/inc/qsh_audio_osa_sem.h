#pragma once

/*=======================================================================================
  @file qsh_audio_osa_sem.h

  @brief
  OS Abstraction layer for Sensors.

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  =====================================================================================*/

/*
*****************************************************************************************
                               Includes
*****************************************************************************************
*/
#include <stdint.h>
#include "qsh_audio_osa_attr.h"
#include "sns_rc.h"

/*
*****************************************************************************************
                               Typedefs
*****************************************************************************************
*/
typedef struct qsh_audio_osa_sem       qsh_audio_osa_sem;       /**< OS dependent semaphore structure */

typedef union
{
  char                   __size[__SIZEOF_ATTR_SEM]; /**< OS dependent */
  long int               __alignment;
} qsh_audio_osa_sem_attr;

/*
*****************************************************************************************
                               Functions
*****************************************************************************************
*/
/*!
  -----------------------------------------------------------------------------
  qsh_audio_osa_sem_attr_init
  -----------------------------------------------------------------------------

  @brief Creates a semaphore attribute structure initialized with defaults

  @param [io] attrib        the attribute structure

  @return
  SNS_RC_SUCCESS            attribute structure initialized
  SNS_RC_FAILED             input parameter is invalid
  -----------------------------------------------------------------------------
*/
sns_rc qsh_audio_osa_sem_attr_init(
  qsh_audio_osa_sem_attr*      attrib);


/*!
  -----------------------------------------------------------------------------
  qsh_audio_osa_sem_attr_set_value
  -----------------------------------------------------------------------------

  @brief Sets the semaphore initial value

  @param [io] attrib        the attribute structure
  @param [i] value          initial value

  @return
  SNS_RC_SUCCESS            stack attribute set
  SNS_RC_FAILED             one or more input parameters are invalid
  -----------------------------------------------------------------------------
*/
sns_rc qsh_audio_osa_sem_attr_set_value(
  qsh_audio_osa_sem_attr      *attrib,
  int32_t               value);


/*!
  -----------------------------------------------------------------------------
  qsh_audio_osa_sem_attr_set_memory_partition
  -----------------------------------------------------------------------------

  @brief Specifies the memory type where the semaphore will be located

  @param [io] attrib        the attribute structure
  @param [i]  mem_type      the memory type

  @return
  SNS_RC_SUCCESS            memory partition attribute set
  SNS_RC_NOT_SUPPORTED      memory partition attribute is not supported
  SNS_RC_FAILED             one or more input parameters are invalid
  -----------------------------------------------------------------------------
*/
sns_rc qsh_audio_osa_sem_attr_set_memory_partition(
  qsh_audio_osa_sem_attr      *attrib,
  qsh_audio_osa_mem_type      mem_type);


/*!
  ---------------------------------------------------------------------------------------
  qsh_audio_osa_sem_create
  ---------------------------------------------------------------------------------------

  @brief  Creates a new semaphore

  @param [i] mem_type         the memory type
  @param [i] value            initial value of the semaphore
  @param [o] sem              destination for the newly created semaphore

  @return
  SNS_RC_SUCCESS              new semaphore successfully created
  SNS_RC_FAILED               one or more input pointers are invalid
  SNS_RC_NOT_AVAILABLE        no memory available in the specified region
  ---------------------------------------------------------------------------------------
*/
sns_rc qsh_audio_osa_sem_create(
  const qsh_audio_osa_sem_attr *attrib,
  qsh_audio_osa_sem**          sem);


/*!
  ---------------------------------------------------------------------------------------
  qsh_audio_osa_sem_delete
  ---------------------------------------------------------------------------------------

  @brief  Deletes the given semaphore

  @param [i] sem              the semaphore

  @return
  SNS_RC_SUCCESS              the given semaphore deleted
  SNS_RC_FAILED               one or more input pointers are invalid
  ---------------------------------------------------------------------------------------
*/
sns_rc qsh_audio_osa_sem_delete(
  qsh_audio_osa_sem*           sem);


/*!
  ---------------------------------------------------------------------------------------
  qsh_audio_osa_sem_wait
  ---------------------------------------------------------------------------------------

  @brief  Wait on the given semaphore

  @param [i] sem              the semaphore

  @return
  SNS_RC_SUCCESS              the given semaphore acquired
  SNS_RC_FAILED               one or more input pointers are invalid

  @note
  Blocks until semaphore value is greater than or equal to 0
  ---------------------------------------------------------------------------------------
*/
sns_rc qsh_audio_osa_sem_wait(
  qsh_audio_osa_sem*         sem);


/*!
  ---------------------------------------------------------------------------------------
  qsh_audio_osa_sem_post
  ---------------------------------------------------------------------------------------

  @brief  Increments value of the given semaphore and wakes any threads waiting on
          this semaphore

  @param [i] sem              the semaphore

  @return
  SNS_RC_SUCCESS              the given semaphore released
  SNS_RC_FAILED               one or more input pointers are invalid
  ---------------------------------------------------------------------------------------
*/
sns_rc qsh_audio_osa_sem_post(
  qsh_audio_osa_sem*         sem);


/*!
  ---------------------------------------------------------------------------------------
  qsh_audio_osa_sem_get_val
  ---------------------------------------------------------------------------------------

  @brief  Gets the semaphore count value

  @param [i] sem              the semaphore

  @return
  semaphore count value
  ---------------------------------------------------------------------------------------
*/
int32_t qsh_audio_osa_sem_get_value(
  qsh_audio_osa_sem*             sem);
