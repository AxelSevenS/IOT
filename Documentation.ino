/** \file documentation.ino 
 * 
 * \mainpage Bienvenue
 * \tableofcontents
 * \section introduction Introduction
 * Cette documentation a pour objectif de vous fournir tous les éléments pour utiliser et maîtriser l'ensemble
 * de ces composants en vue de vous donner l'autonomie pour apprendre à les utiliser et développer vos objets
 * connectés.
 * 
 * Ce module d'IoT fait appel à de très nombreuses notions en informatique :
 * - Electronique
 * - Electricité
 * - Infrastructure réseau
 * - Développement informatique
 * - Protocoles réseaux
 * - ...
 * 
 * \section mode Mode de fonctionnement
 * Nous disposons de 8 cours d'une demi-journée pour travailler ensemble sur l'IoT.
 * - <b>Cours #1</b><br>
 *   La première séance est dédiée à la mise en place de l'environnement de travail et de développement ainsi qu'au partage du mode de fonctionnement de ce module.<br>
 *   Ce point est essentiel pour la qualité du suivi de l'ensemble du module.
 * - <b>Cours #2 à #7</b><br>
 *   Vous disposez de tout le code et de toute la documentation pour mener à bien le projet qui servira d'évaluation.<br>
 *   Les différents composants seront présentés au fur et à mesure, comme indiqué ci-dessous, afin que vous puissiez vous les approprier et les intégrer progressivement.<br>
 *   A l'issue de la présentation / démonstration du ou des composants prévus à chaque cours, je suis à votre disposition pour vous accompagner dans la prise en main dudit module
 *   et/ou de votre projet.
 *   Ainsi libres d'avancer à votre rythme, à vous de suivre la présentation des composants ou d'avancer en tout autonomie. A vous de voir !<br>
 * - <b>Cours #8</b><br>
 *   L'évaluation sera réalisée pendant ce dernier cours.<br>
 *   L'ensemble du module s'étale sur 3 mois; vous disposez de temps entre les cours pour travailler de manière progressive sur le projet d'évaluation en 
 *   intégrant les composants logiciels au fur et à mesure. Vous êtes invités à organiser votre travail.
 * 
 * Les dates sont les suivantes :
 * -# Lundi 06 mars 2023 après-midi
 *   - Prise en main d'Arduino
 *   - Découverte des principales fonctionnalités
 *   - Découverte du WiFi
 * -# Lundi 13 mars 2023 matin
 *   - WiFi
 *   - Serveur Web
 *   - Système de fichier + JSON
 * -# Lundi 20 mars 2023 après-midi
 *   - Capteurs/LED
 * -# Lundi 03 avril 2023 matin
 *   - Network Tranport Protocol
 *   - Ticker
 *   - Timer
 *   - Deep Sleep
 * -# Mardi 11 avril 2023 matin
 *   - MQTT / Adafruit
 * -# Lundi 15 mai 2023 matin
 *   - Bluetooth
 * -# Mardi 30 mai 2023 matin
 *   - Over The Air
 *   - Remote Debug
 *   - Core 0
 * -# Lundi 05 juin 2023 matin
 *   - Evaluation : démonstration par groupe
 *
 * \section prerequis Prérequis
 * Pour utiliser ce code il est nécessaire de disposer de :
 * - \ref arduino
 * - \ref doxygen
 * - \ref esp32
 *
 * \section arduino IDE Arduino
 * Rendez-vous sur la page de téléchargement d'Arduino : https://www.arduino.cc/en/main/software
 * 
 * Téléchargez et installez votre version préférée associée à votre ordinateur et votre système d'exploitation.
 * 
 * Voici quelques fonctionnalités pour utiliser Arduino :
 * - \b Téléverser (icône "flèche vers la droite" ou menu "Croquis/Téléverser")<br>
 *   Cette action permet de compiler votre code et de le télécharger sur votre carte.<br>
 *   A noter qu'il faut au préalable avoir défini le type de carte et le port sur lequel elle est branchée.
 * - <b>Moniteur ou Traceur Série</b><br>
 *   La carte est connectée à l'ordinateur et ils communiquent au travers d'un port série.<br>
 *   Le moniteur série affiche les retours que la carte envoie à l'ordinateur sur ce port série. Bien utile pour voir ce qu'il se passe sur la carte et récupérer des logs.<br>
 *   Sans cela, la carte fonctionne en toute autonomie sans que vous sachiez ce qui se passe.
 * - <b>Librairies / bibliothèques</b><br>
 *   Nous allons utiliser de nombreuses fonctionnalités pour lesquelles nous allons récupérer le code.<BR>
 *   Le menu "Outils/Gérer les bibliothèques" vous donne accès aux bibliothèques pour les télécharger ou les mettre à jour.
 * 
 * \section doxygen Doxygen
 * Doxygen est un générateur de documentation capable de produire une documentation logicielle à partir du code source.
 * Vous aurez besoin d'utiliser Doxygen (http://www.doxygen.nl/) pour documenter votre code et faire le rendu sous la même forme que cette documentation.
 * 
 * Step 1 : Sélectionner le répertoire de travail Arduino
 * Step 2 : Passer en mode \b Expert et renseigner les éléments suivants : 
 * - Topics \b Project
 *   - OUTPUT_DIRECTORY : renseigner "."
 *   - OUTPUT_LANGUAGE : Sélectionner "French"
 *   - EXTENSION_MAPPING : Ajouter "ino=c" à la liste
 * - Topics \b Input 
 *   - IMAGE_PATH : Ajouter 'images" à la liste
 *   - FILE_PATTERN : Ajouter "*.ino" à la liste
 * - Topics \b HTML
 *   - GENERATE_TREEVIEW : cocher l'option
 * - Topics \b LaTeX
 *   - GENERATE_LATEX : décocher l'option
 * 
 * L'onglet "Run" propose "Run Doxygen" qui va générer votre documentation. Vous pouvez la voir avec "Show HTML output".
 * 
 * Voici un guide vers l'ensemble des commandes à votre disposition pour commenter votre code et générer la documentation de votre projet : http://www.doxygen.nl/manual/commands.html
 *  
 * \section esp32 ESP32
 * 
 * Une carte ESP32 vous est remise pour travailler.
 * 
 * Arduino est originellement prévu pour fonctionner pour les cartes qu'ils proposent eux-mêmes et qui sont disponibles sur leur store (https://store.arduino.cc/).
 * Comme de nombreux clones, déclinaisons, substituts, évolutions ... sont nés depuis la création d'Arduino, l'environnement de 
 * développement permet d'utiliser d'autres types de cartes. 
 * 
 * C'est notre cas, car nous allons travailler avec une carte ESP32. Il faut donc configurer l'environnement de développement 
 * pour qu'il prenne en compte les spécificités de notre carte :
 * - Lancer le programme Arduino.
 * - Ouvrir la fenêtre des Préférences.
 * - Dans le champ prévu pour URL de gestionnaire de cartes supplémentaires (Additional Boards Manager URLs), vous pouvez renseigner 
 *   une ou plusieurs URLs (séparés par des virgules) qui va charger la description et les informations sur les cartes à prendre en 
 *   charge. Pour les cartes ESP32, renseigner l'URL suivante : https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 * - Dans le menu "Outils", "Type de carte", "Gestionnaire de carte" vous trouvez une liste de composants. 
 *   Dans le champ de recherche, saisissez "ESP32" puis sélectionnez \b ESP32 by \b Espressif Systems,
 *   Sélectionner la dernière version (hors beta ou rc), puis Installer.
 * - Une fois l'installation terminée, vous trouvez désormais une liste de cartes disponibles dans le menu "Outils", 
 *   "Type de carte" en sélectionnant "ESP32 Arduino / ESP32 DEV MODULE".
 * - Brancher votre carte ESP32 sur un des ports USB de votre ordinateur.
 * - Sélectionner le port sur lequel est connectée la carte afin qu'Arduino sache comment communiquer avec elle.<br>
 *   Menu "Outils/Port" puis sélectionner le port associé (sur Windows le dernier de la liste est le dernier branché).
 * 
 * Vous êtes désormais prêt pour travailler avec la carte ESP32.
 * 
*/

