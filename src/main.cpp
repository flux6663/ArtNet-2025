#include <Arduino.h>
#include "wifimqtt.h"
#include "DMXInterface.h"

MQTT transmissionMqtt;

void setup()
{
  Serial.begin(115200);
  initialiserWiFi();
  initialiserMQTT();
  initialiserDMX();
}

void loop()
{
  transmissionMqtt.receptionDataMQTT();

  bool flag = transmissionMqtt.getFlag();
  String topic = transmissionMqtt.getTopic();
  String message = transmissionMqtt.getMessage();

  Serial.println((String)flag);

  if (flag == true)
  {

    // if (topic == (MQTT_TOPIC_ENVOIE_CANAUX + "/" + (String)UNIVERS))
    // {
    //   envoyerCanaux(message);
    //   
    // }

    transmissionMqtt.setFlag(false);
  }
}