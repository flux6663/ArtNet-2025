#include "communication.h"
#include <Arduino.h>

String _mqttTopic;
String _mqttMessage;
bool _mqttFlag = RESET_FLAG;

WiFiClient connexionWiFi;
PubSubClient clientMQTT(connexionWiFi);

String Communication::getTopic()
{
    return _mqttTopic;
}

void Communication::setTopic(String mqttTopic)
{
    _mqttTopic = mqttTopic;
}

String Communication::getMessage()
{
    return _mqttMessage;
}

void Communication::setMessage(String mqttMessage)
{
    _mqttMessage = mqttMessage;
}

bool Communication::getFlag()
{
    return _mqttFlag;
}

void Communication::setFlag(bool mqttFlag)
{
    _mqttFlag = mqttFlag;
}

int Communication::getUnivers()
{
    return _univers;
}

void Communication::envoyerMessage(String mqtt_topic, String data)
{
    clientMQTT.publish(mqtt_topic.c_str(), data.c_str());
}

void Communication::envoieConfiguration(int univers) {

    String adressMac = WiFi.macAddress();
    String dernierNumeroAdressMac = adressMac.substring(adressMac.length() - 4);
    String nomModuleWifi = (String)"Module-" + (String)dernierNumeroAdressMac;

    String topicEnvoieConfig = (String)MQTT_TOPIC_CONFIG_MODULE + (String)nomModuleWifi;

    this->envoyerMessage(topicEnvoieConfig, (String)univers);

}

void Communication::receptionDataMQTT()
{
    // TODO faire la gestion de la reconnexion
    // if (!clientMQTT.connected()) {
    //     initialiserMQTT();
    // }
    clientMQTT.loop();
}

void Communication::initialiserWiFi(String nomModuleWifi, String ssid, String password)
{

    _nomModuleWifi = nomModuleWifi;

    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connexion au Wi-Fi..");
    }
    Serial.println("module connecter au Wi-Fi");
}

void Communication::initialiserMQTT(String mqttBroker, uint16_t mqttPort, String mqttUsername, String mqttPassword)
{

    clientMQTT.setServer(mqttBroker.c_str(), mqttPort);
    clientMQTT.setCallback(nouveauMessageMQTT);

    while (!clientMQTT.connected())
    {

        Serial.println("Connexion au broker..");

        if (clientMQTT.connect(_nomModuleWifi.c_str(), mqttUsername.c_str(), mqttPassword.c_str())) {
            Serial.printf("Le client : %s est connecter au broker\r\n", _nomModuleWifi.c_str());
        } else {
            Serial.print("ERREUR : ");
            Serial.println(clientMQTT.state());
            delay(2000);
        }
    }

    this->sinscrireAuxTopic();
}

void Communication::sinscrireAuxTopic() {

    String topicLectureCanaux = (String)MQTT_TOPIC_RECEPTION_CANAUX + "/" + (String)_univers;

    clientMQTT.subscribe(topicLectureCanaux.c_str());

    Serial.print("Abonner au topic : ");
    Serial.println(topicLectureCanaux);

}

void Communication::initialiserUnivers(int univers)
{
    _univers = univers;
}

void nouveauMessageMQTT(char *mqttTopic, byte *mqttPayload, unsigned int nombreCarathere)
{
    String message = "";
    static String old_message;

    for (int i = 0; i < nombreCarathere; i++)
    {
        message += (char)mqttPayload[i]; 
    }

    if (old_message != message)
    {
        Serial.println(message);

        _mqttFlag = NEW_FLAG;
        _mqttTopic = mqttTopic;
        _mqttMessage = message;

        old_message = message;
    }
    
}