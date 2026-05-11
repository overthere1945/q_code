/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#ifndef __PDTSW_UTILITY_GLINK_DEFS_H__
#define __PDTSW_UTILITY_GLINK_DEFS_H__

/**
 * @brief Enum to indicate the identity of the message signal
 */
typedef enum
{
    PDTSW_UTILITY_GLINK_SIGNAL_ID_INVALID = 0x0,   /**< Signal is Invalid */
#if defined(CONFIG_QC_PDTSW_PSRAM_ARBITER_ENABLE) || defined(CONFIG_QC_PDTSW_PSRAM_CLIENT_ENABLE)
    PDTSW_UTILITY_GLINK_SIGNAL_ID_PSRAM,           /**< Signal is for PSRAM control */
#endif
} pdtsw_utility_glink_signal_id_t;

/**
 * @brief Union for GLINK signal representation
 */
typedef union
{
    uint32_t raw_value;    /**< Raw signal value */
    struct {
        uint32_t id:4;     /**< Signal ID */
        uint32_t value:28; /**< Signal value */
    };
} pdtsw_utility_glink_signal_t;

#endif // #ifndef __PDTSW_UTILITY_GLINK_DEFS_H__
