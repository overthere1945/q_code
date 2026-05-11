#pragma once
/** ===========================================================================
 * @file
 *
 * @brief CCD service Internal Header file which contains service API definition
 * These APIs are only for internal purposes should never be used by external
 * sources.
 *
 * @note should only be accessed by CCD and DAE sensors
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/

#include <dirent.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "sns_printf_int.h"
#include "sns_rc.h"
#include "sns_service.h"
#include "sns_sensor.h"
#include "sns_time.h"
/*=============================================================================
  Macros and constants
  ===========================================================================*/

/*=============================================================================
  Internal Dependencies
  ===========================================================================*/

/* ======================== Version History ===================================
 *  Version  comments
 *  4        Added gyro ppe bias field
 *  3        Added secondary accel to CCD enable
 *  2        Moved dae binary data buffer utility into ccd service
 *  1        Initial version
 */
#define SNS_CCD_SERVICE_VERSION (4)

/*!< Notification Mask for this service */
#define SNS_CCD_SERVICE_CCD_ENABLE          0x1
#define SNS_CCD_SERVICE_SET_GYRO_OFFSET_CAL 0x2
#define SNS_CCD_SERVICE_RESET_REQ           0x4
#define SNS_CCD_SERVICE_RESET_REQ_COMPLETE  0x8
#define SNS_CCD_SERVICE_BIN_CONFIG_EVENT    0x10
#define SNS_CCD_SERVICE_BIN_DATA_EVENT      0x20
/*!< PPEn Status Updated (status of first ppe reset) */
#define SNS_CCD_SERVICE_PPE_RESET_REQ 0x80
/*!<  PPEn RESET REQ Made (status of the request for second ppe reset) */
#define SNS_CCD_SERVICE_PPE_RESET_STATUS_REQ   0x100
#define SNS_CCD_SERVICE_GYRO_PPE_BIAS          0x200
#define SNS_CCD_SERVICE_CCD_SENSORS_START_INIT 0x400

#define SNS_CCD_SERVICE_REMOVE_CLIENT 0x10000

#define CCD_SERVICE_DBG_ENABLE 1

#define SNS_CCD_SERVICE_CHECK_BIT(var, pos) ((var) & (pos))

#if CCD_SERVICE_DBG_ENABLE
#define CCD_SERVICE_DBG(prio, sensor, ...)                                     \
  do                                                                           \
  {                                                                            \
    SNS_PRINTF(prio, sensor, __VA_ARGS__);                                     \
  } while(0)
#else
#define CCD_SERVICE_DBG(prio, sensor, ...) UNUSED_VAR(sensor);
#endif

#define BIN_DATA_EVENT_BUF_SIZE  10
#define MAX_NUM_BIN_INPUTS       8
#define SENSOR_NAME_ARRAY_LENGTH 64

#define CD_CLIENT_CONFIG 1
#define TE_CLIENT_CONFIG 2

/*=============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * @brief Internal structure for CCD service.
 *
 */
typedef struct sns_ccd_service_internal
{
  sns_service service;
  struct sns_ccd_service_internal_api *api;
} sns_ccd_service_internal;

/**
 * @brief Struct for storing gyroscope calibration information.
 *
 */
typedef struct _sns_ccd_service_gyro_offset_cal
{
  bool has_offset_cnt;
  bool has_offset_runtime_cnt;
  int32_t offset[3];
  int32_t offset_runtime[3];
  uint64_t ts;
} sns_ccd_service_gyro_offset_cal;

/**
 * @brief Enum for specifying the status of the first round of ppe resets
 * started by DAE.
 *
 */
typedef enum
{
  PENDING_RESET = 0,        /*!< ccd is waiting on a ppeN reset */
  IS_RESET,                 /*!< dae has report a ppeN reset */
  NO_RESET_SCHEDULED,       /*!< ccd has acknowledged the ppeN reset */
  NO_CHANGE_TO_RESET_STATUS /*!< ccd has requested no change to DB value of
                               ppeN*/
} ppe_reset_status_val;

/**
 * @brief Enum for specifying if requests for PPE resets are being made by
 * ccd_sensor_instance to DAE for the second round of PPE resets.
 *
 */
