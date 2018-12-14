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
	const int led=13;
	const int reset=7;
	const int pwrkey=8;
	String mqttServer;
	int mqttPort;
	String mqttUser;
	String mqttPassword;
	unsigned long lastTime;
	const unsigned long TimeToConn=120000;
	char inChar;
};
#endif