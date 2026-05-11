/*
 * @file sns_router.h
 * @brief sns_router APIs and strctures
 *
 */
 
 
/*
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 */


#pragma once


#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_client.pb.h"
#include "sns_fw_data_stream.h"
#include "sns_fw_event_service.h"
#include "sns_fw_request.h"
#include "sns_ipc_router.h"
#include "sns_isafe_list.h"
#include "sns_mem_util.h"
#include "sns_signal.h"
#include "sns_std_type.pb.h"
#include "sns_suid.pb.h"


/*
*-------------------------------------------------*
* macro definitions
*-------------------------------------------------*
*/

#define MAX_LINK_NAME_LEN       32
#define MAX_SENSOR_DATATYPE_LEN 32
#define MAX_DECODE_BUFFER_LEN   32
#define MAX_EVENT_BUFFER_LEN    100
#define SNS_ROUTER_DEBUG_PRINT

#ifdef SNS_ISLAND_INCLUDE_SNS_ROUTER
#define SNS_ROUTER_HEAP SNS_HEAP_ISLAND
#else
#define SNS_ROUTER_HEAP SNS_HEAP_MAIN
#endif

#ifdef SNS_ROUTER_DEBUG_PRINT
#define SNS_ROUTER_PRINTF      SNS_PRINTF
#define SNS_ROUTER_SPRINTF     SNS_SPRINTF
#else
#define SNS_ROUTER_PRINTF(prio, sensor, ...)  UNUSED_VAR(sensor);
#define SNS_ROUTER_SPRINTF(prio, sensor, ...) UNUSED_VAR(sensor);
#endif


/*
*-------------------------------------------------*
* Variable declarations
*-------------------------------------------------*
*/
extern sns_ipc_router_api sns_glink_api;


/*
*-------------------------------------------------*
* Structure definitions
*-------------------------------------------------*
*/

/* remote client connection status   */
typedef enum sns_remote_hub_conn_status
{
  REMOTE_CONN_UNKNOWN = 0,
  REMOTE_CONN_AVAILABLE,
  REMOTE_CONN_REMOVING,
} sns_remote_hub_conn_status;


/* remote client connection type   */
typedef enum sns_remote_client_connection_type
{
  SNS_REMOTE_GLINK = 0,
  /*SNS_REMOTE_QMI*/
  /*SNS_REMOTE_QSOCKET*/
}sns_remote_client_connection_type;


/* remote client QMI config */
/*typedef struct sns_remote_client_qmi_config
{
  uint32_t service_id;
}sns_remote_client_qmi_config;*/


/* remote client GLINK config   */
typedef struct sns_remote_client_glink_config
{
  /* Remote sub system link name*/
  char      link_name[MAX_LINK_NAME_LEN];

  /* Number of channels to be queued at client side*/
  uint16_t  number_of_channels;
}sns_remote_client_glink_config;


/* remote client config information */
typedef struct sns_remote_client_config
{
  /* Type of the IPC router */
  sns_remote_client_connection_type  router_type;

  /* Sensing Hub ID */
  uint16_t                           hub_id;

  /* router config */
  union{
    //sns_remote_client_qmi_config     qmi_config;
    sns_remote_client_glink_config   glink_config;
  }router_config;
}sns_remote_client_config;


/* remote hub connection information */
typedef struct sns_remote_hub_conn
{
  /*list enty item for active and pending connections list*/
  sns_isafe_list_item         list_entry;
  
  /*connection handle for remote sensing hub*/
  uint32_t conn_handle;
  
  /*stream associated with the connection handle*/
  struct sns_fw_data_stream   *stream;
  
  /*Connection status*/
  sns_remote_hub_conn_status  conn_status;

  /*sensing hub which this connection belongs to*/
  struct sns_sensing_hub      *sensing_hub;

  /*suid of the remote sensor*/
  sns_sensor_uid              suid;

  /*pointer to the sns_remote_sensor which this connection is associated*/
  struct sns_remote_sensor    *source_sensor;
}sns_remote_hub_conn;


/* remote sensing hub information */
typedef struct sns_sensing_hub
{
  /*Used for SUID and atrribute request/events*/
  sns_remote_hub_conn       suid_attrib_conn;

  /*request list lock*/
  sns_osa_lock              request_list_lock;
  
  /*active conn request lock*/
  sns_osa_lock 	            active_conn_req_lock;

  /*attribute lock*/
  sns_osa_lock              attribute_lock;
  
  /*pending sensors lock*/
  sns_osa_lock              pending_sensors_lock;

  /*remote sensing hub sensors list*/
  sns_isafe_list            sensors; //sns_remote_sensor
  
  /*pending sensing hub sensors list*/
  sns_isafe_list            pending_sensors; //sns_remote_sensor
  
  /*active connections*/
  sns_isafe_list            active_conns; //sns_remote_hub_conn
  
  /*connections in progress*/
  sns_isafe_list            pending_conns; //sns_remote_hub_conn
  
  /*task list*/
  sns_isafe_list            request_tasks; //sns_fw_request

  /*remote sensing hub router config*/
  sns_ipc_router_config     *router_config;
  
  /*remote sensing hub router handle*/
  sns_ipc_router_handle     *router_handle;

  /*Signal thread handle*/
  sns_signal_handle         *sig_handle;

  /*sensing hub ID*/
  uint16_t                  hub_id;

  /*remote sensing hub connection status*/
  sns_ipc_conn_status       conn_status;

  /*Flag to check if SUID request is sent to the
    remote sensing hub*/
  bool                      suid_req_done;

  /*okay to send request*/
  bool                      okay_to_send;

}sns_sensing_hub;