typedef enum
{
  NO_RESET_REQ = 0,      /*!< ccd has not requested a reset */
  RESET_REQ,             /*!< ccd has requested a reset */
  NO_CHANGE_TO_RESET_REQ /*!< ccd has requested no change to DB value of ppeN */
} ppe_reset_req_val;

/**
 * @brief Struct for recording the first PPE RESET
 *
 */
typedef struct _sns_ccd_service_ppe_reset
{
  /**
   * @brief ppeN distinction needed for
   * determining which ppe block
   * needs to be reset after config
   * changes when CCD_PPEn_RESET_MODE_6_0
   * is enabled.
   */
  ppe_reset_status_val ppe0_reset;
  ppe_reset_status_val ppe1_reset;
  ppe_reset_status_val ppe2_reset;
} sns_ccd_service_ppe_reset_status;

/**
 * @brief Struct for recording PPEn RESET Requests.
 *
 */
typedef struct _sns_ccd_service_ppe_reset_req
{
  ppe_reset_req_val ppe0_reset_req;
  ppe_reset_req_val ppe1_reset_req;
  ppe_reset_req_val ppe2_reset_req;
} sns_ccd_service_ppe_reset_req;

/**
 * @brief Struct for recording CCD Enable.
 *
 */
typedef struct _sns_ccd_service_ccd_enable
{
  bool enable_primary_accel;
  bool enable_secondary_accel;
  bool enable_primary_gyro;
} sns_ccd_service_ccd_enable;

/**
 * @brief Struct for recording Gyroscope PPE Bias.
 *
 */
typedef struct _sns_ccd_service_gyro_ppe_bias
{
  uint32_t gyro_ppe_bias_0;
  uint32_t gyro_ppe_bias_1;
  uint32_t gyro_ppe_bias_2;
} sns_ccd_service_gyro_ppe_bias;

/**
 * @brief Function prototype for handling CCD service events.
 *
 * @param[in] sensor        Sensor pointer.
 * @param[in] signal        Signal mask.
 *
 * @return
 *   - None.
 *
 */
typedef void (*sns_handle_ccd_service_sensor_event)(struct sns_sensor *sensor,
                                                    uint32_t signal);

/**
 * @brief Function prototype for handling CCD service instance events.
 *
 * @param[in] instance          Sensor instance pointer.
 * @param[in] signal            Signal mask.
 *
 * @return
 *   - None.
 *
 */
typedef void (*sns_handle_ccd_service_instance_event)(
    struct sns_sensor_instance *instance, uint32_t signal);

/**
 * @brief Configuration event info.
 *
 */
typedef struct dae_config_event_info
{
  bool cfg_valid;       /*!< Config valid. */
  int8_t client_config; /*!< CD_CLIENT_CONFIG or TE_CLIENT_CONFIG*/
  int8_t block_num;     /*!< Block num (1,2,3,4). */
  int8_t sdc_num;       /*!< proxy name.*/
  char *data_source;    /*!< Data source. */
} dae_config_event_info;

/**
 * @brief Binary data event info.
 *
 */
typedef struct dae_bin_data_event_info
{
  int8_t bin_data;    /*!< Bin data. */
  uint32_t block_num; /*!< Block num (1,2,3,4).*/
  uint32_t sdc_num;   /*!< proxy name.*/
  sns_time timestamp; /*!< Timestamp. */
} dae_bin_data_event_info;

/**
 * @brief Binary data buffer.
 *
 */
typedef struct
{
  dae_config_event_info config[MAX_NUM_BIN_INPUTS]; /*!< Config array. */
  uint32_t read_idx;                                /*!< Read index.*/
  uint32_t write_idx;                               /*!< Write index.*/
  dae_bin_data_event_info bin_event_info[BIN_DATA_EVENT_BUF_SIZE];
} sns_ccd_dae_bin_data_buf;

