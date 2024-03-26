# Projet de Drone Sous-marin - TITAN II - 2e Année ENSEA

## Description du Projet

Ce projet de deuxième année d'école d'ingénieur en électronique informatique vise à concevoir et développer un drone sous-marin contrôlable à distance. L'objectif de ce projet est de récupérer des objets dans le fond de l'étang de Cergy : plus précisement des lunettes de plongée. 
Le drone sera équipé de deux moteurs submersibles afin de propulser l'ensemble sous l'eau, de différents capteurs explicités plus tard, de seringues en guise de ballast, et finalement d'une STM32 couplée a une Raspeberry Pi. 

Le drone sera équipé de capteurs de pression et d'une centrale inertielle, connectés à une carte STM32. La communication entre les capteurs, les moteurs et les utilisateurs sera gérée par la STM32, tandis qu'une Raspberry Pi sera utilisée pour recevoir les informations des utilisateurs, héberger un serveur, et afficher une interface homme-machine (IHM) avec un flux vidéo provenant des caméras Pi.

## Fonctionnalités Principales

1. **STM32 pour la Gestion des Capteurs et des Moteurs**
   - La carte STM32 sera responsable de la communication avec les capteurs.
   - Elle assurera également le contrôle des moteurs en réponse aux commandes des utilisateurs.

2. **Raspberry Pi pour l'Interface Utilisateur**
   - La Raspberry Pi recevra les informations des utilisateurs.
   - Elle hébergera un serveur qui affichera le flux vidéo des caméras Pi et permettra aux utilisateurs d'envoyer des commandes au drone sous-marin.

3. **UART STM32 et la Raspberry Pi**
   - L'UART sera bidirectionnel
   - La Rasberry communiquera vers la STM32 les commandes utilisateurs reçu du serveur.
   - La STM32 communiquera vers la Raspberry les informations des différents capteurs (position, vitesse, niveau de batterie...)

## Configuration Matérielle

- **STM32**
  - Pin A0: PWM Moteur Gauche
  - Pin A1: PWM Moteur Droit
  - Pin RX: Reception de l'UART Raspberry
  - Pin TX: Transmission via UART vers Raspberry

- **Raspberry Pi**
  - A définir

- **Caméras Pi**
  - Voir le HowTo (a définir également)

## Installation et Exécution

1. Cloner le dépôt du projet sur la machine hôte.
   ```bash
   git clone https://github.com/votre-utilisateur/projet-drone-sous-marin.git
## Tester les parties indépendemments

[LA RECEPTION STM32](/RECEPTION_UART_H7A3ZIQ/HOWTO.md)
