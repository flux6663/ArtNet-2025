#include "affichage.h"
#include <Arduino.h>

#define TIMER_INTERVAL_SEC    15000

bool _etatConnexion = DECONNECTER;
volatile bool _flagChangementMenu = NEW_FLAG;
hw_timer_t * timerMenu = NULL;

Adafruit_SSD1306 oLed(LARGEUR_OLED, HAUTEUR_OLED, &Wire, OLED_RESET);

void Affichage::initialiserLed() {
  pinMode(PIN_LED_ETAT, OUTPUT);
}

void Affichage::initialiserEcrant()
{
  Wire.begin(BROCHE_SDA_0, BROCHE_SCL_0);
  oLed.begin(SSD1306_SWITCHCAPVCC, ADRESSE_OLED);
  oLed.display();
  oLed.clearDisplay();

  oLed.setTextSize(TAILLE_CARATHERE);
  oLed.setTextColor(SSD1306_WHITE);
  this->initialiserLed();
  this->initialiserTimer();
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

void Affichage::ecrireLigne(String ligne1) {
  this->effacer();
  this->ecrire(LIGNE_1, ligne1);
  this->afficher();
}

void Affichage::ecrireLigne(String ligne1, String ligne2) {
  this->effacer();
  this->ecrire(LIGNE_1, ligne1);
  this->ecrire(LIGNE_2, ligne2);
  this->afficher();
}

void Affichage::ecrireLigne(String ligne1, String ligne2, String ligne3) {
  this->effacer();
  this->ecrire(LIGNE_1, ligne1);
  this->ecrire(LIGNE_2, ligne2);
  this->ecrire(LIGNE_3, ligne3);
  this->afficher();
}

void Affichage::ecrireLigne(String ligne1, String ligne2, String ligne3, String ligne4) {
  this->effacer();
  this->ecrire(LIGNE_1, ligne1);
  this->ecrire(LIGNE_2, ligne2);
  this->ecrire(LIGNE_3, ligne3);
  this->ecrire(LIGNE_4, ligne4);
  this->afficher();
}

void Affichage::menuWifi(String wifiSsid, String adressIp, String qualiteLienWifi) {
  wifiSsid = "SSID : " + wifiSsid;
  adressIp = "IP : " + adressIp;
  qualiteLienWifi = "Signal : " + qualiteLienWifi;
  this->ecrireLigne("Conf. Wi-Fi : ", wifiSsid, adressIp, qualiteLienWifi);
}

void Affichage::menuMqtt(String nomModuleWifi, String mqttIp) {
  nomModuleWifi = "NOM : " + nomModuleWifi;
  mqttIp = "IP broker : " + mqttIp;
  this->ecrireLigne("Conf. MQTT : ", nomModuleWifi, mqttIp);
}

void Affichage::menuUnivers(String univers) {
  univers = "Univers : " + univers;
  this->ecrireLigne("Conf. Univers : ", univers);
}

void Affichage::effacer()
{
  oLed.clearDisplay();
}

void Affichage::afficher()
{
  oLed.display();
}

bool Affichage::getflagChangementMenu() {
  return _flagChangementMenu;
}

void Affichage::resetflagChangementMenu() {
  _flagChangementMenu = RESET_FLAG;
}

void ledAttenteConnexion() {
  if (_etatConnexion == DECONNECTER) {
    static long currentTime = 0;
    static unsigned long previousTime = 0;
    static bool etatLed;

    currentTime = millis();
    if((currentTime - previousTime) > TEMPS_CLIGNOTEMENT_LED){

      previousTime = currentTime;
      etatLed = !etatLed;
      digitalWrite(PIN_LED_ETAT, etatLed);
      
    }

  } else {
    digitalWrite(PIN_LED_ETAT, ETEIN);
  }
}

void ledEtatDeconnecter() {
  _etatConnexion = DECONNECTER;
}

void ledEtatConnecter() {
  _etatConnexion = CONNECTER;
}