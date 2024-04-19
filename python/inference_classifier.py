import pickle

import cv2
import mediapipe as mp
import numpy as np

import sys
import socket
import time

# target_ip = '127.0.0.1'
# target_ip = '192.168.1.111'
# target_port = 42069

# udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

model_dict = pickle.load(open('./model.p', 'rb'))
model = model_dict['model']

mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles

hands = mp_hands.Hands(static_image_mode=True, min_detection_confidence=0.3)

num_symbols = 31

labels_dict = {
    0: '.', 1: '3', 2: '4',
    3: 'A', 4: 'B', 5: 'C', 6: 'D', 7: 'E', 8: 'F', 9: 'G', 10: 'H', 
    11: 'I', 12: 'K', 13: 'L', 14: 'M', 15: 'N', 16: 'O', 17: 'P', 
    18: 'Q', 19: 'R', 20: 'S', 21: 'T', 22: 'U', 23: 'V', 24: 'W', 
    25: 'X', 26: 'Y', 
    27: 'ME', 28: 'WELCOME', 29: 'OUR', 
    # 30: 'DEMONSTRATION'
    30: 'DEMO'
}

import socket

# UDP_IP = "192.168.7.1"
# UDP_PORT = 12345


# sock = socket.socket(socket.AF_INET, # Internet
#                      socket.SOCK_DGRAM) # UDP

predictions = np.zeros(num_symbols)
# while True:

# TODO: implement something that reads videos as if they were in a queue
# within a directory. There should always be a short clip to process
# but obviously the program should still just idle if not.
# If a sign is recognized in the video then process as normal, otherwise just idle.

sock = socket.socket(socket.AF_INET , socket.SOCK_DGRAM) 
ip="192.168.7.2"   
port=3000
answer = 1

num_inferences = 20

cap = cv2.VideoCapture("udp://192.168.7.1:12345?overrun_nonfatal=1&fifo_size=50000000")   

flag,frame = cap.read()

while flag:

    # cap = cv2.VideoCapture("./test_A3.mov")
    # while not cap.isOpened():
    # print("Wait for the header")
    #     cap = cv2.VideoCapture("./test_A3.mov")
    #     cv2.waitKey(1000)

    # cap = cv2.VideoCapture(1)



    # while flag:
    # for h in range(1):
        # flag, frame = cap.read()
        # print("new iteration")

    start = time.time()
    answer = answer + 1
    if not flag:
        break

# run a number of inferences
    for i in range(num_inferences):
        flag, frame = cap.read()
        data_aux = []
        x_ = []
        y_ = []


        # sys.stdout.flush()

        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        results = hands.process(frame_rgb)
        if results.multi_hand_landmarks:
            # for hand_landmarks in results.multi_hand_landmarks:
            #     mp_drawing.draw_landmarks(
            #         frame,  # image to draw
            #         hand_landmarks,  # model output
            #         mp_hands.HAND_CONNECTIONS,  # hand connections
            #         mp_drawing_styles.get_default_hand_landmarks_style(),
            #         mp_drawing_styles.get_default_hand_connections_style())

            for hand_landmarks in results.multi_hand_landmarks:
                for i in range(len(hand_landmarks.landmark)):
                    x = hand_landmarks.landmark[i].x
                    y = hand_landmarks.landmark[i].y

                    x_.append(x)
                    y_.append(y)

                for i in range(len(hand_landmarks.landmark)):
                    x = hand_landmarks.landmark[i].x
                    y = hand_landmarks.landmark[i].y
                    data_aux.append(x - min(x_))
                    data_aux.append(y - min(y_))

            if len(data_aux) > 42:
                continue
            prediction = model.predict([np.asarray(data_aux)])

            if int(prediction[0]) > num_symbols - 1:
                continue

            predicted_character = labels_dict[int(prediction[0])]
            predictions[int(prediction[0])] += 1



    index_max = np.argmax(predictions)
    # sys.stdout.flush()
    # print(index_max)

    # send prediction to website
    # message = "yo"
    message = labels_dict[index_max]
    # print(message)
    # if answer % 100 == 0:
    if(message != '.'):
        for i in range(len(message)):
            
            sock.sendto(str(message[i]).encode(), (ip,port))
        # message = ''
        print(str(answer) + ' ' + message)
    end = time.time()

    print(str(end - start))
    # cv2.imshow('frame',frame)
    # cv2.waitKey(1)

    predictions = np.zeros(num_symbols)

    # # just for testing
    # print("waiting...")
    # cv2.waitKey(2000)
    # print("Done.")
# print("WE'RE DONE\N")
cap.release()
cv2.destroyAllWindows()
