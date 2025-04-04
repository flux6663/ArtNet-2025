// #include <WiFi.h>
// #include <ESPAsyncWebServer.h>
// #include <PubSubClient.h>
// #include <Preferences.h>  // Pour sauvegarder les paramètres

// Preferences preferences;   // Stockage non volatil

// // Point d'accès WiFi
// const char* ssid_ap = "Artnet";
// const char* password_ap = "";

// // Stockage des informations de connexion
// String ssid_sta, password_sta, mqtt_server, mqtt_user, mqtt_password;
// int univers = 0;

// // Serveur web et client MQTT
// AsyncWebServer server(80);
// WiFiClient espClient;
// PubSubClient client(espClient);

// // Page HTML du formulaire
// const char* htmlPage = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
//     <title>Configuration WiFi & MQTT</title>
//     <style>
//         body { font-family: Arial, sans-serif; text-align: center; margin: 50px; }
//         input { margin: 5px; padding: 8px; width: 80%; }
//         button { padding: 10px; background-color: blue; color: white; border: none; cursor: pointer; }
//     </style>
// </head>
// <body>
//     <h2>Configuration WiFi & MQTT</h2>
//     <form action="/connect" method="POST">
//         <label>SSID WiFi:</label><br>
//         <input type="text" name="ssid"><br>
//         <label>Mot de passe WiFi:</label><br>
//         <input type="password" name="password"><br>
//         <label>Adresse IP du Broker MQTT:</label><br>
//         <input type="text" name="mqtt_server"><br>
//         <label>Utilisateur MQTT:</label><br>
//         <input type="text" name="mqtt_user"><br>
//         <label>Mot de passe MQTT:</label><br>
//         <input type="password" name="mqtt_password"><br>
//         <label>Numéro d'univers:</label><br>
//         <input type="number" name="univers"><br>
//         <button type="submit">Enregistrer et Connecter</button>
//     </form>
// </body>
// </html>
// )rawliteral";

// // Sauvegarde des paramètres en mémoire
// void saveSettings() {
//     preferences.begin("config", false);
//     preferences.putString("ssid", ssid_sta);
//     preferences.putString("password", password_sta);
//     preferences.putString("mqtt_server", mqtt_server);
//     preferences.putString("mqtt_user", mqtt_user);
//     preferences.putString("mqtt_password", mqtt_password);
//     preferences.putInt("univers", univers);
//     preferences.end();
//     Serial.println(" Paramètres enregistrés !");
// }

// // Chargement des paramètres au démarrage
// void loadSettings() {
//     preferences.begin("config", true);
//     ssid_sta = preferences.getString("ssid", "");
//     password_sta = preferences.getString("password", "");
//     mqtt_server = preferences.getString("mqtt_server", "");
//     mqtt_user = preferences.getString("mqtt_user", "");
//     mqtt_password = preferences.getString("mqtt_password", "");
//     univers = preferences.getInt("univers", 0);
//     preferences.end();

//     Serial.println(" Paramètres chargés :");
//     Serial.println("SSID : " + ssid_sta);
//     Serial.println("MQTT Server : " + mqtt_server);
//     Serial.println("MQTT User : " + mqtt_user);
//     Serial.println("Univers : " + String(univers));
// }

// // Démarrer le point d'accès WiFi
// void startAccessPoint() {
//     WiFi.softAP(ssid_ap, password_ap);
//     Serial.println("Point d'accès WiFi activé.");
//     Serial.print("IP AP : ");
//     Serial.println(WiFi.softAPIP());
// }

// // Gérer le serveur web
// void setupServer() {
//     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//         request->send(200, "text/html", htmlPage);
//     });

//     server.on("/connect", HTTP_POST, [](AsyncWebServerRequest *request) {
//         if (request->hasParam("ssid", true) && request->hasParam("password", true) &&
//             request->hasParam("mqtt_server", true) && request->hasParam("mqtt_user", true) &&
//             request->hasParam("mqtt_password", true) && request->hasParam("univers", true)) {
            
//             ssid_sta = request->getParam("ssid", true)->value();
//             password_sta = request->getParam("password", true)->value();
//             mqtt_server = request->getParam("mqtt_server", true)->value();
//             mqtt_user = request->getParam("mqtt_user", true)->value();
//             mqtt_password = request->getParam("mqtt_password", true)->value();
//             univers = request->getParam("univers", true)->value().toInt();

//             saveSettings();  // Sauvegarde des nouvelles valeurs

//             request->send(200, "text/plain", "Connexion en cours...");

//             Serial.println("Tentative de connexion au WiFi : " + ssid_sta);
//             WiFi.softAPdisconnect(true);
//             WiFi.begin(ssid_sta.c_str(), password_sta.c_str());

//             unsigned long startAttemptTime = millis();
//             while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
//                 delay(500);
//                 Serial.print(".");
//             }

//             if (WiFi.status() == WL_CONNECTED) {
//                 Serial.println("\n Connecté au WiFi !");
//                 Serial.print("IP : ");
//                 Serial.println(WiFi.localIP());

//                 client.setServer(mqtt_server.c_str(), 1883);
//                 connectToMQTT();
//             } else {
//                 Serial.println("\n Échec de connexion. Retour au mode AP.");
//                 startAccessPoint();
//             }
//         } else {
//             request->send(400, "text/plain", " Paramètres manquants.");
//         }
//     });

//     server.begin();
// }

// // Connexion au serveur MQTT
// void connectToMQTT() {
//     Serial.println(" Connexion au broker MQTT...");
//     while (!client.connected()) {
//         Serial.print("Tentative de connexion à ");
//         Serial.println(mqtt_server);
//         if (client.connect("ESP32Client", mqtt_user.c_str(), mqtt_password.c_str())) {
//             Serial.println(" Connecté à MQTT !");
//             String topic = "artnet/" + String(univers);
//             client.subscribe(topic.c_str());
//             Serial.println(" Abonné à : " + topic);
//         } else {
//             Serial.print(" Échec, code erreur : ");
//             Serial.print(client.state());
//             Serial.println(" => Nouvelle tentative dans 5s...");
//             delay(5000);
//         }
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     loadSettings(); // Charger les paramètres enregistrés

//     if (ssid_sta != "") {  // Si des paramètres existent, essayer de se connecter
//         WiFi.begin(ssid_sta.c_str(), password_sta.c_str());

//         unsigned long startAttemptTime = millis();
//         while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
//             delay(500);
//             Serial.print(".");
//         }

//         if (WiFi.status() == WL_CONNECTED) {
//             Serial.println("\n Connecté au WiFi !");
//             client.setServer(mqtt_server.c_str(), 1883);
//             connectToMQTT();
//         } else {
//             Serial.println("\n Échec de connexion. Activation du mode AP.");
//             startAccessPoint();
//         }
//     } else {
//         startAccessPoint();  // Si pas de configuration, démarrer en mode AP
//     }

//     setupServer();
// }

// void loop() {
//     if (WiFi.status() == WL_CONNECTED) {
//         client.loop(); // Gestion MQTT
//     }
// }