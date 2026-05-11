#pragma once
/*=============================================================================
  @file qsh_oem1_sensor_instance.h

  The qsh_oem1 virtual Sensor Instance

  Copyright (c) 2021-2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "qsh_location.pb.h"
#include "qsh_oem1.pb.h"
#include "qsh_oem1_sensor.h"
#include "qsh_wifi.pb.h"
#include "sns_amd.pb.h"
#include "sns_event_service.h"
#include "sns_sensor_instance.h"
#include "sns_std_sensor.pb.h"

/*============================================================================
  Preprocessor Definitions and Constants
  ===========================================================================*/

/** PRINTF type specifier to print WiFi BSSID MAC address used in confjunction with PRI_MAC_ARG */
#define PRI_MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
/** PRINTF argument used in junction with PRI_MAC_FMT type specifier */
#define PRI_MAC_ARG(mac_addr)                                                                      \
  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]

/** Interval at which location position events are received */
#define LOCATION_UPDATE_INTERVAL_MS 5000

/** Maximum count of wifi access points */
#define MAX_WIFI_AP_LIST_COUNT 40

/*============================================================================
  Type Declarations
  ===========================================================================*/

/** QSH_OEM1 instance configurations */
typedef struct
{
  int wifi_svc_version;                        // qsh_wifi driver version
  qsh_wifi_capabilities wifi_svc_capabilities; // capabilities supported by the qsh_wifi driver
  int wifi_svc_result_count; // Total no. of access points detected in previous wifi scan

  int location_version; // qsh_location driver version
  qsh_location_capabilities
      location_capabilities;     // capabilities supported by the qsh_location driver
  bool location_session_running; // True if location update session is running
} qsh_oem1_inst_config;

/** Location update results */
typedef struct
{
  uint64_t fix_utc_ts; // UTC timestamp for location fix in milliseconds
  int32_t latitude;    // Fixed point latitude in degrees times 10^7
  int32_t longitude;   // Fixed point longitude in degrees times 10^7
} qsh_oem1_location_data;

/** WiFi scan results */
typedef struct
{
  uint64_t wifi_scan_ts; // Timestamp when the Ranging was completed in QTIMER ticks
  int ap_count;          // Total no. of access points detected in current wifi scan
  qsh_oem1_wifi_info
      ap_list[MAX_WIFI_AP_LIST_COUNT]; // List of access points info for current wifi scan
} qsh_oem1_wifi_data;

/** QSH_OEM1 instance state */
typedef struct qsh_oem1_inst_state
{
  sns_sensor_uid const *suid; // qsh_oem1 SUID

  sns_data_stream *amd_stream;      // AMD stream
  sns_data_stream *wifi_svc_stream; // WiFi stream
  sns_data_stream *location_stream; // Location stream

  sns_event_service *event_service; // sensor service to send sensor event to the client

  uint64_t amd_event_ts;   // AMD event timestamp
  sns_amd_event amd_state; // current AMD state

  qsh_oem1_inst_config inst_config;     // Instance config
  qsh_oem1_location_data location_data; // Location update results
  qsh_oem1_wifi_data wifi_data;         // WiFi scan results
} qsh_oem1_inst_state;

sns_rc qsh_oem1_island_exit(void);
sns_rc qsh_oem1_inst_init(sns_sensor_instance *this, sns_sensor_state const *state);
sns_rc qsh_oem1_inst_deinit(sns_sensor_instance *const this);
sns_rc qsh_oem1_inst_set_client_config(sns_sensor_instance *const this,
                                       sns_request const *client_request);

/**
 * Send location update reqest to qsh_location driver to Start or
 * Stop a position tracking session
 *
 * @param this        sensor instance reference
 * @param start       true to start tracking position, false to stop.
 * @return            true if the request was sent successfully
 */
sns_rc qsh_oem1_send_location_update_request(sns_sensor_instance *const this, bool start);

/**
 * Send request to qsh_wifi driver to scan wifi access points
 *
 * @param this        sensor instance reference
 * @return            true if the request was sent successfully
 */
sns_rc qsh_oem1_send_wifi_scan_request(sns_sensor_instance *const this);

/** Process qsh_wifi ACK events */
void qsh_oem1_process_wifi_cmd_ack(sns_sensor_instance *const this, pb_istream_t *pbstream);

/** Process qsh_wifi scan result events */
void qsh_oem1_process_wifi_evt_scan_result(sns_sensor_instance *const this, pb_istream_t *pbstream);

/** Process qsh_location ACK event */
void qsh_oem1_process_location_ack(sns_sensor_instance *const this, pb_istream_t *pbstream);

/** Process qsh_location position events */
void qsh_oem1_process_location_position_event(sns_sensor_instance *const this,
                                              pb_istream_t *pbstream);

/** Process qsh_location measurement and clock event */
void qsh_oem1_process_location_meas_and_clk_event(sns_sensor_instance *const this,
                                                  pb_istream_t *pbstream);
