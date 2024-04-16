import cv2
import socket
import threading
import string

sock = socket.socket(socket.AF_INET , socket.SOCK_DGRAM) 
ip="192.168.7.2"   
port=3000
answer = 1


cap = cv2.VideoCapture("udp://192.168.7.1:12345")  

ret,frame = cap.read()

while ret: 
    if not ret:
        print("not ret")
        continue
    cv2.imshow('frame', frame) 
    answer = answer + 1

    if(answer % 100 == 0):
        answerString = str(answer).encode()
        sock.sendto(answerString,(ip,port))

    # the 'q' button is set as the 
    # quitting button you may use any 
    # desired button of your choice 
    if cv2.waitKey(1) & 0xFF == ord('q'): 
        break
    
    ret, frame = cap.read()
    
cap.release() 
cv2.destroyAllWindows() 

# replyThread.join()


