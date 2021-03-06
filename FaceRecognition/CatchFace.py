import cv2
import sys
from time import sleep
import serial

cascPath = "haarcascade_frontalface_default.xml"
faceCascade = cv2.CascadeClassifier(cascPath)

video_capture = cv2.VideoCapture(1)
anterior = 0
ser = serial.Serial("COM5", 9600, timeout=2) # Establish the connection on a specific port

dataCount, mx, my = 0, 0, 0
dalay = 10

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
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
        mx = x+w/2-320
        my = -(y+h/2-240)
        print(mx, my)
        if mx > 60:
            ser.write(bytes('l', encoding = 'utf8'))
        if mx < -60:
            ser.write(bytes('r', encoding = 'utf8'))
        # if my > 30:
        #     ser.write(bytes('d', encoding = 'utf8'))
        # if my < -30:
        #     ser.write(bytes('u', encoding = 'utf8'))

        # if dataCount == dalay:
        #     ser.flushOutput()
        #     mx /= dalay
        #     my /= dalay
        #     dataCount = 0
        #     print(mx, my)
            
        # else:
        #     mx += x+w/2-320
        #     my += -(y+h/2-240)
        #     dataCount += 1
    # Display the resulting frame
    cv2.imshow('Video', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


# When everything is done, release the capture
video_capture.release()
ser.close()
cv2.destroyAllWindows()
