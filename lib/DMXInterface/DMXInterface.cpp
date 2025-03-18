#include "DMXInterface.h"
#include <Arduino.h>

#define PERSONALITER_DMX 1

void initialiserDMX(uint8_t numeroPortDMX = DMX_NUM_1, uint8_t pinTranmissionDMX = DMX_TX_PIN, uint8_t pinReceptionDMX = DMX_RX_PIN, uint8_t pinRTS_DMX) {
    dmx_config_t config = DMX_CONFIG_DEFAULT;

    dmx_personality_t personalities[] = {
        {1, "Default Personality"}
    };

    dmx_driver_install(numeroPortDMX, &config, personalities, PERSONALITER_DMX);
    dmx_driver_install(numeroPortDMX, &config, personalities, PERSONALITER_DMX);

    dmx_set_pin(numeroPortDMX, pinTranmissionDMX, pinReceptionDMX, pinRTS_DMX);
}


void changerCanal(uint16_t adressDMX, uint8_t canalDMX, uint8_t valeurCanal) {
    dmx_write_slot(PORT_DMX, adressDMX + canalDMX - 1, valeurCanal);
    dmx_wait_sent(PORT_DMX, DMX_TIMEOUT_TICK);
    dmx_send(PORT_DMX);
}