# importing libaries ----
import numpy as np
from sklearn.ensemble import IsolationForest
import matplotlib.pyplot as plt
import pandas as pd

# ----------------------------------------------------
# Isolation Forest ----
dfApprentissage = pd.read_csv('../data/Apprentissage.csv')
dfDetection = pd.read_csv('../data/Detection.csv')

# Donnée d'apprentissage
X_train = pd.DataFrame(np.c_[dfApprentissage['temperature']], columns=['x1'])
# Donnée de détection
X_test = pd.DataFrame(np.c_[dfDetection['temperature']], columns=['x1'])

# training the model
clf = IsolationForest(max_samples=20, max_features=1.0)

clf.fit(X_train.values)
# On prédit
dfDetection['anomaly'] = clf.predict(X_test.values)


# On récupère les anomalies
nb_test = 0
for i in range(len(dfDetection['anomaly'])):
    if dfDetection['anomaly'][i] != 1:
        nb_test += 1

print("Nombre anomalie dans la base de test = " + str(nb_test) + "/" + str(len(X_test)))
# ----------------------------------------------------

# xx, yy = np.meshgrid(np.linspace(20, 30, 50), np.linspace(20, 30, 50))
# Z = clf.decision_function(np.c_[xx.ravel(), yy.ravel()])
# Z = Z.reshape(xx.shape)
#
# plt.title("IsolationForest")
# plt.contourf(xx, yy, Z, cmap=plt.cm.Blues_r)
# b1 = plt.scatter(X_train.x1, X_train.x2, c='red',
#                  s=20, edgecolor='k')
# b2 = plt.scatter(X_test.x1, X_test.x2, c='green',
#                  s=20, edgecolor='k')
# # c = plt.scatter(X_outliers[:, 0], X_outliers[:, 1], c='red',
# #                 s=20, edgecolor='k')
# plt.axis('tight')
# plt.xlim((26, 30))
# plt.ylim((26, 30))
# plt.legend([b1, b2],
#            ["Apprentissage",
#             "Détection"],
#            loc="upper left")
# plt.show()