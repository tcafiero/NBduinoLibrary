/*
  After loading is complete open the IDE serial monitor and wait until the module attaching to the Network

  NOTE: LAUNCH THIS SKETCH JUST THE FIRST TIME YOU USE THE SHIELD!

  Arduino SHIELD: NBduino
  Arduino BOARD: ARDUINO UNO Rev3 (or similar)

  Author: Antonio Cafiero
  Date: 14/12/2018
*/
#include <NBduinoLibrary.h>

// Network settings 
const char* APN = "ep.inetd.gdsp"; //(Vodafone)
NBduino NBiot;


void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Let's start!");
  delay(300);
  NBiot.setAPN(APN);
  Serial.println("WAIT UNTIL THE \"NET\" LED STARTS TO FLASH EVERY 3 SECONDS");
  }

void loop() { 
}



