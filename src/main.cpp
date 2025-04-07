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

  Serial.println("wifiSsid : " + wifiSsid);
  Serial.println("wifiMdp : " + wifiMdp);
  Serial.println("nomModuleWifi : " + nomModuleWifi);
  transmission.initialiserWiFi(nomModuleWifi, wifiSsid, wifiMdp);

}

void initialiserUnivers() {

  int univers = sauvegarde.getUnivers();

  Serial.println("univers : " + (String)univers);
  transmission.initialiserUnivers(univers);

}

void initialiserMqtt() {

  String mqttIp = sauvegarde.getIpMqtt();
  int mqttPort = sauvegarde.getPortMqtt();
  String mqttUser = sauvegarde.getUserMqtt();
  String mqttMdp = sauvegarde.getMdpMqtt();

  Serial.println("mqttIp : " + mqttIp);
  Serial.println("mqttPort : " + (String)mqttPort);
  Serial.println("mqttUser : " + mqttUser);
  Serial.println("mqttMdp : " + mqttMdp);

  transmission.initialiserMQTT(mqttIp, mqttPort, mqttUser, mqttMdp);

}

void setup()
{
  Serial.begin(115200);
  sauvegarde.initialiserMemoire();

  if (sauvegarde.configurationSauvegarder() == true)
  {

    Serial.println("Recuperation des donners sauvegarder :");

    initialiserWifi();
    initialiserUnivers();
    initialiserMqtt();

    dmx.initialiser();

  } else {
    sauvegarde.creationServeurWeb();
    Serial.println("pas de valeur sauvegarder !");
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

void loop()
{
  nouveauMessage();
}