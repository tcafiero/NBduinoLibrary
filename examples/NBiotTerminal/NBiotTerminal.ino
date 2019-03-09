#include <SoftwareSerial.h>

const int _led = 13;
const int _reset = 7;
const int _pwrkey = 8;
static SoftwareSerial mySerial(10, 11); //RX, TX
char c, Input[200];
unsigned int i;

void setup() {
  Serial.begin(115200);
  delay(1000);
  digitalWrite(_reset, LOW);
  digitalWrite(_pwrkey, HIGH);
  delay(2000);
  digitalWrite(_pwrkey, LOW);
  mySerial.begin(4800);
  delay(1000);
  i = 0;
}

void serialEvent() {
  char c;
  c = Serial.read();
  if (c == 0xd)
  {
    Input[i] = 0x0;
    i = 0;
    Serial.println(Input);
    mySerial.println(Input);
  }
  else
  {
    Input[i++] = c;
  }
}


void loop() {
}
