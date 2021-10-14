#include <vector>

#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#include "wifi-credentials.h"
// #define WIFI_SSID "xxx"
// #define WIFI_PASS "xxx"
// #define SERVER_HOST "192.168.1.100"
// #define SERVER_PORT 12345

#include "Sensor.h"
#include "Display.h"
#include "PostData.h"

#define ONE_WIRE_BUS_FIRST_SENSOR   2
#define ONE_WIRE_BUS_SECOND_SENSOR  0

Sensor first_sensor(1, ONE_WIRE_BUS_FIRST_SENSOR, "first", 2);
Sensor second_sensor(2, ONE_WIRE_BUS_SECOND_SENSOR, "second", 24);
std::vector<Sensor> sensors{first_sensor, second_sensor};

Display display;
PostData post_data(display);

void setup() {
    Serial.begin(115200);

    for_each(sensors.begin(), sensors.end(), [](Sensor &sensor) {
        sensor.begin();
    });

    display.begin();

    post_data.connect_to_wifi();

    display.clear();
//    display.draw_rect();
    display.show();
}

void loop() {
    for_each(sensors.begin(), sensors.end(), [](Sensor &sensor) {
        sensor.request_temperature();
    });
    delay(200);

    display.clear();

    for_each(sensors.begin(), sensors.end(), [](Sensor &sensor) {
        post_data.process_temperature(sensor);
    });

    post_data.show_network_status();

    display.show();
}
