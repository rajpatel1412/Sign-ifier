# A program which uses the model created from the previous train_classifier.py file
# to make inferences on which ASL symbol is held up, if any, on the webcam video.
# This code has been adapted from Computer Vision Engineer's video
# to work with video over a UDP connection, as well as made more robust/reliable
# for our uses by requiring an argmax of a number of inferences before an output is given.
# https://www.youtube.com/watch?v=MJCSjXepaAM&t=2991s

import pickle

import cv2
import mediapipe as mp
import numpy as np

import socket
import time


model_dict = pickle.load(open('./model.p', 'rb'))
model = model_dict['model']

mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles

hands = mp_hands.Hands(static_image_mode=True, min_detection_confidence=0.3)

num_symbols = 31
# '.' here is not a predicted symbol. It is a 'default' for when no inference has been made
labels_dict = {
    0: '.', 1: '3', 2: '4',
    3: 'A', 4: 'B', 5: 'C', 6: 'D', 7: 'E', 8: 'F', 9: 'G', 10: 'H', 
    11: 'I', 12: 'K', 13: 'L', 14: 'M', 15: 'N', 16: 'O', 17: 'P', 
    18: 'Q', 19: 'R', 20: 'S', 21: 'T', 22: 'U', 23: 'V', 24: 'W', 
    25: 'X', 26: 'Y', 
    27: 'ME', 28: 'WELCOME', 29: 'OUR', 
    30: 'DEMO'
}

# initialize array for prediction confidences
predictions = np.zeros(num_symbols)

# establish connection for sending information to C
sock = socket.socket(socket.AF_INET , socket.SOCK_DGRAM) 
ip="192.168.7.2"   
port=3000
answer = 1

# number of inferences to make before sending a result back.
# num_inferences set to 20 means that one letter will be
# sent every 5 seconds.
num_inferences = 20

cap = cv2.VideoCapture("udp://192.168.7.1:12345?overrun_nonfatal=1&fifo_size=50000000")   

flag,frame = cap.read()

while flag:
# time the program, just for console output
    start = time.time()
    answer = answer + 1
    # if cap.read() can't find a frame, stop inferring. That means
    # that the program is done.
    if not flag:
        break

# run a number of inferences
    for i in range(num_inferences):
        flag, frame = cap.read()
        data_aux = []
        x_ = []
        y_ = []
        # convert BGR footage to RGB for Mediapipe
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        results = hands.process(frame_rgb)
        if results.multi_hand_landmarks:
            # (Optional) draw hand landmarks for a video output in VLC (must uncomment imshow below)
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

            # if prediction value is somehow larger than the max
            if int(prediction[0]) > num_symbols - 1:
                continue

            # add predicted character for this round
            predicted_character = labels_dict[int(prediction[0])]
            predictions[int(prediction[0])] += 1



    index_max = np.argmax(predictions)

    # send prediction to website
    message = labels_dict[index_max]
    # if message is a recognized sign, send it via UDP
    if(message != '.'):
        # We send character by character for the case of words.
        # This may not be ideal, but is an easy way to handle sending words.
        for i in range(len(message)):            
            sock.sendto(str(message[i]).encode(), (ip,port))

        print(str(answer) + ' ' + message)
    end = time.time()

    print(str(end - start))

    predictions = np.zeros(num_symbols)

    # # just for testing
    # print("waiting...")
    # cv2.waitKey(2000)
    # print("Done.")

cap.release()
cv2.destroyAllWindows()
