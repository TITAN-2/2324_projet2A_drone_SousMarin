# /myflaskapp/app.py

from flask import Flask, render_template, Response, request
from streaming.server import StreamingHandler, StreamingServer
from streaming.camera import StreamingOutput
import threading
from picamera2 import Picamera2
from picamera2.encoders import JpegEncoder
from picamera2.outputs import FileOutput
import serial  

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

# Fonction pour envoyer les données via UART
def send_uart(command):
    uart_port.write(command.encode())
    uart_port.flush()


# Route pour la mise à jour des valeurs via UART
@app.route('/update_values', methods=['POST'])
def update_values():
    vitesse = request.form['vitesse']
    profondeur = request.form['profondeur']
    direction = request.form['direction']

    # Construire la commande UART à partir des valeurs reçues
    uart_command = f"V{vitesse}P{profondeur}D{direction}"

    # Envoyer la commande via UART
    send_uart(uart_command)

    return 'Success'  # Réponse du serveur, peut être personnalisée

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
