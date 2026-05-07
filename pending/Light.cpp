#include <Arduino.h>

#define LED_PIN     1

void setup() {
    pinMode(LED_PIN , OUTPUT);
    digitalWrite(LED_PIN, HIGH);
}

void loop() {
    delay(1000);
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}