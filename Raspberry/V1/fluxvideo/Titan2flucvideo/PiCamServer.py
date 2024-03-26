import io
import logging
import socketserver
from http import server
from threading import Condition

from picamera2 import Picamera2
from picamera2.encoders import JpegEncoder
from picamera2.outputs import FileOutput

PAGE =""" \
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<style>
body {
    margin: 0;
    overflow: hidden; /* Pour éviter le défilement de la page */
}

#joystick-profondeur-container {
    width: 50px;
    height: 250px;
    background-color: #ccc;
    border-top-left-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    border-top-right-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    border-bottom-left-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    border-bottom-right-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    position: fixed;
    bottom: 100px; /* Ajustez la distance par rapport au bas de la page */
    left: 100px; /* Ajustez la distance par rapport à la droite de la page */
}

#joystick-profondeur-handle {
    width: 50px;
    height: 50px;
    background-color: #333;
    border-radius: 50%;
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    cursor: pointer;
}

#joystick-vitesse-container {
    width: 50px;
    height: 250px;
    background-color: #ccc;
    border-top-left-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    border-top-right-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    border-bottom-left-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    border-bottom-right-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    position: fixed;
    bottom: 100px; /* Ajustez la distance par rapport au bas de la page */
    left: 200px; /* Ajustez la distance par rapport à la droite de la page */
}

#joystick-vitesse-handle {
    width: 50px;
    height: 50px;
    background-color: #333;
    border-radius: 50%;
    position: absolute;
    top: 90%;
    left: 50%;
    transform: translate(-50%, -50%);
    cursor: pointer;
}

#joystick-direction-container {
    width: 400px;
    height: 50px;
    background-color: #ccc;
    border-top-left-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    border-top-right-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    border-bottom-left-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    border-bottom-right-radius: 25px; /* Ajustez la valeur pour changer le rayon */
    position: fixed;
    right: 150px; /* Ajustez la distance par rapport au bas de la page */
    bottom: 100px; /* Ajustez la distance par rapport à la droite de la page */
}

#joystick-direction-handle {
    width: 50px;
    height: 50px;
    background-color: #333;
    border-radius: 50%;
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    cursor: pointer;
}

#joystick-values {
    position: fixed;
    top: 10px; /* Ajustez la distance par rapport au haut de la page */
    left: 50%; /* Centre horizontalement */
    transform: translateX(-50%); /* Centre horizontalement */
    text-align: center; /* Centrer le texte horizontalement */
    display: flex; /* Utiliser le modèle de boîte flexible */
}

#joystick-values p {
    margin: 0 20px; /* Marge entre chaque compteur */
}
</style>
</head>
<body>

    <div id="joystick-vitesse-container">
        <div id="joystick-vitesse-handle"></div>
    </div>
    <div id="joystick-profondeur-container">
        <div id="joystick-profondeur-handle"></div>
    </div>
    <div id="joystick-direction-container">
        <div id="joystick-direction-handle"></div>
    </div>
    <div id="joystick-values">
        <p>Vitesse: <span id="vitesse-value">0</span></p>
        <p>Profondeur: <span id="profondeur-value">0</span></p>
        <p>Direction: <span id="direction-value">0</span></p>
    </div>

<script>
var joystickVitesseHandle = document.getElementById('joystick-vitesse-handle');
var joystickVitesseContainer = document.getElementById('joystick-vitesse-container');
var isDraggingVitesse = false;

var joystickProfondeurHandle = document.getElementById('joystick-profondeur-handle');
var joystickProfondeurContainer = document.getElementById('joystick-profondeur-container');
var isDraggingProfondeur = false;

var joystickDirectionHandle = document.getElementById('joystick-direction-handle');
var joystickDirectionContainer = document.getElementById('joystick-direction-container');
var isDraggingDirection = false;

resetVitesseValue();
resetProfondeurValue();
resetDirectionValue();

// Event listeners for Joystick Vitesse
joystickVitesseHandle.addEventListener('mousedown', function (e) {
    isDraggingVitesse = true;
    joystickVitesseContainer.style.transition = 'none';
    updateJoystickVitesse(e, joystickVitesseHandle, joystickVitesseContainer, isDraggingVitesse);
});

// Event listeners for Joystick Profondeur
joystickProfondeurHandle.addEventListener('mousedown', function (e) {
    isDraggingProfondeur = true;
    joystickProfondeurContainer.style.transition = 'none';
    updateJoystickProfondeur(e, joystickProfondeurHandle, joystickProfondeurContainer, isDraggingProfondeur);
});

// Event listeners for Joystick Direction
joystickDirectionHandle.addEventListener('mousedown', function (e) {
    isDraggingDirection = true;
    joystickDirectionContainer.style.transition = 'none';
    updateJoystickDirection(e, joystickDirectionHandle, joystickDirectionContainer, isDraggingDirection);
});

document.addEventListener('mousemove', function (e) {
    if (isDraggingVitesse) {
        updateJoystickVitesse(e, joystickVitesseHandle, joystickVitesseContainer, isDraggingVitesse);
    }
    if (isDraggingProfondeur) {
        updateJoystickProfondeur(e, joystickProfondeurHandle, joystickProfondeurContainer, isDraggingProfondeur);
    }
    if (isDraggingDirection) {
        updateJoystickDirection(e, joystickDirectionHandle, joystickDirectionContainer, isDraggingDirection);
    }
});

document.addEventListener('mouseup', function () {
    if (isDraggingVitesse) {
        isDraggingVitesse = false;
        joystickVitesseContainer.style.transition = 'all 0.3s ease';
        resetJoystick(joystickVitesseHandle);
        resetVitesseValue();
    }
    if (isDraggingProfondeur) {
        isDraggingProfondeur = false;
        joystickProfondeurContainer.style.transition = 'all 0.3s ease';
        resetJoystick(joystickProfondeurHandle);
        resetProfondeurValue();
    }
    if (isDraggingDirection) {
        isDraggingDirection = false;
        joystickDirectionContainer.style.transition = 'all 0.3s ease';
        resetJoystick(joystickDirectionHandle);
        resetDirectionValue();
    }
});

function updateJoystickVitesse(e) {
    var containerRect = joystickVitesseContainer.getBoundingClientRect();
    var y = e.clientY - containerRect.top - containerRect.height;

    var distance = containerRect.height /2;
    var offsetY = Math.min(Math.max(y, -1.80 * distance), -distance / 4.8);

    joystickVitesseHandle.style.transform = 'translate(-50%, ' + offsetY + 'px)';

    var vitesseValue = calculateJoystickValueVitesse(joystickVitesseContainer, joystickVitesseHandle);
    document.getElementById('vitesse-value').innerText = vitesseValue.toFixed(1); // Ajustez le formatage selon vos besoins
}

function updateJoystickProfondeur(e) {
    var containerRect = joystickProfondeurContainer.getBoundingClientRect();
    var y = e.clientY - containerRect.top - containerRect.height / 2  -25;

    var distance = containerRect.height / 2;
    var offsetY = Math.min(Math.max(y, -distance), distance / 1.68);

    joystickProfondeurHandle.style.transform = 'translate(-50%, ' + offsetY + 'px)';

    var profondeurValue = calculateJoystickValueProfondeur(joystickProfondeurContainer, joystickProfondeurHandle);
    document.getElementById('profondeur-value').innerText = profondeurValue.toFixed(1); // Ajustez le formatage selon vos besoins
}

function updateJoystickDirection(e) {
    var containerRect = joystickDirectionContainer.getBoundingClientRect();
    var x = e.clientX - containerRect.left - containerRect.width /2  - 25;

    var distance = containerRect.width / 2;
    var offsetX = Math.min(Math.max(x, -distance), distance/1.3515);

    joystickDirectionHandle.style.transform = 'translate(' + offsetX + 'px, -50%)';

    var directionValue = calculateJoystickValueDirection(joystickDirectionContainer, joystickDirectionHandle);
    document.getElementById('direction-value').innerText = directionValue.toFixed(1); // Ajustez le formatage selon vos besoins
}

function resetJoystick(handle) {
    handle.style.transform = 'translate(-50%, -50%)';
}



function calculateJoystickValueProfondeur(container, handle) {
    // Calculer la valeur relative de la position de la poignée dans le conteneur
    var containerRect = container.getBoundingClientRect();
    var handleRect = handle.getBoundingClientRect();

    var value;

    value = (handleRect.top - containerRect.top) / containerRect.height;
    value = (0.8-value)*100/0.8 -50;
    return value;
}

function calculateJoystickValueVitesse(container, handle) {
    // Calculer la valeur relative de la position de la poignée dans le conteneur
    var containerRect = container.getBoundingClientRect();
    var handleRect = handle.getBoundingClientRect();

    var value;

    value = (handleRect.top - containerRect.top) / containerRect.height;
    value = (0.8-value)*100/0.8;
    return value;
}

function calculateJoystickValueDirection(container, handle) {
    // Calculer la valeur relative de la position de la poignée dans le conteneur
    var containerRect = container.getBoundingClientRect();
    var handleRect = handle.getBoundingClientRect();

    var value;
    value = (handleRect.left - containerRect.left) / containerRect.width;
    value = value*100/0.87 -50;
    return value;
}


function resetVitesseValue() {
    document.getElementById('vitesse-value').innerText = '0.0'; // Mettez la valeur par défaut selon vos besoins
}

function resetProfondeurValue() {
    document.getElementById('profondeur-value').innerText = '0.0'; // Mettez la valeur par défaut selon vos besoins
}

function resetDirectionValue() {
    document.getElementById('direction-value').innerText = '0.0'; // Mettez la valeur par défaut selon vos besoins
}

</script>
     <img src="stream.mjpg" width="650" height="400" />


</body>
</html>
"""


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
            self.send_header('Location', '/index.html')
            self.end_headers()
        elif self.path == '/index.html':
            content = PAGE.encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', len(content))
            self.end_headers()
            self.wfile.write(content)
        elif self.path == '/stream.mjpg':
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
