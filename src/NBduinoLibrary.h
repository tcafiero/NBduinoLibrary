#ifndef NBDUINOLIBRARY_H
#define NBDUINOLIBRARY_H
#include <SoftwareSerial.h>


class NBduino
{
  public:
	NBduino(const char* mqttServerP, const int mqttPortP, const char* mqttUserP, const char* mqttPasswordP);
	bool begin();
	wakeup();
	sleep();
	String reqIMEI();
	publish(const char* topic, const char* value);
  private:
	const int led=13;
	const int reset=7;
	const int pwrkey=8;
	char mqttServer[20];
	int mqttPort;
	char mqttUser[20];
	char mqttPassword[20];
	unsigned long lastTime;
	const unsigned long TimeToConn=120000;
	char inChar;
};
#endif