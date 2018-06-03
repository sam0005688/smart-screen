import cv2
import sys
from time import sleep
import serial

cascPath = "haarcascade_frontalface_default.xml"
faceCascade = cv2.CascadeClassifier(cascPath)

video_capture = cv2.VideoCapture(0)
anterior = 0
ser = serial.Serial("COM3", 9600, timeout=2) # Establish the connection on a specific port

dataCount, data = 0, 0

while True:
    if not video_capture.isOpened():
        print('Unable to load camera.')
        sleep(5)
        pass

    # Capture frame-by-frame
    ret, frame = video_capture.read()

    frame = cv2.flip(frame, 1)

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(30, 30)
    )

    # Draw a rectangle around the faces
    if len(faces) == 1:
        x, y, w, h = faces[0][0], faces[0][1], faces[0][2], faces[0][3]
        # print(x+w/2-320,y+h/2-240)
        if dataCount == 30:
            ser.write(data/30)
            print(data/30)
            dataCount = 0
            data = 0
        else:
            data += x+w/2-320
            dataCount += 1
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    # Display the resulting frame
    cv2.imshow('Video', frame)


    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    # Display the resulting frame
    cv2.imshow('Video', frame)

# When everything is done, release the capture
video_capture.release()
ser.close()
cv2.destroyAllWindows()
