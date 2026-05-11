/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#ifndef __PDTSW_UTILITY_GLINK_H__
#define __PDTSW_UTILITY_GLINK_H__

#include "pdtsw_utility_glink_defs.h"

/**
 * @brief Utility GLINK channel identifiers
 */
typedef enum {
    PDTSW_UTILITY_GLINK_AM_WM_CHANNEL = 0,   /* AM-WM channel */
    PDTSW_UTILITY_GLINK_AM_Q6_CHANNEL,       /* AM-Q6 channel */
    PDTSW_UTILITY_GLINK_NUM_CHANNELS         /* Number of GLINK channels */
} pdtsw_utility_glink_channel_t;

/**
 * @brief Return codes for GLINK utility functions
 */
typedef enum {
    PDTSW_UTILITY_GLINK_SUCCESS = 0,         /* Operation completed successfully */
    PDTSW_UTILITY_GLINK_INVALID_STATE_ERR,   /* GLINK channel not connected */
    PDTSW_UTILITY_GLINK_ERROR                /* General error */
} pdtsw_utility_glink_ret_t;
/**
 * @brief Send a signal over the specified GLINK channel
 *
 * @param[in] channel The GLINK channel to send the signal on
 * @param[in] signal The signal value to be sent
 *
 * @return pdtsw_utility_glink_ret_t Return status of the operation
 */
pdtsw_utility_glink_ret_t pdtsw_utility_glink_send_signal(
    pdtsw_utility_glink_channel_t channel, pdtsw_utility_glink_signal_t signal);

#endif // #ifdef __PDTSW_UTILITY_GLINK_H__