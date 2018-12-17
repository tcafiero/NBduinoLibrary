/**
*	@file NBduinoLibrary.cpp
*	@brief contains the NBduino class mathods implementation. This class can be used to ease
*	the programming of an Arduino sketch using the TopView NBduino shield. 
*
*	@author	Antonio Cafiero
*
*	@date 14/12/2018
*/


#include <arduino.h>
#include <NBduinoLibrary.h>

static SoftwareSerial mySerial(10, 11); //RX, TX

/**
*	This method is a constructor for the NBduino class
*	@code
*
*	Usage example:
*		NBduino NBiot();
*	@endcode
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
NBduino::NBduino()
{
}

/**
*	This method is a constructor for the NBduino class
*	@code
*
*	Usage example:
*		NBduino NBiot("54.76.137.235", 18224, "test", "test");
*	@endcode
*	@param mqttServer The name or the IP address of the MQTT broker (String)
*	@param mqttPort The port number of the MQTT broker (Int)
*	@param mqttUser The username enabeled to access the MQTT broker (String)
*	@param mqttPassword The password to autenticate the userto access the MQTT broker (String).
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
NBduino::NBduino(const String mqttServer, const int mqttPort, const String mqttUser, const String mqttPassword)
{
    _mqttServer=mqttServer;
    _mqttPort=mqttPort;
    _mqttUser=mqttUser;
    _mqttPassword=mqttPassword;
	_timeToConn=120000;
}

/**
*	This method is a constructor for the NBduino class
*	@code
*
*	Usage example:
*		NBduino NBiot("54.76.137.235", 18224, "test", "test", 60000);
*	@endcode
*	@param mqttServer The name or the IP address of the MQTT broker (String)
*	@param mqttPort The port number of the MQTT broker (Int)
*	@param mqttUser The username enabeled to access the MQTT broker (String)
*	@param mqttPassword The password to autenticate the userto access the MQTT broker (String)
*	@param timeToConn The timeout in ms in connection phase (unsigned long)
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
NBduino::NBduino(const String mqttServer, const int mqttPort, const String mqttUser, const String mqttPassword, unsigned long timeToConn)
{
    _mqttServer=mqttServer;
    _mqttPort=mqttPort;
    _mqttUser=mqttUser;
    _mqttPassword=mqttPassword;
	_timeToConn=timeToConn;
}

/**
*	This method set the APN
*	@code
*
*	Usage example:
*		NBiot.setAPN("broker.iothingsware.com");
*	@endcode
*	@param APN The name of the APN (String)
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
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

/**
*	This method must be called at beginning before any publish, subscribe, wakeup and sleep call.
*	@code
*
*	Usage example:
*		bool status
*		status=NBiot.begin();
*	@endcode
*	@return true if NBduino correctly initiated - false othewise
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
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
        if (millis() - _lastTime < _timeToConn) {
            // get incoming byte:
            _inChar = mySerial.read();
            if ( _inChar == '+') {
                return true;
            }
        }
        else return false;
    }
}

/**
*	This method wakeup the NBduino shield.
*	@code
*
*	Usage example:
*		NBiot.wakeup();
*	@endcode
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
NBduino::wakeup()
{
    digitalWrite(_reset, LOW);
    digitalWrite(_pwrkey, HIGH);
    delay(2000);
    digitalWrite(_pwrkey, LOW);
}

/**
*	This method asleep the NBduino shield.
*	@code
*
*	Usage example:
*		NBiot.sleep();
*	@endcode
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
NBduino::sleep()
{
    digitalWrite(_pwrkey, HIGH);
    delay(2000);
    digitalWrite(_pwrkey, LOW);
}

/**
*	This method publish a string to the MQTT broker.
*	@code
*
*	Usage example:
*		NBiot.publish("topic001", "Hello World!");
*	@endcode
*	@param topic The topic name for publishing (String)
*	@param value The string to publish (String)
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
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

/**
*	This method return the NBduno IMEI string (String).
*	@code
*
*	Usage example:
*		String IMEI
*		IMEI=NBiot.reqIMEI();
*	@endcode
*	@return The string contains IMEI
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
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