#pragma once
/** ============================================================================
 * @file
 *
 * @brief Utilities used in conjunction with registry.
 * All utilities in this file are intended for physical sensor
 * dirvers and can be used in island mode.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

#include <stdbool.h>
#include "sns_attribute_service.h"
#include "sns_list.h"
#include "sns_registry.pb.h"

/*=============================================================================
  Macros
  ===========================================================================*/

/** @brief maximum length of registry name */
#define SNS_REGISTRY_MAX_NAME_LEN    51

/** @brief maximum number of parse groups */
#define SNS_REGISTRY_MAX_PARSE_GROUP 3

/** @brief maximum length of PID */
#define SNS_REGISTRY_MAX_PID_LEN     2

/** @brief signal for registry write operation */
#define SNS_REGISTRY_WRITE_SIGNAL             0x01
/** @brief signal for download load operation */
#define SNS_REGISTRY_DL_LOAD_SIGNAL           0x02

/** @brief signal for RPCD up operation */
#define SNS_REGISTRY_RPCD_UP_SIGNAL           0x04

/** @brief signal for RPCD down operation */
#define SNS_REGISTRY_RPCD_DN_SIGNAL           0x08
/** @brief signal for thread up operation */
#define SNS_REGISTRY_THRD_UP_SIGNAL           0x10

/** @brief signal for power rail off operation */
#define SNS_REGISTRY_PWR_RAIL_OFF_SIGNAL      0x20

/** @brief signal for library delete operation */
#define SNS_REGISTRY_LIB_DELETE_SIGNAL        0x40

/** @brief signal for secure file write operation */
#define SNS_REGISTRY_SECURE_FILE_WRITE_SIGNAL 0x80

/** @brief maximum length of dataype in dependency json */
#define SNS_MAX_DATATYPE_LEN 32

/*=============================================================================
  Forward Declarations
  ===========================================================================*/
struct sns_registry_data_item;
struct pb_buffer_arg;

/**
 * @brief Function used to parse items belonging to a specific registry
 * group.
 *
 * @param[in] reg_item        Pointer to sns_registry_data_item.
 * @param[in] item_name       Pointer to decoded registry item name as
 *                            defined in sns_registry_data_item.
 * @param[in] item_str_val    Pointer to decoded registry item
 *                            string value as defined in sns_registry_data_item.
 * @param[out] parsed_buffer  Opaque buffer pointing to location
 *                            where parsed items are to be stored.
 *
 * @return
 * - True  Successfully parsed registry item.
 * - False Failed to parse registry item.
 *
 */
typedef bool (*sns_registry_parse_group)(sns_registry_data_item *reg_item,
                                         struct pb_buffer_arg *item_name,
                                         struct pb_buffer_arg *item_str_val,
                                         void *parsed_buffer);

/**
 * @brief Registry Group Parsing information used in
 * sns_registry_decode_arg. Defines how to parse registry
 * items belonging to a given registry group.
 *
 */
typedef struct
{
  char group_name[SNS_REGISTRY_MAX_NAME_LEN]; /*!< Name of the registry
                                               *   group.
                                               */

  sns_registry_parse_group parse_func; /*!< The function that must be
                                        *   called when items of the
                                        *   above mentioned group are
                                        *   found.
                                        */

  void *parsed_buffer; /*!< The pointer to buffer that
                        *   must be passed to the
                        *   parse_func where parsed
                        *   output will be stored.
                        */
} sns_registry_group_parse_info;

/**
 * @brief PB Decode argument used with sns_registry_item_decode_cb.
 *
 */
typedef struct
{
  uint8_t parse_info_len; /*!< The length of the
                           *   parse_info array
                           */

  sns_registry_group_parse_info
      parse_info[SNS_REGISTRY_MAX_PARSE_GROUP]; /*!< Parsing information for
                                                 *   each registry group
                                                 *   present in the registry
                                                 *   read event.
                                                 */

  struct pb_buffer_arg *item_group_name; /*!< The registry group that
                                          *    currently decoded item
                                          *    belongs to.
                                          */

  uint32_t version; /*!<  The registry item version
                     *    number.
                     */

} sns_registry_decode_arg;

