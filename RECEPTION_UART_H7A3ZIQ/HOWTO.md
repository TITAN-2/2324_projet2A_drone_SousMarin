# TIPS POUR TESTER CE MODULE

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