/* remote sensor information*/
typedef struct sns_remote_sensor
{
  /*remote sensor data type*/
  char                           data_type[MAX_SENSOR_DATATYPE_LEN];

  /*remote sensor vendor*/
  char                           vendor[MAX_SENSOR_DATATYPE_LEN];

  /*remote sensor suid*/
  sns_std_suid                   suid;

  /*pointer to the remote sensing hub this sensor belongs to*/
  sns_sensing_hub                *sensing_hub;

  /*connection handle*/
  uint32_t                       conn_handle;

  /*remote sensor data type length*/
  uint32_t                       data_type_len;

  /*remote sensor vendor length*/
  uint32_t                       vendor_len;

  /*decoded attributes for framework to access*/
  int32_t                        event_size;
  float                          selected_resolution;
  float                          selected_range;
  sns_std_sensor_stream_type     stream_type;
  sns_std_sensor_rigid_body_type rigid_body;
  bool                           physical_sensor;
  bool                           available;

  /*if attributes are requested*/
  bool                           attrib_requested;
  bool                           attrid_datatype_received;

}sns_remote_sensor;


/*
* @brief Callback function used in sns_remote_sensor_foreach().
*
* @param[in] remote_sensor  Pointer to the sns_remote_sensor.
* @param[in] arg         Pointer to the User-specified argument.
*
* @return 
*  - User specific return. 
*
*/
typedef bool (*sns_remote_sensor_foreach_func)(struct sns_remote_sensor *remote_sensor, void *arg);



/*
*-------------------------------------------------*
* sns_router APIs
*-------------------------------------------------*
*/


/*
* @brief Called by for each remote sensing hub.
*        This will initialise sns_router and get remote sensors information
*
* @param[in] none
*
* @return
*     - SNS_RC_SUCCESS: Action succeeded
*
*/
sns_rc sns_router_init(void);


/*
* @brief Sends request to the remote sensor.
*
* @param[in] stream             Pointer to the stream on which the request is sent
* @param[in] fw_request         Pointer to the request
*
* @return
*     - SNS_RC_SUCCESS: Action succeeded
*
*/
sns_rc sns_router_send_request(struct sns_fw_data_stream *stream,
                               sns_fw_request const *fw_request);


/*
* @brief This will connect to the remote sensing hub to request connection handle
*        Connection handle will be received as part of sns_router_connect_callback
*
* @param[in] stream             Pointer to the stream which is created
* @param[in] find_sensor_arg    Pointer to the argument which contains the sensor info
*
* @return
*     - SNS_RC_SUCCESS: Action succeeded
*
*/
sns_rc sns_router_connect(struct sns_fw_data_stream *stream,
                          sns_find_sensor_arg const *find_sensor_arg);


/*
* @brief To be calld by stream service
*        This will remove the connection with the remote sensing hub
*
* @param[in] stream               Pointer to the stream which is created
*
* @return
*     - SNS_RC_SUCCESS: Action succeeded
*
*/
sns_rc sns_router_disconnect(struct sns_fw_data_stream *stream);


/*
* @brief Lookup and return the sensor available flag.
*
* @param[in] remote_sensor  Pointer to the sns_remote_sensor.
* @return 
*  - True  If sensor available.
*  - False Otherwise.
*
*/
bool sns_router_get_remote_sensor_available(struct sns_remote_sensor const *remote_sensor);


/*
* @brief Look-up and return the Sensor vendor string as long as vendor_len > 0.
* vendor will always be null-terminated.
*
* @param[in]  remote_sensor  Pointer to the sns_remote_sensor.
* @param[out] vendor         Pointer to the vendor name.
* @param[in]  vendor_len     Length of the vendor name.
*
* @return 
*  - None.
*
*/
void sns_router_get_remote_sensor_vendor(struct sns_remote_sensor const *remote_sensor,
                                         char *vendor, uint32_t vendor_len);


/*
* @brief Look-up and return the Sensor data type string as long as
* data_type_len > 0. data_type will always be null-terminated.
*
* @param[in]  remote_sensor  Pointer to the sns_remote_sensor.
* @param[out] data_type      Pointer to the data type.
* @param[in]  attr_info      Length of the data type.
*
* @return
*  - None.
*
*/
void sns_router_get_remote_sensor_data_type(struct sns_remote_sensor const *remote_sensor,
                                            char *data_type, uint32_t data_type_len);


/*
* @brief Lookup and return the Sensor UID.
*
* @param[in] remote_sensor  Pointer to the sns_remote_sensor.
*
* @return
*  - sns_sensor_uid Sensor SUID.
*
*/
sns_sensor_uid
sns_router_get_remote_sensor_suid(struct sns_remote_sensor const *remote_sensor);


/*
* @brief Lookup and return the connection handle for the remote sensor.
*
* @param[in] remote_sensor  Pointer to the sns_remote_sensor.
*
* @return
*  - connection handle.
*
*/
uint32_t
sns_router_get_remote_sensor_conn_handle (struct sns_remote_sensor const *remote_sensor);


/*
* @brief For each sensor in the sns_router, call func with arg. No locks may be
* acquired within the callback function.
*
* @param[in] func Function called for each known Sensor.
* @param[in] arg  User-specified argument to be used in func callback.
*
* @return 
*  - False if func() returned false for a given sensor input.
*
*/
bool sns_remote_sensor_foreach(sns_remote_sensor_foreach_func func,
                               void *arg);
