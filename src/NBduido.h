#ifndef NBDUINO_H
#define NBDUINO_H
#include <SoftwareSerial>
#include <timer.h>
class NBduino
{
  public:
	void NBduino(const char* mqttServerP, const int mqttPortP, const char* mqttUserP, const char* mqttPasswordP);
	bool begin();
	void wakeup();
	void sleep();
	String reqIMEI();
	void publish(const char* topic, const char* value);
  private:
	SofwareSerial mySerial(10, 11); //RX, TX
	const int led=13;
	const int reset=7;
	const int pwrkey=8;
	const char* mqttServer;
	const int mqttPort;
	const char* mqttUser;
	const char* mqttPassword;
	unsigned long lastTime;
	char inChar;
};
#endif