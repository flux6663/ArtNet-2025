#include <Arduino.h>
#include "configuration.h"
#include "communication.h"
#include "DMXInterface.h"

Configuration sauvegarde;
Communication transmission;
Interface dmx;

void initialiserWifi() {

  String wifiSsid = sauvegarde.getSsidWifi();
  String wifiMdp = sauvegarde.getMdpWifi();
  String nomModuleWifi = sauvegarde.getNameModuleWifi();

  Serial.println(F("Connexion au WiFi : "));
  Serial.println("wifiSsid : " + wifiSsid);
  Serial.println("wifiMdp : " + wifiMdp);
  Serial.println("nomModuleWifi : " + nomModuleWifi);
  transmission.initialiserWiFi(nomModuleWifi, wifiSsid, wifiMdp);

}

void initialiserUnivers() {

  int univers = sauvegarde.getUnivers();

  Serial.println(F("Information Univers : "));
  Serial.println("univers : " + (String)univers);
  transmission.initialiserUnivers(univers);

}

void initialiserMqtt() {

  bool etatWifi = transmission.getEtatWifi();
  bool etatMqtt = transmission.getEtatMqtt();

  if(etatWifi == CONNECTER && etatMqtt == DECONNECTER) {

    String mqttIp = sauvegarde.getIpMqtt();
    int mqttPort = sauvegarde.getPortMqtt();
    String mqttUser = sauvegarde.getUserMqtt();
    String mqttMdp = sauvegarde.getMdpMqtt();

    Serial.println(F("Connexion au MQTT : "));
    Serial.println("mqttIp : " + mqttIp);
    Serial.println("mqttPort : " + (String)mqttPort);
    Serial.println("mqttUser : " + mqttUser);
    Serial.println("mqttMdp : " + mqttMdp);

    transmission.initialiserMQTT(mqttIp, mqttPort, mqttUser, mqttMdp);

  }

}

void setup()
{
  Serial.begin(115200);
  sauvegarde.initialiserMemoire();

  if (sauvegarde.configurationSauvegarder() == true)
  {

    Serial.println(F("Recuperation des donners sauvegarder :"));

    initialiserWifi();
    initialiserUnivers();
    initialiserMqtt();

    dmx.initialiser();

  } else {
    sauvegarde.creationPointAcces();
    Serial.println(F("pas de valeur sauvegarder !"));
  }

  sauvegarde.creationServeurWeb();
  
}

void reinitilisationModule() {

  bool flagResetConfig = sauvegarde.getFlagResetConfig();

  if (flagResetConfig == NEW_FLAG)
  {
    flagResetConfig = RESET_FLAG;
    resetConfiguration();
  }
  
}

void nouveauMessage() {

  transmission.receptionDataMQTT();

  bool flag = transmission.getFlag();
  String topic = transmission.getTopic();
  String message = transmission.getMessage();
  String topicReceptionCanaux = (String)MQTT_TOPIC_RECEPTION_CANAUX + "/" + (String)transmission.getUnivers();

  if (flag == NEW_FLAG)
  {

    if (topic == topicReceptionCanaux)
    {
      dmx.envoyerCanaux(message);
    }

    transmission.setFlag(RESET_FLAG);
  }

}

void envoieConfig() {

  bool flagTimerConfig = transmission.getFlagTimerConfig();
  int univers = sauvegarde.getUnivers();
  String adressIp = sauvegarde.getIpAdress();
  String adressMac = sauvegarde.getMacAdress();
  uint8_t qualiteLienWifi = sauvegarde.getQualiteLienWifi();

  if (flagTimerConfig == NEW_FLAG)
  {
    transmission.setFlagTimerConfig(RESET_FLAG);
    envoieConfiguration(univers, adressIp, adressMac, qualiteLienWifi);
  }
  
}

void loop()
{
  reinitilisationModule();

  bool etatWifi = transmission.getEtatWifi();
  bool etatMqtt = transmission.getEtatMqtt();

  if (etatWifi == CONNECTER && etatMqtt == CONNECTER)
  {
    nouveauMessage();
    envoieConfig();
  } else {
    initialiserMqtt();
  }
}