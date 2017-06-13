#include <Arduino.h>

void hMain() {
    pinMode(A5, INPUT);
    int val = analogRead(A5);
    Serial.print(val);
}
