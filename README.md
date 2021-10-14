
### Two temperature sensors with WiFi

  Client for [temperature sensors](https://github.com/olegnet/iot-data-server) project

#### Devices

  * [D1 mini board](https://aliexpress.com/item/32651747570.html)
  * [64x48 OLED screen](https://aliexpress.com/item/32800979738.html)
  * [DS18B20 temperature sensor](https://aliexpress.com/item/4000143479592.html)

#### Platformio libraries

```sh
pio lib install "adafruit/Adafruit GFX Library"
pio lib install "adafruit/Adafruit BusIO"
pio lib install "stblassitude/Adafruit SSD1306 Wemos Mini OLED"
pio lib install "paulstoffregen/OneWire"
pio lib install "milesburton/DallasTemperature"
```

#### wifi-credentials.h content

```c++
#define WIFI_SSID "access point name"
#define WIFI_PASS "access point password"
```

  host and port to upload data

```c++
#define SERVER_HOST "192.168.1.100" 
#define SERVER_PORT 12345
```

#### TODO

  * Better UI for WiFi status
  * Upload temperature data to the server
  * Battery monitoring
  * Soldering and boxing

#### Development version

  ![](hardware.jpg)
