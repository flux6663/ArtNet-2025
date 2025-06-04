![C++ Badge](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=fff&style=plastic)
<a href="https://github.com/bts-lasalle-avignon-projets/plug-in-pool-2025">
	<img src="https://img.shields.io/badge/Projet-BTS%20CIEL-darkviolet.svg" alt="Projet BTS CIEL"/>
</a>
<a href="https://github.com/bts-lasalle-avignon-projets/plug-in-pool-2025/releases">
	<img src="https://img.shields.io/github/v/release/bts-lasalle-avignon-projets/plug-in-pool-2025" alt="release"/>
</a>

# ArtNet - Module Wi-Fi DMX

</br>

<table>
  <tr>
    <td><img src="images/Regie eclairage.jpeg" alt="Régie d'éclairage" width="400"></td>
    <td><img src="images/scène.png" alt="scène" width="400"></td>
  </tr>
</table>

</br>

- [Le projet](#le_projet)
  - [Logiciels utilisés](#logiciels_utilisés)
- [Installation et Configuration](#installation-et-configuration)
  - [Prérequis](#Prérequis)
  - [Alimentation](#Alimentation)
  - [Temoins Lumineux](#Temoins_Lumineux)
  - [Affichage de la configuration](#Affichage_de_la_configuration)
    - [Menu de configuration Wi-Fi](#Menu_de_configuration_Wi-Fi)
    - [Menu de configuration MQTT](#Menu_de_configuration_MQTT)
    - [Menu de configuration Univer](#Menu_de_configuration_Univer)
  - [Réinitialisation](#Réinitialisation)
  - [Premiére mise en service](#Premiére_mise_en_service)
  - [Configuration de PlatformIO](#Configuration_de_PlatformIO)

# Le projet

Le système développé dans le cadre de ce projet a pour objectif d’assurer la
supervision et le pilotage automatisé des éclairages scéniques en s’appuyant
sur le protocole DMX-512. Il permet de centraliser le contrôle de divers
équipements lumineux.

Grâce à une interface logicielle, l’utilisateur peut gérer les jeux de lumière de
manière dynamique, sans recourir à une console DMX traditionnelle.
Ce système vise à offrir une solution flexible, évolutive et accessible, adaptée
aussi bien aux DJs qu’aux animateurs souhaitant automatiser et personnaliser leurs
ambiances lumineuses depuis une tablette.

L’objectif du projet est de développer un système de contrôle d’éclairage,
reposant sur des **modules WiFi-DMX**, afin de piloter différentes zones scéniques
de façon centralisée, tout en conservant une gestion indépendante pour chaque
zone, via une interface numérique (la régie).

### Logiciels utilisés

- Visual Studio Code
- PlatformIO
- Proteus 8.3

## Installation et Configuration

### Prérequis

Avant d'utiliser votre module Wi-Fi DMX il vous faudra configuré la régie d'éclairage : https://github.com/bts-lasalle-avignon-projets/artnet-2025

### Alimentation

Utiliser un cable USB-C avec au minimum 500 mA.

### Temoins Lumineux

<img src="images/Oled.png" alt="LED" width="300">

- **LED Verte** : temoins de présence de tension 5V et 3.3V.
- **LED Orange** : temoins d'etat de la carte. Tant que la carte n'est pas connecter au Wi-Fi ou le serveur MQTT, alors elle clignote.

<img src="images/LED DMX.png" alt="LED" width="300">

- **LED Rouge** : S'allume aprés l'initialisation du bus DMX.
- **LED Verte** : Clignote a chaque nouvelle envoie sur la liaison DMX.

### Affichage de la configuration

Pour l'affichage, un ecran avec trois menu change toutes les 10s.

#### Menu de configuration Wi-Fi

<img src="images/Menu Wifi.png" alt="menu univer" width="400" />

| Menu | Élément                | Exemple de valeur         |
|-----------|------------------------|----------------------------|
| Wi-Fi     | SSID                   | "MonWiFi"                   |
| Wi-Fi     | Adresse IP du module WI-Fi DMX                   | 192.168.1.102                   |
| Wi-Fi     | Qualité du signal Wi-Fi                   | Assez bon                   |

#### Menu de configuration MQTT

<img src="images/Menu Mqtt.png" alt="Menu mqtt" width="400"/>

| Menu | Élément                | Exemple de valeur         |
|-----------|------------------------|----------------------------|
| MQTT     | Nom du module Wi-Fi DMX             | "Artnet-0AB4"                   |
| MQTT     | Adresse IP du serveur MQTT (broker)                  | 192.168.1.104                   |

#### Menu de configuration Univer

<img src="images/Menu univers.png" alt="menu univer" width="400" />

| Menu | Élément                | Exemple de valeur         |
|-----------|------------------------|----------------------------|
| Univer     | Univers du module Wi-Fi DMX | 2 |

### Réinitialisation

<img src="images/Oled.png" alt="Bouton reset" width="300">

Pour réinitialiser au paramettre d'usine, il faut appuyer sur le bouton présent sur la carte.

### Premiére mise en service

Lors du démarage si il n'y a pas de valeur de configuré alors, le module se met en mode point d'acces Wi-Fi.

<img src="images/attente de conf.jpg" alt="menu attente de configuration" width="300">

1. Connecter vous au Wi-Fi du module :

<img src="images/connexion wifi.png" alt="wifi" width="300">

2. Entrée l'adresse IP du module Wi-Fi DMX renseigner sur l'écran dans votre navigateur.

3. Renseigné les valeurs de votre configuration : Wi-Fi, MQTT et numéro d'univer

<img src="images/page web.png" alt="page web" width="500">

4. Aprés avoir renseigner les valeurs de la configuration, appuyer sur Connexion. Le module Wi-Fi DMX va alors redémarré.

5. En cas de réussite de connexion, l'écran doit afficher les valeurs configuré.

### Configuration de PlatformIO

Fichier `platformio.ini` pour le module Wi-Fi DMX.

```ini
[platformio]
default_envs = esp32dev

[common]
monitor_speed = 115200
lib_deps_external = 
	someweisguy/esp_dmx@^4.1.0
	knolleary/PubSubClient@^2.8
	bblanchon/ArduinoJson@^7.3.1
	esp32async/ESPAsyncWebServer@^3.7.4
	esp32async/AsyncTCP @ ^3.3.8
	adafruit/Adafruit SSD1306

[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = ${common.monitor_speed}
lib_deps = 
	${common.lib_deps_external}
```

&copy; 2025 La Salle Avignon - ArtNet - MARTIN Mathis
