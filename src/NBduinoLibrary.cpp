/*
  NBduinoLibrary
  
  
  Arduino SHIELD: NBduino
  Arduino BOARD: ARDUINO UNO Rev3 (or similar)

  Author: Antonio Cafiero
  Date: 14/12/2018
*/

#include <arduino.h>
#include <NBduinoLibrary.h>

static SoftwareSerial mySerial(10, 11); //RX, TX

NBduino::NBduino()
{
}

NBduino::NBduino(const String mqttServer, const int mqttPort, const String mqttUser, const String mqttPassword)
{
    _mqttServer=mqttServer;
    _mqttPort=mqttPort;
    _mqttUser=mqttUser;
    _mqttPassword=mqttPassword;
	_TimeToConn=120000;
}

NBduino::NBduino(const String mqttServer, const int mqttPort, const String mqttUser, const String mqttPassword, unsigned long TimeToConn)
{
    _mqttServer=mqttServer;
    _mqttPort=mqttPort;
    _mqttUser=mqttUser;
    _mqttPassword=mqttPassword;
	_TimeToConn=TimeToConn;
}

NBduino::setAPN(String APN)
{
  wakeup();
  mySerial.begin(115200);
  delay(1000);
  mySerial.println("AT+IPR=4800");
  delay (500);
  mySerial.end();
  mySerial.begin(4800);
  delay(500);
  mySerial.print("AT*MCGDEFCONT=");
  mySerial.print("\"");
  mySerial.print("IP");
  mySerial.print("\"");
  mySerial.print(",");
  mySerial.print("\"");
  mySerial.print(APN);
  mySerial.println("\"");
  delay(500);
  mySerial.print("AT+CGDCONT=1,");
  mySerial.print("\"");
  mySerial.print("IP");
  mySerial.print("\"");
  mySerial.print(",");
  mySerial.print("\"");
  mySerial.print(APN);
  mySerial.print("\"");
  mySerial.print(",");
  mySerial.print("\"");
  mySerial.print("\"");
  mySerial.println(",0,0,0,,,,,,0,,0");
  delay(500);
  mySerial.println("AT+CEREG=5");
  delay(500);
  mySerial.println("AT+COPS=?");	
}

bool NBduino::begin()
{
    wakeup();
    mySerial.begin(115200);
    delay(1000);
    mySerial.println("AT+IPR=4800");
    delay (500);
    mySerial.end();
    mySerial.begin(4800);
    delay (500);
    mySerial.println("ATE0");
    mySerial.println("AT+CEREG=5"); //restituisce i parametri della cella LTE
    delay (1500);
    mySerial.flush();
    _lastTime = millis();
    while (1)
    {
        if (millis() - _lastTime < _TimeToConn) {
            // get incoming byte:
            _inChar = mySerial.read();
            if ( _inChar == '+') {
                return true;
            }
        }
        else return false;
    }
}

NBduino::wakeup()
{
    digitalWrite(_reset, LOW);
    digitalWrite(_pwrkey, HIGH);
    delay(2000);
    digitalWrite(_pwrkey, LOW);
}

NBduino::sleep()
{
    digitalWrite(_pwrkey, HIGH);
    delay(2000);
    digitalWrite(_pwrkey, LOW);
}

NBduino::publish(const String topic, const String value)
{
    char outword[200];
	String s=value;
    unsigned int len, i;
    len = value.length();
    if(value[len-1]=='\n')
        s[--len] = '\0';

    for(i = 0; i<len; i++) {
        sprintf(outword+i*2, "%02X", s[i]);
    }
    digitalWrite( _led, HIGH );
    delay(2000);
    mySerial.print("AT+CMQNEW=\"");
    mySerial.print(_mqttServer);
    mySerial.print("\",""\"");
    mySerial.print(_mqttPort);
    mySerial.println("\",2400,100");
    delay(2000);
    mySerial.print("AT+CMQCON=0,3,");
    mySerial.print("\"");
    mySerial.print(topic);
    mySerial.print("\"");
    mySerial.print(",1200,0,0,");
    mySerial.print("\"""");
    mySerial.print(_mqttUser);
    mySerial.print("\",""\"");
    mySerial.print(_mqttPassword);
    mySerial.println("\"");
    delay(2000);
    mySerial.print("AT+CMQPUB=0,\"");
    mySerial.print("NBduino/");
    mySerial.print(topic);
    mySerial.print("\",1,0,0,");
    mySerial.print(value.length());
    mySerial.print(",");
    mySerial.print("\"");
    mySerial.print(outword);
    mySerial.println("\"");
    delay(2000);
    mySerial.println("AT+CMQDISCON=0");
    delay(2000);
    digitalWrite(_led, LOW);    //LED OFF
}

String NBduino::reqIMEI()
{
    char aMessage[200];
	unsigned int messageSize;
	char cha;
    mySerial.println("AT+GSN");
    mySerial.flush();
    if (mySerial.available()) {
        for (int c = 0; c <= 99; c++) aMessage[c] = 0;    // clear aMessage in prep for new message
        messageSize = 0;                                  // set message size to zero

        while (mySerial.available()) {            // loop through while data is available
            cha = mySerial.read();                  // get character
            aMessage[messageSize] = cha;            // append to aMessage
            messageSize++;                          // bump message size
            delay(20);                              // just to slow the reads down a bit
        } // while

        aMessage[messageSize] = '\0';             // set last character to a null

        String exString = aMessage;
        exString.remove(0, 2);
        exString.remove(15);
        return exString;
    }
}