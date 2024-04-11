import pickle

import cv2
import mediapipe as mp
import numpy as np

import sys

model_dict = pickle.load(open('./model.p', 'rb'))
model = model_dict['model']

mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles

hands = mp_hands.Hands(static_image_mode=True, min_detection_confidence=0.3)

labels_dict = {0: 'A', 1: 'B', 2: 'L'}

while True:

# TODO: implement something that reads videos as if they were in a queue
# within a directory. There should always be a short clip to process
# but obviously the program should still just idle if not.
# If a sign is recognized in the video then process as normal, otherwise just idle.

    cap = cv2.VideoCapture("./test_A.mov")
    while not cap.isOpened():
        cap = cv2.VideoCapture("./test_A.mov")
        cv2.waitKey(1000)
        print("Wait for the header")

    flag, frame = cap.read()

    while flag:

        data_aux = []
        x_ = []
        y_ = []

    # skip a number of frames for efficiency
        for i in range(7):
            flag, frame = cap.read()

        if not flag:
            cv2.waitKey(300)
            continue

        sys.stdout.flush()

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

            prediction = model.predict([np.asarray(data_aux)])

            predicted_character = labels_dict[int(prediction[0])]

        cv2.waitKey(10)


    sys.stdout.flush()
    print(predicted_character)

    # just for testing
    print("waiting...")
    cv2.waitKey(2000)
    print("Done.")
# print("WE'RE DONE\N")
cap.release()
cv2.destroyAllWindows()
