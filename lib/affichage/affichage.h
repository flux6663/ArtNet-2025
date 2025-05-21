#ifndef _AFFICHAGE_H__
#define __AFFICHAGE_H__

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "esp_timer.h"
#include "driver/timer.h"

#define MENU_1 1
#define MENU_2 2
#define MENU_3 3

#define RESET_FLAG false
#define NEW_FLAG true

#define BROCHE_SDA_0 6
#define BROCHE_SCL_0 7

#define ADRESSE_OLED 0x3C

#define LARGEUR_OLED 128
#define HAUTEUR_OLED 32
#define OLED_RESET -1

#define PIN_LED_ETAT 5
#define ALLUMER true
#define ETEIN false
#define TEMPS_CLIGNOTEMENT_LED 500

#define TAILLE_CARATHERE 1
#define LIGNE_1 0
#define LIGNE_2 1
#define LIGNE_3 2
#define LIGNE_4 3

#define DECONNECTER false
#define CONNECTER true

class Affichage
{
    public:
    void initialiserEcrant();
    void ecrire(byte ligne, String texte);
    void ecrireLigne(String ligne1);
    void ecrireLigne(String ligne1, String ligne2);
    void ecrireLigne(String ligne1, String ligne2, String ligne3);
    void ecrireLigne(String ligne1, String ligne2, String ligne3, String ligne4);
    void effacer();
    void afficher();
    void menuWifi(String wifiSsid, String adressIp, String qualiteLienWifi);
    void menuMqtt(String nomModuleWifi, String mqttIp);
    void menuUnivers(String univers);
    bool getflagChangementMenu();
    void resetflagChangementMenu();

    private:
    void initialiserTimer();
    void initialiserLed();
};

void ledEtatDeconnecter();
void ledEtatConnecter();
void ledAttenteConnexion();

#endif