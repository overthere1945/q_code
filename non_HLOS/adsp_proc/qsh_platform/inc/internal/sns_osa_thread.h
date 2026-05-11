#pragma once
/** ============================================================================
 *  @file
 *
 *  @brief OS Abstraction layer for threads used in Sensors.
 *
 *  @copyright Copyright (c) Qualcomm Technologies, Inc. and / or its
 *  subsidiaries. All Rights Reserved. Confidential and Proprietary - Qualcomm
 *  Technologies, Inc.
 *  ===========================================================================
 */

/*=============================================================================
  Include Files
  ===========================================================================*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "sns_osa_attr.h"
#include "sns_osa_thread_signal.h"
#include "sns_rc.h"

/*=============================================================================
  Constants
  ===========================================================================*/

#define SNS_OSA_THREAD_NAME_MAX_LEN 15 /*!< Excluding terminating NULL. */
#define SNS_OSA_THREAD_EXIT_SIG     0x80000000 /*!< Reserved MSB for exit.*/

/*=============================================================================
  Type Definitions
  ===========================================================================*/
struct sns_thread_utilization;
struct sns_watchdog_voter;

typedef struct sns_osa_thread sns_osa_thread; /*!< OS dependent thread */

/**
 * @brief Opaque structure for OS dependent thread attribute.
 *
 */
typedef union
{
  char __size[__SIZEOF_ATTR_THREAD]; /*!< Size of attr. */
  long int __alignment;
} sns_osa_thread_attr;

/**
 * @brief Structure which stores worker thread id and maps it to array index.
 *
 */
typedef struct sns_thread_map
{
  int32_t thread_id; /*!< Thread identifier. */
} sns_thread_map;

/**
 * @brief Argument type for thread function.
 *
 */
typedef void *sns_osa_thread_func_arg;

/**
 * @brief Signature of thread function.
 *
 */
typedef void (*sns_osa_thread_func)(sns_osa_thread_func_arg);
/*=============================================================================
  Public Functions
  ===========================================================================*/

/**
 * @brief Creates a thread attribute structure initialized with defaults.
 *
 * @param[inout] attrib      The attribute structure.
 *
 * @return
 * - None.
 *
 */
void sns_osa_thread_attr_init(sns_osa_thread_attr *attrib)
    __attribute__((nonnull));

/**
 * @brief Sets the thread stack.
 *
 * @param[inout] attrib      The attribute structure.
 * @param[in] stack_start    Start of the preallocated stack; if NULL, stack
 *                           will be allocated from heap
 * @param[in] stack_size     Stack size in bytes.
 *
 * @return
 * - None.
 *
 */
void sns_osa_thread_attr_set_stack(sns_osa_thread_attr *attrib,
                                   uintptr_t stack_start, size_t stack_size)
    __attribute__((nonnull(1)));

/**
 * @brief Sets the kernel stack size attribute for island threads.
 *
 * @note This has no effect on big-image threads. Default kernal stack size is
 * 2k.
 *
 * @param[inout] attrib            The attribute structure.
 * @param[in] kernel_stack_size    Stack size in bytes.
 *
 * @return
 *   - SNS_RC_SUCCESS:            Stack attribute set.
 *   - SNS_RC_NOT_SUPPORTED:      The current OS doesn't support this.
 *
 */
sns_rc sns_osa_thread_attr_set_kernel_stack_size(sns_osa_thread_attr *attrib,
                                                 size_t kernel_stack_size)
    __attribute__((nonnull));

/**
 * @brief Sets the thread priority attribute.
 *
 * @param[inout] attrib       The attribute structure.
 * @param[in] priority        Thread priority.
 *
 * @return
 * - SNS_RC_SUCCESS:            Priority attribute set.
 * - SNS_RC_NOT_SUPPORTED:      Priority attribute is not supported.
 * - SNS_RC_INVALID_VALUE:      The given priority is invalid.
 *
 */
sns_rc sns_osa_thread_attr_set_priority(sns_osa_thread_attr *attrib,
                                        uint8_t priority)
    __attribute__((nonnull));

