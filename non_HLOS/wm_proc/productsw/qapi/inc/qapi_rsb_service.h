/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

/**
 * @file qapi_rsb_service.h
 * @brief This file contains RSB service QAPI declarations
*/

#ifndef __QAPI_RSB_SERVICE_H__
#define __QAPI_RSB_SERVICE_H__

#include <stdbool.h>

/**
 * @brief Return status for RSB service QAPIs
 */
typedef enum {
    QAPI_RSB_SUCCESS = 0,           /**< Operation was successful */
    QAPI_RSB_NULL_PTR_ERROR,        /**< Operation failed due to NULL pointer arguments */
    QAPI_RSB_SAME_STATE_ERROR,      /**< Operation failed due to RSB service already being in the state which was requested */
    QAPI_RSB_WRONG_STATE_ERROR,     /**< Operation failed due to RSB service being in incorrect state */
    QAPI_RSB_ERROR                  /**< Operation failed */
} qapi_rsb_status_t;

/**
 * @brief RSB availability status
 */
typedef enum {
    QAPI_RSB_AVAILABLE = 0, /**< RSB available -> qapi_rsb_service_enable was successful */
    QAPI_RSB_UNAVAILABLE    /**< RSB unavailable -> qapi_rsb_service_disable was successful */
} qapi_rsb_avail_status_t;

/**
 * @brief RSB Rotation States
 */
typedef enum {
    QAPI_RSB_ROTATION_UP = 0, /**< RSB rotating upwards */
    QAPI_RSB_ROTATION_DOWN,   /**< RSB rotating downwards */
    QAPI_RSB_ROTATION_NO_CHANGE
} qapi_rsb_rotation_state_t;

/**
 * @brief RSB event types
 */
typedef enum {
    QAPI_RSB_EVT_ENABLE = 1, /**< Enable operation was successful */
    QAPI_RSB_EVT_DISABLE,    /**< Disable operation was successful */
    QAPI_RSB_EVT_SET_CFG,    /**< Set configuration operation was successful */
    QAPI_RSB_EVT_GET_CFG,    /**< Get configuration operation was successful */
    QAPI_RSB_EVT_SET_STANDBY,/**< Set standby operation was successful */
    QAPI_RSB_EVT_INVALID
} qapi_rsb_evt_t;

/**
 * @brief Configurable parameters for RSB service
 */
typedef enum {
    QAPI_RSB_CFG_SLEEP_MODE = 0x01,           /**< Sleep mode configuration */
    QAPI_RSB_CFG_SLEEP1_PARAM = 0x02,         /**< Sleep1 parameter configuration */
    QAPI_RSB_CFG_SLEEP2_PARAM = 0x04,         /**< Sleep2 parameter configuration */
    QAPI_RSB_CFG_SLEEP3_PARAM = 0x08,         /**< Sleep3 parameter configuration */
    QAPI_RSB_CFG_MOTION_INT_INTERVAL = 0x10,  /**< Motion interrupt interval configuration */
    QAPI_RSB_CFG_MOTION_INT_THRESHOLD = 0x20, /**< Motion interrupt threshold configuration */
    QAPI_RSB_CFG_X_RESOLUTION = 0x40          /**< X-axis resolution configuration */
} qapi_rsb_cfg_param_t;

/**
 * @brief Types of sleep configurations for RSB service
 */
typedef enum {
    QAPI_RSB_SLEEP1 = 0,          /**< Sleep1 type */
    QAPI_RSB_SLEEP2,              /**< Sleep2 type */
    QAPI_RSB_SLEEP3,              /**< Sleep3 type */
    QAPI_RSB_NUM_SLEEP_TYPES      /**< Number of sleep types */
} qapi_rsb_sleep_type_t;

/**
 * @brief Sleep mode configurations for RSB service
 */
typedef enum {
    QAPI_RSB_SLEEP1_ENABLED = 0,  /**< Sleep1 mode enabled */
    QAPI_RSB_SLEEP12_ENABLED,     /**< Sleep1 and Sleep2 modes enabled */
    QAPI_RSB_SLEEP123_ENABLED,    /**< Sleep1, Sleep2, and Sleep3 modes enabled */
    QAPI_RSB_SLEEP123_DISABLED    /**< All sleep modes disabled */
} qapi_rsb_sleep_mode_t;

/**
 * @brief Enum for different motion 
 * interrupt interval options in ms
 */
