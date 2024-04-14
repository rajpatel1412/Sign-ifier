# import cv2, socket, numpy, pickle

# s=socket.socket(socket.AF_INET , socket.SOCK_DGRAM)  # Gives UDP protocol to follow
# # ip="192.168.7.2"   # Server public IP
# ip="0.0.0.0"
# port=12345             # Server Port Number to identify the process that needs to recieve or send packets
# s.bind((ip,port))     # Bind the IP:port to connect 

# # In order to iterate over block of code as long as test expression is true
# while True:
#     # x=s.recvfrom(100000000)    # Recieve byte code sent by client using recvfrom
#     x=s.recvfrom(65507)
#     print("after recieve")
#     clientip = x[1][0]         # x[1][0] in this client details stored,x[0][0] Client message Stored
#     data=x[0]                  # Data sent by client
#     data=pickle.loads(data)    # All byte code is converted to Numpy Code 
#     data = cv2.imdecode(data, cv2.IMREAD_COLOR)  # Decode 
#     cv2.imshow('my pic', data) # Show Video/Stream
#     if cv2.waitKey(10) == 13:  # Press Enter then window will close
#         break
# cv2.destroyAllWindows()        # Close all windows

import cv2
import socket

sock = socket.socket(socket.AF_INET , socket.SOCK_DGRAM)  # Gives UDP protocol to follow
ip="192.168.7.2"   # Server public IP
# ip="0.0.0.0"
port=3000             # Server Port Number to identify the process that needs to recieve or send packets
# sock.bind((ip,port)) 

cap = cv2.VideoCapture("udp://192.168.7.1:12345")   
while(True): 
    ret, frame = cap.read() 
    cv2.imshow('frame', frame) 
    answer = 'answer'
    answerBytes = answer.encode('utf-8')
    sock.sendto(answerBytes,(ip,port))
      
    # the 'q' button is set as the 
    # quitting button you may use any 
    # desired button of your choice 
    if cv2.waitKey(1) & 0xFF == ord('q'): 
        break
cap.release() 
cv2.destroyAllWindows() 


