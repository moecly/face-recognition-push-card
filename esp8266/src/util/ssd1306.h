#ifndef __SSD1306_H_
#define __SSD1306_H_

#include "ESP8266WiFi.h"
#include "NTPClient.h"
#include "WiFiUdp.h"
#include "U8g2lib.h"

namespace SSD1306 {

class SSD1306_128_64_SW_I2C {

  public :
    SSD1306_128_64_SW_I2C();
    ~SSD1306_128_64_SW_I2C();
    SSD1306_128_64_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset);

    void setFont(const uint8_t *font);
    void displayStrClear(u8g2_uint_t x, u8g2_uint_t y, const char *s);
    void displayStrNoClear(u8g2_uint_t x, u8g2_uint_t y, const char *s);
    void displayCountdown(int count);
    void displayESPWiFiInfo();
    void displayConnectedSSID();
    void displayTime(String time);
    void displayHey();

  private :
    U8G2_SSD1306_128X64_NONAME_F_SW_I2C *u8g2;
    String time;
    // WiFiUDP wifiUdp;
    // NTPClient *ntpClient;

};

}

#endif