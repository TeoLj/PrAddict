from Influx import*
import time


def evaluate_mean(client, threshold, measurement, field, upper):
    df = get_mean_query(client, measurement, field)
    for entry in df["mean_" + field]:
        if upper:
            if entry > threshold:
                print("%f above %d! Upper threshold broken in %s!!!"%(entry, threshold,field))
                print("--------------------------------------------------")
        else:
            if entry < threshold:
                print("%f below %d! Lower threshold broken in %s!!!"%(entry, threshold, field))
                print("--------------------------------------------------")
        time.sleep(3)


def evaluate_max(client, threshold, measurement, field):
    df = get_max_query(client, measurement, field)
    for entry in df["max_" + field]:
        if entry > threshold:
            print("%f above %d! Max threshold broken in %s!!!"%(entry, threshold, field))
            print("--------------------------------------------------")
        time.sleep(3)


def evaluate_min(client, threshold, measurement, field):
    df = get_min_query(client, measurement, field)
    for entry in df["min_" + field]:
        if entry < threshold:
            print("%f below %d! Min threshold broken in %s!!!"%(entry, threshold, field))
            print("--------------------------------------------------")
        time.sleep(3)