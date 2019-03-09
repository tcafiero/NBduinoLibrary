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
	NBduino(const String mqttServer, const int mqttPort, const String mqttUser, const String mqttPassword);
	NBduino(const String mqttServer, const int mqttPort, const String mqttUser, const String mqttPassword, unsigned long timeToConn);
	setAPN(String APN);
	bool begin();
	wakeup();
	sleep();
	String reqIMEI();
	publish(const String topic, const String value);
	connect(const String clientname);
	disconnect();
  private:
	const int _led=13;
	const int _reset=7;
	const int _pwrkey=8;
	String _mqttServer;
	int _mqttPort;
	String _mqttUser;
	String _mqttPassword;
	unsigned long _lastTime;
	unsigned long _timeToConn=120000;
	char _inChar;
};
#endif