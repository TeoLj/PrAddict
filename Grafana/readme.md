1. install necessary tools (installation on mac)

brew update
brew install grafana
brew install telegraf
brew install influxdb

brew services start grafana
brew services stop grafana

brew services start influxdb
brew services stop influxdb

2. start and configure tools

a) brew services start grafana

access grafana using browser url http://localhost:3000/

b) start influxd demon
influxd -config /usr/local/etc/influxdb.conf

c) create sample data set with telegraf

telegraf -sample-config -input-filter cpu:mem -output-filter influxdb > telegraf.conf

now run the telegraf using:

telegraf --config nameOfconfigfile.conf

to test it use: telegraf --test --config telegraf.conf

how to configure csv parser see https://github.com/influxdata/telegraf/tree/master/plugins/parsers/csv

3. verify if telegraf created sample database in influxdb

influx
show databases #this should list the database named telegraf

use telegraf #telling influx that we want to use telegraf database
show measurements #now that we are already in telegraf database we want to see what data is in the table measurements

4. Generating test data
   generator.js is a JS file used to generate to test live data visualization.
   To run generator.js, nodejs must be installed first.
   Generator produces live_data_demo.csv file containing live data for two measurements.

   To run it, on command line type the following:
   node generator.js

5. Test case , fill different values in .csv file and verify the change is picked up
   Due to how historical data works in influxdb just changing data in past after data is processed
   will not reflect the change in masurement values. To make this happen, we first need to:

- stop telegraf job
- connect to influxdb and delete the database: (on command line execute the following)
  influx
  show databases #this will list influxdb database names

  ....
  drop database measured_data #this will delete database
  exit #use this command to exit influx client

- start telegraf again. telegraf creates new database

6. start shell

mark as executable in mac:

chmod +x StartTelegraf.sh

7. git pull

vin editor opens
esc or git pull --no-edit
:
wq (write and quit)

LINKS:

https://diyi0t.com/visualize-mqtt-data-with-influxdb-and-grafana/

https://docs.influxdata.com/influxdb/v2.0/write-data/developer-tools/csv/

### Update to influxDB 2.0 (using flex query language)

#### influxDB 2.0 documentation

https://docs.influxdata.com/influxdb/v2.0/

On mac os use the following to list running services:
brew services list

To start / stop services use:

brew services stop <service_name>

example (stop / start chronograf):

brew services stop chronograf
brew services start chronograf
brew services restart chronograf

Use the following to open chronograf gui:
chronograf - Chronograf is the complete interface for the InfluxDB 1.x Platform
http://localhost:8888/

Use the following to open influxdb gui:
http://localhost:8086/

The influxdb V2 is still not available via brew package manager.
One needs to follow the manual installation process described here:
https://docs.influxdata.com/influxdb/v2.0/get-started/

Start influxdb v2 with (open terminal and type):

influxd (keep it open)

Note: On Mac OS for now this is not a service, so it needs to be started every time when you reboot your machine

https://github.com/influxdata/telegraf/tree/master/plugins/outputs/influxdb_v2

### Telegraf output plugin for influxDB v2 documentation

https://github.com/influxdata/telegraf/tree/master/plugins/outputs/influxdb_v2

#### Creating and using bucket (using influx GUI)

Before telegraf is able to insert data into influxdb we need to create two things:

- Create new organiztion and bucket (if you dont have one already)
  Admin > Create Organization

- Create access token
  Data > Tokens

#### Use Data Explorer to have a first view of the measurements

1. Go to Explore and set the right time range
2. in Filter choose the measurments to visualize
3. Click Submit

For using pivot example see the following documentation:
https://docs.influxdata.com/influxdb/v2.0/query-data/flux/mathematic-operations/
