#include "DMXInterface.h"
#include <Arduino.h>

#define PERSONALITER_DMX 1

uint8_t canauxReset[DMX_PACKET_SIZE] = {0};

void Interface::initialiser(uint8_t numeroPortDMX, uint8_t pinTranmissionDMX, uint8_t pinReceptionDMX, uint8_t pinRTS_DMX)
{
    dmx_config_t config = DMX_CONFIG_DEFAULT;

    dmx_personality_t personalities[] = {
        {1, "Default Personality"}};

    dmx_driver_install(numeroPortDMX, &config, personalities, PERSONALITER_DMX);
    dmx_set_pin(numeroPortDMX, pinTranmissionDMX, pinReceptionDMX, pinRTS_DMX);
    
    this->initialiserLed();

    digitalWrite(PIN_DMX_LED_2, ALLUMER);
}

void Interface::initialiserLed() {
  pinMode(PIN_DMX_LED_1, OUTPUT);
  pinMode(PIN_DMX_LED_2, OUTPUT);
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
  if (_arretUrgence == false)
  {
    this->lireJson(message);
    _transmissionDMX = NOUVELLE_TRANMISSION_DMX;

    dmx_write(PORT_DMX, _canauxDmx, DMX_PACKET_SIZE);
    dmx_wait_sent(PORT_DMX, DMX_TIMEOUT_TICK);
    dmx_send_num(PORT_DMX, DMX_PACKET_SIZE);
  }
}

void Interface::arretUrgence()
{
  _arretUrgence = true;
  if (_execution == false)
  {
    dmx_write(PORT_DMX, canauxReset, DMX_PACKET_SIZE);
    dmx_send_num(PORT_DMX, DMX_PACKET_SIZE);
    Serial.println("Arret d'urgence");
    digitalWrite(PIN_DMX_LED_2, ETEIN);
    _execution = true;
  }
}

void Interface::resetUrgence()
{
  _arretUrgence = false;
  _execution = false;
  digitalWrite(PIN_DMX_LED_2, ALLUMER);
}

void Interface::ledTransmissionDonnerDMX() 
{
  if (_transmissionDMX) {
    static long currentTime = 0;
    static unsigned long previousTime = 0;

    digitalWrite(PIN_DMX_LED_1, ALLUMER);

    currentTime = millis();
    if((currentTime - previousTime) > TEMPS_CLIGNOTEMENT_LED){

      digitalWrite(PIN_DMX_LED_1, ETEIN);
      _transmissionDMX = PAS_DE_TRANSMISSION_DMX;

      previousTime = currentTime;
    }
  }
}