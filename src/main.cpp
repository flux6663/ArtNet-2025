#include <Arduino.h>
#include "communication.h"
#include "DMXInterface.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <SPIFFS.h>

Communication transmission;
Interface dmx;

AsyncWebServer server(80);
Preferences preferences;

void setupServer() {

  if(!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/w3.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });
}

void setup()
{
  Serial.begin(115200);
  // transmission.initialiserWiFi();
  // transmission.initialiserMQTT();
  // dmx.initialiser();

  WiFi.softAP("ESP32_Config", "12345678"); // AP pour accéder à la page de config
  Serial.println("Point d'accès lancé : ESP32_Config");

  setupServer();      // Démarre les routes
  server.begin();     // Lance le serveur
}

void nouveauMessage() {

  transmission.receptionDataMQTT();

  bool flag = transmission.getFlag();
  String topic = transmission.getTopic();
  String message = transmission.getMessage();

  if (flag == NEW_FLAG)
  {

    if (topic == (String)MQTT_TOPIC_RECEPTION_CANAUX)
    {
      dmx.envoyerCanaux(message);
    }

    transmission.setFlag(RESET_FLAG);
  }

}

void loop()
{
  // nouveauMessage();
}