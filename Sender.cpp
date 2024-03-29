#include "Sender.h"
#include <PubSubClient.h>

//DynamicJsonDocument doc(1024);


SenderClass::SenderClass() {
    //JsonVariant variant = doc.as<JsonVariant>();
    _jsonVariant = doc.to<JsonVariant>();
}
void SenderClass::add(String id, float value) {
    _jsonVariant[id] = value;
}
void SenderClass::add(String id, String value) {
    _jsonVariant[id] = value;
}
void SenderClass::add(String id, uint32_t value) {
    _jsonVariant[id] = value;
}
void SenderClass::add(String id, int32_t value) {
    _jsonVariant[id] = value;
}

bool SenderClass::sendMQTT(String server, uint16_t port, String username, String password, String name) {
    _mqttClient.setClient(_client);
    _mqttClient.setServer(server.c_str(), port);
    //_mqttClient.setCallback([this](char *topic, byte *payload, unsigned int length) { this->mqttCallback(topic, payload, length); });

//    byte i = 0;
    bool connected = false;
    
//    while (!_mqttClient.connected() && (i < 3)) {
    if (!_mqttClient.connected()) {
        DEBUG_PRINTLN(F("Sender: Attempting MQTT connection"));
        // Attempt to connect
        if (_mqttClient.connect(name.c_str(), username.c_str(), password.c_str())) {
            DEBUG_PRINTLN(F("Sender: Connected to MQTT"));
            connected = true;
        } else {
            DEBUG_PRINTLN(F("Sender: Failed MQTT connection, return code:"));

            int Status = _mqttClient.state();

            switch (Status) {
            case -4:
              DEBUG_PRINTLN(F("Sender: Connection timeout"));
              break;

            case -3:
              DEBUG_PRINTLN(F("Sender: Connection lost"));
              break;

            case -2:
              DEBUG_PRINTLN(F("Sender: Connect failed"));
              break;

            case -1:
              DEBUG_PRINTLN(F("Sender: Disconnected"));
              break;

            case 1:
              DEBUG_PRINTLN(F("Sender: Bad protocol"));
              break;

            case 2:
              DEBUG_PRINTLN(F("Sender: Bad client ID"));
              break;

            case 3:
              DEBUG_PRINTLN(F("Sender: Unavailable"));
              break;

            case 4:
              DEBUG_PRINTLN(F("Sender: Bad credentials"));
              break;

            case 5:
              DEBUG_PRINTLN(F("Sender: Unauthorized"));
              break;
            }
            //DEBUG_PRINTLN(F("Sender: Retrying MQTT connection in 0.5 seconds"));
            // Wait 5 seconds before retrying
            // i++;
            // delay(500);
        }
    }
    
    if (connected == false) return false;
    //MQTT publish values
    for (JsonPair kv : doc.as<JsonObject>()) {
      DEBUG_PRINTLN("Sender: MQTT publish: " + name + "/" + kv.key().c_str() + ": " + kv.value().as<String>().c_str());
      _mqttClient.publish((name + "/" + kv.key().c_str()).c_str(), kv.value().as<String>().c_str());
      //_mqttClient.loop(); //This should be called regularly to allow the client to process incoming messages and maintain its connection to the server.
    }
    
    DEBUG_PRINTLN(F("Sender: Closing MQTT connection"));
    _mqttClient.disconnect();
    delay(100); // allow gracefull session close
    return true;
}

// void SenderClass::mqttCallback(char *topic, byte *payload, unsigned int length) {
    // DEBUG_PRINTLN(F("MQTT message arrived ["));
    // DEBUG_PRINTLN(topic);
    // DEBUG_PRINTLN(F("] "));
    // for (unsigned int i = 0; i < length; i++) {
        // DEBUG_PRINTLN((char)payload[i]);
    // }
// }