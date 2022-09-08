#include "ESP8266WiFi.h"
#include "DNSServer.h"
#include "ESP8266WebServer.h"
#include "WiFiManager.h"
#include "util/ssd1306.h"
#include "Ticker.h"

#include "NTPClient.h"
#include "WiFiUdp.h"
#include "util/httpPost.h"

SSD1306::SSD1306_128_64_SW_I2C ssd1306;
WiFiManager wiFiManager;
Ticker ticker1;
Ticker ticker2;
WiFiUDP wifiUdp;
NTPClient ntpClient(wifiUdp, "ntp1.aliyun.com", 60*60*8, 30*30*1000);

void displayCountdown();
void displayTime();

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  ssd1306.setFont(u8g2_font_ncenB08_tr);
  ssd1306.displayHey();
  ticker1.attach(1, displayCountdown);
  wiFiManager.autoConnect("moecly", "12345678"); 
  ticker1.detach();
  ssd1306.displayConnectedSSID();
  ntpClient.begin();
  ticker1.attach(1, displayTime);
}

void loop() {
  // put your main code here, to run repeatedly:
  const static String url = "/Servlet_Esp";
  const char* host = "119.23.179.64";
  const int httpPort = 80;
  ntpClient.update();
  if (Serial.available()) {
    String name = Serial.readString();
    // Serial.println(name);
    if (WiFi.status() == WL_CONNECTED) {
      ticker1.detach();
      if (name.compareTo("^C") == 0) {
        HttpPost::httpPostSend(name, url, host, httpPort, true, ssd1306);
      } else {
        HttpPost::httpPostSend(name, url, host, httpPort, false, ssd1306);
      }
      ticker1.attach(1, displayTime);
    }
  }
}

void displayCountdown() {

  static int count = 9;
  ssd1306.displayCountdown(count --);
  if (count == 0) {
    ticker1.detach();
  }
}

void displayTime() {
  if (WiFi.status() != WL_CONNECTED) {
    // Serial.end();
    ticker2.detach();
    ssd1306.displayESPWiFiInfo();
    wiFiManager.autoConnect("moecly", "12345678"); 
    // Serial.begin(115200);
  } else {
    ssd1306.displayTime(ntpClient.getFormattedTime().substring(0, 5));
  }
}