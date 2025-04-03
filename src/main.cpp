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

  if (transmissionMqtt.getFlag())
  {
    String message = transmissionMqtt.getMessage();
    envoyerCanaux(message);
    transmissionMqtt.setFlag(RESET_FLAG);
  }
}