from influxdb import DataFrameClient
from influxdb_client import InfluxDBClient
from datetime import datetime
import pandas as pd
from Influx import *
from BasicEvals import *
from DecisionTree import best_clf

db_name = "telegraf"
measurement = "win_cpu"
field = "Percent_Idle_Time"
field2 = "Percent_Processor_Time"
client = DataFrameClient(database=db_name, host="192.168.178.20")



while(True):
    #evaluate_mean(client, 100, measurement, field, False)
    #evaluate_max(client, 15, measurement, field2)
    #print(get_mean_query(client, measurement, field))
    make_prediction(get_sample(client_teo))
    #get_sample(client_teo)
    time.sleep(3)#


#print(best_clf.predict_proba([[36, 90]]))



