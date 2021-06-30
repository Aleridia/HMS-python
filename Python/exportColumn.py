import pandas as pd

fic = pd.read_excel("../data/SERA3.xlsx", sheet_name="XSens1200")

temperature = fic[["time_code", "temperature"]]
temperature.to_csv('../data/temperature.tsv', sep=";")

acc_x = fic[["time_code", "acc_x"]]
acc_x.to_csv('../data/acc_x.tsv', sep=";")
