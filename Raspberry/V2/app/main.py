# server.py

import io
import logging
import socketserver
from http import server
from threading import Condition

from flask import Flask, send_from_directory,render_template
from flask_socketio import SocketIO

from picamera2 import Picamera2
from picamera2.encoders import JpegEncoder
from picamera2.outputs import FileOutput

app = Flask(__name__)
socketio = SocketIO(app)

PAGE_PATH = "app/index.html"

with open(PAGE_PATH, "r") as file:
    PAGE = file.read()

class StreamingOutput(io.BufferedIOBase):
    def __init__(self):
        self.frame = None
        self.condition = Condition()

    def write(self, buf):
        with self.condition:
            self.frame = buf
            self.condition.notify_all()

class StreamingHandler(server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(301)
            self.send_header('Location', '/app/index.html')
            self.end_headers()
        elif self.path == '/app/index.html':
            content = PAGE.encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', len(content))
            self.end_headers()
            self.wfile.write(content)
        elif self.path.startswith('/app/static/'):
            # Serve static files
            try:
                filename = self.path.split('/app/static/')[1]
                content = self.serve_static(filename)
                self.send_response(200)
                self.send_header('Content-Type', 'text/css' if filename.endswith('.css') else 'application/javascript')
                self.send_header('Content-Length', len(content))
                self.end_headers()
                self.wfile.write(content)
            except FileNotFoundError:
                self.send_error(404)
                self.end_headers()
        elif self.path == '/app/stream.mjpg':
            self.send_response(200)
            self.send_header('Age', 0)
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
            self.end_headers()
            try:
                while True:
                    with output.condition:
                        output.condition.wait()
                        frame = output.frame
                    self.wfile.write(b'--FRAME\r\n')
                    self.send_header('Content-Type', 'image/jpeg')
                    self.send_header('Content-Length', len(frame))
                    self.end_headers()
                    self.wfile.write(frame)
                    self.wfile.write(b'\r\n')
            except Exception as e:
                logging.warning(
                    'Removed streaming client %s: %s',
                    self.client_address, str(e))
        else:
            self.send_error(404)
            self.end_headers()

    def serve_static(self, filename):
        return send_from_directory('app/static', filename)

class StreamingServer(socketserver.ThreadingMixIn, server.HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

picam2 = Picamera2()
picam2.configure(picam2.create_video_configuration(main={"size": (640, 480)}))
output = StreamingOutput()
picam2.start_recording(JpegEncoder(), FileOutput(output))

try:
    address = ('', 8000)
    server = StreamingServer(address, StreamingHandler)
    server.serve_forever()
finally:
    picam2.stop_recording()


@app.route('/')
def index():
    return render_template('app/index.html')

@app.route('/uart_data', methods=['POST'])
def uart_data():
    # Lire les données UART ici
    # Mettre à jour les variables en fonction des données reçues
    return 'Données UART reçues avec succès'

@socketio.on('message_from_uart')
def handle_uart_data(message):
    # Traiter les données UART et envoyer les mises à jour au client
    socketio.emit('update_data', {'data': 'Nouvelles données actualisées'})
