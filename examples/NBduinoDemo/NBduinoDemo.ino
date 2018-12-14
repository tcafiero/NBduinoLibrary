#include <NBduinoLibrary.h>

NBduino NBiot("54.76.137.235", 18224, "test", "test")

void setup() {
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Let's start!");
  delay(300);
  NBiot.begin();
  }

void loop() {
  
 }
  