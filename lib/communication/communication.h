#ifndef _COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define WIFI_MDP ""
#define WIFI_CHANNEL 0

#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_MDP ""

#define MQTT_TOPIC_GENERALE "artnet/"
#define MQTT_TOPIC_CONFIG_MODULE  MQTT_TOPIC_GENERALE + "config"
#define MQTT_TOPIC_RECEPTION_CANAUX  MQTT_TOPIC_GENERALE + "univers"

#define RESET_FLAG false
#define NEW_FLAG true

class Communication
{

public:
    void envoieConfiguration(int univers);
    void envoyerMessage(String mqtt_topic, String data);
    void initialiserWiFi(String nomModuleWifi, String ssid, String password = WIFI_MDP);
    void initialiserMQTT(String mqttBroker, uint16_t mqttPort = MQTT_PORT, String mqttUsername = MQTT_USER, String mqttPassword = MQTT_MDP);
    void initialiserUnivers(int univers);
    void receptionDataMQTT();

    String getTopic();
    void setTopic(String mqttTopic);
    String getMessage();
    void setMessage(String mqttMessage);
    bool getFlag();
    void setFlag(bool mqttFlag);
    int getUnivers();

private:
    void sinscrireAuxTopic();

    int _univers;
    String _nomModuleWifi;

};

void nouveauMessageMQTT(char *mqttTopic, byte *payload, unsigned int nombreCarathere);

#endif