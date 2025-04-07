#include "configuration.h"
#include <Arduino.h>

AsyncWebServer serverWeb(PORT_SERVEUR_WEB);
Preferences memoire;

void resetConfiguration() {
  memoire.begin("config", false);
  memoire.remove(CLE_MEMOIRE_WIFI_SSID);
  memoire.remove(CLE_MEMOIRE_WIFI_MDP);

  memoire.remove(CLE_MEMOIRE_MQTT_IP);
  memoire.remove(CLE_MEMOIRE_MQTT_PORT);
  memoire.remove(CLE_MEMOIRE_MQTT_USER);
  memoire.remove(CLE_MEMOIRE_MQTT_MDP);

  memoire.remove(CLE_MEMOIRE_UNIVERS);
  memoire.end();

  delay(1000);

  ESP.restart();
}

void IRAM_ATTR appuisBoutonReset() {
  static unsigned long tempsCourant = 0;
  static unsigned long nouveauTemps = 0;

  tempsCourant = millis();
  if((tempsCourant - nouveauTemps) > TEMPS_REBONS) {
    nouveauTemps = tempsCourant;

    resetConfiguration();

  }
}

void Configuration::initialiserBoutonReset() {
  pinMode(PIN_BOUTON_RESET, INPUT_PULLUP);
	attachInterrupt(PIN_BOUTON_RESET, appuisBoutonReset, FALLING);
}

void Configuration::initialiserMemoire() {

  if(!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  this->lireMemoire();
  this->initialiserBoutonReset();
}

void Configuration::lireMemoire() {
  memoire.begin("config", true);
  _ssidWifi = memoire.getString(CLE_MEMOIRE_WIFI_SSID, "");
  _mdpWifi = memoire.getString(CLE_MEMOIRE_WIFI_MDP, "");

  _ipMqtt = memoire.getString(CLE_MEMOIRE_MQTT_IP, "");
  _portMqtt = memoire.getInt(CLE_MEMOIRE_MQTT_PORT, 0);
  _userMqtt = memoire.getString(CLE_MEMOIRE_MQTT_USER, "");
  _mdpMqtt = memoire.getString(CLE_MEMOIRE_MQTT_MDP, "");

  _univers = memoire.getInt(CLE_MEMOIRE_UNIVERS, 0);
  memoire.end();
}

String Configuration::getNameModuleWifi()
{
  String adressMac = WiFi.macAddress();
  String dernierNumeroAdressMac = adressMac.substring(adressMac.length() - 5);
  dernierNumeroAdressMac.replace(":", "");
  _nomModuleWifi = "Module-" + dernierNumeroAdressMac;
  return _nomModuleWifi;
}

void Configuration::creationServeurWeb() {

  this->creationPointAcces();

  serverWeb.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  serverWeb.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/w3.css", "text/css");
  });

  serverWeb.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  serverWeb.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){

    String ssid = request->hasParam("wifi_ssid") ? request->getParam("wifi_ssid")->value() : "";
    String wifiMdp = request->hasParam("wifi_mdp") ? request->getParam("wifi_mdp")->value() : "";
    String mqttIp = request->hasParam("mqtt_ip") ? request->getParam("mqtt_ip")->value() : "";
    String mqttPort = request->hasParam("mqtt_port") ? request->getParam("mqtt_port")->value() : "";
    String mqttUser = request->hasParam("mqtt_user") ? request->getParam("mqtt_user")->value() : "";
    String mqttMdp = request->hasParam("mqtt_mdp") ? request->getParam("mqtt_mdp")->value() : "";
    String univers = request->hasParam("univers") ? request->getParam("univers")->value() : "";

    memoire.begin("config", false);
    memoire.putString(CLE_MEMOIRE_WIFI_SSID, ssid);
    memoire.putString(CLE_MEMOIRE_WIFI_MDP, wifiMdp);

    memoire.putString(CLE_MEMOIRE_MQTT_IP, mqttIp);
    memoire.putInt(CLE_MEMOIRE_MQTT_PORT, mqttPort.toInt());
    memoire.putString(CLE_MEMOIRE_MQTT_USER, mqttUser);
    memoire.putString(CLE_MEMOIRE_MQTT_MDP, mqttMdp);

    memoire.putInt(CLE_MEMOIRE_UNIVERS, univers.toInt());
    memoire.end();

    Serial.println("Nouvelle valeur de configuration recue !");

    Serial.print("ssid : ");
    Serial.println(ssid);
    Serial.print("mdp : ");
    Serial.println(wifiMdp);

    Serial.print("mqttIp : ");
    Serial.println(mqttIp);
    Serial.print("mqttPort : ");
    Serial.println(mqttPort);
    Serial.print("mqttUser : ");
    Serial.println(mqttUser);
    Serial.print("mqttMdp : ");
    Serial.println(mqttMdp);

    Serial.print("univers : ");
    Serial.println(univers);

    request->send(200, "text/plain", "Configuration sauvegardée. Redémarrage...");
    Serial.print("Configuration sauvegardée. Redémarrage...");
    delay(2000);
    ESP.restart();
  });

  serverWeb.begin();

}

void Configuration::creationPointAcces() {
  WiFi.mode(WIFI_MODE_AP);
  WiFi.softAP(_nomModuleWifi, POINT_ACCES_MDP);
}

bool Configuration::configurationSauvegarder() {

  if (_ssidWifi != "" && _ipMqtt != "" && _univers != 0)
  {
    return true;
  }

  return false;

}

String Configuration::getSsidWifi() {
  return _ssidWifi;
}

String Configuration::getMdpWifi() {
  return _mdpWifi;
}

String Configuration::getIpMqtt() {
  return _ipMqtt;
}

int Configuration::getPortMqtt() {
  return _portMqtt;
}

String Configuration::getUserMqtt() {
  return _userMqtt;
}

String Configuration::getMdpMqtt() {
  return _mdpMqtt;
}

int Configuration::getUnivers() {
  return _univers;
}