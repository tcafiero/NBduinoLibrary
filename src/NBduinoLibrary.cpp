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

clean()
{
	char buffer[200];
	int len;
	mySerial.setTimeout(1000);
	while(1)
	{
		len=mySerial.readBytes(buffer,100);
		if(len == 0) return;
	}
}

getIMEI(char *imei)
{
	char buffer[200];
	int len;
	int i,j;
	j=0;
	while(1)
	{
	len=mySerial.readBytesUntil('\n', buffer, 100);
	for(i=0; i<len; i++) if((buffer[i] >= '0') && (buffer[i] <= '9')) imei[j++]=buffer[i];
	if(strstr(buffer,"OK") != NULL)
	{
		imei[j]=0;
		return;
	}
	};
}

sync()
{
	char buffer[200];
	while(1)
	{
	mySerial.readBytesUntil('\n', buffer, 100);
	if(strstr(buffer,"OK") != NULL) return;
	};
}

/**
*	This method is the constructor of the NBduuino
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
NBduino::NBduino()
{
}

/**
*	This method is the constructor of the NBduuino
*	@author	Antonio Cafiero
*	@param mqttServer The name or the IP address of the MQTT broker (String)
*	@param mqttPort The port number of the MQTT broker (Int)
*	@param mqttUser The username enabeled to access the MQTT broker (String)
*	@param mqttPassword The password to autenticate the userto access the MQTT broker (String)
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
*	This method is the constructor of the NBduuino
*	@author	Antonio Cafiero
*	@param mqttServer The name or the IP address of the MQTT broker (String)
*	@param mqttPort The port number of the MQTT broker (Int)
*	@param mqttUser The username enabeled to access the MQTT broker (String)
*	@param mqttPassword The password to autenticate the userto access the MQTT broker (String)
*	@param timeToConn The timeout in ms in connection phase (unsigned long)
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
*	@author	Antonio Cafiero
*	@param APN The name or the APN (String)
*	@date 14/12/2018
*/
NBduino::setAPN(String APN)
{
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
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
bool NBduino::begin()
{
    wakeup();
    mySerial.begin(4800);
	clean();
	mySerial.println("AT+CPIN?");
	sync();
	clean();
	mySerial.println("AT+CSQ");
	sync();
	clean();
	mySerial.println("AT+CGREG?");
	sync();
	clean();
	mySerial.println("AT+CGACT?");
	sync();
	clean();
	mySerial.println("AT+COPS?");
	sync();
	clean();
	mySerial.println("AT+CGCONTRDP");
	sync();	//delay(1500);
}

/**
*	This method wakeup the NBduino shield.
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
*	@author	Antonio Cafiero
*	@param topic The topic name for publishing (String)
*	@param value The string to publish (String)
*	@date 14/12/2018
*/
NBduino::publish(const String topic, const String value)
{
	char command[200];
	sprintf(command,"AT+CMQPUB=0,\"%s\",1,0,0,%d,\"%s\"",topic.c_str(), value.length(), value.c_str());
	clean();
	mySerial.println(command);
	sync();
}

/**
*	This method connect to MQTT broker.
*	@author	Antonio Cafiero
*	@param clientname The unique name for MQTT connection (String)
*	@date 8/3/2019
*/
NBduino::connect(const String clientname)
{
	char command[200];
	sprintf(command,"AT+CMQNEW=\"%s\",\"%d\",12000,100",_mqttServer.c_str(),_mqttPort);
	clean();
	mySerial.println(command);
	sync();
	sprintf(command,"AT+CMQCON=0,3,\"%s\",600,0,0,\"%s\",\"%s\"", clientname.c_str(), _mqttUser.c_str(), _mqttPassword.c_str());
	clean();
	mySerial.println(command);
	sync();
	clean();
	mySerial.println("AT+CREVHEX=0");
	sync();
    digitalWrite(_led, HIGH);    //LED OFF
}

/**
*	This method disconnect from MQTT broker.
*	@author	Antonio Cafiero
*	@date 8/3/2019
*/
NBduino::disconnect()
{	
    clean();
	mySerial.println("AT+CMQDISCON=0");
	sync();
    digitalWrite(_led, LOW);    //LED OFF
}

/**
*	This method return the NBduno IMEI string (String).
*	@author	Antonio Cafiero
*	@date 14/12/2018
*/
String NBduino::reqIMEI()
{
		char aMessage[200];
		unsigned int messageSize;
		char cha;
		aMessage[0] = 0;
        messageSize = 0;		// set message size to zero
		delay(4000);
		mySerial.flush();
		clean();
        mySerial.println("AT+GSN");
		getIMEI(aMessage);
		String exString = aMessage;
		return aMessage;
}
