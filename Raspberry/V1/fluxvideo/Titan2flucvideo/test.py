import serial
import time

# Définition du port série (UART) sur le Raspberry Pi
ser = serial.Serial('/dev/ttyS0', baudrate=115200, timeout=1)

# Liste de bits à envoyer
data_to_send = [1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0]

# Conversion de la liste en chaîne de caractères pour l'envoi
data_string = ''.join(str(bit) for bit in data_to_send)

# Envoi des données via UART
ser.write(data_string.encode())

# Attente d'une réponse (peut être optionnel)
received_data = ser.readline().decode().strip()
print("Réponse reçue :", received_data)

# Fermeture du port série
ser.close()