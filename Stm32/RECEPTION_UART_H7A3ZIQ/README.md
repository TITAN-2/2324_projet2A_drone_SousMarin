# TESTER LA RECEPTION STM32

## Installation de minicom
### Pour MAC
Installation avec HomeBrew :

```
brew install minicom
exec zsh
```


## Utilisation de minicom
 - **Envoie** de symbole, ici on se connecte au Raspberry Pi : 
```
minicom -b 115200 -o -D /dev/ttyS0 
```
>[!NOTE]
>Vous pouvez envoyer des commandes personnaliser avec  `echo 'votre message' < /dev/ttyS0`

 - **Reception** de symbole, ici sur le terminal connecter a la STM32
```
minicom -b 115200 -D /dev/tty.usbmodem1303
```
>[!NOTE]
> Remplacer le .usbmodem1303 par ce qui convient

## BRANCHEMENT SUR STM32
### SUR LA H7A3
On banche raspberry (TX : pin 6) sur la STM32 (RX : PD0, uart4)

## DETAILS DE COMMUNICATION
### RECEPTION | Procédure "maison"
On recoit un message de 8 octet de la forme:
- 2 bits : Check Header --> 0xFFFE
- 1 bit : Check Length --> 8
- 5 bits : Message --> thrust | angle | depth | paquetNumber
- 1 bit : Check Sum --> méthode XOR
- x bits : RPI Response --> Renvoie un message de confirmation ou non à la Raspberry

```
stty -F /dev/ttyS0 115200
echo -ne "\xFF\xFE\x09\x53\x1A\x64\x10\xFF\xCB" > /dev/ttyS0
```
```
stty -F /dev/ttyS0 115200
echo -ne "\xFF\xFE\x09\x53\x2D\x64\x10\xFF\xFD" > /dev/ttyS0
```
### RECEPTION | DYNAMIXEL 2.0
- [ ] https://github.com/TITAN-2/2324_projet2A_drone_SousMarin/issues/2
> Réaliser le même protocole que celui possiblement utilisé par les servo-moteurs. 1 pierre 2 coups.
