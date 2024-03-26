Week 1 : Debut du projet

Cahier des charges :

    Fonctionnalités :
        - Être un système embarqué :
            - Être capable d'avancer dans l'eau
            - Gérer la profondeur du drone
            - Être commandable depuis la terre ferme
        - Récupérer les lunettes de Monsieur Fiack au fond des étangs :
            - Visualiser le fond des étangs
            - Transmettre des informations du drone vers la terre ferme
    
    Contraintes :
        - Énergie limitée
            - Milieu aquatique

            - Présence de rocher au fond et d’algues
            - Poids du sous marin
            - Pression

            - Faible propagation dans l’eau des OEM
            - Allure et orientation du drone variable
        - 
            - Capture vidéo
            - Faible luminosité au fond de l’étang

            - Transmission vidéo en direct (ou faible latence)
            - Transmission de multiple type de donnée (profondeur, vitesse…)



Week 2:
891 grammes

Week 3 : 

- On a récupéré le tube en PVC collé et rempli de sable par M. PAPAZOGLOU, puis on a trouvé la masse nécesaire pour que l'on se retrouve au point de flotaison : 3,120kg
- On s'est également penché sur la manière de faire avancer le sous-marin dans l'eau, comparant les prix entre les hélices sur le côté ou une hélice arrière commandée par un aimant qui tourne.


24/10/2023

Pierre/Alexandre : Établir une connection ssh avec la RaspBerry Pi.

À faire : Établir un flux vidéo

07/11/2023

Pierre/Alexandre/Maëna : 

Mise au point de la prise de photos avec la RaspBerry Pi et début de recherches pour faire un flux vidéo.
Début d'avancée sur le flux vidéo. Problème : frame = None alors qu'il devrait avoir les images permettant de faire la vidéo.

À faire : corriger cette erreur pour obtenir la vidéo.

23/01/2024:
La vidéo est réglée
La communication UART entre Raspberry et STM32 est réussie
Le PCB est en cours de réalisation pour régler les cpateurs et la communication entre les composants
Le mouvement à l'intérieur des seringues est en cours de réalisation, pour gérer la profondeur du Sous Marin.
Il reste à faire:
1- Finir le PCB
2- Finir les mouvements à l'intérieur des seringues
3- Une fois fini faire le code des capteurs pour le faire apparaitre sur le site web
4- Gérer la propulsion lorsque qu'on modifie les joysticks (Lien entre Joystick -> Raspberry -> STM32-> Propulseurs)
5- Gérer la profondeur en modifiant les joysticks (Lien entre Joystick-> Raspberry -> STM32 ->Moteur Seringues)
6- Gérer les batteries, Création batterie (BSM, Relier toutes les cellules entre elles)
7- Tout mettre dans le Sous-Marin
8- Mettre les billes de plombs
9- Lien entre le sous marin et dehors (Lapin)
