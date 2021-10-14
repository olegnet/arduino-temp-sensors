
#ifndef PIO_TEMP_SENSOR_POSTDATA_H
#define PIO_TEMP_SENSOR_POSTDATA_H

#define NO_TEMPERATURE_DATA "--.--"

class PostData {
private:
    Display &display;

    WiFiClient client;

    const String host = SERVER_HOST;
    const int16_t port = SERVER_PORT;

    bool wifi_cycle_ui = true;

public:
    explicit PostData(Display &display) : display(display) {
        client = WiFiClient();
    }

    void process_temperature(Sensor &sensor) {
        float temp = sensor.get_temperature();
        display.prepare_to_print(sensor.offset_y);

        if (temp != DEVICE_DISCONNECTED_C) {
            Serial.printf("Temperature for the %s device is %.2f", sensor.device_name, temp);
            Serial.println();

            display.print(temp);

            // FIXME move to separate thread
            post_temperature(sensor.sensor_id, temp);
        } else {
            Serial.printf("Could not read temperature data from the %s device", sensor.device_name);
            Serial.println();

            display.print(NO_TEMPERATURE_DATA);
        }
    }

    void post_temperature(int8_t sensor_id, float temp) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.printf("WiFi is not connected, skipping for sensor_id %d", sensor_id);
            Serial.println();
            return;
        }

        if (client.connected() == 0) {
            client.connect(host, port);
        }

        client.printf("POST /sensor/%d/%.2f HTTP/1.1\r\n", sensor_id, temp);
        client.printf("Host: %s:%d\r\n", host.c_str(), port);
        client.printf("User-Agent: pio-temp-sensor/1.0\r\n");
        client.printf("Accept: */*\r\n");
        client.printf("\r\n");
        client.flush();
        String str = client.readString();

        Serial.printf("sensor_id=%d temp=%.2f", sensor_id, temp);
        Serial.println();
        Serial.println(str);
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
            display.clear();
            display.show_connecting_to(WIFI_SSID);
            display.paint_dot(offset_x);
            display.show();
            offset_x += 2;

            delay(100);
        }

        Serial.println();
        Serial.print("Connected! IP address: ");
        Serial.println(WiFi.localIP());
    }

    void show_network_status() {
        // TODO better indication and handling for disconnected state
        if (WiFi.status() == WL_CONNECTED) {
            display.paint_dot(wifi_cycle_ui ? 0 : 2);
        }
        wifi_cycle_ui = !wifi_cycle_ui;
    }
};

#endif //PIO_TEMP_SENSOR_POSTDATA_H
