/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#include "sensor_factory.h"
#include "qsh_sensor.h"
#include "sns_std_sensor.pb.h"

static const char *QSH_DATATYPE_AMBIENT_TEMP = "ambient_temperature";

class ambient_temperature : public qsh_sensor
{
public:
    ambient_temperature(suid sensor_uid, sensor_wakeup_type wakeup);
    static const char* qsh_datatype() { return QSH_DATATYPE_AMBIENT_TEMP; }
};

ambient_temperature::ambient_temperature(suid sensor_uid, sensor_wakeup_type wakeup):
    qsh_sensor(sensor_uid, wakeup)
{
    set_type(SENSOR_TYPE_AMBIENT_TEMPERATURE);
    set_string_type(SENSOR_STRING_TYPE_AMBIENT_TEMPERATURE);
    set_sensor_typename("Ambient Temperature Sensor");
    set_nowk_msgid(SNS_STD_SENSOR_MSGID_SNS_STD_SENSOR_PHYSICAL_CONFIG_EVENT);
    _qsh_intf_id = qsh_intf_resource_manager::get_instance().register_sensor(get_sensor_info());
}

static bool ambient_temperature_module_init()
{
    /* register supported sensor types with factory */
    sensor_factory::register_sensor(
        SENSOR_TYPE_AMBIENT_TEMPERATURE,
        qsh_sensor::get_available_sensors<ambient_temperature>);
    sensor_factory::request_datatype(QSH_DATATYPE_AMBIENT_TEMP);
    return true;
}

SENSOR_MODULE_INIT(ambient_temperature_module_init);