/**
 * @brief Triaxial sensor axis indexing convention.
 *
 */
typedef enum
{
  TRIAXIS_X = 0,  /*!< X axis. */
  TRIAXIS_Y = 1,  /*!< Y axis. */
  TRIAXIS_Z = 2,  /*!< Z axis. */
  TRIAXIS_NUM = 3 /*!< Number of axes. */
} triaxis;

/**
 * @brief Triaxial sensor input to output axis conversion.
 *
 */
typedef struct
{
  triaxis ipaxis; /*!< Input axis index. */

  triaxis opaxis; /*!< Output axis index the above input axis maps to.*/

  bool invert; /*!< Whether the input needs to be negated when
                * translating to the output axis.
                */

} triaxis_conversion;

/**
 * @brief Registry items supported as part of physical sensor
 * configuration registry group.
 *
 */
typedef struct
{
  int64_t hw_id;    /*!< Hardware ID*/
  uint8_t is_dri;   /*!< Whether the devices supports DRI mode */
  uint8_t res_idx;  /*!< Resolution index */
  bool sync_stream; /*!< Sync stream supported? */
} sns_registry_phy_sensor_cfg;

/**
 * @brief Registry items supported as part of physical sensor
 * platform configuration registry group.
 *
 */
typedef struct
{
  uint32_t slave_config;

  uint32_t min_bus_speed_khz;               /*!< Minimum bus speed in kHz 
                                             *   (deprecated, set to 0) 
                                             */          
  uint32_t max_bus_speed_khz;               /*!< Maximum bus speed in kHz */
  uint32_t dri_irq_num;                     /*!< Data ready interrupt number */
  uint16_t max_odr;                         /*!< Maximum output data rate */
  uint8_t min_odr;                          /*!< Minimum output data rate */
  uint8_t bus_type;                         /*!< Type of bus (e.g., I2C, SPI) */
  uint8_t bus_instance;                     /*!< Instance of the bus */ 
  uint8_t reg_addr_type;                    /*!< Type of register address */
  uint8_t irq_pull_type;                    /*!< Interrupt pull type (e.g., 
                                            *    pull-up, pull-down) 
                                            */
  uint8_t irq_drive_strength;               /*!< Interrupt drive strength */
  uint8_t irq_trigger_type;                 /*!< Interrupt trigger type 
                                             *   (e.g., edge, level) 
                                             */
  uint8_t num_rail;                         /*!< Number of power rails */
  uint8_t rail_on_state;                    /*!< State of the rail when on */
  uint8_t rigid_body_type;                  /*!< Type of rigid body */
  uint8_t i3c_address;                      /*!< I3C address */
  uint8_t sub_sys_type;                     /*!< Subsystem type */
  bool irq_is_chip_pin;                     /*!< Indicates if IRQ is a 
                                             *   chip pin 
                                             */
  char vddio_rail[SNS_REGISTRY_MAX_NAME_LEN];  /*!< VDDIO rail name */
  char vdd_rail[SNS_REGISTRY_MAX_NAME_LEN];    /*!< VDD rail name */
  char clk_in_rail[SNS_REGISTRY_MAX_NAME_LEN]; /*!< Clock input rail name */
  uint8_t qup_type;                          /*!< QUP type */
  uint8_t qup_instance;                      /*!< QUP instance */
  uint32_t manufacturer_id;                  /*!< Manufacturer ID */
  uint32_t provisional_id;                   /*!< Provisional ID */  
} sns_registry_phy_sensor_pf_cfg;

/**
 * @brief Registry items supported as part of MD
 * configuration registry group.
 *
 */
typedef struct
{
  float thresh; /*!< threshold in m/s2 */
  float win;    /*!< window in sec */
  bool disable; /*!< disable */
} sns_registry_md_cfg;

/**
 * @brief Structure to be used in sns_registry_parse_dependency_cb.
 *
 */
