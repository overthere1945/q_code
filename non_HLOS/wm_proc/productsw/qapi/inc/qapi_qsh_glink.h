/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/

#ifndef __QSH_GLINK_ROUTER_H__
#define __QSH_GLINK_ROUTER_H__

#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include "sns_client_glink.pb.h"

#define QAPI_QSH_GLINK_MAX_EVT_PAYLOAD_SZ       256

/** Enum to indicate qsh_glink availability status.*/
typedef enum
{
    QSH_GLINK_STATUS_DOWN = 0,
    QSH_GLINK_STATUS_UP
}qapi_qsh_glink_status_t;

/** Types of events that are receive on connection callback.*/
typedef enum {
    QSH_GLINK_EVT_CONN_ALLOC_RESP,    /**< Allocate response event.*/
    QSH_GLINK_EVT_CONN_FREE_RESP,     /**< Free response event.*/
    QSH_GLINK_EVT_CONN_HNDL_ERR,      /**< Error event.*/
    QSH_GLINK_EVT_SEND_RESP,          /**< Response to data send event.*/
    QSH_GLINK_EVT_DATA                /**< Data received from QSH.*/
}qapi_qsh_glink_conn_evt_type_t;

/** Struct to represent the event and associated data on connection callback.*/
typedef struct {
    qapi_qsh_glink_conn_evt_type_t evt_type;
    union {
        int conn_resp;          /**< Response status for allocate/free event.*/
        int conn_err;           /**< Error on the connection. Handle becomes
                                     invalidated.*/
        int send_resp;          /**< Response to send data event.*/
        /*Todo: sns_client_event_msg data;*/  /**< Data associated with indication event.*/
        struct {
            pb_istream_t *data;  /**< Data associated with indication event.*/
            uint32_t data_sz; /**< Size of data associated with indication event.*/
        };
    };
}qapi_qsh_glink_conn_evt_t;

/** Type to represent client connection handle.*/
typedef void* qapi_qsh_glink_hndl_t;

/**
 * Connection callback type.
 * @param status Status of qsh glink router.
 * @param priv Priviliged data passed by client at allocation.
*/
typedef void (*qsh_glink_status_cb_t)(qapi_qsh_glink_status_t status,
    const void* priv);

/**
 * Connection callback type.
 * @param evt Event data representing the event.
 * @param priv Priviliged data passed by client at allocation.
*/
typedef void (*qsh_glink_conn_cb_t)(const qapi_qsh_glink_conn_evt_t *evt,
        const void* priv);

/**
 * Register for qsh glink router availablity status.
 * @param cb Callback to be registered for the connection.
 * @param priv Client's priviliged data that will be passed in the callback.
 */
int qapi_qsh_glink_register_status(qsh_glink_status_cb_t cb, const void* priv);

/**
 * Deregister for qsh glink router availablity status.
 * @param cb Callback to be registered for the connection.
 * @param priv Client's priviliged data that will be passed in the callback.
 */
int qapi_qsh_glink_deregister_status(qsh_glink_status_cb_t cb, const void* priv);

/**
 * Allocate a connection for client.
 * @note If a client registered for a sensor and tries to register same sensor
 * with different configuration, it will be treated as reconfig.
 * To avoid this, clients should create separate connection to register for
 * same sensor with different configurations.
 * @param hndl Handle allocted fo the client on success. This will be passed for
 * subsequent operations on the same connection.
 * @param cb Callback to be registered for the connection.
 * @param priv Client's priviliged data that will be passed in the callback.
 */
int qapi_qsh_glink_allocate_conn(qapi_qsh_glink_hndl_t *hndl,
        qsh_glink_conn_cb_t cb, const void* priv);

/**
 * Free connection.
 * @param hndl handle provided during connection allocation.
 * @return refer sys errno.h
 */
int qapi_qsh_glink_free_conn(qapi_qsh_glink_hndl_t hndl);

/**
 * Send data over connection.
 * @param hndl handle provided during connection allocation.
 * @param req sns_client_request_msg proto struct data that needs to be sent.
 * @param size size of the data that needs to be sent.
 * @return refer sys errno.h
 */
int qapi_qsh_glink_send(qapi_qsh_glink_hndl_t hndl, sns_client_request_msg* req,
                size_t size);

#endif /**< __QSH_GLINK_ROUTER_H__ */