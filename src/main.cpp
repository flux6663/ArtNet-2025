#include <Arduino.h>
#include "configuration.h"
#include "communication.h"
#include "DMXInterface.h"
#include "affichage.h"

#define TEMPS_ATTENTE_MQTT 2000

Configuration sauvegarde;
Communication transmission;
Interface dmx;
Affichage ecran;

enum {Connexion, Wifi, Mqtt, Univers} menuModule;

void initialiserWifi() {

  String wifiSsid = sauvegarde.getSsidWifi();
  String wifiMdp = sauvegarde.getMdpWifi();
  String nomModuleWifi = sauvegarde.getNameModuleWifi();

  ecran.ecrireLigne("Connexion Wifi");
  Serial.println(F("Connexion au WiFi : "));
  Serial.println("wifiSsid : " + wifiSsid);
  Serial.println("wifiMdp : " + wifiMdp);
  Serial.println("nomModuleWifi : " + nomModuleWifi);
  transmission.initialiserWiFi(nomModuleWifi, wifiSsid, wifiMdp);

}

void initialiserUnivers() {

  int univers = sauvegarde.getUnivers();

  ecran.ecrireLigne("Lecture Univers");
  Serial.println(F("Information Univers : "));
  Serial.println("univers : " + (String)univers);
  transmission.initialiserUnivers(univers);

}

void initialiserMqtt() {

  static long currentTime = 0;
  static unsigned long previousTime = 0;
  currentTime = millis();

  if((currentTime - previousTime) >= TEMPS_ATTENTE_MQTT) {

    bool etatWifi = transmission.getEtatWifi();
    bool etatMqtt = transmission.getEtatMqtt();

    if(etatWifi == CONNECTER && etatMqtt == DECONNECTER) 
    {
      previousTime = currentTime;
      String mqttIp = sauvegarde.getIpMqtt();
      int mqttPort = sauvegarde.getPortMqtt();
      String mqttUser = sauvegarde.getUserMqtt();
      String mqttMdp = sauvegarde.getMdpMqtt();

      Serial.println(F("Connexion au MQTT : "));
      ecran.ecrireLigne("Connexion MQTT");
      Serial.println("mqttIp : " + mqttIp);
      Serial.println("mqttPort : " + (String)mqttPort);
      Serial.println("mqttUser : " + mqttUser);
      Serial.println("mqttMdp : " + mqttMdp);

      transmission.initialiserMQTT(mqttIp, mqttPort, mqttUser, mqttMdp);
    }

  }

}

void setup()
{
  ecran.initialiserEcrant();

  ecran.ecrireLigne("Conf : Serial");
  Serial.begin(115200);

  ecran.ecrireLigne("Conf : SPPIF");
  sauvegarde.initialiserMemoire();

  if (sauvegarde.configurationSauvegarder() == true)
  {

    Serial.println(F("Recuperation des donners sauvegarder :"));

    initialiserWifi();
    initialiserUnivers();
    ecran.ecrireLigne("Conf : DMX");
    dmx.initialiser();

    ecran.ecrireLigne("Conf terminer");

  } else {
    Serial.println(F("pas de valeur sauvegarder !"));
    sauvegarde.creationPointAcces();
    delay(2000);
    ecran.ecrireLigne("Attente de conf...", WiFi.softAPIP().toString());
  }

  sauvegarde.creationServeurWeb();
}

void reinitilisationModule() {

  bool flagResetConfig = sauvegarde.getFlagResetConfig();

  if (flagResetConfig == NEW_FLAG)
  {
    flagResetConfig = RESET_FLAG;
    ecran.ecrireLigne("Conf : Reset");
    resetConfiguration();
  }
  
}

void nouveauMessage() {

  transmission.receptionDataMQTT();
  dmx.ledTransmissionDonnerDMX();

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

void menuEcran() {
  switch (menuModule) {
    case Connexion:
      ecran.ecrireLigne("Connexion en cours...");
    break;

    case Wifi:
      ecran.menuWifi(sauvegarde.getSsidWifi(), sauvegarde.getIpAdress(), (String)sauvegarde.getQualiteLienWifi());
    break;

    case Mqtt:
      ecran.menuMqtt(sauvegarde.getNameModuleWifi(), sauvegarde.getIpMqtt());
    break;

    case Univers:
      ecran.menuUnivers((String)sauvegarde.getUnivers());
    break;

    default:
    break;
  }
}

void changementMenuEcran() 
{
  static byte compteur = 0;
  bool flagTimerChangementMenu = ecran.getflagChangementMenu();

  if (flagTimerChangementMenu == NEW_FLAG)
  {
    ecran.resetflagChangementMenu();
    compteur++;

    switch (compteur)
    {
      case MENU_1:
      menuModule = Wifi;
      break;

      case MENU_2:
      menuModule = Mqtt;
      break;

      case MENU_3:
      menuModule = Univers;
      break;
    
    default:
      break;
    }

    if (compteur >= 3) compteur = 0;
  }

  menuEcran();
}

void loop()
{
  reinitilisationModule();
  ledAttenteConnexion();

  bool etatWifi = transmission.getEtatWifi();
  bool etatMqtt = transmission.getEtatMqtt();

  if (etatWifi == CONNECTER && etatMqtt == CONNECTER)
  {
    dmx.resetUrgence();
    ledEtatConnecter();
    changementMenuEcran();
    nouveauMessage();
    envoieConfig();
  } else {
    dmx.arretUrgence();
    menuModule = Connexion;
    ledEtatDeconnecter();
    initialiserMqtt();
  }
}