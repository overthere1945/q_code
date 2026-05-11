#pragma once
/** ============================================================================
 * @file
 *
 * @brief Common definitions used by synch_com_port (SCP) and
 * asynch_com_port (ACP).
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
/**
 * @brief Macros to extract the manufacturer id and vendor value from the PID
 * 
 */
#ifdef SSC_TARGET_ASPEN_SWM
#define EXTRACT_MANUFACTURER_ID(bytes) \
(((uint16_t)(bytes[5]) << 8) | (uint16_t)(bytes[0])) & 0x7FFF

#define EXTRACT_VENDOR_VALUE(bytes) \
        ((uint32_t)(bytes[3]) << 24) | ((uint32_t)(bytes[2]) << 16) | \
    ((uint32_t)(bytes[1]) << 8) | (uint32_t)(bytes[0])
#else
#define EXTRACT_MANUFACTURER_ID(bytes) \
    (((uint16_t)(bytes[0]) << 8) | (uint16_t)(bytes[1])) & 0x7FFF

#define EXTRACT_VENDOR_VALUE(bytes) \
    ((uint32_t)(bytes[2]) << 24) | ((uint32_t)(bytes[3]) << 16) | \
    ((uint32_t)(bytes[4]) << 8) | (uint32_t)(bytes[5])

#define EXTRACT_PROVISIONAL_ID_TYPE_SELECTOR(bytes) \
((bytes[1] & 0x01))
#endif



/**
 * @brief Types of register addresses.
 * These values are used for specificying the size the
 * register being accessed.
 *
 */
typedef enum
{
  SNS_REG_ADDR_8_BIT,  /*!< For accessing 8 Bit  sized register. */
  SNS_REG_ADDR_16_BIT, /*!< For accessing 16 Bit sized register. */
  SNS_REG_ADDR_32_BIT, /*!< For accessing 32 Bit sized register. */
  /* Additional register types will be added here. */
} sns_reg_addr_type;

/**
 * @brief Types of communication ports.
 *
 */
typedef enum
{
  SNS_BUS_MIN = 0,           /*!< When used, default to I2C communication. */
  SNS_BUS_I2C = SNS_BUS_MIN, /*!< I2C communication. */
  SNS_BUS_SPI = 1,           /*!< SPI communication. */
  SNS_BUS_UART = 2,          /*!< DEPRECATED. Please use the async_uart
                              *   sensor.
                              */
  SNS_BUS_I3C = 3,           /*!< I3C Standard Data Rate. */
  SNS_BUS_I3C_SDR = SNS_BUS_I3C, /*!< DEPRECATED. Please use sns_bus_mode. */
  SNS_BUS_I3C_HDR_DDR = 4,       /*!< DEPRECATED. Please use sns_bus_mode. */
  SNS_BUS_I3C_I2C_LEGACY = 5,    /*!< For I2C devices on an I3C bus. */
  SNS_BUS_REMOTE = 6,            /*!< For communicating with a device remotly
                                  *   (Not directly connected).
                                  */
  SNS_BUS_MAX = SNS_BUS_REMOTE,  /*!< When used, default to
                                  *   Remote communication.
                                  */
} sns_bus_type;

/**
 * @brief Bus Operation Modes.
 *
 */
typedef enum
{
  SNS_BUS_MODE_0, /*!< I3C : SDR     || SPI : CPOL = 0, CPHA = 0 */
  SNS_BUS_MODE_1, /*!< I3C : HDR-DDR || SPI : CPOL = 0, CPHA = 1 */
  SNS_BUS_MODE_2, /*!< I3C : HDR-TSP || SPI : CPOL = 1, CPHA = 0 */
  SNS_BUS_MODE_3, /*!< I3C : HDR-TSL || SPI : CPOL = 1, CPHA = 1 */
} sns_bus_mode;

/**
 * @brief SPI R/W Bit Options.
 * For Modes 0,1 the RW Bit will always be set at the Most Significant bit
 * of the Most Significant byte and the address bytes arranged in Big Endian
 * Order. e.g for a 2 Byte SPI Write using 16-bit addressing the format will be
 * : [RW A14 A13 A12 A11 A10 A9 A8] [A7 A6 A5 A4 A3 A2 A1 A0] [D7 D6 D5 D4 D3 D2
 * D1 D0] [D15 D14 D13 D12 D11 D10 D9 D8]. Mode 2 is custom Mode and the caller
 * should format the address bytes as needed.
 *
 */
