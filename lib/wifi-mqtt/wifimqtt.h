#ifndef _WIFIMQTT_H__
#define __WIFIMQTT_H__

#include <Arduino.h>
#include "WiFi.h"
#include "PubSubClient.h"

#define WIFI_SSID "MaisonMartin"
#define WIFI_MDP "7ffPvN70WwC3K"
#define WIFI_CHANNEL 0

#define MQTT_IP_BROKER "10.0.0.10"
#define MQTT_PORT 1883
#define MQTT_USER "mqtt_home"
#define MQTT_MDP "QTN6svRX%CLwgkH"
#define MQTT_TOPIC "artnet"

class messageMQTT {
    public:
        String voirTopic();
        String voirMessage();
};

void initialiserWiFi(String ssid = WIFI_SSID, String password = WIFI_MDP);
// // surcharge
// void initialiserWiFi(char *ssid = WIFI_SSID, char *password = WIFI_MDP, int32_t channel = WIFI_CHANNEL, const uint8_t *bssid = NULL, bool connect = true);

// void initialiserMQTT(char *broker = MQTT_BROKER, uint16_t port = MQTT_PORT, char *topicMQTT = MQTT_TOPIC);
// // surcharge
void initialiserMQTT(String mqtt_broker = MQTT_IP_BROKER, uint16_t mqtt_port = MQTT_PORT, String mqtt_username = MQTT_USER, String mqtt_password = MQTT_MDP, String mqtt_topic = MQTT_TOPIC);

void envoyerDataMQTT(String mqtt_topic, String data);
void receptionDataMQTT();

#endif