#include "include/mqtt.h"
#include <Arduino.h>

WiFiClient connexionWiFi;
PubSubClient clientMQTT(connexionWiFi);
communication reception;

String communication::getTopic()
{
    return _mqttTopic;
}

void communication::setTopic(String mqttTopic)
{
    _mqttTopic = mqttTopic;
}

String communication::getMessage()
{
    return _mqttMessage;
}

void communication::setMessage(String mqttMessage)
{
    _mqttMessage = mqttMessage;
}

bool communication::getFlag()
{
    return _mqttFlag;
}

void communication::setFlag(bool mqttFlag)
{
    _mqttFlag = mqttFlag;
}

void MQTT::envoieConfiguration(int univers) {

    String adressMac = WiFi.macAddress();
    String dernierNumeroAdressMac = adressMac.substring(adressMac.length() - 4);
    String nomModuleWifi = (String)"Module-" + (String)dernierNumeroAdressMac;

    String topicEnvoieConfig = (String)MQTT_TOPIC_CONFIG_MODULE + (String)nomModuleWifi;

    envoyerDataMQTT(topicEnvoieConfig, (String)univers);

}

void MQTT::receptionDataMQTT()
{
    // TODO faire la gestion de la reconnexion
    // if (!clientMQTT.connected()) {
    //     initialiserMQTT();
    // }
    clientMQTT.loop();
}

void initialiserWiFi(String ssid, String password)
{
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connexion au Wi-Fi..");
    }
    Serial.println("module connecter au Wi-Fi");
}

void nouveauMessageMQTT(char *mqttTopic, byte *payload, unsigned int nombreCarathere)
{
    String messageMqtt;

    for (int i = 0; i < nombreCarathere; i++)
    {
        messageMqtt += (char)payload[i]; 
    }

    Serial.println(messageMqtt);

    reception.setMessage(mqttTopic);
    reception.setMessage(messageMqtt);
    reception.setFlag(NEW_FLAG);
}

void initialiserMQTT(String mqttBroker, uint16_t mqttPort, String mqttUsername, String mqttPassword)
{

    clientMQTT.setServer(mqttBroker.c_str(), mqttPort);
    clientMQTT.setCallback(nouveauMessageMQTT);

    while (!clientMQTT.connected())
    {
        String nomClient = "Module-WiFi-DMX_";
        nomClient += String(WiFi.macAddress());

        Serial.println("Connexion au broker..");

        if (clientMQTT.connect(nomClient.c_str(), mqttUsername.c_str(), mqttPassword.c_str())) {
            Serial.printf("Le client : %s est connecter au broker\r\n", nomClient.c_str());
        } else {
            Serial.print("ERREUR : ");
            Serial.println(clientMQTT.state());
            delay(2000);
        }
    }

    sinscrireAuxTopic();
}

void sinscrireAuxTopic() {

    String topicLectureCanaux = (String)MQTT_TOPIC_ENVOIE_CANAUX + (String)"/" + (String)UNIVERS;

    clientMQTT.subscribe(topicLectureCanaux.c_str());

    Serial.println("Abonner au topic : ");
    Serial.println(topicLectureCanaux);

}

void envoyerDataMQTT(String mqtt_topic, String data)
{
    clientMQTT.publish(mqtt_topic.c_str(), data.c_str());
}