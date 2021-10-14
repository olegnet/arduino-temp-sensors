
#ifndef PIO_TEMP_SENSOR_SENSOR_H
#define PIO_TEMP_SENSOR_SENSOR_H

#include <cstdint>
#include <string>
#include <WString.h>

#include <OneWire.h>
#include <DallasTemperature.h>
// make CLion happy FIXME
#include "../.pio/libdeps/d1_mini/OneWire/OneWire.h"
#include "../.pio/libdeps/d1_mini/DallasTemperature/DallasTemperature.h"


class Sensor {
private:
    OneWire one_wire;
    DallasTemperature sensor;

public:
    int8_t sensor_id;
    const char *device_name;
    int16_t offset_y;

    Sensor(int8_t _sensor_id, int8_t _one_wire_bus, const char *_device_name, int16_t _offset_y)
            : one_wire(_one_wire_bus), sensor(&one_wire),
              sensor_id(_sensor_id), device_name(_device_name), offset_y(_offset_y) {}

    void begin() {
        sensor.begin();
    }

    void request_temperature() {
        sensor.requestTemperatures();
    }

    float get_temperature() {
        return sensor.getTempCByIndex(0);
    }
};

#endif //PIO_TEMP_SENSOR_SENSOR_H
