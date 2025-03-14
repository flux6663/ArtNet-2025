/*

  DMX Write

  This sketch allows you to write DMX data to a DMX listener using a standard
  DMX shield, such SparkFun ESP32 Thing Plus DMX to LED Shield. This sketch was
  made for the Arduino framework!

  Created 10 September 2021
  By Mitch Weisbrod

  https://github.com/someweisguy/esp_dmx

*/
#include <Arduino.h>
#include <esp_dmx.h>

/* First, lets define the hardware pins that we are using with our ESP32. We
  need to define which pin is transmitting data and which pin is receiving data.
  DMX circuits also often need to be told when we are transmitting and when we
  are receiving data. We can do this by defining an enable pin. */
int transmitPin = 3;
int receivePin = 4;
int enablePin = 2;
/* Make sure to double-check that these pins are compatible with your ESP32!
  Some ESP32s, such as the ESP32-WROVER series, do not allow you to read or
  write data on pins 16 or 17, so it's always good to read the manuals. */

/* Next, lets decide which DMX port to use. The ESP32 has either 2 or 3 ports.
  Port 0 is typically used to transmit serial data back to your Serial Monitor,
  so we shouldn't use that port. Lets use port 1! */
dmx_port_t dmxPort = DMX_NUM_1;

/* Now we want somewhere to store our DMX data. Since a single packet of DMX
  data can be up to 513 bytes long, we want our array to be at least that long.
  This library knows that the max DMX packet size is 513, so we can fill in the
  array size with `DMX_PACKET_SIZE`. */
byte data[DMX_PACKET_SIZE];

/* This variable will allow us to update our packet and print to the Serial
  Monitor at a regular interval. */
unsigned long lastUpdate = millis();

void setup()
{
  /* Start the serial connection back to the computer so that we can log
    messages to the Serial Monitor. Lets set the baud rate to 115200. */
  Serial.begin(115200);

  /* Now we will install the DMX driver! We'll tell it which DMX port to use,
    what device configuration to use, and what DMX personalities it should have.
    If you aren't sure which configuration to use, you can use the macros
    `DMX_CONFIG_DEFAULT` to set the configuration to its default settings.
    Because the device is being setup as a DMX controller, this device won't use
    any DMX personalities. */
  dmx_config_t config = DMX_CONFIG_DEFAULT;

  dmx_personality_t personalities[] = {
      {1, "Default Personality"}};

  const int personality_count = 1;
  dmx_driver_install(DMX_NUM_1, &config, personalities, personality_count);
  dmx_driver_install(DMX_NUM_1, &config, personalities, personality_count);

  dmx_set_start_address(DMX_NUM_1, 10);

  /* Now set the DMX hardware pins to the pins that we want to use and setup
    will be complete! */
  dmx_set_pin(DMX_NUM_1, transmitPin, receivePin, enablePin);
}

void loop()
{

  dmx_write_slot(DMX_NUM_1, 10, 20);
  dmx_write_slot(DMX_NUM_1, 11, 0);
  dmx_write_slot(DMX_NUM_1, 12, 0);

  dmx_wait_sent(DMX_NUM_1, DMX_TIMEOUT_TICK);
  dmx_send(DMX_NUM_1);

  delay(500);

  dmx_write_slot(DMX_NUM_1, 10, 0);
  dmx_write_slot(DMX_NUM_1, 11, 20);
  dmx_write_slot(DMX_NUM_1, 12, 0);

  dmx_wait_sent(DMX_NUM_1, DMX_TIMEOUT_TICK);
  dmx_send(DMX_NUM_1);

  delay(500);

  dmx_write_slot(DMX_NUM_1, 10, 0);
  dmx_write_slot(DMX_NUM_1, 11, 0);
  dmx_write_slot(DMX_NUM_1, 12, 20);

  dmx_wait_sent(DMX_NUM_1, DMX_TIMEOUT_TICK);
  dmx_send(DMX_NUM_1);

  delay(500);
}