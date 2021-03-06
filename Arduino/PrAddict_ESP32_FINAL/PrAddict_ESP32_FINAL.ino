#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
// #include <WiFi.h> 
#include <Wire.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTTYPE DHT11
//DHT Sensor;
uint8_t DHTPin = 4; 
uint8_t WIFILED = 5;
uint8_t redLED = 16;
uint8_t greenLED = 17;
DHT dht(DHTPin, DHTTYPE); 
float Temperature;
float Humidity;
long addicted_state;

// OLED definitions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// WiFi AP SSID
#define WIFI_SSID "Kanal7_VIP"
// WiFi password
#define WIFI_PASSWORD "justusjonas"
// InfluxDB v2 server url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
#define INFLUXDB_URL "http://192.168.178.83:8086"
// InfluxDB v2 server or cloud API authentication token (Use: InfluxDB UI -> Data -> Tokens -> <select token>)
#define INFLUXDB_TOKEN "Vy9BX-Kn4s2I_L7vq7ITYgChqxgJqUM0vNtlf3tEDVOA7P52b9f_VKv_VYIs1CHoOyHn-S8POOj0tvBlzA3YzQ=="
// InfluxDB v2 organization id (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_ORG "HackOff"
// InfluxDB v2 bucket name (Use: InfluxDB UI ->  Data -> Buckets)
#define INFLUXDB_BUCKET "arduino_data"

// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
//  Pacific Time: "PST8PDT"
//  Eastern: "EST5EDT"
//  Japanesse: "JST-9"
//  Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
Point sensorT("Temperature");
Point sensorH("Humidity");

void setup() {
  Serial.begin(115200);
  pinMode(DHTPin, INPUT);
  pinMode(WIFILED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  dht.begin();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  
  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    digitalWrite(WIFILED,HIGH);
    delay(500);
    digitalWrite(WIFILED,LOW);
  }
  Serial.println();

  // Accurate time is necessary for certificate validation and writing in batches
  // For the fastest time sync find NTP servers in your area: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop() {
  Temperature = dht.readTemperature(); 
  Humidity = dht.readHumidity();
  
  // Clear fields for reusing the point. Tags will remain untouched
  sensorT.clearFields();
  sensorH.clearFields();
  
  // Store measured value into point
  sensorT.addField("values", Temperature);
  sensorH.addField("values", Humidity);

  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(sensorT.toLineProtocol());
  Serial.println(sensorH.toLineProtocol());

  // If no Wifi signal, try to reconnect it
  if ((WiFi.RSSI() == 0) && (wifiMulti.run() != WL_CONNECTED)) {
    Serial.println("Wifi connection lost");
  }

  // Write point
  if (!client.writePoint(sensorT)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
  if (!client.writePoint(sensorH)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  //Wait 10s
  Serial.println("Wait 10s");
  digitalWrite(WIFILED,HIGH);
  delay(1000);
  digitalWrite(WIFILED,LOW);
  
  // Construct a Flux query
  // Query will find RSSI for last 24 hours for each connected WiFi network with this device computed by given selector function
  String query = "from(bucket: \"arduino_data\") |> range(start: -10s) |> filter(fn: (r) => r._measurement == \"PredTest\" and r._field == \"value\")";
  query += "|> max()";
  
  // Send query to the server and get result
  FluxQueryResult result = client.query(query);
  
  // Iterate over rows. Even there is just one row, next() must be called at least once.
  while (result.next()) {
    // Get converted value for flux result column '_value' where there is RSSI value
    addicted_state = result.getValueByName("_value").getLong();
    Serial.print(addicted_state);
  
    Serial.println();
  }
  
  // Check if there was an error
  if(result.getError() != "") {
    Serial.print("Query result error: ");
    Serial.println(result.getError());
  }

  // Close the result
  result.close();
  
  // read and change OLED
  if(addicted_state == 1){
    digitalWrite(redLED,HIGH);
    digitalWrite(greenLED,LOW);
    
    //OLED Display: stop!
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    // Display static text
    display.println("STOP! DON'T DO IT");
    display.display(); 
  }else{
    digitalWrite(greenLED,HIGH);
    digitalWrite(redLED,LOW);

    //OLED Display: you are doing well!
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    // Display static text
    display.println("Well done :)");
    display.display(); 
  }
  
  delay(9000);
}