/**
 * @brief Sets the thread name attribute.
 *
 * @param[inout] attrib        The attribute structure.
 * @param[in] name             Name.
 *
 * @return
 * - SNS_RC_SUCCESS:            Name attribute set.
 * - SNS_RC_INVALID_VALUE:      The given priority is invalid.
 *
 */
sns_rc sns_osa_thread_attr_set_name(sns_osa_thread_attr *attrib,
                                    const char *name) __attribute__((nonnull));

/**
 * @brief Specifies the memory type where the thread will be located.
 *
 * @param[inout] attrib        The attribute structure.
 * @param[in]  mem_type        The memory type.
 *
 * @return
 *  - SNS_RC_SUCCESS:            Memory partition attribute set.
 *  - SNS_RC_NOT_SUPPORTED:      Memory partition attribute is not supported.
 *
 */
sns_rc sns_osa_thread_attr_set_memory_partition(sns_osa_thread_attr *attrib,
                                                sns_osa_mem_type mem_type)
    __attribute__((nonnull));

/**
 * @brief Assigns user defined information to be stored with a thread.
 *
 * @param[inout] attrib       The attribute structure.
 * @param[in]  info           The user defined info associated with thread.
 *
 * @return
 * - SNS_RC_SUCCESS:            User data was set.
 * - SNS_RC_NOT_SUPPORTED:      User data attribute is not supported.
 *
 */
sns_rc sns_osa_thread_attr_set_user_info(sns_osa_thread_attr *attrib,
                                         void *info)
    __attribute__((nonnull(1)));

/**
 * @brief  Creates a new thread.
 *
 * @param[in] thread_func           Thread main function.
 * @param[in] thread_func_arg       Input parameter to thread_func.
 * @param[in] attrib                The initialized attribute structure.
 * @param[out] thread               Destination for the newly created thread.
 *
 * @return
 *   - SNS_RC_SUCCESS:              New thread successfully created.
 *   - SNS_RC_RESOURCE_UNAVAIL:     No memory.
 *
 */
sns_rc sns_osa_thread_create(sns_osa_thread_func thread_func,
                             sns_osa_thread_func_arg thread_func_arg,
                             const sns_osa_thread_attr *attrib,
                             sns_osa_thread **thread)
    __attribute__((nonnull(1, 3, 4)));

/**
 *
 * @brief  Creates a new thread.
 *
 * @note Prerequisite to call this API: Update thread configuration in\n
 * "/ssc/utils/osa/chipset/${CHIPSET}/sns_thread_table.c"\n
 * "/ssc/inc/internal/${CHIPSET}/sns_thread_table.h"\n
 * This API populates thread attribute configuration from the sns_thread_table.
 *
 * @param[in] thread_name         Name for the thread. (Up to
 *                                 SNS_OSATHREAD_NAME_MAX_LEN characters)
 * @param[in] thread_func         Thread main function.
 * @param[in] thread_func_arg     Input parameter to thread_func.
 * @param[out] thread             Destination for the newly created thread.
 *
 * @return
 * - SNS_RC_SUCCESS:              New thread successfully created.
 * - SNS_RC_NOT_AVAILABLE:        If thread entry missing in the
 *                                sns_thread_table.
 *
 */

sns_rc sns_osa_thread_create_v2(char *thread_name,
                                sns_osa_thread_func thread_func,
                                sns_osa_thread_func_arg thread_func_arg,
                                sns_osa_thread **thread)
    __attribute__((nonnull(1, 2, 4)));

/**
 * @brief Deletes the given thread.
 *
 * @param[in] thread                The thread to delete.
 *
 * @return
 * - SNS_RC_SUCCESS:            Given thread deleted.
 * - SNS_RC_INVALID_VALUE:      Given thread not found.
 *
 */
sns_rc sns_osa_thread_delete(sns_osa_thread *thread);

