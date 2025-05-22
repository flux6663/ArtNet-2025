#include "communication.h"
#include <Arduino.h>

String _mqttTopic;
String _mqttMessage;
String _nomModuleWifi;
int _univers;

String _wifiSsid;
String _wifiMdp;

volatile bool _etatConnexionWifi = DECONNECTER;
volatile bool _etatConnexionMqtt = DECONNECTER;
volatile uint8_t _compteurMqtt = 0;
volatile uint8_t _compteurWifi = 0;
volatile bool _mqttFlagNouveauMessage = RESET_FLAG;
volatile bool _mqttFlagTimerConfig = RESET_FLAG;
hw_timer_t * timerConfig = NULL;

static const char *DEBUG = "DEBUG : ";
esp_mqtt_client_handle_t mqtt_client = nullptr;

WiFiClient connexionWiFi;
JsonDocument jsonConfig;

void interruptionWifiConnecter(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
    Serial.println(F("Wifi : Module connecter au reseau"));
}

void interruptionWifiDeconnecter(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
    if (_compteurWifi >= LIMITE_COMPTEUR) ESP.restart();
    Serial.println(F("Wifi : Module non connecter, connexion en cours..."));
    WiFi.begin(_wifiSsid, _wifiMdp);
    _etatConnexionWifi = DECONNECTER;
    _compteurWifi++;
}

void interruptionWifiNouvelleAdressIp(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
    Serial.print(F("Wifi : Nouvelle adress ip : "));
    Serial.println(WiFi.localIP());
    _etatConnexionWifi = CONNECTER;
    _compteurWifi = 0;
}

void interuptionNouveauMessageMQTT(char *mqttTopic, byte *mqttPayload, unsigned int nombreCarathere)
{
    // Pré-alloue la taille du message pour éviter les reallocations
    String message((char*)mqttPayload, nombreCarathere);

    Serial.println(message);

    _mqttFlagNouveauMessage = NEW_FLAG;
    _mqttTopic = mqttTopic;
    _mqttMessage = message;

}

void onMQTTConnected(esp_mqtt_event_handle_t event) {
  Serial.println("MQTT CONNECTÉ !");
  // Ex : abonnement automatique
  esp_mqtt_client_subscribe(event->client, "maison/salon/temp", 0);
}

void onMQTTDisconnected(esp_mqtt_event_handle_t event) {
  Serial.println("MQTT DÉCONNECTÉ !");
  // Tu peux aussi gérer une reconnexion manuelle si besoin
}

void onMQTTDataReceived(esp_mqtt_event_handle_t event) {
  String topic = String(event->topic, event->topic_len);
  String data = String(event->data, event->data_len);

  Serial.printf("Reçu sur %s : %s\n", topic.c_str(), data.c_str());

  // Exemple : traitement en fonction du topic
  if (topic == "maison/salon/temp") {
    Serial.println("Traitement de la température reçue...");
  }
}

void onMQTTPublished(esp_mqtt_event_handle_t event) {
  Serial.printf("Message publié (msg_id=%d)\n", event->msg_id);
}

void onMQTTError(esp_mqtt_event_handle_t event) {
  Serial.println("Erreur MQTT !");
}

esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {
  switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
      onMQTTConnected(event);
      break;

    case MQTT_EVENT_DISCONNECTED:
      onMQTTDisconnected(event);
      break;

    case MQTT_EVENT_DATA:
      onMQTTDataReceived(event);
      break;

    case MQTT_EVENT_PUBLISHED:
      onMQTTPublished(event);
      break;

    case MQTT_EVENT_ERROR:
      onMQTTError(event);
      break;

    default:
      Serial.printf("🔔 Autre événement MQTT : %d\n", event->event_id);
      break;
  }
  return ESP_OK;
}


void IRAM_ATTR interuptionEnvoieConfiguration() {
    _mqttFlagTimerConfig = NEW_FLAG;
}

