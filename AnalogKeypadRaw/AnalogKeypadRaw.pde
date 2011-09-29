int sensorPin = A0;    // select the input pin for the potentiometer

void setup() {
  Serial.begin(115200);
}

void loop() {
  // read the value from the sensor:
  int analogValue = analogRead(sensorPin);    
  Serial.print("Analog value: "); Serial.println(analogValue);
}