/**
 * @brief Waits for the given signals in the given thread.
 *
 * @note  The received signals are consumed.
 *
 * @param[in] sigs_mask             The signals for which to wait.
 * @param[out] sigs_rcvd            Destination for signals set when thread
 *                                  wokeup.
 * @param[in] thread                Thread pointer of the thread calling this
 *                                  function.
 *
 * @return
 *   - SNS_RC_SUCCESS:              One or more signals received.
 *   - SNS_RC_INVALID_VALUE:        One or more input parameters are invalid.
 *
 */
sns_rc sns_osa_thread_sigs_wait(sns_osa_thread_sigs sigs_mask,
                                sns_osa_thread_sigs *sigs_rcvd,
                                sns_osa_thread *thread);

/**
 * @brief Sets the given signals to wake up the given thread.
 *
 * @param[in] thread                The thread.
 * @param[in] sigs                  The signals to set.
 *
 * @return
 *   - SNS_RC_SUCCESS:              Signals set.
 *   - SNS_RC_INVALID_VALUE:        One or more input parameters are invalid.
 *
 */
sns_rc sns_osa_thread_sigs_set(sns_osa_thread *thread,
                               sns_osa_thread_sigs sigs);

/**
 * @brief Checks the currently running thread for any pending signals.
 *
 * @param[in] sigs_mask             The signals for which to check.
 * @param[in] consume               Whether to consume/clear the signals.
 * @param[out] sigs_rcvd            Destination for pending signals.
 *
 * @return
 *   - SNS_RC_SUCCESS:              One or more signals received.
 *   - SNS_RC_INVALID_VALUE:        One or more input parameters are invalid.
 *
 */
sns_rc sns_osa_thread_sigs_check(sns_osa_thread_sigs sigs_mask, bool consume,
                                 sns_osa_thread_sigs *sigs_rcvd);

/**
 * @brief Returns the current thread's priority.
 *
 * @return
 *   - uint8_t:   The current thread's priority.
 *
 */
uint8_t sns_osa_thread_get_priority(void);

/**
 * @brief Sets the current thread's priority to the given priority.
 *
 * @param[in] priority         The priority to set.
 *
 * @return
 *   - uint8_t:   The priority before it was changed.
 *
 */
uint8_t sns_osa_thread_set_priority(uint8_t priority);

/**
 * @brief Gets the user defined information that was stored with current thread.
 *
 * @return
 * - Pointer to user defined information if user information was set.
 * - NULL:    Otherwise.
 *
 */
void *sns_osa_thread_get_user_info(void);

/**
 * @brief Gets the thread id of the currently running thread.
 *
 * @return
 *   - int:   Thread ID.
 *
 */
int sns_osa_thread_get_thread_id(void);

/**
 * @brief Gets the thread id of the thread whose name is @a thread_name.
 *
 * @param[in] thread_name       Name of the thread.
 *
 * @return
 * - int:   Thread id if found.
 * - -1:    Otherwise.
 *
 */
int sns_osa_thread_get_thread_id_by_name(char *thread_name)
    __attribute__((nonnull));

/**
 * @brief Gets the process id of the current thread.
 *
 * @return
 *   - int:  Process ID.
 *
 */
int sns_osa_thread_get_process_id(void);

/**
 * @brief Initializes all OSA global state.
 *
 * @return
 * - SNS_RC_SUCCESS:   On success.
 * - SNS_RC_FAILED:    Otherwise.
 *
 */
sns_rc sns_osa_init(void);

/**
 * @brief Marks the currently running thread as active.
 *
 * @param[in] wd_voter      Watchdog voter.
 *
 * @return
 * - None.
 *
 */
void sns_osa_thread_active(struct sns_watchdog_voter *wd_voter);

/**
 * @brief Marks the currently running thread as idle.
 *
 * @param[in] wd_voter      Watchdog voter.
 *
 * @return
 *    - None.
 *
 */
void sns_osa_thread_idle(struct sns_watchdog_voter *wd_voter);

/**
 * @brief Returns count of how many times all threads went to idle.
 *
 * @return
 *   - uint64_t:   Active-to-idle transition count.
 *
 */

uint64_t sns_osa_thread_get_active_to_idle_transitions_count(void);

