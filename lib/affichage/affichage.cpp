#include "affichage.h"
#include <Arduino.h>

Adafruit_SSD1306 oLed(LARGEUR_OLED, HAUTEUR_OLED, &Wire, OLED_RESET);

void Affichage::initialiserEcrant()
{
  Wire.begin(BROCHE_SDA_0, BROCHE_SCL_0);
  oLed.begin(SSD1306_SWITCHCAPVCC, ADRESSE_OLED);
  oLed.display();
  oLed.clearDisplay();

  oLed.setTextSize(TAILLE_CARATHERE);
  oLed.setTextColor(SSD1306_WHITE);
}

void Affichage::ecrire(byte ligne, String texte)
{

  switch (ligne)
  {
  case LIGNE_1:
    oLed.setCursor(0, 0);
    oLed.print(texte);
    break;

  case LIGNE_2:
    oLed.setCursor(0, oLed.height() * 1 / 4);
    oLed.print(texte);
    break;

  case LIGNE_3:
    oLed.setCursor(0, oLed.height() * 1 / 2);
    oLed.print(texte);
    break;

  case LIGNE_4:
    oLed.setCursor(0, oLed.height() * 3 / 4);
    oLed.print(texte);
    break;

  default:
    break;
  }
}

void Affichage::effacer()
{
  oLed.clearDisplay();
}

void Affichage::afficher()
{
  oLed.display();
}