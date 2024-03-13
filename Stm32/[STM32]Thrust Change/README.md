# Modifier le thrust des moteurs

## Description 

Le but est de pouvoir modifier la puissance des moteurs controlés par un ASC, pour cela on envoie un PWM de rapport cyclique variant.
Il existe un protocole pour démarrer les ASC : 

**Start Protocole**

 - Envoyer un signal périodique créneaux de 50Hz avec un temps haut de 1ms, cela doit correspondre à notre rapport cyclique le plus faible ie \alpha = 0
 - Envoyer ce signal durant 1s
 - Modifier, ensuite, le temps haut à 2ms, cela correspond à notre rapport cyclique le plus élevée ie \alpha = 1
 - Envoyer ce signal durant 1s

**End Protocole**