typedef struct sns_dep_sensor_attr
{
  sns_list_item list_entry;

  sns_attribute_id attr_id; /*!< attribute id */

  size_t attr_val_size; /*!< attribute value's size */

  void *attr_val; /*!< attribute value */
} sns_dep_sensor_attr;

/**
 * @brief Structure to be used in sns_registry_parse_dependency_cb.
 *
 */
typedef struct sns_dep_sensor_info
{
  sns_list_item list_entry;

  sns_list attr_list; /*!< List of attributes.
                       *   List item type : sns_dep_sensor_attr
                       */

  char datatype[SNS_MAX_DATATYPE_LEN]; /*!< datatype of dependency */
} sns_dep_sensor_info;

/**
 * @brief Callback function used to decode sns_registry_read_event
 * Function will extract each item within the read event
 * including items within subgroups and will optionally run a
 * parsing function on the item passed.
 *
 * @param[in] stream Refer pb_callback_s::decode.
 * @param[in] field  Refer pb_callback_s::decode.
 * @param[in] arg    sns_registry_decode_arg type.
 *
 * @return
 *  - True  Successfully decoded registry item.
 *  - False Fail to decode registry item.
 *
 */
bool sns_registry_item_decode_cb(pb_istream_t *stream, const pb_field_t *field,
                                 void **arg);

/**
 * @brief Function used to parse items belonging to physical sensor
 * configuration registry group.
 *
 * @param[in] reg_item        Pointer to sns_registry_data_item.
 * @param[in] item_name       Pointer to decoded registry item name as
 *                            defined in sns_registry_data_item.
 * @param[in] item_str_val    Pointer to decoded registry item
 *                            string value as defined in sns_registry_data_item.
 * @param[out] parsed_buffer  sns_registry_phy_sensor_cfg type
 *                            where parsed items are stored.
 *
 * @return
 *  - True  Successfully parsed registry physical sensor config.
 *  - False Failure to parse registry physical sensor config.
 *
 */
bool sns_registry_parse_phy_sensor_cfg(sns_registry_data_item *reg_item,
                                       struct pb_buffer_arg *item_name,
                                       struct pb_buffer_arg *item_str_val,
                                       void *parsed_buffer);

/**
 * @brief Function used to parse items belonging to physical sensor
 * platform configuration registry group.
 *
 * @param[in] reg_item        Pointer to sns_registry_data_item
 * @param[in] item_name       Pointer to decoded registry item name as
 *                            defined in sns_registry_data_item.
 * @param[in] item_str_val    Pointer to decoded registry item
 *                            string value as defined in sns_registry_data_item.
 * @param[out] parsed_buffer  Of type sns_registry_phy_sensor_pf_cfg,
 *                            where parsed items are stored.
 *
 * @return
 *  - True  Successfully parsed registry physical sensor platform config.
 *  - False Fail to parse registry physical sensor platform config.
 *
 */
bool sns_registry_parse_phy_sensor_pf_cfg(sns_registry_data_item *reg_item,
                                          struct pb_buffer_arg *item_name,
                                          struct pb_buffer_arg *item_str_val,
                                          void *parsed_buffer);

/**
 * @brief Function used to parse items belonging to physical sensor
 * platform configuration registry group.
 *
 * @param[in] reg_item        Pointer to sns_registry_data_item
 * @param[in] item_name       Pointer to decoded registry item name as
 *                            defined in sns_registry_data_item.
 * @param[in] item_str_val    Pointer to decoded registry item
 *                            string value as defined in sns_registry_data_item.
 * @param[out] parsed_buffer  Of type sns_registry_phy_sensor_pf_cfg
 *                            where parsed items are stored.
 *
 * @return
 *  - True   Successfully parse item.
 *  - False  Fail to parse item.
 *
 */
bool sns_registry_parse_axis_orientation(sns_registry_data_item *reg_item,
                                         struct pb_buffer_arg *item_name,
                                         struct pb_buffer_arg *item_str_val,
                                         void *parsed_buffer);

