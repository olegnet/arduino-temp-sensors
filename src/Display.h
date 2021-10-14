
#ifndef PIO_TEMP_SENSOR_DISPLAY_H
#define PIO_TEMP_SENSOR_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// make CLion happy FIXME
#include "../.pio/libdeps/d1_mini/Adafruit GFX Library/Adafruit_GFX.h"
#include "../.pio/libdeps/d1_mini/Adafruit SSD1306 Wemos Mini OLED/Adafruit_SSD1306.h"

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h or use different Adafruit_SSD1306 library!");
#endif

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0


class Display {
private:
    Adafruit_SSD1306 display;

public:
    Display() : display(OLED_RESET) {}

    void begin() {
        // initialize with the I2C addr 0x3C (for the 64x48)
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

        clear();
        show();
    }

    void show() {
        display.display();
    }

    void clear() {
        display.clearDisplay();
    }

    void draw_rect() {
        display.drawRect(0, 0, SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, WHITE);
    }

    void paint_dot(int16_t offset_x) {
        display.drawPixel(offset_x, SSD1306_LCDHEIGHT - 1, WHITE);
        display.drawPixel(offset_x + 1, SSD1306_LCDHEIGHT - 1, WHITE);
        display.drawPixel(offset_x, SSD1306_LCDHEIGHT - 2, WHITE);
        display.drawPixel(offset_x + 1, SSD1306_LCDHEIGHT - 2, WHITE);
    }

    void show_connecting_to(const char *ssid_name) {
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("Connecting");
        display.print("to ");
        display.println(ssid_name);
    }

    void prepare_to_print(int16_t offset_y) {
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, offset_y);
    }

    void print(float data) {
        display.print(data);
    }

    void print(const char data[]) {
        display.print(data);
    }
};

#endif //PIO_TEMP_SENSOR_DISPLAY_H
