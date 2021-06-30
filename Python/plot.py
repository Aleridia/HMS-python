import pandas as pd
import plotly.express as px
import os

pd.set_option('display.max_rows', 500)
pd.set_option('display.max_columns', 500)
pd.set_option('display.width', 1000)

df = pd.read_csv('XSens1200.csv')

if not os.path.exists("plot"):
    os.mkdir("plot")

fig = px.line(df, y = 'acc_x', x = 'time_code', title = 'test')
fig.write_html("images/acc_x.html")

fig = px.line(df, y = 'acc_y', x = 'time_code', title = 'test')
fig.write_html("images/acc_y.html")

fig = px.line(df, y = 'acc_z', x = 'time_code', title = 'test')
fig.write_html("images/acc_z.html")

fig = px.line(df, y = 'gyro_x', x = 'time_code', title = 'test')
fig.write_html("images/gyro_x.html")

fig = px.line(df, y = 'gyro_y', x = 'time_code', title = 'test')
fig.write_html("images/gyro_y.html")

fig = px.line(df, y = 'gyro_z', x = 'time_code', title = 'test')
fig.write_html("images/gyro_z.html")

fig = px.line(df, y = 'mag_y', x = 'time_code', title = 'test')
fig.write_html("images/mag_y.html")

fig = px.line(df, y = 'mag_z', x = 'time_code', title = 'test')
fig.write_html("images/mag_z.html")

fig = px.line(df, y = 'mag_x', x = 'time_code', title = 'test')
fig.write_html("images/mag_x.html")

fig = px.line(df, y = 'roulis', x = 'time_code', title = 'test')
fig.write_html("images/roulis.html")

fig = px.line(df, y = 'tangage', x = 'time_code', title = 'test')
fig.write_html("images/tangage.html")

fig = px.line(df, y = 'lacet', x = 'time_code', title = 'test')
fig.write_html("images/lacet.html")