typedef enum {  
    QAPI_RSB_MOTION_INT_INTERVAL_0MS = 0,
    QAPI_RSB_MOTION_INT_INTERVAL_8MS,
    QAPI_RSB_MOTION_INT_INTERVAL_16MS,
    QAPI_RSB_MOTION_INT_INTERVAL_24MS,
    QAPI_RSB_MOTION_INT_INTERVAL_64MS,
    QAPI_RSB_MOTION_INT_INTERVAL_72MS,
    QAPI_RSB_MOTION_INT_INTERVAL_80MS,
    QAPI_RSB_MOTION_INT_INTERVAL_88MS
} qapi_rsb_motion_int_interval_t;

/**
 * @brief Individual sleep mode configuration structure
 * freq: Sampling frequency in ms
 *  For sleep1 - Each step is equivalent to 4ms
 *     |0 -> 4ms, 1 -> 8ms, 2 -> 12ms, ... , 15 -> 64ms
 *  For sleep2 - Each step is equivalent to 64ms
 *     |0 -> 64ms, 1 -> 128ms, 2 -> 192ms, ... , 15 -> 1024ms
 *  For sleep3 - Each step is equivalent to 512ms
 *     |0 -> 512ms, 1 -> 1024ms, 2 -> 1536ms, ... , 15 -> 8192ms
 * 
 * etm: Entering time in ms
 *  For sleep1 - Each step is equivalent to 21.33ms
 *     |0 -> 21.33ms, 1 -> 42.66ms, 2 -> 63.99ms, ... , 15 -> 341.28ms
 *  For sleep2 - Each step is equivalent to 20.48ms
 *     |0 -> 20.48ms, 1 -> 40.96ms, 2 -> 63.44ms, ... , 15 -> 327.68ms
 *  For sleep3 - Each step is equivalent to 20.48ms
 *     |0 -> 20.48ms, 1 -> 40.96ms, 2 -> 63.44ms, ... , 15 -> 327.68ms
 */
typedef struct {
    uint8_t freq;     /**< Sampling frequency enum in the range [0, 15] */
    uint8_t etm;      /**< Entering time enum in the range [0, 15] */
} qapi_rsb_sleep_cfg_t;

/**
 * @brief Configuration data for RSB service
 */
typedef struct {
    uint8_t motion_int_threshold; /**< Motion interrupt threshold in the range [0, 255] */
    uint16_t x_resolution; /**< X-axis resolution RES_X in range [0, 2047]. CPI_X = 7*RES_X */
    qapi_rsb_sleep_mode_t sleep_mode; /**< Sleep mode */
    qapi_rsb_motion_int_interval_t motion_int_interval; /**< Motion interrupt interval enum */
    qapi_rsb_sleep_cfg_t sleep_cfg[QAPI_RSB_NUM_SLEEP_TYPES]; /**< Sleep configuration */
} qapi_rsb_cfg_data_t;

/**
 * @brief Configuration structure for RSB service
 * params: 
 *  - specify all the parameters that needs to be configured
 *  - OR of all the parameters of type qapi_rsb_cfg_param_t
 *  required for get/set through the data field
 * 
 * data: 
 * - provide the data in the struct of type qapi_rsb_cfg_data_t for 
 * the params specified
 * - Remaining fields in the struct is don't care
 */
typedef struct {
    uint32_t params;             /**< Configuration parameters ->  */
    qapi_rsb_cfg_data_t data;    /**< Configuration data */
} qapi_rsb_cfg_t;

/**
 * @brief RSB event data structure
 */
typedef struct {
    int16_t x_delta;          /**< X-axis delta value */
    qapi_rsb_rotation_state_t rotation_state;  /**< Rotation State */
} qapi_rsb_data_t;

/**
 * @brief Handle returned by the RSB service 
 * for each of the registered client
*/
typedef uint8_t qapi_rsb_client_handle_t;

/**
 * @brief Data event callback function for RSB client
 *
 * @param[in] rsb_data RSB event data
 * @param[in] p_client_priv_data Pointer to client private data
 */
typedef void (* qapi_rsb_client_cb_t)(qapi_rsb_data_t rsb_data, void *p_client_priv_data);

/**
 * @brief Status callback function for RSB client
 *
 * @param[in] rsb_avail_status RSB service availability status
 * @param[in] p_client_priv_data Pointer to client private data
 */
typedef void (* qapi_rsb_status_cb_t)(qapi_rsb_avail_status_t rsb_avail_status, void *p_client_priv_data);

/**
 * @brief Callback function for RSB events
 *
 * @param[in] evt Event type
 * @param[in] evt_status Event status
 * @param[in] data Event data
 */
