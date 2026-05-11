#pragma once
/*=============================================================================
  @file sns_async_uart.h

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/**==========================================================================
  Includes
=============================================================================*/

#include <stdbool.h>
#include "stddef.h"
#include <stdint.h>

#include "sns_rc.h"

/**==========================================================================
  Type Definitions
=============================================================================*/

/**
 * @brief Number of bits per data byte
 */
typedef enum
{
  SNS_UART_5_BITS_PER_CHAR = 0, /*!< 5 bits per character */
  SNS_UART_6_BITS_PER_CHAR = 1, /*!< 6 bits per character */
  SNS_UART_7_BITS_PER_CHAR = 2, /*!< 7 bits per character */
  SNS_UART_8_BITS_PER_CHAR = 3, /*!< 8 bits per character */
} sns_uart_bits_per_char;

/**
 * @brief Number of stop bits per data frame
 */
typedef enum
{
  SNS_UART_0_5_STOP_BITS = 0, /*!< 0.5 stop bits */
  SNS_UART_1_0_STOP_BITS = 1, /*!< 1 stop bits */
  SNS_UART_1_5_STOP_BITS = 2, /*!< 1.5 stop bits */
  SNS_UART_2_0_STOP_BITS = 3, /*!< 2 stop bits */
} sns_uart_num_stop_bits;

/**
 * @brief Type of parity
 */
typedef enum
{
  SNS_UART_NO_PARITY = 0,    /*!< No parity bit */
  SNS_UART_ODD_PARITY = 1,   /*!< Odd parity */
  SNS_UART_EVEN_PARITY = 2,  /*!< Even parity */
  SNS_UART_SPACE_PARITY = 3, /*!< Space parity */
} sns_uart_parity_mode;

/**
 * @brief Type of flow control
 */
typedef enum
{
  SNS_UART_FLOW_CTRL_NONE = 0,                 /*!< No Flow control */
  SNS_UART_FLOW_CTRL_REQ_CLR = 1,              /*!< RTS/CTS Flow Control */
  SNS_UART_FLOW_CTRL_TERM_READY_SET_READY = 2, /*!< DTS/DTR Flow Control */
} sns_uart_flow_ctrl;

/**
 * @brief Callback function for uart transfers
 */
typedef void (*sns_uart_cb_func)(uint32_t, void *);

/**
 * @brief Opaque handle to uart interface
 */
typedef struct sns_uart_handle sns_uart_handle;

/**
 * @brief Uart configuration parameters
 */
typedef struct sns_uart_config
{
  bool enable_loopback;
  sns_uart_flow_ctrl flow_ctrl;
  uint32_t uart_peripheral_instance;
  uint32_t baud_rate;
  sns_uart_parity_mode parity_mode;
  sns_uart_num_stop_bits num_stop_bits;
  sns_uart_bits_per_char bits_per_char;
  sns_uart_cb_func tx_cb;
  sns_uart_cb_func rx_cb;

} sns_uart_config;

/**
 * @brief Uart transfer structure
 */
typedef struct sns_uart_transfer
{
  uint8_t *buf;    /*!< Buffer for rx/tx transfer */
  size_t buf_size; /*!< Size of buffer  */
  void *cb_data;   /*!< Data to be passed to callback function */
} sns_uart_transfer;

/**==========================================================================
  Public Functions
=============================================================================*/

/**
 * @brief Open the UART port with the given configuration.
 *
 * @param[in, out] handle - Address of uart handle
 * @param[in] config - uart port configuration. See sns_uart_config.
 *
 * @return
 * SNS_RC_SUCESS
 * SNS_RC_FAILED
 */
sns_rc sns_uart_open(sns_uart_handle **handle, sns_uart_config *config);

/**
 * @brief Close the UART port. Cancels pending transfers. Cleanup interal state.
 *
 * @param[in, out] handle - Address of uart handle
 *
 * @return
 * SNS_RC_SUCESS
 * SNS_RC_FAILED
 */
sns_rc sns_uart_close(sns_uart_handle **handle);

/**
 * @brief Puts the buffer in a queue for transmitting data. The TX callback
 * provided in sns_uart_open() will be invoked when the transfer is complete.
 *
 * Only one buffer buffer can be queued at a time
 *
 * @param[in] handle - uart handle
 * @param[in] tx_msg - tx message structure. See sns_uart_transfer.
 *
 * @return
 * SNS_RC_SUCESS - success
 * SNS_RC_FAILED - failure
 */
sns_rc sns_uart_transmit(sns_uart_handle *handle, sns_uart_transfer *tx_msg);

/**
 * @brief Puts the buffer in a queue for receiving data. The RX callback
 * provided in sns_uart_open() will be invoked when the transfer is complete.
 *
 * @param[in] handle - uart handle
 * @param[in] rx_msg - rx message structure. See sns_uart_transfer.
 *
 * @return
 * SNS_RC_SUCESS - success
 * SNS_RC_FAILED - failure
 */
sns_rc sns_uart_receive(sns_uart_handle *handle, sns_uart_transfer *rx_msg);
