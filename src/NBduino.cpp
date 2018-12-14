#include "NBduino.h"

void NBduino::NBduino(const char* mqttServerP, const int mqttPortP, const char* mqttUserP, const char* mqttPasswordP)
{
    mqttServer=mqttServerP;
    mqttPort=mqttPortP;
    mqttUser=mqttUserP;
    mqttPassword=mqttPasswordP;
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
    lastTime = millis();
    while (1)
    {
        if (millis() - lastTime < TimeToConn) {
            // get incoming byte:
            inChar = mySerial.read();
            if ( inChar == '+') {
                return true;
            }
        }
        else return false;
    }
}

void NBduino::wakeup()
{
    digitalWrite(reset, LOW);
    digitalWrite(pwrkey, HIGH);
    delay(2000);
    digitalWrite(pwrkey, LOW);
}

void NBduino::sleep()
{
    digitalWrite(pwrkey, HIGH);
    delay(2000);
    digitalWrite(pwrkey, LOW);
}

void publish(const char* topic, const char* value)
{
    char outword[200];
    int len, i;
    len = strlen(value);
    if(value[len-1]=='\n')
        value[--len] = '\0';

    for(i = 0; i<len; i++) {
        sprintf(outword+i*2, "%02X", value[i]);
    }
    printf("%s\n", outword);
    digitalWrite( led, HIGH );
    delay(2000);
    mySerial.print("AT+CMQNEW=\"");
    mySerial.print(mqttServer);
    mySerial.print("\",""\"");
    mySerial.print(mqttPort);
    mySerial.println("\",2400,100");
    delay(2000);
    mySerial.print("AT+CMQCON=0,3,");
    mySerial.print("\"");
    mySerial.print(topic);
    mySerial.print("\"");
    mySerial.print(",1200,0,0,");
    mySerial.print("\"""");
    mySerial.print(mqttUser);
    mySerial.print("\",""\"");
    mySerial.print(mqttPassword);
    mySerial.println("\"");
    delay(2000);
    mySerial.print("AT+CMQPUB=0,\"");
    mySerial.print("NBduino/");
    mySerial.print(topic);
    mySerial.print("\",1,0,0,");
    mySerial.print(strlen(value);
                   mySerial.print(",");
                   mySerial.print("\"");
                   mySerial.print(outword);
                   mySerial.println("\"");
                   delay(2000);
                   mySerial.println("AT+CMQDISCON=0");
                   delay(2000);
                   digitalWrite(LED_PIN, LOW);    //LED OFF
}

String reqIMEI()
{

    char aMessage[200];
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
        return exString
    }