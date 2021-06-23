from sklearn.datasets import make_blobs
import pandas as pd
from sklearn.cluster import DBSCAN
import matplotlib.pyplot as plt
import numpy as np

n_samples = 400
random_state = 170
X, y = make_blobs(n_samples=n_samples, random_state=random_state, cluster_std = 0.4)
X = pd.DataFrame(X, columns = ['abscisse','ordonnée'])

# ----------------------------------------------

dfApprentissage = pd.read_csv('Apprentissage.csv')
dfDetection = pd.read_csv('Detection.csv')

# Donnée d'apprentissage
X_train = pd.DataFrame(np.c_[dfApprentissage['temperature'], dfApprentissage['temperature']], columns=['x1', 'x2'])
# Donnée de détection
X_test = pd.DataFrame(np.c_[dfDetection['temperature'], dfDetection['temperature']], columns=['x1', 'x2'])

print(X_test)

y_pred = DBSCAN(eps=0.2, min_samples=5).fit_predict(X_test)
plt.scatter(X_test['x1'], X_test['x2'], c=y_pred)
plt.show()

# ----------------------------------------------

# ----------------------------------------------