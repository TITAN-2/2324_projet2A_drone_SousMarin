import cv2

camera = cv2.VideoCapture(0)

if not camera.isOpened():
    print("c'est nul")
else:
    print("c'est bien")

camera.release