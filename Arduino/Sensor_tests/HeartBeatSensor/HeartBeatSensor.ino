void setup() {
  Serial.begin(115200);
  pinMode(27, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(27));
  delay(50);
}