/**
 * @brief Gets the currently running thread.
 *
 * @return
 *   - sns_osa_thread:    The currently running thread.
 *
 */
sns_osa_thread *sns_osa_get_current_thread(void);

/**
 * @brief Wrapper function for sns_osa_thread_sigs_wait().
 * This function calls sns_osa_thread_idle() before waiting on signals and calls
 * sns_osa_thread_active() once the signal wait is completed.
 *
 * @param[in] sigs_mask         The signals for which to wait.
 * @param[out] sigs_rcvd        Destination for signals set when thread woke up.
 * @param[in] thread            The thread calling this function.
 *
 * @return
 * - SNS_RC_SUCCESS:        One or more signals received.
 * - SNS_RC_INVALID_VALUE:  One or more input parameters are invalid.
 *
 */
sns_rc sns_osa_thread_sigs_wait_wrapper(sns_osa_thread_sigs sigs_mask,
                                        sns_osa_thread_sigs *sigs_rcvd,
                                        sns_osa_thread *thread);

/**
 * @brief Wrapper function for sns_osa_thread_signal_wait().
 * This function calls sns_osa_thread_idle() before waiting on signals and calls
 * sns_osa_thread_active() once the signal wait is completed.
 *
 * @param[in] signal            Pointer to the signal instance.
 * @param[in] mask              The signals for which to wait.
 * @param[in] wd_voter          Watchdog voter.
 *
 * @return
 * - int:     Received signals.
 *
 */
unsigned int
sns_osa_thread_anysignal_wait_wrapper(sns_osa_thread_signal *signal,
                                      unsigned int mask,
                                      struct sns_watchdog_voter *wd_voter);

/**
 * @brief Gets a pointer to the thread utilization item.
 *
 * @param[in] thread      The thread.
 *
 * @return
 * - sns_thread_utilization:   The thread utilization item.
 *
 */
struct sns_thread_utilization *
sns_osa_thread_get_thread_utilization_item(sns_osa_thread *thread);

/**
 * @brief Adds a thread id to worker thread map.
 *
 * @note This function only works on low priority threads.
 *
 * @param[in] thread            The thread.
 * @param[in] index             The thread index for the thread.
 *
 * @return
 *   - SNS_RC_SUCCESS:              Thread id was added to map.
 *   - SNS_RC_INVALID_VALUE:        Index parameter is invalid.
 *
 */
sns_rc sns_osa_thread_update_worker_thread_map(sns_osa_thread *thread,
                                               uint8_t index);

/**
 * @brief Gets the thread map index from current running thread.
 *
 * @return
 * - int8_t: The thread map index.
 *
 */
int8_t sns_osa_thread_get_worker_thread_index(void);

/**
 * @brief Gets the watchdog voter for given thread.
 *
 * @param[in] thread            The thread.
 *
 * @return
 * - sns_watchdog_voter:  Pointer to the watchdog voter for the given thread.
 *
 */
struct sns_watchdog_voter *sns_osa_thread_get_wd_voter(sns_osa_thread *thread);

/**
 * @brief Gets the thread signal for given thread.
 *
 * @param[in] thread            The thread.
 *
 * @return
 * - sns_osa_thread_signal:  Pointer to the thread signal for the given thread.
 *
 */
sns_osa_thread_signal *sns_osa_thread_get_thread_signal(sns_osa_thread *thread);

/**
 * @brief Gets the count of currently active ssc threads.
 *
 * @return
 *    - uint32_t:   Number of active ssc threads.
 *
 */
uint32_t sns_osa_get_active_threads_count(void);

/**
 * @brief Acquires active threads count lock.
 *
 * @return
 *   - None.
 *
 */
void sns_osa_acquire_active_threads_count_lock(void);

/**
 * @brief Releases active threads count lock.
 *
 * @return
 *   - None.
 *
 */
void sns_osa_release_active_threads_count_lock(void);

/**
 * @brief Gets number of hardware threads
 *
 * @return Number of hardware threads. 0 if not supported.
 *
 */
uint32_t sns_osa_thread_get_hardware_thread_count(void);
