#include <SoftwareSerial.h>

const int _led = 13;
const int _reset = 7;
const int _pwrkey = 8;
static SoftwareSerial mySerial(10, 11); //RX, TX
char c, Input[200];
unsigned int i;
char *CapCMD[]={
"AT+NRB",
"AT+URAT=8",
"AT+CEREG=5",
"AT+CSCON=1",
"AT+CFUN=1",
"AT+CIMI",
"AT+CGDCONT=1, \"IP\",\"\"",
"AT+COPS=1,2,\"22210\"",
"AT+NUESTATS",
"AT+NPING=\"8.8.8.8\",,60000",
"AT+CGDCONT?",
"AT+COPS=?"};

char *PDNAutoActivation[]={
"AT+CPIN?",
"AT+CSQ",
"AT+CGREG?",
"AT+CGACT?",
"AT+COPS?",
"AT+CGCONTRDP",
"AT+CIPPING=\"54.76.137.235\"",
"AT+CMQNEW=\"54.76.137.235\",\"18224\",12000,100",
"AT+CMQCON=0,3,\"myclient\",600,0,0,\"test\",\"test\"",
"AT+CREVHEX=0",
"AT+CMQPUB=0,\"myimei\",1,0,0,12,\"Hello World!\""
};

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
  /*
  for(int j=0; j < (sizeof(CapCMD)/sizeof(char*)); j++)
  {
    mySerial.println(CapCMD[j]);
    delay(1000);
  }
  */
  for(int j=0; j < (sizeof(PDNAutoActivation)/sizeof(char*)); j++)
  {
    mySerial.println(PDNAutoActivation[j]);
    delay(5000);
  }  
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
