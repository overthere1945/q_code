#pragma once
/** ===========================================================================
 * @file
 *
 * @brief Watchdog API.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/
/*============================================================================
  INCLUDES
  ============================================================================*/
#include "sns_rc.h"
#include "sns_time.h"

/*============================================================================
  MACROS
  ============================================================================*/

#define SNS_WATCHDOG_TIMEOUT_IN_SEC 10
#define SNS_WATCHDOG_INVALID_INDEX  -1

/*============================================================================
  Structure Definitions and Typedefs
  ============================================================================*/
/**
 * @brief Structure holding parameters required for watchdog
 * index and time out by each thread
 *
 */
typedef struct sns_watchdog_voter
{
  int8_t wd_voter_idx;       /*!< Watchdog voter index. */
  int16_t wd_timeout_in_sec; /*!< Watchdog timeout in seconds. */
} sns_watchdog_voter;

/**
 * @brief Structure holding information about the watchdog.
 *
 * This structure contains atomic variables to store the type of voter and the
 * timeout in seconds. The use of atomic variables ensures thread safety when
 * accessing and modifying these values.
 */
typedef struct sns_watchdog_info
{
  int32_t voter_type;
  int32_t timeout_in_sec;
} sns_watchdog_info;

/*============================================================================
  FUNCTIONS
  ============================================================================*/
/**
 * @brief  Invoked once at boot to initialize watchdog operation.
 *
 * @return
 * - SNS_RC_SUCCESS:              Initialized successfully.
 * - !SNS_RC_SUCCESS:             An error occurred.
 *
 */
sns_rc sns_watchdog_init(void);

/**
 * @brief  Registers the given thread as a watchdog voter.
 *
 * @param[out] voter_idx       Voter's index.
 *
 * @return
 * - SNS_RC_SUCCESS:              Voter registered successfully.
 * - !SNS_RC_SUCCESS:             An error occurred.
 *
 */
sns_rc sns_watchdog_register(int8_t *voter_idx);

/**
 * @brief  Deregisters the given watchdog voter index.
 *
 * @param[in] voter_idx        Pointer to watchdog voter index,
 *
 * @return
 *   - None.
 *
 */
void sns_watchdog_deregister(int8_t *voter_idx);

/**
 * @brief  The given voter is voting to start watchdog operation.
 *
 * @param[in] voter_idx            A watchdog voter index.
 * @param[in] wd_timeout_in_sec    Timeout period for this watchdog voter.
 *
 * @return
 *   - None.
 *
 */
void sns_watchdog_vote_start(int8_t voter_idx, int16_t wd_timeout_in_sec);

/**
 * @brief  Restarts the given voter's watchdog timeout.
 *
 * @param[in] voter_idx            A watchdog voter index.
 * @param[in] wd_timeout_in_sec    Timeout period for this watchdog voter.
 *
 * @return
 *   - None.
 *
 */
void sns_watchdog_vote_restart(int8_t voter_idx, int16_t wd_timeout_in_sec);

/**
 * @brief  The given voter is voting to stop watchdog operation.
 *
 * @param[in] voter_idx            A watchdog voter index.
 *
 * @return
 *   - None.
 *
 */
void sns_watchdog_vote_stop(int8_t voter_idx);

/**
 * @brief Retrieves information about the given watchdog voter.
 * This function provides the caller with the current state of the watchdog
 * voter, including its type and timeout period.
 *
 * @param[in] voter_idx            A watchdog voter index.
 * @param[out] wd_voter_info       A pointer to a sns_watchdog_info structure
 *                                 that will be populated with the voter's
 *                                 information.
 *
 * @return
 *   - None.
 */
void sns_watchdog_voter_info(int8_t voter_idx,
                             sns_watchdog_info *wd_voter_info);
