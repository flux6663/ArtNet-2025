::: mermaid

sequenceDiagram
    actor R as Régie d'éclairage
    box Module Wifi DMX
    participant W as Wi-Fi
    participant Mqtt as Mqtt
    participant Mem as Memoire
    participant M as Microcontroleur
    participant B as Bouton reset
    participant E as Ecran
    participant L as LED
    end
    actor D as DMX

    autonumber
    M ->> +L : Initialiser LED
    M ->> +B : Initialiser Bouton reset

    M ->> +E : Initialiser affichage
    E ->> -E: Log erreur

    M ->> +Mem : Initialiser memoire
    Mem ->> Mem: Log erreur
    Mem ->> -M : Lire configuration

    alt Pas de valeur sauvegarder
    activate W
    M ->> W : Se mettre en mode point d'accès
    M ->> W : Initialisation de l'interface Web
    M ->> L : Faire clignoter la led d'etat
    W --> W : Attente de configuration
    W ->> M : Nouvelle configuration
    deactivate W
    M ->> M : Redémarrage
    end

    M ->> D : Initialiser DMX
    M ->> L : Allumer led d'etat

    loop
    activate W
    M ->> W : Connexion au Wi-Fi
    W ->> W: Connexion...

    activate Mqtt
    W ->> Mqtt : Connexion au Broker
    Mqtt ->> Mqtt: Connexion...
    Mqtt ->> M: Inscription au Topic

    
    alt Interrution message MQTT
    R ->> Mqtt : Envoie canaux
    Mqtt ->> M : Reception tram canaux
    M ->> M : Extration des canaux du json
    end

    M ->> D : Envoie des canaux
    M ->> L : Faire clignoter la led lors d'un transmission sur le DMX

    alt Interruption envoie config
    M ->> Mqtt: création tram de config
    Mqtt ->> R : Envoie tram de config
    Note over M,R: Action sur timer 15s
    end

    alt Interruption changement menu ecran
    M ->> E : Changement menu ecran
    Note over M,E: Action sur timer 10s
    end

    alt Interruption Bouton reset
    B ->> M : Demande de réinitialisation
    M ->> Mem : Effacer les valeurs
    M ->> M : Redémarrage
    end

    deactivate W
    deactivate Mqtt
    end