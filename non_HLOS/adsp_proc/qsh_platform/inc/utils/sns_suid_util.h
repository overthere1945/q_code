#pragma once
/** ============================================================================
 * @file
 *
 * @brief A simple utility to look-up dependent SUIDs.  Intended for use by
 * Sensors;
 * is a replacement for/wrapper around sending requests to the SUID Lookup
 * Sensor directly.
 * The Sensor developer should add SNS_SUID_LOOKUP_DATA as field to their Sensor
 * state structure, specifying the number of data types they intend to look-up;
 * this should correspond to the number of times they plan to call
 * sns_suid_lookup_add().  Number should be chosen precisely: larger than
 * necessary will result in wasted memory; if smaller the requested data types
 * will not be discovered.
 * If the Sensor is expected to choose amongst multiple SUIDs of the same
 * data type, they may register a sns_suid_lookup_cb within
 * SNS_SUID_LOOKUP_INIT or SNS_SUID_LOOKUP_INIT_V2 if using the
 * generic_lookup_cb defined in this utiliy.
 * If so they will receive one callback per SUID discovered, and be given
 * the attributes event for it.  The Sensor must return true/false whether it
 * wishes to store and use this SUID.
 *
 * @note: All utilities in this file can be used in island mode.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 * ===========================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_attribute_util.h"
#include "sns_data_stream.h"
#include "sns_mem_util.h"
#include "sns_sensor_uid.h"
#include "sns_types.h"
#include <stdint.h>

/*=============================================================================
  Forward Declarations
  ===========================================================================*/

struct sns_sensor;
struct sns_sensor_event;

/*=============================================================================
  Macro Definitions
  ===========================================================================*/

/**
 * @brief Anonymous structure definition generator.
 *
 * @param[in] cnt Number of data types to look-up.
 *
 */
#define SNS_SUID_LOOKUP_DATA(cnt)                                              \
  struct                                                                       \
  {                                                                            \
    uint16_t sensors_cnt;                                                      \
    uint16_t suid_idx;                                                         \
    sns_suid_lookup_cb suid_cb;                                                \
    sns_data_stream *suid_stream;                                              \
    sns_data_stream *attr_stream;                                              \
    struct                                                                     \
    {                                                                          \
      bool is_selected;                                                        \
      char *data_type;                                                         \
      sns_sensor_uid suid;                                                     \
    } sensors[cnt];                                                            \
  }

/**
 * @brief Anonymous structure definition generator with modified suid_cb type.
 *
 * @param[in] cnt Number of data types to look-up.
 *
 */
#define SNS_SUID_LOOKUP_DATA_V3(cnt)                                           \
  struct                                                                       \
  {                                                                            \
    uint16_t sensors_cnt;                                                      \
    uint16_t suid_idx;                                                         \
    sns_suid_generic_lookup_cb suid_cb;                                        \
    sns_data_stream *suid_stream;                                              \
    sns_data_stream *attr_stream;                                              \
    struct                                                                     \
    {                                                                          \
      bool is_selected;                                                        \
      char *data_type;                                                         \
      sns_sensor_uid suid;                                                     \
    } sensors[cnt];                                                            \
  }

/**
 * @brief Wild Card based lookup (will return all SUIDs).
 *
 */
#define SNS_SUID_LOOKUP_ANY (char *)&suid_lookup_any_data_type

#define SNS_SUID_END_OF_DATA_TYPES ""

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief Optional callback registered by Sensor.
 * If callback is provided, all SUIDs associated with the data type will be
 * discovered. For each discovered SUID, the registered callback will be called
 * once with the corresponding attribute event.
 * If callback is not provided, only the default SUID associated with the data
 * type will be discovered.
 *
 * @param[in] sensor    Same Sensor as provided in sns_suid_lookup_add.
 * @param[in] data_type Data type under review.
 * @param[in] event     Encoded attribute event.
 *
 * @return
 * - True to save this SUID.
 * - False if to continue search.
 *
 */
typedef bool (*sns_suid_lookup_cb)(struct sns_sensor *const sensor,
                                   char const *data_type,
                                   struct sns_sensor_event *event);

/**
 * @brief Structure containing sensor name and list of desired attributes
 *
 */
typedef struct sns_sensor_attribute_data
{
  char data_type[32];
  sns_sensor_util_generic_attrib *generic_attrib_list;
} sns_sensor_attribute_data;

/**
 * @brief Optional callback registered by Sensor.
 * If callback is provided, all SUIDs associated with the data type will be
 * discovered. For each discovered SUID, the registered callback will be called
 * once with the corresponding attribute event.
 * If callback is not provided, only the default SUID associated with the data
 * type will be discovered.
 *
 * @param[in] sensor    Same Sensor as provided in sns_suid_lookup_add.
 * @param[in] event     Encoded attribute event.
 * @param[in] generic_attrib_list List of attributes used to match sensor in cb
 *
 * @return
 * - True to save this SUID.
 * - False if to continue search.
 *
 */
