#pragma once
/** ===========================================================================
 * @file
 *
 * @brief This file contains the sensors thread
 *               stack size definitions.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/
#ifndef SNS_THREAD_DEFAULT_KERNEL_STACK_SIZE
/** @brief Default Stack size */
#define SNS_THREAD_DEFAULT_KERNEL_STACK_SIZE 2048

#endif

/** @brief Stack size for signal thread */
#define SNS_SIG_THREAD_STACK_SIZE        (2 * 1024)
/** @brief Stack size for signal thread kernel stack size*/
#define SNS_SIG_THREAD_KERNEL_STACK_SIZE SNS_THREAD_DEFAULT_KERNEL_STACK_SIZE

/** @brief Stack size for signal sensor thread */
#define SNS_SIG_SENSOR_THREAD_STACK_SIZE (2 * 1024)
/** @brief Stack size for signal sensor thread kernel stack size*/
#define SNS_SIG_SENSOR_THREAD_KERNEL_STACK_SIZE                                \
  SNS_THREAD_DEFAULT_KERNEL_STACK_SIZE
