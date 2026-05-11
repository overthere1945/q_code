/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#ifndef __QSH_DISPLAY_BRIDGE_H__
#define __QSH_DISPLAY_BRIDGE_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#define QAPI_QSH_METRIC_NAME_MAX_LEN (48)                /**< Maximum length of metric name */
#define QAPI_QSH_METRIC_DATA_MAX_LEN (6 * sizeof(float)) /**< Maximum length of metric data */

/**
 * Supported sensor event types.
 */
typedef enum
{
    QAPI_QSH_EVENT_TYPE_PEDO = 0,   /**< Pedometer.*/
    QAPI_QSH_EVENT_TYPE_ALS,        /**< Ambeint light sensor.*/
    QAPI_QSH_EVENT_TYPE_TILT,       /**< Tilt sensor.*/

    QAPI_QSH_EVENT_TYPE_FORMATTER_LISTENER, /**< Formatter sensor event */
    QAPI_QSH_EVENT_TYPE_HEALTH_LISTENER, /**< health offload sensor event */

    QAPI_QSH_EVENT_TYPE_MAX
}qapi_qsh_event_type_t;

/**
 * Supported sensor event sub-types.
 */
typedef enum
{
    QAPI_QSH_EVENT_SUBTYPE_BRIDGE,
    QAPI_QSH_EVENT_SUBTYPE_STANDARD,
    QAPI_QSH_EVENT_SUBTYPE_METRIC_STATUS,
    QAPI_QSH_EVENT_SUBTYPE_ERROR,
    QAPI_QSH_EVENT_SUBTYPE_MAX
}qapi_qsh_event_subtype_t;

/**
 * Status of the sensor
 */
typedef enum
{
    QAPI_QSH_SNS_UNAVAILABLE,
    QAPI_QSH_SNS_AVAILABLE,
    QAPI_QSH_SNS_CONNECTED,
    QAPI_QSH_SNS_DISCONNECTED,
    QAPI_QSH_SNS_ERROR
}qapi_qsh_sns_status_t;

/**
 * Sensor metric id.
 */
typedef uint32_t qapi_qsh_metric_id_t;

/**
 * Sensor standard error types.
 */
typedef enum
{
    QAPI_QSH_NO_ERROR        = 0,
    QAPI_QSH_FAILED          = 1,
    QAPI_QSH_NOT_SUPPORTED   = 2,
    QAPI_QSH_INVALID_TYPE    = 3,
    QAPI_QSH_INVALID_STATE   = 4,
    QAPI_QSH_INVALID_VALUE   = 5,
    QAPI_QSH_NOT_AVAILABLE   = 6,
    QAPI_QSH_POLICY          = 7
}qapi_qsh_std_sensor_error_t;

typedef enum
{
    QAPI_QSH_ACCURACY_UNKNOWN = 0,
    QAPI_QSH_ACCURACY_NO_CONTACT,
    QAPI_QSH_ACCURACY_UNRELIABLE,
    QAPI_QSH_ACCURACY_LOW,
    QAPI_QSH_ACCURACY_MEDIUM,
    QAPI_QSH_ACCURACY_HIGH
}qapi_qsh_accuracy_t;

typedef struct
{
    uint32_t num_axis;
    float data[16];
}qapi_qsh_std_sensor_evt_data_t; 
 
typedef struct
{
    float sample_rate_hz;
    uint32_t batch_period_us;
    uint32_t flush_period_us;
    qapi_qsh_accuracy_t min_accuracy;
}qapi_qsh_evt_config_t;

/**
 * Structure holding formatter metric event data.
 */
typedef struct
{
    uint32_t metric_id;     /**< Metric id.*/
    uint8_t goal_comparision; /**< 0=unknown, 1=greater or equal,2= lesser or equal.*/
    uint8_t value_type;    /**< data type; int=3, float=2, bytes=1 */
    uint8_t len;           /**< metric data len */
    int8_t data[QAPI_QSH_METRIC_DATA_MAX_LEN];  /**< metric data */
}qapi_qsh_health_formatter_metric_data_t;

/**
 * Structure for ALS data
 */
typedef struct
{
    float lux; /**< lux value.*/
    float raw_adc_value; /**< raw adc value associated with lux.*/
}qapi_qsh_als_data_t;

/**
 * Health data types.
 */
typedef enum
{
    QAPI_QSH_HEALTH_METRIC_DATA = 0,
    QAPI_QSH_HEALTH_DETECT_EVT,
    QAPI_QSH_HEALTH_STATS,
    QAPI_QSH_HEALTH_AUTO_EVT,
    QAPI_QSH_HEALTH_EXERCISE_EVT,
    QAPI_QSH_HEALTH_LOCATION
}qapi_qsh_health_event_type_t;