typedef enum
{
  SNS_SPI_R_W_BIT_MODE_0, /*!< For Register READ  = R/W Bit = 1. */
                          /*!< For Register Write = R/W Bit = 0. */
  SNS_SPI_R_W_BIT_MODE_1, /*!< For Register READ  = R/W Bit = 0. */
                          /*!< For Register Write = R/W Bit = 1. */
  SNS_SPI_R_W_BIT_MODE_2, /*!< For Register READ  = R/W Bit = No Change. */
                          /*!< For Register Write = R/W Bit = No Change. */
} sns_spi_reg_addr_mode;

/**
 * @brief Sub system types.
 *
 */
typedef enum
{
  SNS_SUB_SYS_0 = 0, /*!< SEE running sub system. */
  SNS_SUB_SYS_1      /*!< Remote sub system1. */
} sns_sub_sys_type;

/**
 * @brief QUP types.
 *
 */
typedef enum
{
  SNS_QUP_MIN = 0,
  SNS_SSC_QUP = SNS_QUP_MIN, /**< default SSC QUP. */
  SNS_TOP_QUP = 1,
  SNS_QUP_MAX = SNS_TOP_QUP
} sns_qup_type;

/**
 * @brief COM Port Configuration.
 *
 */
typedef struct
{
  sns_bus_type bus_type;  /*!< Bus type from sns_bus_type.*/
  uint32_t slave_control; /*!< Slave Address for I2C.
                           *   Dynamic Slave Address for I3C.
                           *   Chip Select for SPI.
                           */

  sns_reg_addr_type reg_addr_type; /*!< Register address type for the slave.*/
  sns_qup_type qup_type;           /*!< 0 for SSC and 1 for Top Level,
                                    *   Default 0.
                                    */

  sns_sub_sys_type sub_sys_type; /*!< 0: SEE running sub sys, rest of them
                                  *   belongs to remote sub sys.
                                  */

  uint32_t min_bus_speed_KHz; /*!< Minimum bus clock supported by slave
                               *   in kHz.
                               */

  uint32_t max_bus_speed_KHz; /*!< Maximum bus clock supported by slave
                               *   in kHz.
                               */

  uint8_t bus_instance; /*!< Platform bus instance number
                         *  (BLSP number).
                         */

  uint8_t qup_instance; /*!< QUP instance number.*/

  uint32_t manufacturer_id; /*!< Manufacturer Identifier*/

  uint32_t provisional_id; /*!< Provisional Identifier */

} sns_com_port_config;

/**
 * @brief COM Port Configuration (extended).
 * Handled by sns_sync_vectored_rw_spi_ext API only.
 *
 */
typedef struct
{
  size_t size_of_this_struct;    /*!< Size of the structure to detect
                                  *   dynamic linking mismatches.
                                  */
  sns_bus_mode bus_mode;         /*!< The I3C or SPI sub Bus Type.*/
  sns_spi_reg_addr_mode rw_mode; /*!< Applicable for Bus Type SPI.*/
} sns_com_port_config_ex;

/**
 * @brief Read or Write transaction on a port.
 *
 */
typedef struct
{
  bool is_write;     /*!< true for a write operation. false for read.*/
  uint32_t reg_addr; /*!< Register address for transfer.*/
  uint32_t bytes;    /*!< Number of bytes to read or write. */
  uint8_t *buffer;   /*!< Buffer to read or write. */
} sns_port_vector;

/**
 * @brief I3C slave information
 * 
 */
typedef struct
{
  uint8_t dcr;           /*!< Device Characteristics Register */
  uint8_t bcr;           /*!< Bus Characteristic Register */
  uint8_t pid_bytes[6];  /*!< 48-bit Provisional ID of the slave */
  uint8_t dynamic_addr;  /*!< Dynamic I3C address of the slave */
} sns_i3c_slave_info;

