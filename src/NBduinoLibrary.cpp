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
static char outword[1000];
static const char hexdigit[] = "0123456789ABCDEF";
static char hexnum[7];

void(* Reset)(void) = 0;


char* int2hex(int number){
	hexnum[0]=hexdigit[(number>>4)&0xF];
	hexnum[1]=hexdigit[number&0xF];
	hexnum[3]=0;
	return hexnum;
}

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

bool getIMEI(char* imei)
{
	char buffer[100];
	int len;
	int i,j=0;
	imei[0]=0;
	mySerial.setTimeout(1000);
	while(1)
	{
	len=mySerial.readBytesUntil('\n', buffer, sizeof(buffer));
	if(len != 0)
	{
	for(i=0; i<len; i++) if((buffer[i] >= '0') && (buffer[i] <= '9')) imei[j++]=buffer[i];
	if(strstr(buffer,"OK") != NULL)
	{
		imei[j]=0;
		return true;
	}
	}
	else return false;
	};
}

bool sync()
{
	char buffer[200];
	int len;
	mySerial.setTimeout(1000);
	while(1)
	{
	len=mySerial.readBytesUntil('\n', buffer, 100);
	if(len == 0) return false;
	if(strstr(buffer,"OK") != NULL) return true;
	if(strstr(buffer,"ERROR") != NULL) return false;
	};
}

void sendCommand(char* command)
{
	int i=0;
	while(1)
	{
	clean();
	mySerial.println(command);
	if(sync()) return;
	if(i++ > 3) Reset();
	}
}

void sendCommand(char* command, char* imei)
{
	int i=0;
	while(1)
	{
	clean();
	mySerial.println(command);
	if(getIMEI(imei)) return;
	if(i++ > 3) Reset();
	}
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
*	@param mqttServer The name or the IP address of the MQTT broker (char*)
*	@param mqttPort The port number of the MQTT broker (Int)
*	@param mqttUser The username enabeled to access the MQTT broker (char*)
*	@param mqttPassword The password to autenticate the userto access the MQTT broker (char*)
*	@date 14/12/2018
*/
NBduino::NBduino(const char * mqttServer, const int mqttPort, const char * mqttUser, const char * mqttPassword)
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
*	@param mqttServer The name or the IP address of the MQTT broker (char*)
*	@param mqttPort The port number of the MQTT broker (Int)
*	@param mqttUser The username enabeled to access the MQTT broker (char*)
*	@param mqttPassword The password to autenticate the userto access the MQTT broker (char*)
*	@param timeToConn The timeout in ms in connection phase (unsigned long)
*	@date 14/12/2018
*/
NBduino::NBduino(const char * mqttServer, const int mqttPort, const char * mqttUser, const char * mqttPassword, unsigned long timeToConn)
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
NBduino::setAPN(const char * APN)
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
	sendCommand("ATZ");
	sendCommand("ATE0");
	sendCommand("AT+CPIN?");
	sendCommand("AT+CSQ");
	sendCommand("AT+CGREG?");
	sendCommand("AT+CGACT?");
	sendCommand("AT+COPS?");
	sendCommand("AT+CGCONTRDP");
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
*	This method publish a topic/value to the MQTT broker.
*	@author	Antonio Cafiero
*	@param topic The topic name for publishing (char*)
*	@param value The string to publish (char*)
*	@date 14/12/2018
*/
NBduino::publish(const char* topic, const char* value)
{
	char command[200];
	char outcode[10];
    unsigned int len, i;
    len = strlen(value);
	sprintf(command,"AT+CMQPUB=0,\"%s\",1,0,0,%d,\"",topic, len*2);
	clean();
	mySerial.print(command);
	for(i = 0; i<len; i++) {
		mySerial.print(int2hex(value[i]));
    }
	mySerial.println("\"");
	sync();
}

/**
*	This method connect to MQTT broker.
*	@author	Antonio Cafiero
*	@param clientname The unique name for MQTT connection (char*)
*	@date 8/3/2019
*/
NBduino::connect(const char* clientname)
{
	char command[200];
	sprintf(command,"AT+CMQNEW=\"%s\",\"%d\",12000,1024",_mqttServer,_mqttPort);
	sendCommand(command);
	sprintf(command,"AT+CMQCON=0,3,\"%s\",20000,0,0,\"%s\",\"%s\"", clientname, _mqttUser, _mqttPassword);
	sendCommand(command);
	sendCommand("AT+CREVHEX=1");
    digitalWrite(_led, HIGH);    //LED OFF
}

/**
*	This method disconnect from MQTT broker.
*	@author	Antonio Cafiero
*	@date 8/3/2019
*/
NBduino::disconnect()
{	
	sendCommand("AT+CMQDISCON=0");
    digitalWrite(_led, LOW);    //LED OFF
}

/**
*	This method return the NBduno IMEI value.
*	@author	Antonio Cafiero
*	@param imei a pointer to buffer were IMEI is returned If no IMEI is read void string is returned (char*)
*	@date 14/12/2018
*/
bool NBduino::reqIMEI(char* imei)
{
        sendCommand("AT+GSN", imei);
}
