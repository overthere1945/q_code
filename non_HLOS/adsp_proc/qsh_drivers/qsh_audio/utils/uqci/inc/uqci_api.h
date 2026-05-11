#pragma once
/*=============================================================================
 @file uqci_api.h

 uQsocket client interface api.

 This utility can be used to connect with a uQsocket based
 server on the remote process.

 Copyright (c) 2020-2021 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/

/*=============================================================================
 Include Files
 ===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "sns_rc.h"

#include "qsocket_ipcr.h"

/*------------------------------------------------------------------------------
   Constants
------------------------------------------------------------------------------*/

#undef TRUE
#undef FALSE

#define TRUE (1)  /* Boolean true value */
#define FALSE (0) /* Boolean false value */

#ifndef NULL
#define NULL (0)
#endif

/*=============================================================================
 Type Definitions
 ===========================================================================*/

typedef char          char_t; /* Character type */
typedef unsigned char bool_t; /* Boolean value type */

/*
 * message header for all the messages sent or received.
 * following this is the message payload
 */
typedef struct uqsocket_msg_header_t
{
   // unique client id
   uint32_t client_id;

   // msg token
   uint32_t token;

   // opcode of the message
   uint32_t opcode;

   // length of the payload in bytes following this message header.
   uint32_t payload_len;

} uqsocket_msg_header_t;

/*=============================================================================
 Public Function Definitions
 ===========================================================================*/
/*!
 @brief

 callback function to return the svc_ptr to the user upon successful
 connection to the remote process.
 It is also called to notify the client about any error which may
 occur after the successful connection. In this case user may clear
 their state and try to reestablish the connection to remote process.

 @param[in]   cb_ctx_ptr  callback function context pointer.

 @param[in]   svc_ptr     service handle.

 @param[in]   conn_status status of the connection.
			  0: live
			  non-zero: error


 @note

 - Dependencies
 - sensor client should use qsh_invoke to protect their state.
 - if an error is reported to the user then user should invoke uqci_deinit to clear the svc_handle.
 */
typedef void (*uqci_init_cb_f)(void *cb_ctx_ptr, void *svc_ptr, int conn_status);

/*!
 @brief

 callback function to send messages which are received from the remote_process

 @param[in]   cb_ctx_ptr  callback function context pointer.

 @param[in]   opcode      opcode of the received msg

 @param[in]   token       token of the received msg

 @param[in]   payload_len length of the msg payload.

 @param[in]   payload_ptr message payload

 @note

 - Dependencies
   - sensor client should use qsh_invoke to protect their state.
   - callback function should be island safe.

 */
typedef void (*uqci_receive_msg)(void *cb_ctx_ptr, uqsocket_msg_header_t *msg_header);

/*!
 @brief

 Initiates a connection with the requested remote process and calls
 the callback function once connection is established/failed.

 @param[in]   rp_name_ptr  Name of the remote process's uqsocket port.

 @param[in]   cb_func	   callback function to notify the successful/failed
			   connection with the remote process.

 @param[in]   cb_ctx_ptr   callback function context pointer.

 @return
 sns error code

 */
sns_rc uqci_init(ipcr_name_t *rp_name_ptr, uqci_init_cb_f cb_func, void *cb_ctx_ptr);

/*!
 @brief

 registers a client for a service.
 service handles connection with a remote process and multiple clients can
 use this connection to talk to the remote process.

 @param[in]   svc_ptr       service-handle returned in the uqci_init_cb_f callback function.

 @param[in]   cb_fun	    callback function to notify the received messages
                            from the remote process.

 @param[in]   cb_ctx_ptr    callback function context pointer.

 @param[out]  client_handle_pptr client handle.

 @return
 sns error code

 @note

 - Dependencies
   - svc_ptr should be valid

 */
sns_rc uqci_register_client(void *svc_ptr, uqci_receive_msg cb_func, void *cb_ctx_ptr, void **client_handle_pptr);

/*!
 @brief

 returns client id of the client-handle.

 @param[in]  client_handle_ptr client handle.

 @param[out]  client_id_ptr client id pointer.

 @return
 sns error code

 @note

 - Dependencies
   - client_handle_ptr should be valid

 */

sns_rc uqci_get_client_id(void* client_handle_ptr, uint32_t *client_id_ptr);

/*!
 @brief

 creates a msg payload to send to remote process.

 @param[in]   client_handle_ptr client-handle.

 @param[in]   opcode        	opcode of the message.

 @param[in]   token        	message token, it is returned in the response.

 @param[in]   payload_len   	length of the payload.

 @param[out]  payload_pptr      payload

 @return
 sns error code

 @note

 - Dependencies
   - client_handle should be valid
   - maximum payload len is 1KB

 */
sns_rc uqci_create_msg(void *client_handle_ptr, uint32_t opcode, uint32_t token, uint32_t payload_len, void **payload_pptr);

/*!
 @brief

 sends the payload to the remote_process and then frees the payload.

 @param[in]   client_handle_ptr client-handle.

 @param[in]   payload_ptr      	payload

 @return
 sns error code

 @note

 - Dependencies
   - client_handle should be valid.
   - payload should be created through uqci_create_msg.

 */
sns_rc uqci_send_msg(void *client_handle_ptr, void *payload_ptr);

/*!
 @brief

 frees the message payload allocated through "uqci_create_msg" api.
 call this api only if "uqci_send_msg" is not called.


 @param[in]   client_handle_ptr client-handle.

 @param[in]   payload_ptr      	payload

 @note

 - Dependencies
   - client_handle should be valid.
   - payload should be created through uqci_create_msg api.

 */
void uqci_free_msg(void *client_handle_ptr, void *payload_ptr);

/*!
 @brief

 de-registers a client.

 @param[in]  client_handle_ptr client-handle.

 @note

 - Dependencies
   - client_handle should be valid

 */
void uqci_deregister_client(void *client_handle_ptr);

/*!
 @brief

 de-init the svc_handle.

 @param[in]  svc_handle_ptr service-handle.

 @note

 - Dependencies
   - svc_handle should be valid
   - all the clients should be de-registered.

 */
void uqci_deinit(void *svc_handle_ptr);