typedef struct sns_ccd_service_internal_api
{
  uint32_t struct_len;

  /**
   * @brief Function to register for callback
   *
   * @note Island Support: Yes
   *
   * @param[in] instance          Sensor instance pointer.
   * @param[in] cb                Service callback.
   * @param[in] notification_mask Mask.
   *
   * @return
   *  - SNS_RC_SUCCESS:   If callback registration successful.
   *  - SNS_RC_FAILED:    If callback registration fails.
   *
   */
  sns_rc (*register_instance_cb)(sns_sensor_instance *instance,
                                 sns_handle_ccd_service_instance_event cb,
                                 uint32_t notification_mask);

  /**
   * @brief Function to de-register for callback.
   * @note Island Support: Yes
   *
   * @param[in] instance Sensor instance pointer.
   *
   * @return
   *  - SNS_RC_SUCCESS:  If callback registration successful.
   *  - SNS_RC_FAILED:   If callback registration fails.
   *
   */
  sns_rc (*deregister_instance_cb)(sns_sensor_instance *instance);

  /**
   * @brief Function to register for callback.
   * @note Island Support: Yes
   *
   * @param[in] sensor        Pointer.
   * @param[in] cb            Service callback.
   * @param[in] notification  Mask.
   *
   * @return
   *   - SNS_RC_SUCCESS:  If callback registration successful.
   *   - SNS_RC_FAILED:   If callback registration fails.
   *
   */
  sns_rc (*register_sensor_cb)(sns_sensor *sensor,
                               sns_handle_ccd_service_sensor_event cb,
                               uint32_t notification_mask);

  /**
   * @brief Function to de-register for callback.
   *
   * @note Island Support: Yes.
   *
   * @param[in] sensor  Pointer.
   *
   * @return
   *   - SNS_RC_SUCCESS:  If callback de-registration successful.
   *   - SNS_RC_FAILED:   If callback de-registration fails.
   *
   */

  sns_rc (*deregister_sensor_cb)(sns_sensor *sensor);

  /**
   * @brief Write gyroscope calibration in secure database.
   *
   * @note Island Support: Yes.
   *
   * @param[in] fac_cal      Factory Calibration.
   * @param[in] runtime_cal  Runtime calibration.
   * @param[in] timestamp    Timestamp.
   *
   *
   * @return
   * - SNS_RC_SUCCESS:   If successful.
   * - SNS_RC_FAILED:    If fails.
   *
   */

  sns_rc (*write_gyro_cal)(const int32_t *restrict fac_cal,
                           const int32_t *restrict runtime_cal,
                           sns_time timestamp);

  /**
   * @brief Get gyro cal from secure database
   *
   * @note Island Support: Yes
   *
   * @param[out] val     Get it from secure database.
   *
   * @return
   *   - SNS_RC_SUCCESS:  If successful.
   *   - SNS_RC_FAILED:   If fails.
   *
   */

  sns_rc (*read_gyro_cal)(sns_ccd_service_gyro_offset_cal *val);

  /**
   * @brief Write ccd enable in secure database.
   *
   * @note Island Support: Yes.
   *
   * @param[in] enable_accel           Boolean flag to enable accel.
   * @param[in] enable_gyro            Boolean flag to enable gyro.
   * @param[in] enable_secondary_accel Boolean flag to enable secondary accel.
   *
   * @return
   *   - SNS_RC_SUCCESS:  If successful.
   *   - SNS_RC_FAILED:   If fails.
   *
   */

  sns_rc (*write_ccd_enable)(bool enable_accel, bool enable_gyro,
                             bool enable_secondary_accel);

  /**
   * @brief Get ccd enable data from secure database.
   *
   * @note Island Support: Yes
   *
   * @param[out] val               Get it from database.
   *
   * @return
   *   - SNS_RC_SUCCESS:     If successful.
   *   - SNS_RC_FAILED:      If fails.
   *
   */

  sns_rc (*read_ccd_enable)(sns_ccd_service_ccd_enable *val);

  /**
   * @brief Update dae binary buffer input configs.
   *
   * @note Island Support: Yes.
   *
   * @param[in] client_config  CD or TE.
   * @param[in] proxy_name     Name of the source proxy sensor.
   * @param[in] block_num      CD or TE number (1,2,3,4).
   * @param[in] sdc_num        proxy0 or proxy1.
   * @param[in] clear          Boolean flag to clear input config.
   *
   * @return
   *  - SNS_RC_SUCCESS:  If successful.
   *  - SNS_RC_FAILED:   If fails.
   *
   */

  sns_rc (*write_dae_bin_cfg)(int8_t client_config, const char *proxy_name,
                              uint32_t block_num, uint32_t sdc_num, bool clear);

  /**
   * @brief Get dae binary buffer input configs.
   *
   * @note Island Support: Yes
   *
   * @param[in] dae_config_event_info    Pointer to configs.
   *
   * @return
   *   - SNS_RC_SUCCESS:    If successful.
   *   - SNS_RC_FAILED:     If fails.
   *
   */

  sns_rc (*read_dae_bin_cfg)(dae_config_event_info *dae_config_event_info);

  /**
   * @brief Update dae binary buffer input data.
   *
   * @note Island Support: Yes.
   *
   * @param[in] data          Data.
   * @param[in] timestamp     Timestamp.
   * @param[in] block_num     CD or TE number (1,2,3,4).
   * @param[in] sdc_num       proxy0 or proxy1.
   *
   * @return
   *   - SNS_RC_SUCCESS:  If successful.
   *   - SNS_RC_FAILED:   If fails.
   *
   */

  sns_rc (*write_dae_bin_event)(uint32_t data, sns_time timestamp,
                                uint32_t block_num, uint32_t sdc_num);

  /**
   * @brief Gets the dae binary buffer data. If there is no
   * pending data to be read, returns SNS_RC_FAILED.
   *
   * @note Island Support: Yes.
   *
   * @param[in] dae_bin_data_event_info
   *
   * @return
   *   - SNS_RC_SUCCESS:    If successful.
   *   - SNS_RC_FAILED:     If fails.
   *
   */

  sns_rc (*read_dae_bin_event)(
      dae_bin_data_event_info *dae_bin_data_event_info);

  /**
   * @brief write ppe reset status data to database.
   *
   * @note Island Support: Yes
   *
   * @param[in] write_ppe0_reset Boolean flag to log status of first ppe0 status
   * @param[in] write_ppe1_reset Boolean flag to log status of first ppe1 status
   * @param[in] write_ppe2_reset Boolean flag to log status of first ppe2 status
   *
   * @return
   *   - SNS_RC_SUCCESS:    If successful.
   *   - SNS_RC_FAILED:     If fails.
   *
   */

  sns_rc (*write_ppe_reset_status)(ppe_reset_status_val write_ppe0_reset,
                                   ppe_reset_status_val write_ppe1_reset,
                                   ppe_reset_status_val write_ppe2_reset);

  /**
   * @brief Get ppe reset status data from database.
   *
   * @note Island Support: Yes.
   *
   * @param[out] val Get it from database.
   *
   * @return
   *   - SNS_RC_SUCCESS:    If successful.
   *   - SNS_RC_FAILED:     If fails.
   *
   */

  sns_rc (*read_ppe_reset_status)(sns_ccd_service_ppe_reset_status *val);

  /**
   * @brief Write ppe reset request data to database.
   *
   * @note Island Support: Yes
   *
   * @param[in] write_ppe0_reset_req Boolean flag to log second reset of ppe0
   * @param[in] write_ppe1_reset_req Boolean flag to log second reset of ppe1
   * @param[in] write_ppe2_reset_req Boolean flag to log second reset of ppe2
   *
   * @return
   *   - SNS_RC_SUCCESS:    If successful.
   *   - SNS_RC_FAILED:     If fails.
   *
   */
  sns_rc (*write_ppe_reset_req)(ppe_reset_req_val write_ppe0_reset_req,
                                ppe_reset_req_val write_ppe1_reset_req,
                                ppe_reset_req_val write_ppe2_reset_req);

  /**
   * @brief Get ppe reset request data from database.
   *
   * @note Island Support: Yes
   *
   * @param[out] val          Get it from database.
   *
   * @return
   *   - SNS_RC_SUCCESS:    If successful.
   *   - SNS_RC_FAILED:     If fails.
   *
   */
  sns_rc (*read_ppe_reset_req)(sns_ccd_service_ppe_reset_req *val);

  /**
   * @brief Write gyro ppe bias data to database.
   *
   * @note Island Support: Yes
   *
   * @param[in] val        Bias values to write.
   *
   * @return
   *   - SNS_RC_SUCCESS:    If successful.
   *   - SNS_RC_FAILED:     If fails.
   *
   */

  sns_rc (*write_gyro_ppe_bias)(const sns_ccd_service_gyro_ppe_bias *val);

  /**
   * @brief Get gyro ppe bias data from database.
   *
   * @note Island Support: Yes
   *
   * @param[out] val       Struct to store bias values.
   *
   * @return
   *   - SNS_RC_SUCCESS:    If successful.
   *   - SNS_RC_FAILED:     If fails.
   *
   */
  sns_rc (*read_gyro_ppe_bias)(sns_ccd_service_gyro_ppe_bias *val);

  /**
   * @brief Notify CCD to begin initialization.
   *
   * @note Island Support: Yes
   *
   * @return
   *   - SNS_RC_SUCCESS:    If successful.
   *   - SNS_RC_FAILED:     If fails.
   *
   */

  sns_rc (*notify_ccd_start_init)(void);

} sns_ccd_service_internal_api;

