
mainpage Projet Iot Trio d'Hommes forts
section intro_sec Introduction
- Projet Trio d'hommes forts est un projet qui a pour but de simuler un application type TousAnticovid. Pour cela une connexion entre plusieurs esp32 et un point d'accès (Hamburger) seront mis en place sous la supervision d'un ordinateur.



page Def Définition

MQTT : 
- Pour "Message Queuing Telemetry Transport", est un protocole open source 
- de messagerie qui assure des communications non permanentes entre des appareils par le transport de leurs messages. 

Adafruit :
- Adafruit Industries est une entreprise fondée en 2005 par Limor Fried, une ingénieure américaine, 
- qui se spécialise dans la vente et la production de composants électroniques et de matériel libre.    

ESP32 :
- ESP32 est une famille de microcontrôleurs à base de processeurs 32 bits, 
- développée par Espressif Systems, une société chinoise basée à Shanghai.

STATION (WIFI_STA) : 
- Le mode Station (STA) est utilisé pour connecter le module ESP32 à un point d'accès Wi-Fi. 
- L'ESP32 se comporte comme un ordinateur qui serait connecté à notre box. Si la box est reliée à Internet, alors l'ESP32 peut accéder à Internet. L'ESP32 peut se comporter en tant que client, c'est-à-dire faire des requêtes aux autres appareils connectés sur le réseau ou en tant que serveur , c'est-à-dire que d'autres appareils connectés sur le réseau vont envoyer des requêtes à l'ESP32. Dans les 2 cas, l'ESP32 peut accéder à Internet.

AP (Access Point) (WIFI_AP) : 
- En mode Access Point, l'ESP32 se comporte comme un réseau Wi-Fi (un peu comme une box) :
- d'autres appareils peuvent s'y connecter. Dans ce mode, l'ESP32 n'est relié à aucun autre réseau et n'est donc pas connecté à Internet.
- Ce mode est plus gourmand en calcul et en énergie (la carte ESP32 va chauffer) puisque l'ESP32 doit simuler un routeur Wifi complet (Soft AP).

 
page Objectif&Materiel.html

section Objectif 
- Mise en place d'une connexion entre les esps (En mode STATION) et le point d'accès (Hamburger).
- Mise en place des utilisateurs
- Mise en place d'un serveur web sur un cœur de l'esp
- Mise en place d'un dashboard Adafruit qui permet de visualiser les données et contrôler l'état de l'Arduino.
- Mise en place d'un serveur MQTT sur un cœur de l'esp
    
section Matériels
   Utilisation de plusieurs esp 32 originalement en mode STATION et d'un ordinateur qui permet de suivre et gérer les esp32.
   Un téléphone (Hamburger = ACCESS POINT) nous sert de point d'accès internet.


page Etapes.html

section EspConnect 1 : Mise en place d'une connexion entre les esp et le point d'accès : 
    - Utilisation de la librairie MyWiFi.h (permets la connexion simultanée de plusieurs appareils sur un même access point).
    - On utilise en général le mode AP provisoirement pour rentrer les paramètres de connexion du réseau WiFi (SSID + MDP). 
    - On peut également l'utiliser pour avoir un réseau séparé de son réseau domestique et non relié à Internet.
     subsection fonctions Fonctions Utilisées 
        -  setup Wifi(): Va permettre de setup l'Access point et vérifier si l'utilisateur en mode station peut se connecter.
        - Connect_WiFi_AP(const char* ssid, const char* password): va permettre de connecter l'esp au point d'accès.
        - Check_WiFi_connection(): permet de vérifier si l'esp est connecté au point d'accès.

