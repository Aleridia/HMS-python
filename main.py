import pandas as pd
import scipy as sc
from sklearn.tree import DecisionTreeRegressor
from openpyxl import Workbook

pd.set_option('display.max_rows', 500)
pd.set_option('display.max_columns', 500)
pd.set_option('display.width', 1000)

# save filepath to variable for easier access
#dfs = pd.read_excel('SERA3.xlsx', engine='openpyxl', sheet_name='XSens1200')
#dfs.to_csv('test.csv', index= False)


data = pd.read_csv('XSens1200.csv')

# print a summary of the data in Melbourne data
print(data.describe())

# column type
print(data.columns)

y = data.acc_x

# selecting data
selected = ['acc_x', 'acc_y', 'acc_z', 'temperature']
X = data[selected]

print(X.describe())

model = DecisionTreeRegressor(random_state=1)

#Fit model
print(model.fit(X, y))
print(X.head())

print("Prediction :")
print(model.predict(X.head()))


