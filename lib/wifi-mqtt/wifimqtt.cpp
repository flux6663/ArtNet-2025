#include "wifimqtt.h"
#include <Arduino.h>

String _mqttTopic;
String _mqttMessage;

String messageMQTT::voirTopic() {
    return _mqttTopic;
}

String messageMQTT::voirMessage() {
    return _mqttMessage;
}

WiFiClient connexionWiFi;
PubSubClient clientMQTT(connexionWiFi);


void initialiserWiFi(String ssid, String password)
{
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the Wi-Fi network");
}

// Reception du message MQTT
void callback(char *mqtt_topic, byte *payload, unsigned int length)
{

    String message_mqtt = "";

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        message_mqtt += (char)payload[i];
    }

    _mqttTopic = mqtt_topic;
    _mqttMessage = message_mqtt;
    
}

void initialiserMQTT(String mqtt_broker, uint16_t mqtt_port, String mqtt_username, String mqtt_password, String mqtt_topic)
{
    clientMQTT.setServer(mqtt_broker.c_str(), mqtt_port);
    clientMQTT.setCallback(callback);
    while (!clientMQTT.connected())
    {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT brokern", client_id.c_str());
        if (clientMQTT.connect(client_id.c_str(), mqtt_username.c_str(), mqtt_password.c_str()))
        {
            Serial.println("Public EMQX MQTT broker connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(clientMQTT.state());
            delay(2000);
        }
    }

    String topic = mqtt_topic + (String)"/";
    String macAddress = WiFi.macAddress();
    String lastTwoChars = macAddress.substring(macAddress.length() - 2); // Récupère les deux derniers caractères
    topic += lastTwoChars + (String)"/canauxDMX";
    clientMQTT.subscribe(topic.c_str());
    Serial.println(topic);
}

void envoyerDataMQTT(String mqtt_topic, String data) {
    clientMQTT.publish(mqtt_topic.c_str(), data.c_str());
}

void receptionDataMQTT() {
    clientMQTT.loop();
}