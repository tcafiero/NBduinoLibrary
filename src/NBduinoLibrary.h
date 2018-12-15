/*
  NBduinoLibrary
  
  
  Arduino SHIELD: NBduino
  Arduino BOARD: ARDUINO UNO Rev3 (or similar)

  Author: Antonio Cafiero
  Date: 14/12/2018
*/

#ifndef NBDUINOLIBRARY_H
#define NBDUINOLIBRARY_H
#include <SoftwareSerial.h>


class NBduino
{
  public:
	NBduino(const String mqttServerP, const int mqttPortP, const String mqttUserP, const String mqttPasswordP);
	bool begin();
	wakeup();
	sleep();
	String reqIMEI();
	publish(const String topic, const String value);
  private:
	const int _led=13;
	const int _reset=7;
	const int _pwrkey=8;
	String _mqttServer;
	int _mqttPort;
	String _mqttUser;
	String _mqttPassword;
	unsigned long _lastTime;
	const unsigned long _TimeToConn=120000;
	char _inChar;
};
#endif