import requests

url = 'http://192.168.178.85/'
query = {'field': "Hey baby girl!"}
res = requests.post(url, data=query)
requests.put(url, query)
print(res.text)