import cv2
import socket
import threading
import string

sock = socket.socket(socket.AF_INET , socket.SOCK_DGRAM) 
ip="192.168.7.2"   
port=3000
# answer = b''
answer = 1
# sock.bind((ip,port)) 

recv_shutdown = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
recv_shutdown_port = 4000
recv_shutdown.bind(ip,port)

recv_shutdown.connect(ip,recv_shutdown_port)

def sendAnswer():
     while(True):
        sock.sendto(answer,(ip,port))
        # print("sent")

# replyThread = threading.Thread(target=sendAnswer)

# replyThread.start()

cap = cv2.VideoCapture("udp://192.168.7.1:12345")   
while(True): 

    data = recv_shutdown.recv(5)
    if(data.decode() == "stop"):
        break

    # ret, frame = cap.read() 
    # cv2.imshow('frame', frame) 
    # global answer 
    # answer = b'answer'
    answer = answer + 1
    # answerString = b'answer = {answer}'
    if(answer % 100 == 0):
        answerString = str(answer).encode()
        sock.sendto(answerString,(ip,port))
    # answerBytes = answer.encode('utf-8')
    # sock.sendto(answerBytes,(ip,port))

    # sock = socket.socket(socket.AF_INET , socket.SOCK_DGRAM)
    # sock.sendto(answer,(ip,port))
    # sock.sendto(answerString,(ip,port))
    # sock.close()
      
    # the 'q' button is set as the 
    # quitting button you may use any 
    # desired button of your choice 
    if cv2.waitKey(1) & 0xFF == ord('q'): 
        break
cap.release() 
cv2.destroyAllWindows() 

# replyThread.join()
# sendto

