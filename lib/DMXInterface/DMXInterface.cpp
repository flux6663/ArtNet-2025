#include "DMXInterface.h"
#include <Arduino.h>

#define PERSONALITER_DMX 1

void initialiserDMX(uint8_t numeroPortDMX, uint8_t pinTranmissionDMX, uint8_t pinReceptionDMX, uint8_t pinRTS_DMX)
{
    dmx_set_pin(numeroPortDMX, pinTranmissionDMX, pinReceptionDMX, pinRTS_DMX);

    dmx_config_t config = DMX_CONFIG_DEFAULT;

    dmx_personality_t personalities[] = {
        {1, "Default Personality"}};

    dmx_driver_install(numeroPortDMX, &config, personalities, PERSONALITER_DMX);
    dmx_driver_install(numeroPortDMX, &config, personalities, PERSONALITER_DMX);
}

void envoyerCanaux(const void *data)
{
    dmx_write(PORT_DMX, data, DMX_PACKET_SIZE);
    dmx_wait_sent(PORT_DMX, DMX_TIMEOUT_TICK);
    dmx_send_num(PORT_DMX, DMX_PACKET_SIZE);
}