section ServerWeb 2 : Mise en place d'un serveur web (Protocole en MQTT : PUB / SUB) :
   - Utilisation de la librairie MyWebServer.h (permet la mise en place d'un server web en local, qui permet de recevoir les informations sur un autre utilisateurs si abonnée).
        
section AdafruitSetup 3 : Mise en place d'un dashboard Adafruit :
- Utilisation d'un compte Adafruit (crée au préalable ).
- Ce qui nous permet par la suite de visualiser les données et contrôler l'état de l'Arduino.
        
section UserStatus 4 : Mise en place, utilisation et test des statuts des utilisateurs unique des esps 
- (Protocole en MQTT : PUB / SUB) 
-  Utilisation de la librairie MyWiFiClient.h (permet de se connecter à un serveur MQTT et de publier des messages sur un topic ou de s'abonner à un topic pour recevoir des messages).

page Architecture.html
image html test.png width=700 height=600cm



mainpage Projet Iot trio d'homme fort
section intro_sec Introduction
- Projet Gogole est un projet qui a pour but de simuler un application type TousAnticovid pour cela une connexion entre plusieurs esp32 et un point d'accès (Hamburger) seront mis en place sous la supervision d'un ordinateur.



page Def Définition

 MQTT : 
- Pour "Message Queuing Telemetry Transport", est un protocole open source 
- de messagerie qui assure des communications non permanentes entre des appareils par le transport de leurs messages. 

Adafruit :
- Adafruit Industries est une entreprise fondée en 2005 par Limor Fried, une ingénieure américaine, 
- qui se spécialise dans la vente et la production de composants électroniques et de matériel libre.    

ESP32 :
- ESP32 est une famille de microcontrôleurs à base de processeurs 32 bits, 
- développée par Espressif Systems, une société chinoise basée à Shanghai.

STATION (WIFI_STA ) : 
- Le mode Station (STA) est utilisé pour connecter le module ESP32 à un point d'accès Wi-Fi. 
- L'ESP32 se comporte comme un ordinateur qui serait connecté à notre box. Si la box est reliée à Internet, alors l'ESP32 peut accéder à Internet. L'ESP32 peut se comporter en tant que client , c'est-à-dire faire des requêtes aux autres appareils connectés sur le réseau ou en tant que serveur , c'est-à-dire que d'autres appareils connectés sur le réseau vont envoyer des requêtes à l'ESP32. Dans les 2 cas, l'ESP32 peut accéder à Internet.

AP (Access Point) (WIFI_AP ) : 
- En mode Access Point, l'ESP32 se comporte comme un réseau Wi-Fi (un peu comme une box) :
- d'autres appareils peuvent s'y connecter dessus. Dans ce mode, l'ESP32 n'est relié à aucun autre réseau et n'est donc pas connecté à Internet.
- Ce mode est plus gourmand en calcul et en énergie (la carte ESP32 va chauffer) puisque l'ESP32 doit simuler un routeur Wifi complet (Soft AP).



page Objectif&Materiel.html

section Objectif 
- Mise en place d'une connection entre les esp (En mode STATION) et le point d'accées (Hamburger).
- Mise en place des utilisateurs
- Mise en place d'un serveur web sur un coeur de l'esp
- Mise en place d'un dashboard Adafruit qui permet de visualiser les données et controler l'état de l'arduino.
- Mise en place d'un serveur MQTT sur un coeur de l'esp
    
section Matériels
- Utilisation de plusieurs esp 32 originalement en mode STATION et d'un ordinateur qui permet de suivre et gérer les esp32.
- Un téléphone (Hamburger = ACCESS POINT) nous sert de point d'accès internet.


page Etapes.html

section EspConnect 1 : Mise en place d'une connexion entre les esp et le point d'accès : 
- Utilisation de la librairie MyWiFi.h (permets la connexion simultanée de plusieurs appareils sur un même access point).
- On utilise en général le mode AP provisoirement pour rentrer les paramètres de connexion du réseau WiFi (SSID + MDP). 
- On peut également l'utiliser pour avoir un réseau séparé de son réseau domestique et non relié à Internet.


subsection fonctions Fonctions Utilisées 
- setup_Wifi(): Va permettre de setup l'access point et vérifier si l'utilisateur en mode station peut se connecter.
- Connect_WiFi_AP(const char* ssid, const char* password): va permettre de connecter l'esp au point d'accès.
- Check_WiFi_connection(): permet de vérifier si l'esp est connecté au point d'accés.

section ServerWeb 2 : Mise en place d'un serveur web (Protocole en MQTT : PUB / SUB) :
- Utilisation de la librairie MyWebServer.h (permet la mise en place d'un server web en local, qui permet de recevoir les informations sur un autre utilisateurs si abonnée).
        
section AdafruitSetup 3 : Mise en place d'un dashboard Adafruit :
- Utilisation d'un compte Adafruit (crée au préalable).
- Ce qui nous permet par la suite de visualiser les données et controler l'état de l'arduino.
        
section UserStatus 4 : Mise en place, utilisation et test des statuts des utilisateurs uniques des esps 
- (Protocole en MQTT : PUB / SUB) 
-  Utilisation de la librairie MyWiFiClient.h (permet de se connecter à un serveur MQTT et de publier des messages sur un topic ou de s'abonner à un topic pour recevoir des messages).

page Architecture.html
image html test.png width=700 height=600cm