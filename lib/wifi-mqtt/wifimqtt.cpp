#include "wifimqtt.h"
#include <Arduino.h>

WiFiClient connexionWiFi;
PubSubClient clientMQTT(connexionWiFi);

void initialiserWiFi(char *ssid, char *password)
{
    WiFi.begin(ssid, password);
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
    Serial.print("Message arrived in topic: ");
    Serial.println(mqtt_topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void initialiserMQTT(char *mqtt_broker, uint16_t mqtt_port, char *mqtt_username, char *mqtt_password, char *mqtt_topic)
{
    clientMQTT.setServer(mqtt_broker, mqtt_port);
    clientMQTT.setCallback(callback);
    while (!clientMQTT.connected())
    {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT brokern", client_id.c_str());
        if (clientMQTT.connect(client_id.c_str(), mqtt_username, mqtt_password))
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

    // Publish et subscribe
    clientMQTT.publish(mqtt_topic, "Hi, I'm ESP32");
    clientMQTT.subscribe(mqtt_topic);
}