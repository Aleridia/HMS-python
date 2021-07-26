# importing libaries ----
import numpy as np
from sklearn.ensemble import IsolationForest
import matplotlib.pyplot as plt
import pandas as pd
import time
import os, sys

# ----------------------------------------------------
# Isolation Forest ----
dfApprentissage = pd.read_csv('./data/Apprentissage.csv')

# Donnée d'apprentissage
X_train = pd.DataFrame(np.c_[dfApprentissage['temperature']], columns=['x1'])

# training the model
clf = IsolationForest(max_samples=20)

clf.fit(X_train)

#--------Detection---------

while 1:
    dirs = os.listdir('./data/Detection')

    for file in dirs:
        name = file.split('_')[0]
        dfDetection = pd.read_csv('./data/Detection/' + file)

        # Donnée de détection
        X_test = pd.DataFrame(np.c_[dfDetection[name]], columns=['x1'])
        # On prédit
        dfDetection['anomaly'] = clf.predict(X_test)

        # On récupère les anomalies
        nb_test = 0
        for i in range(len(dfDetection['anomaly'])):
            if dfDetection['anomaly'][i] != 1:
                nb_test += 1
        
        print("Nombre anomalie dans la base de test = " + str(nb_test) + "/" + str(len(X_test)))
    
    time.sleep(10)
