#pragma once
/** ============================================================================
 * @file
 *
 * @brief Common return codes used by the Sensor Framework.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*==============================================================================
  Type Definitions
  ============================================================================*/

/**
 * @brief Enumeration for common return codes used by the sensor framework.
 *
 */
typedef enum sns_rc
{
  SNS_RC_SUCCESS = 0, /*!< No error occurred; success. */

  SNS_RC_FAILED, /*!< Unfixable or internal error
                  *   occurred.
                  */

  SNS_RC_NOT_SUPPORTED, /*!< This API is not supported or is
                         *   not implemented.
                         */

  SNS_RC_INVALID_TYPE, /*!< Function argument contains
                        *   invalid data type, e.g., unknown
                        *   message ID, unknown registry
                        *   group, or unexpected. Sensor UID.
                        */

  SNS_RC_INVALID_STATE, /*!< Sensor[Instance] in bad or
                         *   invalid state; is unable to
                         *   recover.
                         */

  SNS_RC_INVALID_VALUE, /*!< One or more argument values were
                         *   outside of the valid range.
                         */

  SNS_RC_NOT_AVAILABLE, /*!< This operation is not available
                         *    at this time. */

  SNS_RC_POLICY, /*!< This action was rejected due to
                  *   the current policy settings.
                  */

  SNS_RC_INVALID_LIBRARY_STATE, /*!< All sensors of the library are in
                                 *   an invalid state and cannot
                                 *   recover from it. No error
                                 *   occurred; success.
                                 */

  SNS_RC_DYNAMIC_OBJECT_LOAD_FAILED, /*!< Loading of a dynamic object
                                      *   failed.
                                      */

  SNS_RC_DYNAMIC_OBJECT_SYMBOL_NOT_FOUND, /*!< A symbol look up in a dynamically
                                           *   loaded library failed.
                                           */

  SNS_RC_DYNAMIC_OBJECT_UNLOAD_FAILED, /*!< Unloading of a dynamic object
                                        *   failed.
                                        */
} sns_rc;
