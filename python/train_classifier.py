import pickle

from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
import numpy as np


data_dict = pickle.load(open('./data.pickle', 'rb'))

# for i in range(3245):
#     print(len(data_dict['data'][i]))

# data = np.asarray(data_dict['data'])
# labels = np.asarray(data_dict['labels'])

data = data_dict['data']
labels = data_dict['labels']

length = len(data)

for i in range(length-1,0,-1):
    if len(data[i]) != 42:
        data.pop(i)
        labels.pop(i)

labels = np.asarray(labels)
data = np.asarray(data)


x_train, x_test, y_train, y_test = train_test_split(data, labels, test_size=0.2, shuffle=True, stratify=labels)

model = RandomForestClassifier(max_depth=15)

model.fit(x_train, y_train)

y_predict = model.predict(x_test)

score = accuracy_score(y_predict, y_test)

print('{}% of samples were classified correctly !'.format(score * 100))

f = open('model.p', 'wb')
pickle.dump({'model': model}, f)
f.close()
