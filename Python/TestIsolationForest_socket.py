# importing libaries ----
import numpy as np
from sklearn.ensemble import IsolationForest
import matplotlib.pyplot as plt
import pandas as pd
import os, sys, socket, time

data = dict()
"""
Fonction d'apprentissage
"""
def learning():
    dfApprentissage = pd.read_csv('../data/Apprentissage.csv')

    object = dict()

    for elem in dfApprentissage.head(0):
        if elem == 'time_code':
            continue

        data[elem] = []

        X_train = pd.DataFrame(np.c_[dfApprentissage[elem]], columns=['x1'])

        clf = IsolationForest(max_samples=20)
        clf.fit(X_train)

        object[elem] = clf

    return object


# --------Detection---------

def detection():
    for cle, valeur in data.items():
        X_test = pd.DataFrame(np.c_[valeur], columns=['x1'])
        # On prédit
        dfDetection['anomaly'] = object[cle].predict(X_test)

        # On récupère les anomalies
        nb_test = 0
        for i in range(len(dfDetection['anomaly'])):
            if dfDetection['anomaly'][i] != 1:
                nb_test += 1

        print("Nombre anomalie dans la base = " + str(nb_test) + "/" + str(len(X_test)))

        # On vide la liste pour la prochaine rotation
        data[cle].clear()


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("127.0.0.1", 2525))
s.listen(5)


def parse(data):
    data = data.split(';')
    return (data[0], data[1])


object = learning()
start = time.time()
end = 0

try:
    while True:
        (client, addr) = s.accept()
        print("someone connected to me")
        chunks = []
        read = 0

        while True:
            chunk = client.recv(128)
            if chunk == b'':
                break

            if (end - start) >= 10:
                detection()
                start = time.time()

            end = time.time()

            messages = chunk.decode('ascii')
            messages = messages.split('|')

            for message in messages[:-1]:
                (value, sensor) = parse(message[:-1])

                data[sensor].append(value)

except Exception as ex:
    print(ex)
finally:
    s.close()