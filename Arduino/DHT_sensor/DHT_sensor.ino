#include <WiFi.h> 
#include <Wire.h>
#include "DHT.h"

#define DHTTYPE DHT11
//DHT Sensor;
uint8_t DHTPin = 4; 
uint8_t WIFILED = 2;
DHT dht(DHTPin, DHTTYPE); 
float Temperature;
float Humidity;

const char* ssid = "Kanal7"; 
const char* password = "LosMejoresKanal7";

WiFiServer server(80);

String header;

void setup() {
  Serial.begin(115200);
  pinMode(DHTPin, INPUT);
  pinMode(WIFILED, OUTPUT);
  dht.begin();
  
  Serial.print("Connecting to Wifi Network");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    blinkLEDonBoard(500);
  }
  Serial.println("");
  Serial.println("Successfully connected to WiFi.");
  Serial.println("IP address of ESP32 is : ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");

}

void loop()
{
  Temperature = dht.readTemperature(); 
  Humidity = dht.readHumidity(); 
  WiFiClient client = server.available();
  
  if (client) 
  { 
    Serial.println("Web Client connected ");
    String request = client.readStringUntil('\r'); 
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");
    client.println("</style></head><body><h1>ESP32 Web Server Reading sensor values</h1>");
    client.println("<h2>DHT11/DHT22</h2>");
    client.println("<h2>Microcontrollerslab.com</h2>");
    client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
    client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
    client.println(dht.readTemperature());
    client.println(" *C</span></td></tr>"); 
    client.println("<tr><td>Temp. Fahrenheit</td><td><span class=\"sensor\">");
    client.println(1.8 * dht.readTemperature() + 32);
    client.println(" *F</span></td></tr>"); 
    client.println("<tr><td>Humidity</td><td><span class=\"sensor\">");
    client.println(dht.readHumidity());
    client.println(" %</span></td></tr>"); 
    client.println("</body></html>"); 
    client.stop();
    client.println();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
void blinkLEDonBoard(float time_waiting_ms){
  digitalWrite(WIFILED,HIGH);
  delay(time_waiting_ms);
  digitalWrite(WIFILED,LOW);
  delay(time_waiting_ms);
}
  
