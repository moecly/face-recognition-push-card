#ifndef __HTTPPOST_H__
#define __HTTPPOST_H__

#include "WiFiClient.h"
#include "ssd1306.h"
#include "ArduinoJson.h"

class HttpPost {

public:
  static void httpPostSend(String name, String url, String host, int httpPort, bool save, SSD1306::SSD1306_128_64_SW_I2C& ssd1306);

};

#endif