/**
 * @brief Function used to parse N dimensional float array.
 *
 * @param[in] reg_item        Pointer to sns_registry_data_item.
 * @param[in] item_name       Pointer to decoded registry item name as
 *                            defined in sns_registry_data_item.
 * @param[in] item_str_val    Pointer to decoded registry item
 *                            string value as defined in sns_registry_data_item.
 * @param[out] parsed_buffer  Of type vector3 where parsed items
 *                            are stored.
 *
 * @return
 *  - True   Successfully parsed N dimensional float array.
 *  - False  Fail to parsed N dimensional float array.
 *
 */
bool sns_registry_parse_float_arr(sns_registry_data_item *reg_item,
                                  struct pb_buffer_arg *item_name,
                                  struct pb_buffer_arg *item_str_val,
                                  void *parsed_buffer);

/**
 * @brief Function used to parse items belonging to factory calibration
 * scale registry group.
 *
 * @param[in] reg_item        Pointer to sns_registry_data_item.
 * @param[in] item_name       Pointer to decoded registry item name as
 *                            defined in sns_registry_data_item.
 * @param[in] item_str_val    Pointer to decoded registry item
 *                            string value as defined in sns_registry_data_item.
 * @param[out] parsed_buffer  Of type matrix3 where parsed items
 *                            are stored.
 *
 * @return
 *  - True   Successfully parse item.
 *  - False  Fail to parse item.
 *
 */
bool sns_registry_parse_scale(sns_registry_data_item *reg_item,
                              struct pb_buffer_arg *item_name,
                              struct pb_buffer_arg *item_str_val,
                              void *parsed_buffer);

/**
 * @brief Function used to parse items belonging to factory calibration
 * 3 * 3 correction matrix registry group.
 *
 * @param[in] reg_item        Pointer to sns_registry_data_item.
 * @param[in] item_name       Pointer to decoded registry item name as
 *                            defined in sns_registry_data_item.
 * @param[in] item_str_val    Pointer to decoded registry item
 *                            string value as defined in sns_registry_data_item.
 * @param[out] parsed_buffer  Of type matrix3 where parsed items
 *                            are stored.
 *
 * @return
 * - True   Successfully parse item.
 * - False  Fail to parse item.
 *
 */
bool sns_registry_parse_corr_matrix_3(sns_registry_data_item *reg_item,
                                      struct pb_buffer_arg *item_name,
                                      struct pb_buffer_arg *item_str_val,
                                      void *parsed_buffer);

/**
 * @brief Function used to parse items belonging to Motion Detect
 * Config registry group.
 *
 * @param[in] reg_item        Pointer to sns_registry_data_item.
 * @param[in] item_name       Pointer to decoded registry item name as
 *                            defined in sns_registry_data_item.
 * @param[in] item_str_val    Pointer to decoded registry item
 *                            string value as defined in sns_registry_data_item.
 * @param[out] parsed_buffer  Of type sns_registry_md_cfg where
 *                            parsed items are stored.
 * @return
 * - True   Successfully parse item.
 * - False  Fail to parse item.
 *
 */
bool sns_registry_parse_md_cfg(sns_registry_data_item *reg_item,
                               struct pb_buffer_arg *item_name,
                               struct pb_buffer_arg *item_str_val,
                               void *parsed_buffer);

/**
 * @brief Function for decoding the registry event for dependency info.
 *
 * @param[in]   stream  The stream from which to decode dependency info from.
 * @param[in]   field   The format of the nested message value.
 * @param[out]  arg     sns_list that gets populated. list item type :
 * sns_dep_sensor_info
 *
 * @return
 *  - True   On success.
 *  - False  On failure.
 *
 */
bool sns_registry_parse_dependency_cb(pb_istream_t *stream,
                                      const pb_field_t *field, void **arg);

/**
 * @brief Function to get signal Registry task.
 *
 * @param[in] sig Signal to registry task.
 *
 */
#ifndef SNS_DISABLE_REGISTRY
void sns_sensor_signal_registry_task(unsigned int sig);
#else
#define sns_sensor_signal_registry_task(sig) NULL
#endif