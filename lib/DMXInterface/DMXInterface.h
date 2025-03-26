#ifndef _DMXINTERFACE_H__
#define __DMXINTERFACE_H__

#include <Arduino.h>
#include <esp_dmx.h>

#define DMX_TX_PIN 3
#define DMX_RX_PIN 4
#define DMX_RTS_PIN 2
#define PORT_DMX DMX_NUM_1

void initialiserDMX(uint8_t numeroPortDMX = PORT_DMX, uint8_t pinTranmissionDMX = DMX_TX_PIN, uint8_t pinReceptionDMX = DMX_RX_PIN, uint8_t pinRTS_DMX = DMX_RTS_PIN);
void envoyerCanaux(const void *data);

#endif