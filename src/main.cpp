#include <Arduino.h>
#include "configuration.h"
#include "communication.h"
#include "DMXInterface.h"

Configuration sauvegarde;
Communication transmission;
Interface dmx;

void setup()
{
  Serial.begin(115200);
  sauvegarde.initialiserMemoire();

  if (sauvegarde.configurationSauvegarder() == true)
  {
    // transmission.initialiserWiFi();
    // transmission.initialiserMQTT();
    // dmx.initialiser();

    Serial.println("Configuration Wi-Fi");
    Serial.println(sauvegarde.getSsidWifi());
    Serial.println(sauvegarde.getMdpWifi());

    Serial.println("Configuration MQTT");
    Serial.println(sauvegarde.getIpMqtt());
    Serial.println(sauvegarde.getPortMqtt());
    Serial.println(sauvegarde.getUserMqtt());
    Serial.println(sauvegarde.getMdpMqtt());

    Serial.println("Configuration Univers");
    Serial.println(sauvegarde.getUnivers());

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

  if (flag == NEW_FLAG)
  {

    if (topic == (String)MQTT_TOPIC_RECEPTION_CANAUX)
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