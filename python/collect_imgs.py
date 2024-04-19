# Thank you to Computer Vision Engineer on YouTube for the code:
# https://www.youtube.com/watch?v=MJCSjXepaAM&t=2991s
# **This file is not needed to run our final project. This is just shown as
# a preliminary for the model.

# Program used for capturing training images for our model manually.
# Not that this code will NOT run on host computer most likely, due
# to the VideoCapture device number mismatch.

import os

import cv2


DATA_DIR = './data'
if not os.path.exists(DATA_DIR):
    os.makedirs(DATA_DIR)


number_of_classes = 30
dataset_size = 100

labels_dict = {
    0: '3', 1: '4',
    2: 'A', 3: 'B', 4: 'C', 5: 'D', 6: 'E', 7: 'F', 8: 'G', 9: 'H', 
    10: 'I', 11: 'K', 12: 'L', 13: 'M', 14: 'N', 15: 'O', 16: 'P', 
    17: 'Q', 18: 'R', 19: 'S', 20: 'T', 21: 'U', 22: 'V', 23: 'W', 
    24: 'X', 25: 'Y', 
    26: 'ME', 27: 'WELCOME', 28: 'OUR', 
    29: 'DEMONSTRATION'
}

cap = cv2.VideoCapture(1)
for j in range(number_of_classes):
    if not os.path.exists(os.path.join(DATA_DIR, str(j))):
        os.makedirs(os.path.join(DATA_DIR, str(j)))

    print('Collecting data for class {}'.format(labels_dict[j]))

    done = False
    while True:
        ret, frame = cap.read()
        cv2.putText(frame, 'Ready? Press "Q" and sign {}! :)'.format(labels_dict[j]), (100, 50), cv2.FONT_HERSHEY_SIMPLEX, 1.3, (0, 255, 0), 3,
                    cv2.LINE_AA)
        cv2.imshow('frame', frame)
        if cv2.waitKey(25) == ord('q'):
            break

    counter = 0
    while counter < dataset_size:
        ret, frame = cap.read()
        cv2.imshow('frame', frame)
        cv2.waitKey(25)
        cv2.imwrite(os.path.join(DATA_DIR, str(j), '{}.jpg'.format(counter)), frame)

        counter += 1

cap.release()
cv2.destroyAllWindows()