void envoyerMessage(String mqtt_topic, String data)
{
    if (mqtt_client) {
        int msg_id = esp_mqtt_client_publish(mqtt_client, mqtt_topic.c_str(), data.c_str(), 0, 1, 0);
        ESP_LOGI(TAG, "Message publié: %s sur %s, msg_id=%d", message, topic, msg_id);
    }
}

void envoieConfiguration(int univers, String adressIp, String adressMac, float puissanceWifi) {

    String topicEnvoieConfig = (String)MQTT_TOPIC_CONFIG_MODULE + "/" + _nomModuleWifi;
    String configModule;

    jsonConfig["univers"] = univers;
    jsonConfig["ip"] = adressIp;
    jsonConfig["mac"] = adressMac;
    jsonConfig["rssi"] = puissanceWifi;
    serializeJson(jsonConfig, configModule);

    envoyerMessage(topicEnvoieConfig, configModule);
}

void Communication::initialiserWiFi(String nomModuleWifi, String ssid, String password)
{

    _nomModuleWifi = nomModuleWifi;
    _wifiSsid = ssid;
    _wifiMdp = password;

    WiFi.mode(WIFI_MODE_STA);

    WiFi.onEvent(interruptionWifiConnecter, ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(interruptionWifiNouvelleAdressIp, ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(interruptionWifiDeconnecter, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    WiFi.begin(ssid.c_str(), password.c_str());

}

void Communication::initialiserUnivers(int univers)
{
    _univers = univers;
}

void Communication::initialiserTimer() {

    timerConfig = timerBegin(NUMERO_TIMER, FREQUENCE_TIMER, true);
    timerAttachInterrupt(timerConfig, &interuptionEnvoieConfiguration, true);
    timerAlarmWrite(timerConfig, TEMPS_ATTENTE, true);
    timerAlarmEnable(timerConfig);

}

void Communication::initialiserMQTT(String mqttBroker, uint16_t mqttPort, String mqttUsername, String mqttPassword)
{
    String ipBroker = "mqtt://" + mqttBroker + "/";

    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = ipBroker.c_str(),
        .event_handle = mqtt_event_handler_cb
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(mqtt_client);

}

void Communication::sinscrireAuxTopic() {

    String topicLectureCanaux = (String)MQTT_TOPIC_RECEPTION_CANAUX + "/" + (String)_univers;

    if (mqtt_client) {
        int msg_id = esp_mqtt_client_subscribe(mqtt_client, topicLectureCanaux.c_str(), 0);
        ESP_LOGI(TAG, "Abonnement au topic: %s, msg_id=%d", topicLectureCanaux.c_str(), msg_id);
    }

    Serial.print(F("Abonner au topic : "));
    Serial.println(topicLectureCanaux);

}

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
    return _mqttFlagNouveauMessage;
}

void Communication::setFlag(bool mqttFlagNouveauMessage)
{
    _mqttFlagNouveauMessage = mqttFlagNouveauMessage;
}

int Communication::getUnivers()
{
    return _univers;
}

void Communication::setFlagTimerConfig(bool mqttFlagTimerConfig)
{
    _mqttFlagTimerConfig = mqttFlagTimerConfig;
}

bool Communication::getFlagTimerConfig()
{
    return _mqttFlagTimerConfig;
}

bool Communication::getEtatWifi()
{
    return _etatConnexionWifi;
}

bool Communication::getEtatMqtt()
{
    _etatConnexionMqtt = clientMQTT.connected();
    return _etatConnexionMqtt;
}

float Communication::getPuissanceWifi()
{
  _puissanceWifi = WiFi.RSSI();
  return _puissanceWifi;
}

String Communication::getQualiterWifi()
{
    this->getPuissanceWifi();

    if (_puissanceWifi >= RSSI_TRES_BON)
    {
        return "Tres bon";
    } else if(_puissanceWifi >= RSSI_ASSEZ_BON) 
    {
        return "Assez bon";
    } else if(_puissanceWifi >= RSSI_PASSABLE) 
    {
        return "Passable";
    } else if(_puissanceWifi >= RSSI_PAS_BON) 
    {
        return "Pas bon";
    } else
    {
        return "Mediocre";
    }
  
}