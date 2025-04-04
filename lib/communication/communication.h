#ifndef _COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define WIFI_SSID "MaisonMartin"
#define WIFI_MDP "7ffPvN70WwC3K"
#define WIFI_CHANNEL 0

#define MQTT_IP_BROKER "10.0.0.10"
#define MQTT_PORT 1883
#define MQTT_USER "mqtt_home"
#define MQTT_MDP "QTN6svRX%CLwgkH"

#define UNIVERS "50"

#define MQTT_TOPIC_GENERALE "artnet/"
#define MQTT_TOPIC_CONFIG_MODULE  MQTT_TOPIC_GENERALE + "config"
#define MQTT_TOPIC_RECEPTION_CANAUX  MQTT_TOPIC_GENERALE + "univers" + "/" + UNIVERS

#define RESET_FLAG false
#define NEW_FLAG true

class Communication
{

public:
    void envoieConfiguration(int univers);
    void envoyerMessage(String mqtt_topic, String data);
    void initialiserWiFi(String ssid = WIFI_SSID, String password = WIFI_MDP);
    void initialiserMQTT(String mqttBroker = MQTT_IP_BROKER, uint16_t mqttPort = MQTT_PORT, String mqttUsername = MQTT_USER, String mqttPassword = MQTT_MDP);
    void receptionDataMQTT();

    String getTopic();
    void setTopic(String mqttTopic);
    String getMessage();
    void setMessage(String mqttMessage);
    bool getFlag();
    void setFlag(bool mqttFlag);

private:
    void sinscrireAuxTopic();

};

void nouveauMessageMQTT(char *mqttTopic, byte *payload, unsigned int nombreCarathere);

#endif