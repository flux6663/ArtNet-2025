#include <Arduino.h>
#include "DMXInterface.h"

void setup()
{
  Serial.begin(115200);
  initialiserDMX();
}

void loop()
{
  changerCanal(10, 1, 255);
  changerCanal(10, 2, 0);
  changerCanal(10, 3, 0);
  Serial.println("Rouge");
  delay(500);

  changerCanal(10, 1, 0);
  changerCanal(10, 2, 255);
  changerCanal(10, 3, 0);
  Serial.println("Vert");
  delay(500);

  changerCanal(10, 1, 0);
  changerCanal(10, 2, 0);
  changerCanal(10, 3, 255);
  Serial.println("Bleu");
  delay(500);
}