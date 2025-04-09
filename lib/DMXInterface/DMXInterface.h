#ifndef _DMXINTERFACE_H__
#define __DMXINTERFACE_H__

#include <Arduino.h>
#include <esp_dmx.h>
#include <ArduinoJson.h>

#define DMX_TX_PIN 3
#define DMX_RX_PIN 4
#define DMX_RTS_PIN 2
#define PORT_DMX DMX_NUM_1

#define PIN_DMX_LED_1 17
#define PIN_DMX_LED_2 16
#define ALLUMER true
#define ETEIN false
#define TEMPS_CLIGNOTEMENT_LED 500

#define NOUVELLE_TRANMISSION_DMX true
#define PAS_DE_TRANSMISSION_DMX false

class Interface
{
    public:
    void initialiser(uint8_t numeroPortDMX = PORT_DMX, uint8_t pinTranmissionDMX = DMX_TX_PIN, uint8_t pinReceptionDMX = DMX_RX_PIN, uint8_t pinRTS_DMX = DMX_RTS_PIN);
    void envoyerCanaux(String message);
    void ledTransmissionDonnerDMX();
    void arretUrgence();
    void resetUrgence();

    private:
    void lireJson(String message);
    void initialiserLed();

    uint8_t _canauxDmx[DMX_PACKET_SIZE];
    bool _transmissionDMX = PAS_DE_TRANSMISSION_DMX;
    bool _arretUrgence = false;
    bool _execution = false;
};

#endif