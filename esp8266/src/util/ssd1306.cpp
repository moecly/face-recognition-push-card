#include "ssd1306.h"

namespace SSD1306 {

SSD1306_128_64_SW_I2C::SSD1306_128_64_SW_I2C() {
  this->u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, 14, 2, U8X8_PIN_NONE);
  this->u8g2->begin();
  this->u8g2->clearBuffer();
  this->time = "";
  // this->ntpClient = new NTPClient(wifiUdp, "ntp1.aliyun.com", 60*60*8, 30*30*1000);
  // this->ntpClient->begin();
}

SSD1306_128_64_SW_I2C::SSD1306_128_64_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset) {
  this->u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(rotation, clock, data, reset);
  this->u8g2->begin();
  this->u8g2->clearBuffer();
  this->time = "";
  // this->ntpClient = new NTPClient(wifiUdp, "ntp1.aliyun.com", 60*60*8, 30*30*1000);
  // this->ntpClient->begin();
}

void SSD1306_128_64_SW_I2C::setFont(const uint8_t *font) {
  this->u8g2->setFont(font);
}

void SSD1306_128_64_SW_I2C::displayStrClear(u8g2_uint_t x, u8g2_uint_t y, const char *s) {
  this->u8g2->clear();
  this->u8g2->drawStr(x, y, s);
  this->u8g2->sendBuffer();
}

void SSD1306_128_64_SW_I2C::displayHey() {
  this->u8g2->clear();
  this->u8g2->drawStr(10, 25, "Hey, guy !");
  this->u8g2->drawStr(10, 45, "Let's to use !");
  this->u8g2->sendBuffer();
  delay(1000);
}

void SSD1306_128_64_SW_I2C::displayConnectedSSID() {
  this->u8g2->clear();
  this->u8g2->drawStr(10, 25, "Connected to");
  this->u8g2->drawStr(10, 45, WiFi.SSID().c_str());
  this->u8g2->sendBuffer();
  delay(1000);
}

void SSD1306_128_64_SW_I2C::displayStrNoClear(u8g2_uint_t x, u8g2_uint_t y, const char *s) {
  this->u8g2->drawStr(x, y, s);
  this->u8g2->sendBuffer();
}

SSD1306_128_64_SW_I2C::~SSD1306_128_64_SW_I2C() {
  delete this->u8g2;
  // delete this->ntpClient;
}

void SSD1306_128_64_SW_I2C::displayCountdown(int count) {

  switch (count) {

    case 9:
      this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Try to connect ... 9");
      this->u8g2->sendBuffer();
      break;

    case 8:
      // this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Try to connect ... 8");
      this->u8g2->sendBuffer();
      break;

    case 7:
      // this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Try to connect ... 7");
      this->u8g2->sendBuffer();
      break;

    case 6:
      // this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Try to connect ... 6");
      this->u8g2->sendBuffer();
      break;

    case 5:
      // this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Try to connect ... 5");
      this->u8g2->sendBuffer();
      break;

    case 4:
      // this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Try to connect ... 4");
      this->u8g2->sendBuffer();
      break;

    case 3:
      // this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Try to connect ... 3");
      this->u8g2->sendBuffer();
      break;

    case 2:
      // this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Try to connect ... 2");
      this->u8g2->sendBuffer();
      break;

    case 1:
      // this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Try to connect ... 1");
      this->u8g2->sendBuffer();
      break;

    case 0:
      this->u8g2->clear();
      this->u8g2->drawStr(10, 34, "Connect failed");
      this->u8g2->sendBuffer();
      delay(1000);
      this->u8g2->clear();
      this->u8g2->drawStr(10, 25, "SSID: moecly");
      this->u8g2->drawStr(10, 45, "PASS: 12345678");
      this->u8g2->sendBuffer();
      break;

  }

}

void SSD1306_128_64_SW_I2C::displayESPWiFiInfo() {
  this->u8g2->setFont(u8g2_font_ncenB08_tr);
  this->u8g2->clear();
  this->u8g2->drawStr(10, 25, "SSID: moecly");
  this->u8g2->drawStr(10, 45, "PASS: 12345678");
  this->u8g2->sendBuffer();
}

void SSD1306_128_64_SW_I2C::displayTime(String time) {

  /*  Display from ssd1306  */
  if (this->time == "") {
    this->time = time; 
    this->u8g2->setFont(u8g2_font_logisoso42_tf);
    // this->ntpClient->update();
    this->u8g2->clear();
    this->u8g2->firstPage();
    do {
      // this->u8g2->drawStr(0, 57, ntpClient->getFormattedTime().substring(0, 5).c_str());
      this->u8g2->drawStr(0, 57, time.c_str());
    } while (this->u8g2->nextPage());
  }
  else if (this->time.compareTo(time) != 0) {
    this->u8g2->setFont(u8g2_font_logisoso42_tf);
    // this->ntpClient->update();
    this->u8g2->clear();
    this->u8g2->firstPage();
    do {
      // this->u8g2->drawStr(0, 57, ntpClient->getFormattedTime().substring(0, 5).c_str());
      this->u8g2->drawStr(0, 57, time.c_str());
    } while (this->u8g2->nextPage());
    this->time = time;
  }

}


}