/**
 * @brief Structure to hold list of I3C slaves.
 *
 */
typedef struct
{
  uint32_t requested_slave_count;    /*!< Number of slaves requested for this
                                      *   manufacturer ID  */
  sns_i3c_slave_info *i3c_slave_info;/*!< List of I3C slave info for 
                                      *   the requested count.
                                      *   @note: Memory for the structure
                                      *   must be allocated by the caller. */
} sns_com_port_i3c_slaves;


/**
 * @brief (Mask) 0 if no interrupts are pending. If more than one interrupt is
 * set, contains the highest priority interrupt. See Mipi Alliance
 * Specificiation for I3C for complete information.
 *
 */
#define SNS_CCC_STATUS_IRQ_PENDING_MASK 0x0F

/**
 * @brief (Mask) Reserved for future use.
 *
 */
#define SNS_CCC_STATUS_RESERVED_MASK 0x10

/**
 * @brief (Mask) Set to 1 if slave detects protocol error since the last status
 * read.
 *
 */
#define SNS_CCC_STATUS_PROTOCOL_ERROR_MASK 0x20

/**
 * @brief (Mask) Slave's current activity mode.
 *
 */
#define SNS_CCC_STATUS_ACTIVITY_MODE_MASK 0xC0

/**
 * @brief (Read/Write Speed) MXDS sustained speed of f_SCL max.
 * is default value.
 *
 */
#define SNS_CCC_MXDS_SPEED_MAX 0

/**
 * @brief (Read/Write Speed) MXDS sustained speed of 8MHz.
 *
 */
#define SNS_CCC_MXDS_SPEED_8MHZ 1

/**
 * @brief (Read/Write Speed) MXDS sustained speed of 6MHz.
 *
 */
#define SNS_CCC_MXDS_SPEED_6MHZ 2

/**
 * @brief (Read/Write Speed) MXDS sustained speed of 4MHz.
 *
 */
#define SNS_CCC_MXDS_SPEED_4MHZ 3

/**
 * @brief (Read/Write Speed) MXDS sustained speed of 2MHz.
 *
 */
#define SNS_CCC_MXDS_SPEED_2MHZ 4

/**
 * @brief (Read/Write Speed) Mask for MXDS speed.
 *
 */
#define SNS_CCC_MXDS_SPEED_MASK 0x07

/**
 * @brief (Clock Turnarounds) Clock to Data turnaround time of:
 * T_SCO less than 8ns.
 *
 */
#define SNS_CCC_MXDS_TURNAROUND_LT_8NS (0 << 3)

/**
 * @brief (Clock Turnarounds) Clock to Data turnaround time of:
 * T_SCO less than 9ns.
 *
 */
#define SNS_CCC_MXDS_TURNAROUND_LT_9NS (1 << 3)

/**
 * @brief (Clock Turnarounds) Clock to Data turnaround time of:
 * T_SCO less than 10ns.
 *
 */
#define SNS_CCC_MXDS_TURNAROUND_LT_10NS (2 << 3)

/**
 * @brief (Clock Turnarounds) Clock to Data turnaround time of:
 * T_SCO less than 11ns.
 *
 */
#define SNS_CCC_MXDS_TURNAROUND_LT_11NS (3 << 3)

/**
 * @brief (Clock Turnarounds) Clock to Data turnaround time of:
 * T_SCO less than 12ns.
 *
 */
#define SNS_CCC_MXDS_TURNAROUND_LT_12NS (4 << 3)

/**
 * @brief (Clock Turnarounds) Mask for clock to data turnaround time.
 *
 */
#define SNS_CCC_MXDS_TURNAROUND_MASK (0x7 << 3)

/**
 * @brief (SETXTIME Command Byte) Command: Sync Tick -- 0 additional bytes.
 *
 */
#define SNS_CCC_SETXTIME_ST 0x7F

/**
 * @brief (SETXTIME Command Byte) Command: Delay Tick -- 1 additional byte.
 *
 */
#define SNS_CCC_SETXTIME_DT 0xBF

/**
 * @brief (SETXTIME Command Byte) Command: Enter async mode 0. See spec.
 *
 */
