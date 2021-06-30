import pandas as pd
import plotly.express as px
import os

pd.set_option('display.max_rows', 500)
pd.set_option('display.max_columns', 500)
pd.set_option('display.width', 1000)

df = pd.read_csv('../data/XSens1200.csv')

if not os.path.exists("../plot"):
    os.mkdir("../plot")

fig = px.line(df, y = 'temperature', x = 'time_code', title = 'temperature')
fig.write_html("../plot/temperature.html")

fig = px.line(df, y = 'acc_x', x = 'time_code', title = 'acc_x')
fig.write_html("../plot/acc_x.html")

fig = px.line(df, y = 'acc_y', x = 'time_code', title = 'acc_y')
fig.write_html("../plot/acc_y.html")

fig = px.line(df, y = 'acc_z', x = 'time_code', title = 'acc_z')
fig.write_html("../plot/acc_z.html")

fig = px.line(df, y = 'gyro_x', x = 'time_code', title = 'gyro_x')
fig.write_html("../plot/gyro_x.html")

fig = px.line(df, y = 'gyro_y', x = 'time_code', title = 'gyro_y')
fig.write_html("../plot/gyro_y.html")

fig = px.line(df, y = 'gyro_z', x = 'time_code', title = 'gyro_z')
fig.write_html("../plot/gyro_z.html")

fig = px.line(df, y = 'mag_y', x = 'time_code', title = 'mag_y')
fig.write_html("../plot/mag_y.html")

fig = px.line(df, y = 'mag_z', x = 'time_code', title = 'mag_z')
fig.write_html("../plot/mag_z.html")

fig = px.line(df, y = 'mag_x', x = 'time_code', title = 'mag_x')
fig.write_html("../plot/mag_x.html")

fig = px.line(df, y = 'roulis', x = 'time_code', title = 'roulis')
fig.write_html("../plot/roulis.html")

fig = px.line(df, y = 'tangage', x = 'time_code', title = 'tangage')
fig.write_html("../plot/tangage.html")

fig = px.line(df, y = 'lacet', x = 'time_code', title = 'lacet')
fig.write_html("../plot/lacet.html")