typedef void (*qapi_rsb_evt_cb_t)(qapi_rsb_evt_t evt, qapi_rsb_status_t evt_status, void *data);

/**
 * @brief Enable the RSB service
 *
 * @param[in] power_on_req flag to indicate whether to power ON RSB
 * @param[in] p_cfg Pointer to the RSB configuration structure (optional)
 * - keep it NULL if none of the configurable parameter needs 
 * to be adjusted and the default configuration should be used
 * @param[in] cb Event callback function to receive the request status
 * 
 * @return QAPI_RSB_SUCCESS on successful request submission
 */
qapi_rsb_status_t qapi_rsb_service_enable(bool power_on_req,
    const qapi_rsb_cfg_t *p_cfg, qapi_rsb_evt_cb_t cb);
 
/**
 * @brief Disable the RSB service
 *
 * @param[in] power_off_req flag to indicate whether to power OFF RSB
 * @param[in] cb Event callback function to receive the request status
 * 
 * @return QAPI_RSB_SUCCESS on successful request submission
 */
qapi_rsb_status_t qapi_rsb_service_disable(bool power_off_req, qapi_rsb_evt_cb_t cb);

/**
 * @brief Set the configuration for the RSB service
 *
 * @param[in] cfg configuration parameters and its values
 * @param[in] cb Event callback function to receive the request status
 * 
 * @return QAPI_RSB_SUCCESS on successful request submission
 */
qapi_rsb_status_t qapi_rsb_service_set_config(const qapi_rsb_cfg_t cfg, qapi_rsb_evt_cb_t cb);
 
/**
 * @brief Get the current configuration of the RSB service
 *
 * @param[in] params configuration parameters to be retrieved
 * @param[in] cb Event callback function to receive the requested configuration
 * 
 * @return QAPI_RSB_SUCCESS on successful request submission
 */
qapi_rsb_status_t qapi_rsb_service_get_config(uint32_t params, qapi_rsb_evt_cb_t cb);
 
/**
 * @brief Set the standby mode for the RSB service
 *
 * @param[in] enable Enable or disable standby mode
 * @param[in] cb Event callback function to receive the request status
 * 
 * @return QAPI_RSB_SUCCESS on successful request submission
 */
qapi_rsb_status_t qapi_rsb_service_set_standby(bool enable, qapi_rsb_evt_cb_t cb);
 
/**
 * @brief Register RSB service data event callback
 *
 * @param[in] cbfun Callback function
 * @param[in] p_client_priv_data Pointer to the client private data
 * @param[out] p_handle Pointer to the client handle for the data event callback
 * 
 * @return QAPI_RSB_SUCCESS on successful registration
 */
qapi_rsb_status_t qapi_rsb_service_register_cb(
    qapi_rsb_client_cb_t cbfun,
    void *p_client_priv_data,
    qapi_rsb_client_handle_t *p_handle);

/**
 * @brief De-Register RSB service data event callback
 *
 * @param[in] handle Client handle for the data event callback
 * 
 * @return QAPI_RSB_SUCCESS on successful deregistration
 */
qapi_rsb_status_t qapi_rsb_service_deregister_cb(
    qapi_rsb_client_handle_t handle);

/**
 * @brief Register RSB availability status callback
 *  - Useful when some client other than the caller of
 *    qapi_rsb_service_enable or qapi_rsb_service_disable
 *    need to know about the status of rsb enable or disable
 * 
 *  - Note that the client handle (*p_handle) for data event
 *    callback and status callback are different
 * 
 * @param[in] cbfun Callback function
 * @param[in] p_client_priv_data Pointer to the client private data
 * @param[out] p_handle Pointer to the client handle for the status callback
 * 
 * @return QAPI_RSB_SUCCESS on successful registration
 */
qapi_rsb_status_t qapi_rsb_service_register_status_cb(
    qapi_rsb_status_cb_t cbfun,
    void *p_client_priv_data,
    qapi_rsb_client_handle_t *p_handle
);

/**
 * @brief De-Register RSB availability status callback
 *
 *  - Note that the client handle for data event
 *    callback and status callback are different
 * 
 * @param[in] handle Client handle for the status callback
 * 
 * @return QAPI_RSB_SUCCESS on successful deregistration
 */
qapi_rsb_status_t qapi_rsb_service_deregister_status_cb(
    qapi_rsb_client_handle_t handle);

#endif // __QAPI_RSB_SERVICE_H__