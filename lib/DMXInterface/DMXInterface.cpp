#include "DMXInterface.h"
#include <Arduino.h>
#include <ArduinoJson.h>

#define PERSONALITER_DMX 1

void Interface::initialiser(uint8_t numeroPortDMX, uint8_t pinTranmissionDMX, uint8_t pinReceptionDMX, uint8_t pinRTS_DMX)
{
    dmx_config_t config = DMX_CONFIG_DEFAULT;

    dmx_personality_t personalities[] = {
        {1, "Default Personality"}};

    dmx_driver_install(numeroPortDMX, &config, personalities, PERSONALITER_DMX);
    dmx_set_pin(numeroPortDMX, pinTranmissionDMX, pinReceptionDMX, pinRTS_DMX);
}

void Interface::lireJson(String message)
{

  JsonDocument messageJson;
  DeserializationError error = deserializeJson(messageJson, message);
  
  if (error) {
    Serial.print("Erreur de parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }
  
  for (JsonObject obj : messageJson.as<JsonArray>()) {
    int canal = obj["canal"];
    int valeur = obj["valeur"];

    Serial.println("canal : " + (String)canal + ", valeur : " + (String) valeur);

    _canauxDmx[canal] = valeur;
  }

}

void Interface::envoyerCanaux(String message)
{
  this->lireJson(message);

  dmx_write(PORT_DMX, _canauxDmx, DMX_PACKET_SIZE);
  dmx_wait_sent(PORT_DMX, DMX_TIMEOUT_TICK);
  dmx_send_num(PORT_DMX, DMX_PACKET_SIZE);
}