// ************************************************************************************
// PROJET
// ************************************************************************************
 /**
 * \page projet Projet et évaluation 
 * 
 * \section contact YNOV Contact Tracer
 * Voici la liste des fonctionnalités à réaliser :
 * - En tant qu'utilisateur
 *   - Je peux me connecter en WiFi à l'objet et un portail web captif me permet de le configurer - <b>3 POINTS</b>
 *     - WiFi : SSID et mot de passe
 *     - Paramètres : plusieurs paramètres me sont proposés pour ajuster son fonctionnement
 *   - Autres Contacts Tracers
 *     - Mon objet détecte les Contacts Tracers à proximité (paramétrable en mètre) - <b>3 POINTS</b>
 *     - Mon objet stocke en mémoire la liste de tous Contacts Tracers que j'ai rencontrés, même moins de 5 min - <b>3 POINTS</b>
 *     - Si mon objet reste à proximité d'un Contact Tracer au moins 5 min (paramétrable en minutes), alors il est identié comme une contact à suivre - <b>3 POINTS</b>
 *     - Je peux consulter l'historique des mes rencontres et contacts à suivre (nombre, date, durée, état de santé ...) - <b>2 POINTS</b>
 *     - Je suis informé du changement de l'état de santé de mes contacts à suivre - <b>5 POINTS</b>
 *   - Mon état de santé
 *     - Mon état de santé, quel qu'il soit, est diffusé afin que ceux qui me "suivent" soient informés - <b>2 POINTS</b>
 *     - Je peux renseigner mon état de santé (sain, cas contact, malade) - <b>1 POINT</b>
 *     - Si un de mes contacts à suivre est malade alors mon état de santé passe à "cas contact", si je ne suis pas déjà malade et diffuse l'information à mes contacts à suivre - <b>2 POINT</b>
 *     - Je suis notifié (notification téléphone / email / LED ou autre moyen) que mon état de santé est passé à "Cas contact" - <b>2 POINTS</b>
 *   - Capteur
 *     - Mon objet capte et remonte régulièrement (paramétrable) une information d'un capteur (exemple capteur de température) - <b>2 POINTS</b>
 *     - Un seuil (paramétrable) est défini, qui une fois franchi me fait passer à l'état malade (exemple température > 39°C) - <b>2 POINTS</b>
 * - En tant qu'administrateur
 *   - Je peux mettre à jour le firmware des objets à distance - <b>3 POINTS</b>
 *   - Je peux activer et lire les traces sur les objets à distance - <b>3 POINTS</b>
 * - En tant qu'évaluateur
 *   - Je veux une démonstration opérationnelle de chaque fonctionnalité réalisée
 * 
 * \section livrables Livrables
 * Voici la liste des éléments à livrer :
 *   - Démonstration - <b>20 POINTS</b>
 *   - Un fichier zip à déposer sur Moodle
 *     - Tous les fichiers de code commentés - <b>3 POINTS</b>
 *     - La documentation HTML générée par Doxygen est présente - <b>5 POINTS</b>
 *     - La documentation est suffisamment précise pour comprendre le code - <b>10 POINTS</b>
 *       - La liste des fonctionnalités développées et démontrées est précisée - 3 points
 *       - Les fonctionnalités développées et démontrées sont documentées - 7 points
 *     - Le lien vers la vidéo est présent et accessible depuis la "home page" de la documentation Doxygen - <b>2 POINTS</b><br>
 *       Par exemple un lien YouTube sur une vidéo non répertoriée.
 *       A noter que si le lien est absent, la vidéo ne pourra pas être évaluée !
 *   - Une video de moins de 5 minutes abordant les thèmes suivants :
 *     - L'équipe et la répartition du travail - <b>1 POINTS</b>
 *     - L'architecture de votre projet où apparaissent - <b>5 POINTS</b>
 *       - Les utilisateurs
 *       - Les Contacts Tracers
 *       - Les éléments du réseau impliqués dans la solution
 *       - Les flux réseaux : protocoles de transport et application
 *     - Le diagramme de séquence de la mise à jour d'un état de santé à "malade" et sa diffusion à ses contacts qui passent à l'état "cas contact" - <b>5 POINTS</b>
 *     - L'organisation des fichiers - <b>2 POINTS</b>
 *     - Le parcours et la présentation du code associé à ce diagramme de séquence - <b>7 POINTS</b>
 *       - Tout est clair, pas besoin d'ouvrir le code : 7
 *       - Un doute, une vérification (moins de 2 minutes) : 5
 *       - Pas clair, besoin de comprendre (entre 2 et 5 minutes) : 3
 *       - Pas clair, fichier manquant ... besoin de plus de 5 min : 0
 * 
 * \section groupe Groupes de travail
 * Pour mener à bien ce projet :
 * - Vous travaillez seuls ou en groupe avec le nombre de membres que vous souhaitez
 * - Tous les membres d'un groupe auront la même note quelque soit la répartition du travail au sein du groupe
 * 
 * \section evalutation Evaluation
 * - Démonstration : 20 coefficient 1
 * - Code : 20 coefficient 1
 * - Vidéo : 20 coefficient 1
*/
