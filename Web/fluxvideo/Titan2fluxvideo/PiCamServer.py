from flask import Flask, Response
import cv2

app = Flask(__name__)
camera = cv2.VideoCapture(0)
camera.set(3, 640)  # Set width
camera.set(4, 480)  # Set height


def generate_frames():
    while True:
        success, frame = camera.read()
        if not success:
            print("Échec de la capture d'image depuis la caméra")
            print("success = ",success)
            print("frame = ",frame)
            break
        else:
            ret, buffer = cv2.imencode('.jpg', frame)
            frame = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

            
@app.route('/')
def index():
    return "Flux vidéo depuis une caméra Raspberry Pi"

@app.route('/video_feed')
def video_feed():
    print("test")
   ## return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')  

if __name__ == '__main__':
    app.run(host='0.0.0.0', port = 5000, debug=True)