/*=============================================================================
  Function Declarations
  ===========================================================================*/
/**
 * @brief Function to get internal service APIs.
 * @return
 *   - sns_ccd_service_internal: Pointer if enabled.
 *   - NULL:                     Otherwise.
 *
 */
sns_ccd_service_internal *sns_ccd_get_internal_service(void);

/**
 * @brief internal ccd service init.
 *
 * @return
 *  - SNS_RC_SUCCESS:  If successful.
 *  - SNS_RC_FAILED:   If fails.
 *
 */
sns_rc sns_ccd_service_init(void);

/* This is defined in sns_ccd_sensor_instance.c: */
/**
 * @brief Function to log CCD configuration registers.
 *
 * @param[in] this         Instance pointer.
 * @param[in] suid         UID of the sensor.
 *
 * @return
 *  - SNS_RC_SUCCESS:  If successful.
 *  - SNS_RC_FAILED:   If fails.
 *
 */
sns_rc sns_ccd_log_cfg_regs(sns_sensor_instance *const this,
                            sns_sensor_uid const *const suid);

/* This is defined in sns_ccd_v*.c: */
/**
 * @brief Function to log PPE reset.
 *
 * @param[in] this         Instance pointer.
 * @param[in] suid         UID of the sensor.
 *
 * @return
 *  - None.
 *
 */
