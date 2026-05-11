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
/*============================================================================
  INCLUDES
  ============================================================================*/
#include <stdint.h>
#include "sns_osa_thread.h"
#include "sns_rc.h"

#include "sns_printf_int.h"

#include "sns_island_util.h"
#include "sns_sensor_uid.h"

/*============================================================================
  MACROS
  ============================================================================*/

/**
 * @brief Exit island mode.
 *
 * @note The system will enter island mode at a later time after current thread
 *   has returned to IDLE
 */

#define SNS_ISLAND_EXIT() sns_island_exit_internal()

/**
 * @brief SNS_SECTION( name )
 * - This macro can be used to place code and variables into a section called
 * name.
 * - To move code or variables into island mode on supported targets, first tag
 * the item using this macro, next use the same sections in addIslandLibrary()
 * method in the scons file to inform the linker that a section must be made
 * available in island memory.
 *
 */

#if SNS_DISABLE_ISLAND
#define SNS_SECTION(name)
#else
#define SNS_SECTION(name) __attribute__((section(name)))
#endif

/*============================================================================
  Functions
  ============================================================================*/
/**
 * @brief Internal call to exit island mode.
 *
 * @note Do not call this function directly. Use the SNS_ISLAND_EXIT() macro
 *  instead.
 *
 * @return
 *   - SNS_RC_SUCCESS:           Island exit was successful.
 *   - SNS_RC_NOT_AVAILABLE:     Island exit transition did not happen.
 *                               This could be due to:
 *                               - Island mode not supported on target/disabled.
 *                               - System is not in island mode when exit was
 *                                 requested.
 *
 */
sns_rc sns_island_exit_internal(void);

/**
 * @brief Generates a log packet of the sns_island_trace_log type and
 * commits it.
 *
 * @param[in] user_defined_id   The value to be added to the cookie field in
 *                              the log packet.
 *
 * @return
 * - SNS_RC_SUCCESS:              Log packet operation was successful.
 * - SNS_RC_NOT_SUPPORTED:        Log packet not supported.
 *
 */
sns_rc sns_island_log_trace(uint64_t user_defined_id);

/**
 * @brief Initialize the island service.
 *
 * @return
 *   - sns_service:     Pointer to island service.
 *
 */
sns_service *sns_island_service_init(void);
