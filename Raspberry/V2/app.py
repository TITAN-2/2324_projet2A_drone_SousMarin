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
        vitesse = format(int(request.form['vitesse']),'02X')
        direction = format(int(request.form['direction']),'02X')
        profondeur = format(int(request.form['profondeur']),'02X')
        # Construire la commande UART à partir des valeurs reçues
        checkHeader= ["FF","FE"]
        command = [vitesse,direction,profondeur,paquetNumber]
        checkLength = [format(len(command)*2+1,'02X')]
        checkSum = calculate_checksum_xor(checkHeader+checkLength+command)

        # Envoyer la commande via UART
        uart_command ="".join(checkHeader+checkLength+command+checkSum)
        uart_port.write(uart_command.encode("utf-8"))
        uart_port.flush()
        int(paquetNumber)+1
        paquetNumber =format(int(paquetNumber,16)+1,"02X")
        return f'Send Success {uart_command}'  # Réponse du serveur, peut être personnalisée
    except Exception as e:
        return f'Error: {str(e)}'

picam2 = Picamera2()
picam2.configure(picam2.create_video_configuration(main={"size": (640, 480)}))

# Configurez l'encodeur vidéo pour le format JPEG
output = StreamingOutput()
picam2.start_recording(JpegEncoder(), FileOutput(output))

try:
    address = ('', 8000)
    server = StreamingServer(address, StreamingHandler)

    flask_thread = threading.Thread(target=app.run, args=('0.0.0.0', 5000))
    flask_thread.start()

    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.start()

    flask_thread.join()
    server_thread.join()

finally:
    picam2.stop_recording()

if __name__ == '__main__':
    app.run(debug=True)
