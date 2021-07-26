# importing libaries ----
import numpy as np
from sklearn.ensemble import IsolationForest
import matplotlib.pyplot as plt
import pandas as pd
import os, sys, socket, time

"""
Fonction d'apprentissage
"""
def learning():
    dfApprentissage = pd.read_csv('../data/Apprentissage.csv')

    object = dict()

    for elem in dfApprentissage.head(0):
        if elem == 'time_code':
            continue

        X_train = pd.DataFrame(np.c_[dfApprentissage[elem]], columns=['x1'])

        clf = IsolationForest(max_samples=20)
        clf.fit(X_train)

        object[elem] = clf

    return object


# --------Detection---------

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("127.0.0.1", 2525))
s.listen(5)


def parse(data):
    data = data.split(';')
    return (data[0], data[1])


object = learning()

while 1:
    dirs = os.listdir('../data/temperature')

    for file in dirs:
        name = file.split('_')[0]
        dfDetection = pd.read_csv('../data/temperature/' + file)

        # Donnée de détection
        X_test = pd.DataFrame(np.c_[dfDetection[name]], columns=['x1'])
        # On prédit
        dfDetection['anomaly'] = object[name].predict(X_test)

        # On récupère les anomalies
        nb_test = 0
        for i in range(len(dfDetection['anomaly'])):
            if dfDetection['anomaly'][i] != 1:
                nb_test += 1
        
        print("Nombre anomalie dans la base de test = " + str(nb_test) + "/" + str(len(X_test)))
    
    time.sleep(10)
