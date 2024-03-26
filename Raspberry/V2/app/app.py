# /app/app.py

from flask import Flask, render_template, Response, request
from streaming.server import StreamingHandler, StreamingServer
from streaming.camera import StreamingOutput
import threading
from picamera2 import Picamera2,Preview
from picamera2.encoders import JpegEncoder
from picamera2.outputs import FileOutput
import serial  
import RPi.GPIO as GPIO
import json
import os
import fileinput

app = Flask(__name__)
uart_port = serial.Serial('/dev/ttyS0', baudrate=115200, timeout=1)

@app.route('/')
def index():
    return render_template('index.html')

def generate():
    while True:
        frame = output.frame
        if frame is not None:
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/video_feed')
def video_feed():
    return Response(generate(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

def calculate_checksum_xor(data):
    checksum = 0
    for byte in data:
        checksum ^= int(byte, 16)
    return [format(checksum,"02X")]

# Route pour la mise à jour des valeurs via UART
paquetNumber = format(0x30,"02X")

@app.route('/update_values', methods=['POST'])
def update_values():
    global paquetNumber
    try: 
        vitesse = format(int(request.form['vitesse']),'02X') #Format HEX
        direction = format(int(request.form['direction']),'02X') #Format HEX
        profondeur = format(int(request.form['profondeur']),'02X') #Format HEX

        # Construire la commande UART à partir des valeurs reçues
        checkHeader= ["FF","FE"] #Format HEX
        command = [vitesse,direction,profondeur,paquetNumber]
        checkLength = [format(len(command)*2,'02X')]
        checkSum = calculate_checksum_xor(checkHeader+checkLength+command)
        # Envoyer la commande via UART
        uart_command =[int(item, 16) for sublist in [checkHeader, checkLength,command,checkSum] for item in sublist]
        uart_escaped=bytearray(uart_command)
        uart_port.write(uart_escaped)
        uart_port.flush()
        int(paquetNumber)+1
        paquetNumber =format(int(paquetNumber,16)+1,"02X")
        return f'Send Success {uart_command} | {uart_escaped}'  # Réponse du serveur, peut être personnalisée
    except Exception as e:
        return f'Error: {str(e)}'

def read_byte():
    return int.from_bytes(uart_port.read(), byteorder='big')

def read_message():
    while True:
        byte = read_byte()
        if byte == 0xFF:  # Vérifie si le premier octet est FF
            next_byte = read_byte()
            if next_byte == 0xFE:  # Vérifie si le deuxième octet est FE
                message = bytearray([0xFF, 0xFE])  # Commence le message avec FF FE
                while True:
                    byte = read_byte()
                    if byte == 0xFF:  # Vérifie si le prochain octet est FF
                        next_byte = read_byte()
                        if next_byte == 0xFE:  # Vérifie si l'octet suivant est FE
                            # Fin du message, retourne le message lu
                            return message
                        else:
                            # Si ce n'est pas la fin du message, ajoute les octets au message
                            message += bytearray([0xFF, next_byte])
                    else:
                        # Ajoute les octets au message
                        message.append(byte)
def read_data():
    while True:
        offset_min,offset_max=3,1
        message = read_message().hex()
        message_list=[int(message[i:i+2],16) for i in range(0, len(message), 2)]
        print("Message reçu:", message)
        print("Message convertit:",[int(message[i:i+2],16) for i in range(offset_min, len(message)-offset_max, 2)])

        keys =["posX","posY","angleDelta","temperature","baterryPower"]
        dict_Js = {keys[i]: message_list[i] for i in range(len(keys))}
        print("dict_Js",dict_Js)
        data_js ,path_file_js= json.dumps(dict_Js),"static/js/data.js"
        with open(path_file_js,'w') as fichier_js:
            fichier_js.write("var donnees ={};".format(data_js))
            fichier_js.write("export default donnees;")




picam2 = Picamera2()
picam2.configure(picam2.create_video_configuration(main={"size": (640, 480)}))

# Configurez l'encodeur vidéo pour le format JPEG
output = StreamingOutput()
picam2.start_recording(JpegEncoder(), FileOutput(output))








try:
    address = ('', 8000)
    server = StreamingServer(address, StreamingHandler)

    flask_thread = threading.Thread(target=app.run, args=('0.0.0.0', 5000))
    print("Starting App thread...")
    flask_thread.start()
    
    uart_thread = threading.Thread(target=read_data)
    print("Starting UART thread...")
    uart_thread.start()
    if uart_thread.is_alive():
        print("UART thread is running.")
    else:
        print("UART thread failed to start or stopped unexpectedly.")

    server_thread = threading.Thread(target=server.serve_forever)
    print("Starting Server thread...")
    server_thread.start()

    flask_thread.join()
    server_thread.join()


finally:
    picam2.stop_recording()

if __name__ == '__main__':
    app.run(debug=True)
