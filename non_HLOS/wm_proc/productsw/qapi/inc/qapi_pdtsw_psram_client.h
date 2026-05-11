/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#ifndef __QAPI_PDTSW_PSRAM_CLIENT_H__
#define __QAPI_PDTSW_PSRAM_CLIENT_H__

/**
 * @brief Status codes for PSRAM arbiter client operations
 */
typedef enum {
    QAPI_PDTSW_PSRAM_CLIENT_STS_INVALID= 0,       /**< Invalid status */
    QAPI_PDTSW_PSRAM_CLIENT_STS_AVAILABLE,        /**< PSRAM is available */
    QAPI_PDTSW_PSRAM_CLIENT_STS_VOTE_SUCCESS,     /**< Vote for PSRAM access succeeded */
    QAPI_PDTSW_PSRAM_CLIENT_STS_VOTE_FAILED,      /**< Vote for PSRAM access failed */
    QAPI_PDTSW_PSRAM_CLIENT_STS_UNVOTE_SUCCESS,   /**< Unvote for PSRAM access succeeded */
    QAPI_PDTSW_PSRAM_CLIENT_STS_UNVOTE_FAILED,    /**< Unvote for PSRAM access failed */
    QAPI_PDTSW_PSRAM_CLIENT_STS_REQ_DROPPED       /**< Vote or Unvote request dropped */
} qapi_pdtsw_psram_client_status_t;

/**
 * @brief Return codes for PSRAM arbiter client QAPIs
 */
typedef enum {
    QAPI_PDTSW_PSRAM_CLIENT_SUCCESS= 0,           /**< Operation successful. */
    QAPI_PDTSW_PSRAM_CLIENT_NULL_PTR_ERR,         /**< Null pointer error. */
    QAPI_PDTSW_PSRAM_CLIENT_INVALID_ARG_ERR,      /**< Invalid argument error. */
    QAPI_PDTSW_PSRAM_CLIENT_UNAVAILABLE_ERR,      /**< PSRAM unavailable error. */
    QAPI_PDTSW_PSRAM_CLIENT_ERROR                 /**< General error. */
} qapi_pdtsw_psram_client_ret_t;

/**
 * @brief Type for PSRAM client handle.
 */
typedef uint8_t qapi_pdtsw_psram_client_handle_t;

/**
 * @brief Callback function type for local clients 
 *        to the PSRAM arbiter client
 *
 * @param[in] status      Status code for the notification
 * @param[in] p_priv_data Pointer to client's private data
 */
typedef void (* qapi_pdtsw_psram_client_cb_t)(
    qapi_pdtsw_psram_client_status_t status, void *p_priv_data);

/**
 * @brief Registers a local client with the PSRAM arbiter client
 *
 * Validates input arguments, assigns a handle, registers the 
 * callback and private data, and immediately notifies the 
 * client of PSRAM status if available
 *
 * @param[in]  cbfun      Callback function to register.
 * @param[in]  p_priv_data Pointer to client's private data.
 * @param[out] p_handle   Pointer to store the assigned client handle.
 *
 * @return qapi_pdtsw_psram_client_ret_t Return status of the operation
 * 
 */
qapi_pdtsw_psram_client_ret_t qapi_pdtsw_psram_client_register_cb(
    qapi_pdtsw_psram_client_cb_t cbfun,
    void *p_priv_data,
    qapi_pdtsw_psram_client_handle_t *p_handle);

/**
 * @brief Deregisters the local client with the PSRAM arbiter client
 *
 * Validates the client handle, ensures the client is registered and 
 * not currently processing, and removes the client from the context
 *
 * @param[in] handle Client handle to deregister
 *
 * @return qapi_pdtsw_psram_client_ret_t Return status of the operation
 * 
 */
qapi_pdtsw_psram_client_ret_t qapi_pdtsw_psram_client_deregister_cb(
    qapi_pdtsw_psram_client_handle_t handle);

/**
 * @brief Submits a vote or unvote request to the PSRAM arbiter client
 *
 * Validates the client handle and PSRAM status, then submits a 
 * vote or unvote request to the work queue for processing
 *
 * @param[in] vote   Vote (true) or Unvote (false)
 * @param[in] handle Client handle for which the request is submitted
 *
 * @return qapi_pdtsw_psram_client_ret_t Return status of the operation
 * 
 */
qapi_pdtsw_psram_client_ret_t qapi_pdtsw_psram_client_vote(
    bool vote, qapi_pdtsw_psram_client_handle_t handle);

#endif // __QAPI_PDTSW_PSRAM_CLIENT_H__