void sns_ccd_log_ppe_reset(sns_sensor_instance *const this,
                           sns_sensor_uid const *const suid);

/* This is defined in sns_ccd_v*.c: */
/**
 * @brief Function to log PPE2 reset.
 *
 * @param[in] this         Instance pointer.
 * @param[in] suid         UID of the sensor.
 *
 * @return
 *  - None.
 *
 */
void sns_ccd_log_ppe2_reset(sns_sensor_instance *const this,
                            sns_sensor_uid const *const suid);

/* This is defined in sns_ccd_v*.c: */
/**
 * @brief Function to log PPE gyroscope reset.
 *
 * @param[in] this         Instance pointer.
 * @param[in] suid         UID of the sensor.
 *
 * @return
 *  - None.
 *
 */
void sns_ccd_log_ppe_gyro_reset(sns_sensor_instance *const this,
                                sns_sensor_uid const *const suid);

/* Reset GBE hardware block. This function define under sns_ccd_hal.c*/
/* This is defined in sns_ccd_v*.c: */
/**
 * @brief Function to reset gbe.
 *
 * @param[in] this         Instance pointer.
 * @param[in] suid         UID of the sensor.
 * @param[in] gmd_sensor   GMD sensor pointer.
 *
 * @return
 *  - None.
 *
 */
void sns_ccd_reset_gbe(sns_sensor_instance *const this,
                       sns_sensor_uid const *const suid,
                       sns_sensor *gmd_sensor);

/* This is defined in sns_ccd_v*.c: */
/**
 * @brief Function to get PPE gyro bias.
 *
 * @param[in] ccd_svc         Internal service pointer.
 * @param[out] gyro_ppe_bias   GYRO ppe bias.
 *
 * @return
 *  - None.
 *
 */
void sns_ccd_get_gyro_ppe_bias(sns_ccd_service_internal *ccd_svc,
                               sns_ccd_service_gyro_ppe_bias *gyro_ppe_bias);

/* This is defined in sns_ccd_v*.c: */
/**
 * @brief Function to update gyro ppe scale cal.
 *
 * @param[in] offset_cal         Gyro ppe scale cal.
 *
 * @return
 *  - None.
 *
 */
void sns_ccd_set_gyro_ppe_scale_cal(const uint32_t offset_cal[3]);
