#include "httpPost.h"

String parseJson(String nameStr, String docStr);

void HttpPost::httpPostSend(String name, String url, String host, int httpPort, bool save, SSD1306::SSD1306_128_64_SW_I2C& ssd1306) {
  WiFiClient wifiClient;
  if (!wifiClient.connect(host, httpPort)) {
    return ;
  }

  String jsonStr;
  if (save == 1) {
    save = 0;
    // url = "/Servlet_Esp";
    jsonStr = String("") + "{\"request\" : " + "\"" + "admit" + "\"" "}";
  } else {
    // url = "/Servlet_Esp";
    jsonStr = String("") + "{\"name\": " + "\"" + name + "\"" "}";
  }
  String getStr = String("POST ") + url + " HTTP/1.1\r\n" + 
    "Host: " + host + "\r\n" + 
    "User-Agent: mjjshizlfbaba\r\n" + 
    "Content-Type: application/json" +
    "Accept-Charset: UTF-8, *" +
    "Connection: close\r\n";
  wifiClient.print(getStr);
  wifiClient.print(String("") + "Content-Length: " + jsonStr.length() + "\r\n\r\n");
  wifiClient.print(jsonStr);
  String line = wifiClient.readStringUntil('\n');
  String line2 = wifiClient.readString();
  String line3 = line2.substring(line2.indexOf('{'), line2.indexOf('}') + 1);
  String httpCode = line.substring(9, line.length()-2);
  if (httpCode.compareTo("200") == 0) {
    ssd1306.setFont(u8g2_font_courB10_tf);
    ssd1306.displayStrClear(10, 30, parseJson(line3, "name").c_str());
    ssd1306.displayStrNoClear(10, 50, "clocked !");
    Serial.println("clocked!");
    delay(1000);
  } else if (httpCode.compareTo("302") == 0) {
    Serial.print("^R");
  } else if (httpCode.compareTo("303") == 0) {

    ssd1306.setFont(u8g2_font_courB10_tf);
    ssd1306.displayStrClear(10, 30, parseJson(line3, "name").c_str());
    ssd1306.displayStrNoClear(10, 50, "admited !");
    Serial.println("admit!");
    delay(1000);
  } else {
    Serial.println("else!");
  }

}

String parseJson(String nameStr, String docStr) {

  const size_t capacity = JSON_OBJECT_SIZE(2) + 40;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, nameStr);
  String name = doc[docStr].as<String>();
  // Serial.print("parseJson: name: ");
  // Serial.print(name);
  return name;

}