typedef enum
{
  QAPI_QSH_HEALTH_UNSPECIFIED = 0,
  QAPI_QSH_HEALTH_GREATER_THAN_OR_EQUAL = 1,
  QAPI_QSH_HEALTH_LESS_THAN_OR_EQUAL = 2
}qapi_qsh_health_goal_t;

/**
 * Represents byte array variable.
 */
typedef struct
{
    uint8_t *arr;
    uint32_t size;
}qapi_qsh_byte_arr_t;

/*
   Type of aggregation
*/
typedef enum
{
    QAPI_QSH_HEALTH_AGGREGATION_TYPE_EXERCISE = 0,
    QAPI_QSH_HEALTH_AGGREGATION_TYPE_SEGMENT = 1
} qapi_qsh_aggregation_type_t;

/*
    One aggregation can be active per aggregation type
*/
typedef struct
{
    qapi_qsh_aggregation_type_t type;
    uint32_t id;
} qapi_qsh_health_aggregation_layer_t;

/* @brief Simple metric value for stats (int/float only, no byte_arr)
 This avoids nested oneof callback issues during decoding */
typedef struct {
    union{
        int32_t int_val;
        float float_val;
    } value;
} qapi_qsh_health_stats_value_t;

/**
 * Represents a health metric value
 */
typedef struct
{
    enum
    {
        QAPI_QSH_VALUE_TYPE_INT,
        QAPI_QSH_VALUE_TYPE_FLOAT,
        QAPI_QSH_VALUE_TYPE_BYTES
    } value_type;

    union
    {
        uint32_t int_val;
        float float_val;
        qapi_qsh_byte_arr_t byte_arr;
    } value;
} qapi_qsh_health_metric_value_t;

/**
 * Represents a single health metric data entry
 */
typedef struct
{
    uint32_t per_event_metric_id; /* Applicable only for detect events.*/
    uint64_t ts_ticks;
    qapi_qsh_accuracy_t accuracy_status;      /* Accuracy enum */
    bool is_data_valid;
    qapi_qsh_health_metric_value_t data;
    bool has_aggregation;
    qapi_qsh_health_aggregation_layer_t aggregation;
} qapi_qsh_health_metric_data_t;

typedef struct
{
    uint32_t payload_count;
    qapi_qsh_health_metric_data_t *payloads;   /* Array of metric data */
} qapi_qsh_health_evt_data_t;

/**
 * Represents health stats data for a metric
 */
typedef struct
{
    qapi_qsh_health_metric_value_t stats[3];   /* min, max, avg */
    uint32_t stats_count;
    bool has_aggregation;
    qapi_qsh_health_aggregation_layer_t aggregation;
} qapi_qsh_health_stats_data_t;

/**
 *  Represents health auto event types
 */
typedef struct
{
    enum
    {
        QAPI_QSH_HEALTH_AUTO_EVENT_START,
        QAPI_QSH_HEALTH_AUTO_EVENT_PAUSE,
        QAPI_QSH_HEALTH_AUTO_EVENT_STOP
    }event_type;

    union
    {
        uint64_t ts_ticks;
        struct
        {
            enum
            {
                QAPI_QSH_HEALTH_START_UNKNOWN  = 0,
                QAPI_QSH_HEALTH_START_DETECTED,
                QAPI_QSH_HEALTH_START_CONFIRMED,
                QAPI_QSH_HEALTH_START_STOPPED
            }start_type;
            uint32_t exercise_id;
        } start;

        struct
        {
            enum {
                QAPI_QSH_HEALTH_PAUSE_UNKNOWN = 0,
                QAPI_QSH_HEALTH_PAUSE_DETECTED,
                QAPI_QSH_HEALTH_RESUME_DETECTED
            }type;
        } pause;
    } data;
} qapi_qsh_health_auto_evt_t;
/**
 * Represents health exercise event.
 */
typedef struct
{
    enum
    {
        QAPI_QSH_HEALTH_EXERCISE_EVENT_GOLF
    } event_type;

    union
    {
        struct{
            enum {
                QAPI_QSH_HEALTH_SWING_UNKNOWN = 0,
                QAPI_QSH_HEALTH_SWING_PUTT,
                QAPI_QSH_HEALTH_SWING_PARTIAL,
                QAPI_QSH_HEALTH_SWING_FULL
            }swing_type;
        }golf_shot_event;
    }data;
} qapi_qsh_health_exercise_evt_t;

/**
 * Represents health location event.
 */
