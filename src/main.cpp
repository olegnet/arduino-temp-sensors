#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "wifi-credentials.h"
// #define WIFI_SSID "xxx"
// #define WIFI_PASS "xxx"

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define NO_TEMPERATURE_DATA "--.--"

#define ONE_WIRE_BUS_FIRST_SENSOR   2
#define ONE_WIRE_BUS_SECOND_SENSOR  0

OneWire oneWire_first_sensor(ONE_WIRE_BUS_FIRST_SENSOR);
OneWire oneWire_second_sensor(ONE_WIRE_BUS_SECOND_SENSOR);

DallasTemperature first_sensor(&oneWire_first_sensor);
DallasTemperature second_sensor(&oneWire_second_sensor);

bool wifi_cycle_ui = true;

struct Sensor {
    const char *device_name;
    int16_t offset_y;
};

const Sensor first = {"first", 2};
const Sensor second = {"second", 24};

void display_dot(int16_t offset_x) {
    display.drawPixel(offset_x, SSD1306_LCDHEIGHT - 1, WHITE);
    display.drawPixel(offset_x + 1, SSD1306_LCDHEIGHT - 1, WHITE);
    display.drawPixel(offset_x, SSD1306_LCDHEIGHT - 2, WHITE);
    display.drawPixel(offset_x + 1, SSD1306_LCDHEIGHT - 2, WHITE);
}

void display_temperature(const Sensor sensor, float temp) {
    display.setCursor(0, sensor.offset_y);

    if (temp != DEVICE_DISCONNECTED_C) {
        Serial.print("Temperature for the ");
        Serial.print(sensor.device_name);
        Serial.print(" device is ");
        Serial.println(temp);

        display.print(temp);
    } else {
        Serial.print("Could not read temperature data from the ");
        Serial.print(sensor.device_name);
        Serial.println(" device");

        display.print(NO_TEMPERATURE_DATA);
    }
}

void connect_to_wifi() {
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    int16_t offset_x = 0;
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");

        if (offset_x > SSD1306_LCDWIDTH - 2) {
            offset_x = 0;
        }
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("Connecting");
        display.print("to ");
        display.println(WIFI_SSID);
        display_dot(offset_x);
        display.display();
        offset_x += 2;

        delay(100);
    }

    display.clearDisplay();
    display.display();

    Serial.println();
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);

    first_sensor.begin();
    second_sensor.begin();

    // initialize with the I2C addr 0x3C (for the 64x48)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();

    connect_to_wifi();
}

void loop() {
    first_sensor.requestTemperatures();
    second_sensor.requestTemperatures();
    delay(200);

    float first_sensor_temp = first_sensor.getTempCByIndex(0);
    float second_sensor_temp = second_sensor.getTempCByIndex(0);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);

    display_temperature(first, first_sensor_temp);
    display_temperature(second, second_sensor_temp);

    // TODO better indication for disconnected state
    if (WiFi.status() == WL_CONNECTED) {
        display_dot(wifi_cycle_ui ? 0 : 2);
    }
    wifi_cycle_ui = !wifi_cycle_ui;

    display.display();
}