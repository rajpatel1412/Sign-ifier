import pickle

import cv2
import mediapipe as mp
import numpy as np

import sys
import socket

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

num_symbols = 30

labels_dict = {
    0: '3', 1: '4',
    2: 'A', 3: 'B', 4: 'C', 5: 'D', 6: 'E', 7: 'F', 8: 'G', 9: 'H', 
    10: 'I', 11: 'K', 12: 'L', 13: 'M', 14: 'N', 15: 'O', 16: 'P', 
    17: 'Q', 18: 'R', 19: 'S', 20: 'T', 21: 'U', 22: 'V', 23: 'W', 
    24: 'X', 25: 'Y', 
    26: 'ME', 27: 'WELCOME', 28: 'OUR', 
    29: 'DEMONSTRATION'
}

import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 12345


sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

predictions = np.zeros(num_symbols)
# while True:

# TODO: implement something that reads videos as if they were in a queue
# within a directory. There should always be a short clip to process
# but obviously the program should still just idle if not.
# If a sign is recognized in the video then process as normal, otherwise just idle.

while True:

    # cap = cv2.VideoCapture("./test_A3.mov")
    # while not cap.isOpened():
    #     cap = cv2.VideoCapture("./test_A3.mov")
    #     cv2.waitKey(1000)
    #     print("Wait for the header")

    cap = cv2.VideoCapture(1)
    flag, frame = cap.read()

    # while flag:
    for h in range(10):
        # print("new iteration")
        data_aux = []
        x_ = []
        y_ = []

    # skip a number of frames for efficiency
        for i in range(7):
            flag, frame = cap.read()

        # if not flag:
        #     cv2.waitKey(300)
        #     continue

        # sys.stdout.flush()

        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        results = hands.process(frame_rgb)
        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    frame,  # image to draw
                    hand_landmarks,  # model output
                    mp_hands.HAND_CONNECTIONS,  # hand connections
                    mp_drawing_styles.get_default_hand_landmarks_style(),
                    mp_drawing_styles.get_default_hand_connections_style())

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

        # cv2.imshow('frame',frame)
        cv2.waitKey(10)

    index_max = np.argmax(predictions)
    sys.stdout.flush()
    # print(index_max)

    # send prediction to website
    message = labels_dict[index_max]
    print(message)

    sock.sendto(message.encode(), (UDP_IP,UDP_PORT))

    predictions = np.zeros(num_symbols)

    # # just for testing
    # print("waiting...")
    # cv2.waitKey(2000)
    # print("Done.")
# print("WE'RE DONE\N")
cap.release()
cv2.destroyAllWindows()
