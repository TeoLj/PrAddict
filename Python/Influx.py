from influxdb import DataFrameClient
import influxdb_client
from influxdb_client import InfluxDBClient
import pandas as pd
import numpy as np
import time
from influxdb_client.client.write_api import SYNCHRONOUS
from DecisionTree import best_clf

# Teo Datenbank
my_token = "Vy9BX-Kn4s2I_L7vq7ITYgChqxgJqUM0vNtlf3tEDVOA7P52b9f_VKv_VYIs1CHoOyHn-S8POOj0tvBlzA3YzQ=="
my_org = "HackOff"
my_url = "http://192.168.178.83:8086"
bucket = "arduino_data"

query_temp = '''
from(bucket: "arduino_data")
|> range(start:-5m, stop: now())
|> filter(fn: (r) => r._measurement == "Temp")'''
query_heart = '''
from(bucket: "arduino_data")
|> range(start:-5m, stop: now())
|> filter(fn: (r) => r._measurement == "Heart")'''
query_humid = '''
from(bucket: "arduino_data")
|> range(start:-5m, stop: now())
|> filter(fn: (r) => r._measurement == "Hum")'''
client_teo = InfluxDBClient(url="http://192.168.178.83:8086", token=my_token, org=my_org, debug=False)


def get_sample(client):
    temp_data = client.query_api().query_data_frame(org=my_org, query=query_temp)
    heart_data = client.query_api().query_data_frame(org=my_org, query=query_heart)
    hum_data = client.query_api().query_data_frame(org=my_org, query=query_humid)
    return [[np.array(temp_data["_value"])[-1], np.array(heart_data["_value"])[-1], np.array(hum_data["_value"])[-1]]]


def write_to_teo(value):
    write_api = client_teo.write_api(write_options=SYNCHRONOUS)

    p = influxdb_client.Point("PredTest").tag("Tag", "Test").field("value", value)
    write_api.write(bucket=bucket, org=my_org, record=p)


def make_prediction(sample):
    pred = best_clf.predict(sample)
    print(best_clf.predict_proba(sample))
    if pred == [1.]:
        print("WARNING! Relapse! Current temp: %f - Current pulse: %f - Current Humditiy: %f" %(sample[0][0], sample[0][1], sample[0][2]))
        print("----------------------------------------------------------")
        write_to_teo(1)
    else:
        print("All good! Current temp: %f - Current pulse: %f - Current Humidity: %f" %(sample[0][0], sample[0][1], sample[0][1]))
        print("----------------------------------------------------------")
        write_to_teo(0)
        return False


def get_mean_query(client, measurement, field):
    # TODO enhance to handle several measurements in list
    # TODO No hardcoding, add fields
    query = client.query("SELECT mean(" + field + ") AS mean_" + field + " FROM " + measurement + " WHERE time >= now() - 5m GROUP BY time(30s)")[measurement]
    return pd.DataFrame(query).iloc[-1:]


def get_max_query(client, measurement, field):
    # TODO enhance to handle several measurements in list
    # TODO No hardcoding, add fields
    query = client.query("SELECT max(" + field + ") AS max_" + field + " FROM " + measurement + " WHERE time >= now() - 5m GROUP BY time(30s)")[measurement]
    return pd.DataFrame(query).iloc[-1:]


def get_min_query(client, measurement, field):
    # TODO enhance to handle several measurements in list
    # TODO No hardcoding, add fields
    query = client.query("SELECT min(" + field + ") AS min_" + field + " FROM " + measurement + " WHERE time >= now() - 5m GROUP BY time(30s)")[measurement]
    return pd.DataFrame(query).iloc[-1:]