#define SNS_CCC_SETXTIME_ENTER_ASYNC_0 0xDF

/**
 * @brief (SETXTIME Command Byte) Command:  Enter async mode 1. See spec.
 *
 */
#define SNS_CCC_SETXTIME_ENTER_ASYNC_1 0xEF

/**
 * @brief (SETXTIME Command Byte) Command: Enter async mode 2. See spec.
 *
 */
#define SNS_CCC_SETXTIME_ENTER_ASYNC_2 0xF7

/**
 * @brief (SETXTIME Command Byte) Command: Enter async mode 3. See spec.
 *
 */
#define SNS_CCC_SETXTIME_ENTER_ASYNC_3 0xFB

/**
 * @brief (SETXTIME Command Byte) Command: Async trigger.
 *
 */
#define SNS_CCC_SETXTIME_ASYNC_TRIGGER 0xFD

/**
 * @brief (SETXTIME Command Byte) Command: Repitition period -- 1 or more bytes.
 *
 */
#define SNS_CCC_SETXTIME_TPH 0x3F

/**
 * @brief (SETXTIME Command Byte) Command: Time unit -- one byte.
 *
 */
#define SNS_CCC_SETXTIME_TU 0x9F

/**
 * @brief (SETXTIME Command Byte) Command: ODR -- one byte.
 *
 */
#define SNS_CCC_SETXTIME_ODR 0x8F

/**
 * @brief (XTIME Mode) Support sync.
 *
 */
#define SNS_CCC_GETXTIME_SUPPORT_SYNC 0x01

/**
 * @brief (XTIME Mode) Async 0 sync.
 *
 */
#define SNS_CCC_GETXTIME_SUPPORT_ASYNC_0 0x02

/**
 * @brief (XTIME Mode) Async 1 sync.
 *
 */
#define SNS_CCC_GETXTIME_SUPPORT_ASYNC_1 0x04

/**
 * @brief (XTIME Mode) Async 2 sync.
 *
 */
#define SNS_CCC_GETXTIME_SUPPORT_ASYNC_2 0x08

/**
 * @brief (XTIME Mode) Async 3 sync.
 *
 */
#define SNS_CCC_GETXTIME_SUPPORT_ASYNC_3 0x10

/**
 * @brief (XTIME States) Sync state.
 *
 */
#define SNS_CCC_GETXTIME_STATE_SYNC 0x01

/**
 * @brief (XTIME States) Async 0 state.
 *
 */
#define SNS_CCC_GETXTIME_STATE_ASYNC_0 0x02

/**
 * @brief (XTIME States) Async 1 state.
 *
 */
#define SNS_CCC_GETXTIME_STATE_ASYNC_1 0x04

/**
 * @brief (XTIME States) Async 2 state.
 *
 */
#define SNS_CCC_GETXTIME_STATE_ASYNC_2 0x08

/**
 * @brief (XTIME States) Async 3 state.
 *
 */
#define SNS_CCC_GETXTIME_STATE_ASYNC_3 0x10

/**
 * @brief (XTIME States) Overflow state.
 *
 */
#define SNS_CCC_GETXTIME_STATE_OVERFLOW 0x80

/**
 * @brief Com Port options.
 *
 */
