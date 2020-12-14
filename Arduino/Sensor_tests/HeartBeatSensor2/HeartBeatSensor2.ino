// Modified by Dr. Abhishek Ghosh, 2019
// see https://thecustomizewindows.com 
// GNU GPL 3.0
 
//  Variables
 
int PulseSensorPurplePin = 34; // Pulse Sensor WIRE connected to Pin 34
int LED13 = 2;   //  The on-board ESP32 LED
 
 
int Signal; // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 2400; // Determine which Signal to "count as a beat", and which to ingore.
 
 
// The SetUp Function:
void setup() {
  pinMode(LED13,OUTPUT); // pin that will blink to your heartbeat
  Serial.begin(9600); // Set's up Serial Communication at certain speed.
 
}
 
// The Main Loop Function
void loop() {
 
  Signal = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value.
                                              // Assign this value to the "Signal" variable.
 
   Serial.println(Signal);                    // Send the Signal value to Serial Plotter.
 
 
 
   if(Signal > Threshold){   // If the signal is above "2400", then "turn-on" ESP32's on-Board LED.
     digitalWrite(LED13,HIGH);
   } else {
     digitalWrite(LED13,LOW); //  Else, the sigal must be below "2400", so "turn-off" this LED.
   }
 
 
delay(10); // too higher delay will 
 
 
}