typedef bool (*sns_suid_generic_lookup_cb)(
    struct sns_sensor *const sensor, struct sns_sensor_event const *event,
    const sns_sensor_util_generic_attrib *generic_attrib_list);

/*=============================================================================
  Function Declarations
  ===========================================================================*/

/**
 * @brief Initialize the data created by SNS_SUID_LOOKUP_DATA.
 *
 * @param[in] suid_lookup_data  Data created by SNS_SUID_LOOKUP_DATA.
 * @param[in] cb                Optional callback function; see
 *                              sns_suid_lookup_cb.
 *
 */
#define SNS_SUID_LOOKUP_INIT(suid_lookup_data, cb)                             \
  do                                                                           \
  {                                                                            \
    sns_memzero(&suid_lookup_data, sizeof(suid_lookup_data));                  \
    suid_lookup_data.sensors_cnt = ARR_SIZE(suid_lookup_data.sensors);         \
    suid_lookup_data.suid_cb = cb;                                             \
  } while(false)

/**
 * @brief Initialize the data created by SNS_SUID_LOOKUP_DATA.
 *
 * @param[in] suid_lookup_data_v3  Data created by SNS_SUID_LOOKUP_DATA.
 * @param[in] cb                   Optional callback function; see
 *                                 sns_suid_lookup_cb.
 *
 */
#define SNS_SUID_LOOKUP_INIT_V3(suid_lookup_data_v3, cb)                       \
  do                                                                           \
  {                                                                            \
    sns_memzero(&suid_lookup_data_v3, sizeof(suid_lookup_data_v3));            \
    suid_lookup_data_v3.sensors_cnt = ARR_SIZE(suid_lookup_data_v3.sensors);   \
    suid_lookup_data_v3.suid_cb = cb;                                          \
  } while(false)

/**
 * @brief Compare decoded attr string against desired attr string
 *
 * @param[in] attrib_decoded attr decoded from suid event
 * @param[in] attrib_match desired attribute to match sensor during cb
 *
 * @return
 * - True if strings match
 * - False otherwise
 *
 */
bool sns_attr_string_comp_fcn(
    const sns_sensor_util_attrib *restrict attrib_decoded,
    const sns_sensor_util_attrib *restrict attrib_match);

/**
 * @brief Compare decoded attr value against desired attr value
 *
 * @param[in] attrib_decoded attr decoded from suid event
 * @param[in] attrib_match desired attribute to match sensor during cb
 *
 * @return
 * - True if values match
 * - False otherwise
 *
 */
bool sns_attr_value_comp_fcn(
    const sns_sensor_util_attrib *restrict attrib_decoded,
    const sns_sensor_util_attrib *restrict attrib_match);

bool sns_generic_suid_lookup_cb(
    struct sns_sensor *const sensor, struct sns_sensor_event const *event,
    const sns_sensor_util_generic_attrib *generic_attrib_list);

/**
 * @brief Deinitialize the data initialized by SNS_SUID_LOOKUP_INIT.  Closes all
 * data streams but leaves all retrieved SUIDs accessible.
 * Should not be called if Sensor is required to act upon subsequently
 * missing dependencies (i.e. a dependency goes away).
 * Causes island mode exit.
 *
 * @param[in] sensor Use same Sensor as provided in sns_suid_lookup_add.
 * @param[in] suid_lookup_data Data created by SNS_SUID_LOOKUP_DATA.
 *
 * @return
 * - None.
 *
 */
void sns_suid_lookup_deinit(struct sns_sensor *sensor, void *suid_lookup_data);

/**
 * @brief Deinitialize the data initialized by SNS_SUID_LOOKUP_INIT.  Closes all
 * data streams but leaves all retrieved SUIDs accessible.
 * Should not be called if Sensor is required to act upon subsequently
 * missing dependencies (i.e. a dependency goes away).
 * Causes island mode exit.
 *
 * @param[in] sensor Use same Sensor as provided in sns_suid_lookup_add.
 * @param[in] suid_lookup_data Data created by SNS_SUID_LOOKUP_DATA.
 *
 * @return
 * - None.
 *
 */
void sns_suid_lookup_deinit_v3(struct sns_sensor *sensor,
                               void *suid_lookup_data);

/**
 * Initiate SUID lookup for a data type. The SUIDs will be discovered according
 * to the parameters set in SNS_SUID_LOOKUP_INIT.
 * Causes island mode exit.
 *
 * @param[in] sensor Use same Sensor as provided in sns_suid_lookup_add.
 * @param[in] suid_lookup_data Lookup data initialized by SNS_SUID_LOOKUP_INIT.
 * @param[in] data_type Null-terminated data type string to lookup.
 *
 * @return
 *  None.
 *
 */
void sns_suid_lookup_add(struct sns_sensor *sensor, void *suid_lookup_data,
                         char *data_type);

