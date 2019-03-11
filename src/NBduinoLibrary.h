/**
*	@file NBduinoLibrary.h
*	@brief contains the NBduino class definition and the include file that the class implementation
*	needs.
*
*	@author	Antonio Cafiero
*
*	@date 14/12/2018
*/


#ifndef NBDUINOLIBRARY_H
#define NBDUINOLIBRARY_H
#include <SoftwareSerial.h>


class NBduino
{
  public:
	NBduino();
	NBduino(const char* mqttServer, const int mqttPort, const char* mqttUser, const char* mqttPassword);
	NBduino(const char* mqttServer, const int mqttPort, const char* mqttUser, const char* mqttPassword, unsigned long timeToConn);
	setAPN(const char* APN);
	bool begin();
	wakeup();
	sleep();
	bool reqIMEI(char* imei);
	publish(const char* topic, const char* value);
	connect(const char* clientname);
	disconnect();
  private:
	const int _led=13;
	const int _reset=7;
	const int _pwrkey=8;
	const char * _mqttServer;
	int _mqttPort;
	const char * _mqttUser;
	const char * _mqttPassword;
	unsigned long _lastTime;
	unsigned long _timeToConn=120000;
	char _inChar;
};
#endif