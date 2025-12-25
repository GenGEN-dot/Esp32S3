#include <Arduino.h>

void setup(){
  Serial.begin(1150200);
  pinMode(1, OUTPUT);
}
void loop(){
  delay(1000);
  digitalWrite(1, HIGH);
  Serial.println("High");
  delay(1000);
  digitalWrite(1, LOW);
  Serial.println("Low");
}