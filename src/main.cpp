#include <Arduino.h>
#include <ArduinoJson.h>
#include "wifimqtt.h"
#include "DMXInterface.h"

uint8_t canauxDmx[DMX_PACKET_SIZE];

messageMQTT message;
JsonDocument doc;

void setup()
{
  Serial.begin(115200);
  initialiserWiFi();
  initialiserMQTT();
  initialiserDMX();
}

void mqttVersDmx() {

  String old_message;
  
  if (old_message != message.voirMessage())
  {
    deserializeJson(doc, message.voirMessage());

    for (uint16_t i = 0; i < DMX_PACKET_SIZE; i++)
    {
      canauxDmx[i] = doc[String("canal") + (String)i];
    }

    old_message = message.voirMessage();
  }

}

void transmissionCanauxDmx() {

  for (uint16_t i = 0; i < DMX_PACKET_SIZE; i++)
  {
    changerCanal(i, canauxDmx[i]);
  }
  
}

void loop()
{

  receptionDataMQTT();
  mqttVersDmx();
  transmissionCanauxDmx();

}