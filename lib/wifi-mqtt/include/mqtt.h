#ifndef _WIFIMQTT_H__
#define __WIFIMQTT_H__

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

#define MQTT_TOPIC_GENERALE "artnet/"
#define MQTT_TOPIC_CONFIG_MODULE  (String)MQTT_TOPIC_GENERALE + (String)"config"
#define MQTT_TOPIC_ENVOIE_CANAUX  (String)MQTT_TOPIC_GENERALE + (String)"univers"

#define RESET_FLAG false
#define NEW_FLAG true

#define UNIVERS 50

class communication
{

public:
    String getTopic();
    void setTopic(String mqttTopic);
    String getMessage();
    void setMessage(String mqttMessage);
    bool getFlag();
    void setFlag(bool mqttFlag);

private:
    String _mqttTopic;
    String _mqttMessage;
    bool _mqttFlag = false;

};

class MQTT : public communication
{ 
public:
    void envoieConfiguration(int univers);
    void receptionDataMQTT();
};

void initialiserWiFi(String ssid = WIFI_SSID, String password = WIFI_MDP);
void initialiserMQTT(String mqtt_broker = MQTT_IP_BROKER, uint16_t mqtt_port = MQTT_PORT, String mqtt_username = MQTT_USER, String mqtt_password = MQTT_MDP);
void sinscrireAuxTopic();

#endif