/**
 * @brief Initiate SUID lookup for a data type. The SUIDs will be discovered
 * according to the parameters set in SNS_SUID_LOOKUP_INIT.
 * Causes island mode exit.
 *
 * @param[in] sensor            Use same Sensor as provided in
 *                              sns_suid_lookup_add.
 * @param[in] suid_lookup_data  Lookup data initialized by SNS_SUID_LOOKUP_INIT.
 * @param[in] data_type         Null-terminated data type string to lookup.
 * @param[in] register_updates  Register for updates to the list of SUIDs for
 *                              the given data_type.
 * @return
 *  None.
 *
 */
void sns_suid_lookup_add_v2(struct sns_sensor *sensor, void *suid_lookup_data,
                            char *data_type, bool register_updates);

/**
 * @brief Initiate SUID lookup for a data type. The SUIDs will be discovered
 * according to the parameters set in SNS_SUID_LOOKUP_INIT.
 * Causes island mode exit.
 *
 * @param[in] sensor            Use same Sensor as provided in
 *                              sns_suid_lookup_add.
 * @param[in] suid_lookup_data_v3  Lookup data initialized by
 * SNS_SUID_LOOKUP_INIT.
 * @param[in] data_type         Null-terminated data type string to lookup.
 * @return
 *  None.
 *
 */
void sns_suid_lookup_add_v3(struct sns_sensor *sensor,
                            void *suid_lookup_data_v3, char *data_type);

/**
 * @brief Peek the SUID being looked-up and associated with data type.
 * Causes island mode exit.
 *
 * @param[in] suid_lookup_data Data on which sns_suid_lookup_add was called.
 * @param[in] data_type Null-terminated data type string to lookup.
 * @param[out] suid Sensor UID if found (optional).
 *
 * @return
 * - True   If SUID has been found.
 * - False  Otherwise.
 *
 */
bool sns_suid_lookup_peek(void *suid_lookup_data, char const *data_type,
                          sns_sensor_uid *suid);

/**
 * @brief Get the SUID already looked-up and associated with data type.
 * Causes island mode exit.
 *
 * @param[in] suid_lookup_data Data on which sns_suid_lookup_add was called.
 * @param[in] data_type Null-terminated data type string to lookup.
 * @param[out] suid Sensor UID if found (optional).
 *
 * @return
 * - True if SUID has been found and valid.
 * - False otherwise.
 *
 */
bool sns_suid_lookup_get(void *suid_lookup_data, char const *data_type,
                         sns_sensor_uid *suid);

/**
 * @brief Get the SUID already looked-up and associated with data type.
 * Causes island mode exit.
 *
 * @param[in] suid_lookup_data_v3 Data on which sns_suid_lookup_add was called.
 * @param[in] data_type Null-terminated data type string to lookup.
 * @param[out] suid Sensor UID if found (optional).
 *
 * @return
 * - True if SUID has been found and valid.
 * - False otherwise.
 *
 */
bool sns_suid_lookup_get_v3(void *suid_lookup_data_v3, char const *data_type,
                            sns_sensor_uid *suid);

/**
 * @brief Whether the SUID lookup process has completed successfully.
 *
 * @param[in] suid_lookup_data Data on which sns_suid_lookup_add was called.
 *
 * @return
 * -True if all requested data_type SUIDs were found.
 * -False otherwise.
 *
 */
bool sns_suid_lookup_complete(void *suid_lookup_data);

/**
 * @brief Whether the SUID lookup process has completed successfully.
 *
 * @param[in] suid_lookup_data Data on which sns_suid_lookup_add was called.
 *
 * @return
 * -True if all requested data_type SUIDs were found.
 * -False otherwise.
 *
 */
bool sns_suid_lookup_complete_v3(void *suid_lookup_data);

/**
 * @brief Handle a SUID or attribute event.  Must be called within the Sensor's
 * notify_event function. Causes island mode exit.
 *
 * @param[in] sensor Use same Sensor as provided in sns_suid_lookup_add.
 * @param[in] suid_lookup_data Lookup data initialized by SNS_SUID_LOOKUP_INIT.
 *
 * @return sns_rc SNS_RC_SUCCESS if successful.
 *
 */
sns_rc sns_suid_lookup_handle(struct sns_sensor *sensor,
                              void *suid_lookup_data);

/**
 * @brief Handle a SUID or attribute event.  Must be called within the Sensor's
 * notify_event function. Causes island mode exit.
 *
 * @param[in] sensor Use same Sensor as provided in sns_suid_lookup_add.
 * @param[in] suid_lookup_data_v3 Lookup data initialized by
 * SNS_SUID_LOOKUP_INIT.
 * @param[in] sensor_dep_list Structure containing sensor name and list of
 * desired attributes
 *
 * @return sns_rc SNS_RC_SUCCESS if successful.
 *
 */
sns_rc sns_suid_lookup_handle_v3(struct sns_sensor *sensor,
                                 void *suid_lookup_data_v3,
                                 sns_sensor_attribute_data *sensor_dep_list);

/**
 * @brief Generate a random SUID.
 *
 * @param[inout] uid             SUID to be populated.
 *
 * @return
 *   - None.
 *
 */
void sns_sensor_util_generate_random_suid(sns_sensor_uid *const uid);