typedef struct
{
    enum
    {
        QAPI_QSH_HEALTH_LOCATION_DATA,
        QAPI_QSH_HEALTH_LOCATION_STATUS
    } location_type;

    union
    {
        uint64_t ts_ticks;
        struct
        {
            double latitude;
            double longitude;
            float horizontalPositionErrorMeters;
            float bearing;
        } location;

        struct
        {
            enum
            {
                QAPI_QSH_HEALTH_LOC_STS_UNKNOWN = 0,
                QAPI_QSH_HEALTH_LOC_STS_UNAVAILABLE,
                QAPI_QSH_HEALTH_LOC_STS_NO_GNSS,
                QAPI_QSH_HEALTH_LOC_STS_ACQUIRING,
                QAPI_QSH_HEALTH_LOC_STS_ACQUIRED_UNTETHERED
            }status_type;
        } status;
    } data;
} qapi_qsh_health_location_evt_t;

/**
 * Structure to hold health event data
 */
typedef struct
{
    uint32_t metric_id;
    qapi_qsh_health_event_type_t data_type;
    union
    {
        qapi_qsh_health_location_evt_t location_data; /**< Location data */
        qapi_qsh_health_auto_evt_t auto_data; /**< Auto event data */
        qapi_qsh_health_evt_data_t detect_data; /**< Detect data */
        qapi_qsh_health_exercise_evt_t exercise_data; /**< Exercise event data */
        qapi_qsh_health_metric_data_t metric_data; /**< Metric data */
        qapi_qsh_health_stats_data_t stats_data; /**< Stats data */
    };
}qapi_qsh_health_evt_t;

/**
 * Formatter structures
 */

/**
 * Formatter data types.
 */
typedef enum
{
    QAPI_QSH_FORMATTER_METRIC_DATA = 0,
    QAPI_QSH_FORMATTER_GROUP_CONFIG,
}qapi_qsh_formatter_event_type_t;

typedef qapi_qsh_health_metric_value_t qapi_qsh_formatter_metric_value_t;

/**
 * Represents a single formatter metric data entry
 */
typedef struct
{
    bool is_accuracy_valid;
    qapi_qsh_accuracy_t accuracy_status;      /* Accuracy enum */
    bool is_data_valid;
    qapi_qsh_formatter_metric_value_t data;
    qapi_qsh_byte_arr_t formatted_output; /* custom metric info */
    bool has_aggregation;
    qapi_qsh_health_aggregation_layer_t aggregation;
} qapi_qsh_formatter_metric_data_t;

/**
 * Represents formatter group configuration.
 */
typedef struct
{
    qapi_qsh_byte_arr_t group_name;
    qapi_qsh_byte_arr_t group_config;
}qapi_qsh_formatter_group_config_t;

/**
 * Structure to hold formatter event data
 */
typedef struct
{
    uint32_t metric_id;
    qapi_qsh_formatter_event_type_t data_type;
    union
    {
        qapi_qsh_formatter_metric_data_t metric_data; /**< Metric data */
        qapi_qsh_formatter_group_config_t group_config; /**< group config data */
    };
}qapi_qsh_formatter_evt_t;

/**
 * Structure holding generic event data.
 */
typedef struct
{
    qapi_qsh_event_type_t type; /**< sensor event type */
    qapi_qsh_event_subtype_t sub_type;
    int client_id;              /**< unused */
    uint64_t evt_timestamp;
    union
    {
        qapi_qsh_als_data_t als_data;           /**< Data for ALS Event */
        uint32_t step_event;                    /**< step count */
        qapi_qsh_sns_status_t sns_status;       /**< Sensor availablity.*/
        qapi_qsh_std_sensor_error_t std_error;  /**< Error event code for the client.*/ 
        qapi_qsh_formatter_evt_t formatter_data;/**< Data for formatter sensor*/
        qapi_qsh_health_evt_t health_data;      /**< Data for health sensor*/
        qapi_qsh_std_sensor_evt_data_t data;    /**< output data from the source sensor */
    };
}qapi_qsh_client_evt_t;

typedef void (*qapi_qsh_bridge_sns_cb_t)(qapi_qsh_client_evt_t* evt);

/**
 * Register for a sensor event.
 * @note Config can be left NULL. Listerner sensors should be registerd first.
 */
int qapi_qsh_display_bridge_register_for_sensor(
    qapi_qsh_event_type_t type, qapi_qsh_evt_config_t* config,
    qapi_qsh_bridge_sns_cb_t cb);

/**
 * Unregister for a sensor event.
 */
int qapi_qsh_display_bridge_unregister_for_sensor(qapi_qsh_event_type_t type,
    qapi_qsh_bridge_sns_cb_t cb);

/**
 * Enable or disable metric for Health or Formatter sensor.
 * @param[in] type Sensor type be either health or formatter
 * @param[in] metric_id Metric id to be enabled or disabled.
 * @param[in] config Sensor configuration. Unused in current implementation
 * @param[in] enable Enable or disable the metric.
 */
int qapi_qsh_display_bridge_update_metrics(qapi_qsh_event_type_t type,
    qapi_qsh_metric_id_t metric_id, qapi_qsh_evt_config_t* config, bool enable);

#endif /**< __QSH_DISPLAY_BRIDGE_H__ */