typedef enum sns_sync_com_port_ccc
{
  SNS_SYNC_COM_PORT_CCC_ENEC, /*!< Enable slave event driven interrupts.
                               *   Write. One byte. Set to 1 to enable
                               *   interrupts. See spec for other values.
                               */

  SNS_SYNC_COM_PORT_CCC_DISEC, /*!< Disable slave event driven interrupts.
                                *   Write. One byte. Set to 1 to disable
                                *   interrupts. See spec for other values.
                                */

  SNS_SYNC_COM_PORT_CCC_SETDASA, /*!< Assigns a dynamic address to a slave
                                  *   with a known static address. Write.
                                  *   One byte. The dynamic address
                                  *   (left shifted one bit to allow for r/w
                                  *   bit). NOTE: this must be sent on a port
                                  *   opened with the slave's I2C static
                                  *   address. The port must be closed &
                                  *   re-opened if the address changes.
                                  */
  SNS_SYNC_COM_PORT_CCC_RSTDAA,  /*!< Resets dynamic address assignment.
                                  *   Zero bytes. NOTE: This must be sent on
                                  *   a port opened with the slave's current
                                  *   I3C dynamic address. The port must be
                                  *   closed & re-opened to communicate with
                                  *   the slave if the I2C static address is
                                  *   not the same as the previously assigned
                                  *   I3C address.
                                  */

  SNS_SYNC_COM_PORT_CCC_SETMWL, /*!< Set maximum write length. Write.
                                 *   Two bytes. Max length in bytes
                                 *   (MSB first).
                                 */

  SNS_SYNC_COM_PORT_CCC_SETMRL, /*!< Set maximum read length in a single
                                 *   command. Write. Two or Three bytes.
                                 *   Max read length: 2 bytes (MSB first).
                                 *   Max IBI read length
                                 *   (if IBI data supported).
                                 */

  SNS_SYNC_COM_PORT_CCC_GETMWL, /*!< Get maximum write length. Read. Two
                                 *   bytes. Max length in bytes (MSB first).
                                 */

  SNS_SYNC_COM_PORT_CCC_GETMRL, /*!< Get maximum read length. Read.
                                 *   Two or Three bytes. Max read length:
                                 *   2 bytes (MSB first). Max IBI read
                                 *   length (if IBI data supported).
                                 */

  SNS_SYNC_COM_PORT_CCC_GETPID, /*!< Get a slave's provisional id (PID).
                                 *   Read. Six bytes.
                                 */

  SNS_SYNC_COM_PORT_CCC_GETBCR, /*!< Get a device's bus characteristic
                                 *   (BCR). Read. One byte.
                                 */

  SNS_SYNC_COM_PORT_CCC_GETDCR, /*!< Get a device's device characteristics
                                 *   (DCR). Read. One byte.
                                 */

  SNS_SYNC_COM_PORT_CCC_GETSTATUS, /*!< Get a device's operating status.
                                    *   Read. Two bytes MSB first: vendor
                                    *   reserved byte LSB second: see
                                    *   SNS_CCC_STATUS_* masks.
                                    */

  SNS_SYNC_COM_PORT_CCC_GETMXDS, /*!< Get sdr mode max read and write data
                                  *   speeds (& optionally max read
                                  *   turnaround time). Read. Two or Five
                                  *   bytes. byte[0]: Max write clock
                                  *   byte[1]: Max Read clock | clock to data
                                  *   turnaround time: optional bytes[2..4]:
                                  *   Max read turnaround time in uSec, LSB
                                  *   first.
                                  */

  SNS_SYNC_COM_PORT_CCC_SETXTIME, /*!< Framework for exchanging event timing
                                   *   information. Write. Various number
                                   *   of bytes byte[0]: SETXTIME defining
                                   *   byte optional bytes: See
                                   *   SNS_CCC_SETXTIME_* for number of
                                   *   optional bytes.
                                   */

  SNS_SYNC_COM_PORT_CCC_GETXTIME, /*!< Framework for exchanging event timing
                                   *   information. Read. Four bytes.
                                   *   byte[0]: Supported modes. See
                                   *   SNS_CCC_GETXTIME_SUPPORT_*.
                                   *   byte[1]: State. See
                                   *   SNS_CCC_GETXTIME_STATE_.
                                   *   byte[2]: Internal oscilator frequency,
                                   *   in increments of 0.5 MHz
                                   *   (0-->~32kHz) byte[3]: Inaccuracy byte.
                                   *   Max variation of slave's oscillator,
                                   *   in 0.1% increments.
                                   */
  SNS_SYNC_COM_PORT_CCC_SETNEWDA, /*!< Assigns a new dynamic address to a slave
                                   *   with an existing dynamic address.
                                   *   Write one byte: the new dynamic address
                                   *   (left shifted 1 bit to allow for r/w
                                   * bit). NOTE: This must be sent on a port
                                   * opened with the slave's current dynamic
                                   * address. The port must be closed and
                                   * re-opened if the address changes.
                                   */

} sns_sync_com_port_ccc;
