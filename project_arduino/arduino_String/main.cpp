/************************************************************
Copyright (C) Husarion 2017
*************************************************************/
#define Arduino_hMain
#include <hFramework.h>
#include <Arduino.h>

hFramework::ArduinoSerial SerialPort(hFramework::Serial);

void setup() 
{
}

void loop() 
{
  float a1 = 10.93;
  int b1 = 123;
  String s1(a1);
  String s2(b1);
  String s3("Hello");

  SerialPort.println(s1);
  SerialPort.println(s2);
  SerialPort.println(s3);

  delay(100);
}
