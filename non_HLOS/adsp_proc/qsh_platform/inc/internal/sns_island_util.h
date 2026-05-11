#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Island control for Sensors.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *===========================================================================*/
/*=============================================================================
  Includes
  ===========================================================================*/
#include <stdatomic.h>
#include <stdint.h>

#include "sns_osa_thread.h"
#include "sns_sensor.h"
#include "sns_types.h"
#include "sns_rc.h"

/*============================================================================
  Forward Declarations
  ===========================================================================*/

/* Start, end, and length of island memory */
extern uint32_t __sensors_island_start;
extern uint32_t __sensors_island_end;
extern uintptr_t sns_island_size;

/* Start and end for island sections in simulation */
extern uint32_t __data_island_start;
extern uint32_t __data_island_end;
extern uint32_t __bss_island_start;
extern uint32_t __bss_island_end;
extern uint32_t __rodata_island_start;
extern uint32_t __rodata_island_end;
extern uint32_t __text_island_start;
extern uint32_t __text_island_end;

/* Start and end for big image sections in simulation */
extern uint32_t __data_bi_start;
extern uint32_t __data_bi_end;
extern uint32_t __bss_bi_start;
extern uint32_t __bss_bi_end;
extern uint32_t __rodata_bi_start;
extern uint32_t __rodata_bi_end;
extern uint32_t __text_bi_start;
extern uint32_t __text_bi_end;

/*============================================================================
  Macros
  ===========================================================================*/

/**
 * @brief Time in ticks taken to execute the CB when entering uImage
 *
 */
#define SNS_USLEEP_ISLAND_CB_ENTER_LATENCY_TICKS (1000)

/**
 * @brief Time in ticks taken to execute the CB when exiting uImage
 *
 */
#define SNS_USLEEP_ISLAND_CB_EXIT_LATENCY_TICKS (1000)

/*=============================================================================
  TYPEDEFS
  ===========================================================================*/

/**
 * @brief Island mode enable/disable request values.
 *
 */
typedef enum
{
  QSH_ISLAND_DISABLE = 0, /*!< Island mode disable request. */
  QSH_ISLAND_ENABLE,      /*!< Island mode enable request. */
  QSH_ISLAND_UNKNOWN      /*!< Uninitialized state. */
} qsh_island_vote;

/**
 * @brief Island mode enable/disable request values.
 *
 */
typedef enum
{

  SNS_ISLAND_DISABLE = 0, /*!< Island mode disable request. */
  SNS_ISLAND_ENABLE,      /*!< Island mode enable request */
  SNS_ISLAND_UNKNOWN      /*!< Uninitialized state */
} sns_ssc_island_vote;

/**
 * @brief Island mode states.
 *
 */
typedef enum
{
  SNS_ISLAND_STATE_ISLAND_DISABLED = 0, /*!< Island mode is disabled.
                                         *   It might be enabled later
                                         */
  SNS_ISLAND_STATE_IN_ISLAND,           /*!< Island mode is available. */

  SNS_ISLAND_STATE_NOT_IN_ISLAND /*!< Island mode is not available and
                                  *  will not be available in the future.
                                  */
} sns_island_state;

/**
 * @brief Island configuration.
 *
 */
typedef struct
{
  bool enable_island;       /*!< Island enable flag */
  bool enable_island_debug; /*!< Island debug enable flag */
  bool enable_island_test;  /*!< Island test enable flag */
} sns_island_config;

typedef void *sns_island_client_handle;

/**
 * @brief Island Exit callback function type.
 *
 */
typedef void (*sns_island_exit_cb)(intptr_t args);

/*=============================================================================
  Functions
  ===========================================================================*/

/**
 * @brief Checks if a pointer can be safely dereferenced when in island mode.
 *         Consider NULL to be a valid island pointer.
 *
 * @param[in] ptr        Pointer to be checked.
 *
 * @return
 *  - True:     Pointer can be safely dereferenced when in island mode.
 *  - False:    Otherwise.
 *
 */

__attribute__((section(".text.sns"))) static inline bool
sns_island_is_island_ptr(intptr_t ptr)
{
#ifdef SNS_DISABLE_ISLAND
  UNUSED_VAR(ptr);
  return false;
#elif defined(SSC_TARGET_X86) && defined(SNS_SIM_ISLAND_ENABLE)
  return (((uintptr_t)ptr >= (uintptr_t)&__data_island_start) &&
          ((uintptr_t)ptr <= (uintptr_t)&__data_island_end)) ||
         (((uintptr_t)ptr >= (uintptr_t)&__bss_island_start) &&
          ((uintptr_t)ptr <= (uintptr_t)&__bss_island_end)) ||
         (((uintptr_t)ptr >= (uintptr_t)&__rodata_island_start) &&
          ((uintptr_t)ptr <= (uintptr_t)&__rodata_island_end)) ||
         (((uintptr_t)ptr >= (uintptr_t)&__text_island_start) &&
          ((uintptr_t)ptr <= (uintptr_t)&__text_island_end)) ||
         ((intptr_t)NULL == ptr);
#else
  return ((uintptr_t)ptr - (uintptr_t)&__sensors_island_start <=
          sns_island_size) ||
         ((intptr_t)NULL == ptr);
#endif
}

/**
 * @brief This function checks whether an address falls inside PRAM region.
 *
 * @param [in] ptr Address which needs to be checked.
 *
 * @return
 *  - True:       Address falls in PRAM.
 *  - False:      Otherwise
 *
 */
bool sns_is_in_pram_region(uint32_t ptr);

/**
 * @brief The API to initialize sns_island utility. This API should be called
 *        by framework only once at the bootup.
 *
 * @note  SNS island mode is disabled by default at the end of sns_island_init.
 *
 * @return
 *  - SNS_RC_SUCCESS:              Init was successful.
 *  - SNS_RC_NOT_SUPPORTED:        Island mode not supported.
 *
 */
