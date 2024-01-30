import serial

def read_uart():
    # Configuration de la communication UART
    ser = serial.Serial('/dev/ttyS0', baudrate=115200)  # Adapter le port et le débit en bauds en conséquence

    try:
        while True:
            uart_data = ser.readline().decode('utf-8').strip()
            print("Message UART reçu:", uart_data)
            
    except KeyboardInterrupt:
        print("Arrêt de la lecture UART.")

def send_uart(data_to_send):
    # Configuration de la communication UART
    ser = serial.Serial('/dev/ttyS0', baudrate=115200)  # Adapter le port et le débit en bauds en conséquence

    try:
        # Envoi des données à l'UART
        ser.write(data_to_send.encode('utf-8'))
        print(f"Données envoyées à l'UART: {data_to_send}")
        
    except KeyboardInterrupt:
        print("Arrêt de l'envoi UART.")
    finally:
        # Assurez-vous de fermer la connexion série
        ser.close()


if __name__ == "__main__":
    read_uart()
    data_to_send = "Hello, STM32!"
    send_uart(data_to_send)