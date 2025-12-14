#include <Arduino.h>

bool ledState = false;

void setup(){
  pinMode(1 , OUTPUT);
  pinMode(4 , OUTPUT);
}

void loop(){
  digitalWrite(4 , HIGH);
  if (digitalRead(2) == HIGH){
    ledState = !ledState;
  }
  if (ledState){
    digitalWrite(1 , HIGH);
  } else {
    digitalWrite(1 , LOW);
  }
}