sns_rc sns_island_init(void);

/**
 * @brief Sensor Island memory pool vote callback function.
 * This API votes for ssc memory pool if atleast one island sensor is
 * active.This API removes ssc memory pool vote if there are no island sensors
 * active.
 *
 * @param [in] sensor  Sensor pointer.
 * @param [in] enable  True: Add vote for sensor island memory pool;
 *                     False: Remove vote for sensor island memory pool.
 * @return
 *  - SNS_RC_SUCCESS: No error occurred; success.
 *  - SNS_RC_FAILED: Internal error occurred.
 *  - SNS_RC_NOT_SUPPORTED: This API is not supported or is not implemented.
 *
 */
sns_rc ssc_island_memory_vote(sns_sensor *const sensor, bool enable);

/**
 * @brief The API to query if ssc island use case is active or not.
 *
 * @return
 *  - True:  If ssc island use case is active.
 *  - False: Otherwise.
 *
 */
bool is_ssc_island_usecase_active(void);

/**
 * @brief The API to initialize qsh_island utility. This API should be called
 *       by framework only once at the bootup.
 *
 * @return
 *  - SNS_RC_SUCCESS:              Init was successful.
 *  - SNS_RC_NOT_SUPPORTED:        Island mode not supported.
 *
 */
sns_rc qsh_island_init(void);

/**
 * @brief The API to Vote to allow island through qsh node.
 *       This API will vote to allow ALL islands.
 *
 * @param[in] active_threads_count    Atomic osa active threads counter.
 *
 * @return
 *   - True:   If island mode was allowed.
 *   - False:  Otherwise.
 *
 */
bool qsh_island_allow(_Atomic unsigned int *active_threads_count);

/**
 * @brief The API to Register a client for the island vote aggregator.
 *
 * @param[in] client_name     Name of the client that is registering.
 *
 * @return
 *   - client_handle:         Client handle used to block or unblock
 *                            island mode.
 *
 */
sns_island_client_handle
sns_island_aggregator_register_client(const char *client_name);

/**
 * @brief The API to Deregister a client from the island vote aggregator.
 *
 * @param[in] client_handle   Client handle to be deregistered.
 *
 * @return
 *    None.
 *
 */
void sns_island_aggregator_deregister_client(
    sns_island_client_handle client_handle);

/**
 * @brief The API to Exit island mode and block re-entry to island mode.
 *
 * @param[in] client_handle  client_handle returned by
 *                           sns_island_aggregator_register_client.
 *
 * @return
 *   - SNS_RC_SUCCESS:        Operation successful.
 *   - SNS_RC_INVALID_TYPE:   Client_handle parameter is bad.
 *   - SNS_RC_NOT_SUPPORTED:  Island mode is not supported.
 *
 */
sns_rc sns_island_block(sns_island_client_handle client_handle);

/**
 * @brief The API to unblock re-entry to island mode.
 *
 * @param[in] client_handle Handle returned in registration.
 *
 * @return
 *   - SNS_RC_SUCCESS:        Operation successful.
 *   - SNS_RC_INVALID_TYPE:   Client_handle parameter is bad.
 *   - SNS_RC_NOT_SUPPORTED:  Island mode is not supported.
 *
 */
sns_rc sns_island_unblock(sns_island_client_handle client_handle);

/**
 * @brief The API to generates and commits an sns_island_state_log log packet.
 *
 * @param[in] req_id       Request ID to be used in log packet.
 *
 * @return
 *   - SNS_RC_SUCCESS         : Log packet operation was successful.
 *   - SNS_RC_NOT_SUPPORTED   : Island mode not supported.
 *
 */
sns_rc sns_island_generate_and_commit_state_log(uint64_t req_id);

/**
 * @brief The API to Configure island transition debug structure based on
 *       registry setting.
 *
 * @param[in] enable_island_debug Enable or disable island transition debug
 *
 * @return
 *   None.
 *
 */
void sns_island_configure_island_transition_debug(bool enable_island_debug);

/**
 * @brief The API to get the current island mode state.
 *
 * @param[in] enable_island_test  Enable or disable island test validation.
 *
 * @return
 *    None.
 *
 */
void sns_island_configure_island_test_debug(bool enable_island_test);

/**
 * @brief The API to configure island test debug structure based on registry
 *        setting.
 *
 * @return
 *  - SNS_ISLAND_STATE_IN_ISLAND:        When system is in island mode.
 *  - SNS_ISLAND_STATE_NOT_IN_ISLAND:    When system is not in island mode.
 *  - SNS_ISLAND_STATE_ISLAND_DISABLED:  When island mode is disabled.
 *
 */
sns_island_state sns_island_get_island_state(void);

/**
 * @brief Qsh sensor Island memory pool vote function.This API votes for QSH
 * memory pool if atleast one island sensor is active.This API removes QSH
 * memory pool vote if there are no island sensors active.
 *
 * @param [in] sensor     Sensor pointer.
 * @param [in] enable     True: Add vote for sensor island memory pool;
 *                        False: Remove vote for sensor island memory pool.
 *
 * @return
 *  - SNS_RC_SUCCESS      : No error occurred; success.
 *  - SNS_RC_FAILED       : Internal error occurred.
 *  - SNS_RC_NOT_SUPPORTED: This API is not supported or is not implemented.
 *
 */
sns_rc qsh_island_memory_vote(sns_sensor *const sensor, bool enable);

/**
 * @brief This function will loop over the island_transition_debug structure and
 * publish the logs.
 *
 * @param [in] force: Force logging even if it's already logged.
 *
 * @return
 *   None.
 *
 */
void qsh_island_transition_logging(bool force);