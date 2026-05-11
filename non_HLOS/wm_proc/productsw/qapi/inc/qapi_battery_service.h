/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#ifndef __BATTERY_SERVICE_H__
#define __BATTERY_SERVICE_H__

#include <stdint.h>
#include <errno.h>
#include <stdlib.h>

/** battery service state. */
typedef enum
{
    QAPI_BATTERY_SERVICE_STATE_ENABLED,     /**< Service enabled.*/
    QAPI_BATTERY_SERVICE_STATE_DISABLED,    /**< Service disabled.*/
    QAPI_BATTERY_SERVICE_STATE_ERROR,       /**< Service disabled due to
                                                    error.*/
    QAPI_BATTERY_SERVICE_STATE_FAIL,        /**< Failed to execute
                                                    the command.*/
}qapi_battery_service_state_t;

/** Type of charger. */
typedef enum 
{
    QAPI_BATTERY_SERVICE_USB_STATUS_POWER_ONLINE = 0x01,
    QAPI_BATTERY_SERVICE_USB_STATUS_DISCHARGING = 0x02,
    QAPI_BATTERY_SERVICE_USB_STATUS_CHARGING = 0x04,
    QAPI_BATTERY_SERVICE_USB_STATUS_CRITICAL = 0x08
}qapi_battery_service_usb_status_bit_t;

/** Type of charger. */
typedef enum 
{
    QAPI_BATTERY_SERVICE_CHARGER_UNKNOWN = 0,
    QAPI_BATTERY_SERVICE_CHARGER_USB_SDP = 1,
    QAPI_BATTERY_SERVICE_CHARGER_DCP = 2,
    QAPI_BATTERY_SERVICE_CHARGER_CDP = 3,
    QAPI_BATTERY_SERVICE_CHARGER_ACA = 4,
    QAPI_BATTERY_SERVICE_CHARGER_TYPE_C = 5,
    QAPI_BATTERY_SERVICE_CHARGER_PD = 6,
    QAPI_BATTERY_SERVICE_CHARGER_PD_DRP = 7,
    QAPI_BATTERY_SERVICE_CHARGER_PD_PPS = 8,
    QAPI_BATTERY_SERVICE_CHARGER_APPLE = 9,
    QAPI_BATTERY_SERVICE_CHARGER_HVDCP2 = 0x80,
    QAPI_BATTERY_SERVICE_CHARGER_HVDCP3 = 0x81,
    QAPI_BATTERY_SERVICE_CHARGER_HVDCP3P5 = 0x82
}qapi_battery_service_charger_t;

/**
 * Battery info structure
 */
typedef struct
{
    uint8_t soc; /**< Ranges from 0-100.*/
    uint8_t usb_plug_in_sts; /** Bits corresponding to qapi_battery_service_usb_status_bit_t will be set.*/
    qapi_battery_service_charger_t charger_type;
    int16_t battery_temperature;/**< battery temperature in degree celsius */
    uint32_t battery_voltage_mv;   /**< voltage of the battery in mV e.g. 3700*/
    int32_t battery_current_ma;    /**< current of the battery in mA e.g. 100*/
}qapi_battery_service_batt_info_t;

/** 
 * Battery service state handler.
 * @param[in] state corresponding state update.
 * @param[in] status 0 on success, or non-zero value corresponding to the error.
  */
typedef void (*qapi_battery_service_enable_resp_cb_t)(
                    qapi_battery_service_state_t state, int err);

/**
 * Battery service client callback.
 * @param[in] info    Battery status.
 * @param[in] priv      Privilized data that is passed during client registration.
 */
typedef void (*qapi_battery_service_client_cb_t)(
    const qapi_battery_service_batt_info_t* status, const void* priv);

/**
 * Register for battery service status events.
 * @note This API should only used by only one entity. All the service
 * status information like enable/disable/failure will be send on the callback.
 * @param[in] cb callback to be invoken on battery service availability status.
 * @return 0 on success, or non-zero value corresponding to the error.
 */
int qapi_battery_service_register_service_status(
                    qapi_battery_service_enable_resp_cb_t cb);

/**
 * Enable or Disable battery service.
 * This enables/disables the battery service. The response will be sent to
 * the callback register with qapi_battery_service_register_service_status.
 * @param[in] enable true to enable and false to disable.
 * @return 0 on success, or non-zero value corresponding to the error.
 */
int qapi_battery_service_enable(bool enable);

/**
 * Register for battery events.
 * @param[in] cb callback to invoke on battery status change.
 * @param[in] priv client priviliged data that would be sent along in the callback.
 * @return 0 on success, or non-zero value corresponding to the error.
 */
int qapi_battery_service_register(qapi_battery_service_client_cb_t cb, const void* priv);

/**
 * De-register for battery events.
 * @param[in] cb callback used for registration.
 * @return 0 on success, or non-zero value corresponding to the error.
 */
int qapi_battery_service_deregister(qapi_battery_service_client_cb_t cb);

/**
 * Get battery status
 * @param[out] info Battery status if the function returns success.
 * @return 0 on success, or non-zero value corresponding to the error.
 */
int qapi_battery_service_get_status(qapi_battery_service_batt_info_t *info);

#endif /**< __BATTERY_SERVICE_H__ */