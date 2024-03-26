import serial
import time

thrust = 53
# thrust compris entre 0 et 100 


#UART transforme thrste en suite bianire UART friendly
def UART(thrust):
    t = thrust
    t = bin(t)[2:]
    t =t.zfill(9)# Le bit de départ 0 est ajouté d office puis suivi d'un octet contenant l'infomation utile ie thrust en binaire
    t = t + "1" # bit de stop
    return t

print(UART(45))



def send_data_to_stm32(thrust, angle, serial_port, baud_rate=9600):
    try:
        # Ouvrir la connexion UART
        ser = serial.Serial(serial_port, baud_rate, timeout=1)

        # Construire la chaîne de données à envoyer
        data_to_send = f"{thrust},{angle}\n"

        # Envoyer les données
        ser.write(data_to_send.encode())

        # Fermer la connexion UART
        ser.close()

        print(f'Données envoyées au STM32 : {data_to_send}')

    except Exception as e:
        print(f'Erreur lors de l\'envoi des données : {e}')

# Exemple d'utilisation
thrust_value = UART(1500)
angle_value = UART5(45)
serial_port = "/dev/ttyS0"  # Assurez-vous de spécifier le port UART correct
send_data_to_stm32(thrust_value, angle_value, serial_port)