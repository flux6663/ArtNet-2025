#include <Arduino.h>
#include "communication.h"
#include "DMXInterface.h"

Communication transmission;

void setup()
{
  Serial.begin(115200);
  transmission.initialiserWiFi();
  transmission.initialiserMQTT();
  initialiserDMX();
}


void loop()
{
  transmission.receptionDataMQTT();

  bool flag = transmission.getFlag();
  String topic = transmission.getTopic();
  String message = transmission.getMessage();

  if (flag == NEW_FLAG)
  {

    if (topic == (String)MQTT_TOPIC_RECEPTION_CANAUX)
    {
      envoyerCanaux(message);
    }

    transmission.setFlag(RESET_FLAG);
  }
}