import cv2 
from cv2.aruco import aruco

VideoCap=False
cap=cv2.VideoCapture(0)

while True:
    _,img=cap.read()
    if cv2.waitKey(1)==113:
        break
    cv2.imshow("img",img)