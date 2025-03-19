#ifndef _WIFIMQTT_H__
#define __WIFIMQTT_H__

#include <Arduino.h>
#include "WiFi.h"
#include "PubSubClient.h"

#define WIFI_SSID "MaisonMartin"
#define WIFI_MDP "7ffPvN70WwC3K"
#define WIFI_CHANNEL 0

#define MQTT_BROKER ""
#define MQTT_PORT 1883
#define MQTT_USER NULL
#define MQTT_MDP NULL
#define MQTT_TOPIC "artnet"

void initialiserWiFi(char *ssid = WIFI_SSID, char *password = WIFI_MDP);
// // surcharge
// void initialiserWiFi(char *ssid = WIFI_SSID, char *password = WIFI_MDP, int32_t channel = WIFI_CHANNEL, const uint8_t *bssid = NULL, bool connect = true);

// void initialiserMQTT(char *broker = MQTT_BROKER, uint16_t port = MQTT_PORT, char *topicMQTT = MQTT_TOPIC);
// // surcharge
void initialiserMQTT(char *broker = MQTT_BROKER, uint16_t port = MQTT_PORT, char *userMQTT = MQTT_USER, char *mdpMQTT = MQTT_MDP, char *topicMQTT = MQTT_TOPIC);

#endif