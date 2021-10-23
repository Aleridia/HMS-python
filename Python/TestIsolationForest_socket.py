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

        clf = IsolationForest(max_samples=1)
        clf.fit(X_train.values)

        object[elem] = clf

    return object


# --------Detection---------

def detection(object):
    anomaly = dict()
    for cle, valeur in data.items():
        if len(valeur) == 0:
            continue

        X_test = pd.DataFrame(np.c_[valeur], columns=['x1'])
        # On prédit
        anomaly['anomaly'] = object[cle].predict(X_test.values)
        # On récupère les anomalies
        nb_test = 0
        for i in range(len(anomaly['anomaly'])):
            if anomaly['anomaly'][i] != 1:
                nb_test += 1
        print("Nombre anomalie dans la base = " + str(nb_test) + "/" + str(len(X_test)))

        # On vide la liste pour la prochaine rotation
        data[cle].clear()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("127.0.0.1", 2525))
s.listen(5)


def parse(data):
    data = data.split(';')

    if len(data) < 2:
        return (None, None)

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
            chunk = client.recv(256)
            if chunk == b'':
                break
            if (end - start) >= 2:
                detection(object)
                start = time.time()

            end = time.time()
            messages = chunk.decode('ascii')
            messages = messages.split('|')

            nbNewElement = 0
            for message in messages[:-1]:
                (value, sensor) = parse(message)
                if value is None:
                    continue

                nbNewElement += 1
                data[sensor].append(value)

            print("Réception de " + str(nbNewElement) + " nouveaux éléments")
            time.sleep(5)

except Exception as ex:
    print("Exception:" + str(ex))
finally:
    s.close()