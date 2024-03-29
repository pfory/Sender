/**************************************************************

    "iSpindel"
    changes by S.Lang <universam@web.de>

 **************************************************************/

#ifndef _SENDER_H_
#define _SENDER_H_

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include <PubSubClient.h>

#define verbose
#ifdef verbose
 #define DEBUG_PRINT(x)         Serial.print (x)
 #define DEBUG_PRINTLN(x)       Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTLN(x)
#endif 

class SenderClass
{
public:
  SenderClass();
  bool sendMQTT(String server, uint16_t port, String username, String password, String name);
  void add(String id, float value);
  void add(String id, String value);
  void add(String id, int32_t value);
  void add(String id, uint32_t value);
  //void mqttCallback(char* topic, byte* payload, unsigned int length);
  // ~SenderClass();

private:
  WiFiClient _client;
  PubSubClient _mqttClient;

  // StaticJsonBuffer<200> _jsonBuffer;
  //DynamicJsonBuffer _jsonBuffer;
  // JsonObject data;
  //DynamicJsonDocument doc(1024);
  StaticJsonDocument<1024> doc;
  JsonVariant _jsonVariant;
};

#endif
