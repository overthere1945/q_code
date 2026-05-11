/*
 * @file sns_ipc_router.h
 *
 */

/*
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 */


#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "sns_rc.h"

typedef void sns_ipc_router_handle;

/**
 * @brief IPC types.
 *
 */
typedef enum sns_ipc_router_type{
  
  SNS_IPC_GLINK = 0,
  /*SNS_IPC_QMI*/
  /*SNS_IPC_QSOCKET*/
}sns_ipc_router_type;

/**
 * @brief IPC conn status
 *
 */
typedef enum sns_ipc_conn_status
{
  SNS_IPC_CONN_UNKNOWN = 0,
  SNS_IPC_CONN_AVAILABLE,
  SNS_IPC_CONN_UNAVAILABLE,
}sns_ipc_conn_status;

/**
 * @brief Indication types
 *
 */
typedef enum sns_ind_type
{
  SNS_IND_TYPE_RESP = 0,
  SNS_IND_TYPE_EVENT,
}sns_ind_type;
/**
 * @brief QMI IPC config
 *
 */
/*typedef struct sns_ipc_qmi_config{

  uint32_t service_id;
  
}sns_ipc_qmi_config;*/


/**
 * @brief Glink IPC config
 *
 */
typedef struct sns_ipc_glink_config{

  char link_name[32]; /*!< Remote sub system link name*/
  
  uint16_t number_of_channels; /*!< Number of channels to be queued at client side*/
  
}sns_ipc_glink_config;

/**
 * @brief Client IPC router config
 *
 */
typedef struct sns_ipc_router_config{
  sns_ipc_router_type router_type; /*!< Type of the IPC router*/
  uint16_t hub_id;                 /*!< Sensing Hub Id*/
  union{
    //sns_ipc_qmi_config qmi_config; /*!< QMI IPC config*/
    sns_ipc_glink_config glink_config; /*!< Glink IPC config*/
  }router_config;
}sns_ipc_router_config;

/**
 * @brief Callback that will be called when connection ack availble 
 * from remote sensing hub client manager
 *
 *
 * @param[in] router_handle      Port handle for the bus.
 * @param[in] args               User-specified argument for this callback fucntion.
 * @param[in] conn_handle        Connection handle for sending request
 *
 * @return
 *     - None.
 *
 */
typedef void (*sns_ipc_router_connect_ack_callback)(sns_ipc_router_handle *router_handle, 
                                                    void *args,
                                                    uint32_t conn_handle);
/**
 * @brief Callback that will be called when event/resp is received at IPC router from
 * remote sensing hub client manager
 *
 *
 * @param[in] router_handle      Port handle for the bus.
 * @param[in] args               User-specified argument for this callback fucntion.
 * @param[in] conn_handle        Connection handle for sending request
 * @param[in] event_payload      Client API event in pb encoded
 * @param[in] event_len          Client API encoded event length
 *
 * @return
 *     - None.
 *
 */
typedef void (*sns_ipc_router_ind_callback)(sns_ipc_router_handle *router_handle,
                                            sns_ind_type type,
                                            void *args,
                                            uint32_t conn_handle,
                                            void const *event_payload,
                                            uint16_t event_len);
/**
 * @brief Callback that will be called when IPC status is changed with remote sensing hub.
 * Example: IPC_CONN_AVAILABLE/IPC_CONN_UNAVAILABLE
 *
 * @param[in] router_handle      Port handle for the bus.
 * @param[in] args               User-specified argument for this callback fucntion.
 * @param[in] status             IPC conn status
 *
 * @return
 *     - None.
 *
 */
typedef void (*sns_ipc_router_status_callback)(sns_ipc_router_handle *router_handle, 
                                               void *args,
                                               sns_ipc_conn_status status);

/**
 * @brief Callback that will be called when IPC is ready to process pending messages.
 *
 * @param[in] router_handle      Port handle for the bus.
 * @param[in] args               User-specified argument for this callback fucntion.
 *
 * @return
 *     - None.
 *
 */
typedef void (*sns_ipc_router_ready_callback)(sns_ipc_router_handle *router_handle, 
                                               void *args);

typedef struct sns_ipc_router_api
{

  size_t struct_len;
 /**
 * @brief Register as a client to ipc router by providing ipc config and callbacks for
 *        connection ack, indications and ipc status. 
 *
 *
 * @param[in] router_config             IPC router config
 * @param[in] connect_ack_callback      Callback to provide connect ack.
 * @param[in] indication_callback       Callback to provide indications.
 * @param[in] status_callback           Callback to update IPC status.
 * @param[in] ready_callback            Callback to notify, ready to process pending messages.
 * @param[in] args                      Arguments for callback.
 * @param[out] router_handle            IPC router handle used to send further requests.
 *
 * @return
 *     - SNS_RC_SUCCESS: Action succeeded
 *
 */
                                          
 sns_rc (*sns_register_ipc_router)(sns_ipc_router_config const *router_config,
                                    sns_ipc_router_connect_ack_callback connect_ack_callback,
                                    sns_ipc_router_ind_callback ind_callback,
                                    sns_ipc_router_status_callback status_callback,
                                    sns_ipc_router_ready_callback ready_callback,
                                    void *args,
                                    sns_ipc_router_handle **router_handle);


 /**
 * @brief Send connection request to remote sensing hub client manager
 *
 * @param[out] router_handle            IPC router handle.
 *
 * @return
 *     - SNS_RC_SUCCESS: Action succeeded
 *
 */ 
 sns_rc (*sns_ipc_router_connect)(sns_ipc_router_handle *router_handle);
 
 /**
 * @brief Send client API request to the remote sensing hub client manager
 *
 *
 * @param[out] router_handle        IPC router handle.
 * @param[in] connection_handle     Connection handle
 * @param[in] req_payload           Client API request in PB encoded
 * @param[in] req_len               Client API encoded request length
 *
 * @return
 *     - SNS_RC_SUCCESS: Action succeeded
 *
 */                                  
 sns_rc (*sns_ipc_router_send_request)(sns_ipc_router_handle *router_handle,
                                       uint32_t connection_handle,
                                       void *req_payload, 
                                       uint16_t req_len);
  
/**
 * @brief Send disconnect request to remote sensing hub client manager
 *
 *
 * @param[out] router_handle        IPC router handle.
 * @param[in] connection_handle     Connection handle
 *
 * @return
 *     - SNS_RC_SUCCESS: Action succeeded
 *
 */                                  
 sns_rc (*sns_ipc_router_disconnect)(sns_ipc_router_handle *router_handle,
                                     uint32_t connection_handle);
											  
}sns_ipc_router_api;
