#pragma once
/**
 * @file sns_interrupt.h
 *
 * @brief Target-specific interrupt configurations.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 **/

#include "sns_interrupt.pb.h"
#include "sns_sensor_instance.h"
#include "sns_request.h"
#include "sns_time.h"
#include <stdio.h>

/**============================================================================
  Type Definitions
  ===========================================================================*/
/**
 * Interrupt timestamp status.
 */
typedef enum
{
  SNS_INTERRUPT_TIMESTAMP_INVALID = 0x0, /**< Timestamp is not valid */
  SNS_INTERRUPT_TIMESTAMP_VALID = 0x1,   /**< Timestamp is valid */
  SNS_INTERRUPT_TIMESTAMP_INVALID_OVERFLOW =
      0x10, /**< Timestamp is invalid, and there has been an overflow */
  SNS_INTERRUPT_TIMESTAMP_VALID_OVERFLOW =
      0x11 /**< Timestamp is valid, and there has been an overflow */
} sns_interrupt_timestamp_status;

typedef struct
{
  /** Timestamp when IBI occurred */
  sns_time timestamp;
  union
  {
    /** For hardware interrupts */
    sns_interrupt_timestamp_status ts_status;

    /** For IBI */
    uint8_t num_ibi_bytes;
  };
} sns_interrupt_ibi_data;

typedef void *(*sns_interrupt_notify_function_ptr)(uint32_t);
typedef void (*sns_interrupt_ibi_notify_function)(
    sns_sensor_instance *const inst, sns_interrupt_ibi_data *ibi_data);

/** Signal state maintained per gpio. */
typedef struct
{
  /** Timestamp of most recent trigger event on the irq_pin. */
  sns_time timestamp;

  /** Interrupt number. */
  uint32_t irq_num;

  /** Pin config of the interrupt. */
  uint32_t irq_pin_cfg;

  /** Number of times an unregistered interrupt fires.
   *  This is used for debug only. */
  uint16_t warning_cnt;

  /** The trigger config of this interrupt. */
  sns_interrupt_trigger_type trigger_type;

  /** True if the interrupt is controlled by a GPIO in the same hardware
   *  block as the sensors subsystem. */
  bool is_chip_pin;

  /** true if this signal is registered. */
  bool is_registered;

  /** true if this interrupt uses the hardware timestamping unit to capture the
   *  timestamp. */
  bool uses_hw_timestamp_unit;

} sns_interrupt_info;

typedef struct
{
  /** Timestamp of most recent trigger event of the callbackw */
  sns_time timestamp;

  sns_interrupt_ibi_notify_function notify;

  /** Place to store IBI data. Must point to memory accessible by the I3C bus
   *  driver in all power modes. */
  uint8_t *ibi_data_buf;

  /** Opaque data used by each PAL  */
  void *pal_config;

  /** Parent sensor instance */
  sns_sensor_instance *inst;

} sns_ibi_info;

typedef struct
{
  sns_sensor_instance *const inst;
  sns_interrupt_info *irq_info;
  sns_interrupt_notify_function_ptr func;
} sns_interrupt_gpio_register_param;

/**============================================================================
  Public Functions
  ===========================================================================*/
/**
 * @brief Register interrupt signal with GPIOInt.
 *
 * @param[in] reg_param  info required for gpio registration.
 *
 * @return
 *  - SNS_RC_SUCCESS - successful registration.
 *  - SNS_RC_INVALID_VALUE - invalid request parameters.
 *  - SNS_RC_NOT_SUPPORTED - interrupt is not supported.
 */
sns_rc sns_interrupt_gpio_register(sns_interrupt_gpio_register_param *reg_param)
    __attribute__((nonnull));

/**
 * @brief Deregister interrupt signal with GPIOInt
 *
 * @param[in] reg_param  Same info used for gpio registration.
 *
 * @return
 *  - SNS_RC_SUCCESS - successful deregistration.
 *  - SNS_RC_INVALID_VALUE - invalid request parameters.
 *  - SNS_RC_NOT_SUPPORTED - interrupt is not supported.
 */
sns_rc
sns_interrupt_gpio_deregister(sns_interrupt_gpio_register_param *reg_param)
    __attribute__((nonnull));

/**
 * @brief Re-enables the given level-triggered interrupt
 *
 * @param[in] this        Pointer to the sensor instance.
 * @param[in] irq_num     Interrupt number.
 *
 * @return
 *  - SNS_RC_SUCCESS - interrupt re-enabled.
 *  - SNS_RC_FAILED - failed to re-enable interrupt.
 *  - SNS_RC_NOT_SUPPORTED - interrupt is not supported.
 *
 */
sns_rc sns_interrupt_reenable(sns_sensor_instance *const this,
                              uint32_t irq_num);

/**
 * @brief Attempt to read the timestamp that was captured by the interrupt's HW
 * time-stamping unit.
 *
 * @param[in] this        Pointer to the sensor instance, for logging purposes.
 * @param[in] gpio        Interrupt pin to read the timestamp from.
 * @param[out] timestamp  Where the timestamp is placed.
 * @param[out] ts_status  Timestamp status.
 *
 * @return
 *  - SNS_RC_SUCCESS if the timestamp was successfully read.
 *  - SNS_RC_NOT_SUPPORTED if HW timestamp is not supported.
 *  - SNS_RC_FAILED otherwise.
 */
sns_rc sns_interrupt_read_timestamp(sns_sensor_instance *const this,
                                    uint32_t gpio, sns_time *timestamp,
                                    sns_interrupt_timestamp_status *ts_status);

/**
 * @brief Register IBI with bus driver

 * @param[in] this       Current instance.
 * @param[in] new_req    The new IBI request.
 * @param[in] ibi_req    The existing IBI request, if any.
 * @param[in] ibi_info   The IBI config.
 *
 * @return
 *  - SNS_RC_SUCCESS - successful IBI registration.
 *  - SNS_RC_INVALID_VALUE - invalid request.
 *  - SNS_RC_NOT_SUPPORTED - IBI is not supported.
 */
sns_rc sns_interrupt_register_ibi(sns_sensor_instance *const this,
                                  sns_ibi_req const *new_req,
                                  sns_ibi_req *ibi_req, sns_ibi_info *ibi_info);

/**
 * @brief De-Register the IBI
 *
 * @param[in] this       Current instance.
 * @param[in] ibi_req    The existing IBI request.
 * @param[in] ibi_info   The IBI config to match.
 *
 * @return
 *  - SNS_RC_SUCCESS -       Successful IBI deregistration.
 *  - SNS_RC_INVALID_VALUE - Invalid request.
 *  - SNS_RC_NOT_SUPPORTED - IBI is not supported.
 */
sns_rc sns_interrupt_deregister_ibi(sns_sensor_instance *const this,
                                    sns_ibi_req *ibi_req,
                                    sns_ibi_info *ibi_info);
