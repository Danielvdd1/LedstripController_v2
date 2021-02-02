#include <Arduino.h> // Required for Visual Studio Code
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESP8266HTTPClient.h> // Http request

#include "ledstrip_light.hpp"

namespace ledstripLights {


// ====== GPIO definitions

GPIO_Out_PWMServoDriver::GPIO_Out_PWMServoDriver(Adafruit_PWMServoDriver &pwmDriver, int pin): pwmDriver(pwmDriver), pin(pin), value(0)
{
	
}

void GPIO_Out_PWMServoDriver::setValue(int value){
  	this->value = value;
	pwmDriver.setPWM(pin, 0, value * 16);
	#ifdef ESP8266 // If this is slow: Move to Ledstrip#.setValue()
      yield(); // take a breather, required for ESP8266
    #endif
}

int GPIO_Out_PWMServoDriver::getValue(){
	//value = digitalRead(pin);
	return value;
}

/*
GPIO_Out_Pin::GPIO_Out_Pin(int pin): pin(pin), value(0)
{
	pinMode(pin, OUTPUT);
}

void GPIO_Out_Pin::setValue(int value){
  	this->value = value;
	analogWrite(pin, value);
}

int GPIO_Out_Pin::getValue(){
	return value;
}
*/

// ====== Ledstrip definitions

Ledstrip::Ledstrip(String name): name(name)
{
}

// ====== LedstripRGB definitions

LedstripRGB::LedstripRGB(String name, Adafruit_PWMServoDriver &pwmDriver, int pinR, int pinG, int pinB): Ledstrip(name), gpio{GPIO_Out_PWMServoDriver(pwmDriver, pinR), GPIO_Out_PWMServoDriver(pwmDriver, pinG), GPIO_Out_PWMServoDriver(pwmDriver, pinB)}
{
}

String LedstripRGB::getInfo(){
	String json = "{\"name\": \"" + name + "\", \"r\": " + String(gpio[0].getValue()) + ", \"g\": " + String(gpio[1].getValue()) + ", \"b\": " + String(gpio[2].getValue()) + "}";
	return json;
}

void LedstripRGB::turnOn(){
  	setValue(255, 255, 255);
}

void LedstripRGB::turnOff(){
  	setValue(0, 0, 0);
}

void LedstripRGB::setValue(int valR, int valG, int valB){
	gpio[0].setValue(valR);
	gpio[1].setValue(valG);
	gpio[2].setValue(valB);
}

// ====== LedstripRGBW definitions

LedstripRGBW::LedstripRGBW(String name, Adafruit_PWMServoDriver &pwmDriver, int pinR, int pinG, int pinB, int pinW): 
	Ledstrip(name), 
	gpio{GPIO_Out_PWMServoDriver(pwmDriver, pinR), GPIO_Out_PWMServoDriver(pwmDriver, pinG), GPIO_Out_PWMServoDriver(pwmDriver, pinB), GPIO_Out_PWMServoDriver(pwmDriver, pinW)}
{
}

String LedstripRGBW::getInfo(){
	String json = "{\"name\": \"" + name + 
	"\", \"r\": " + String(gpio[0].getValue()) + 
	", \"g\": " + String(gpio[1].getValue()) + 
	", \"b\": " + String(gpio[2].getValue()) + 
	", \"w\": " + String(gpio[3].getValue()) + 
	", \"transition\": " + String(transition) + 
	", \"animType\": " + String(animType) + 
	", \"animSpeed\": " + String(animSpeed) + 
	"}";
	return json;
}

void LedstripRGBW::turnOn(){
	setValue(255, 255, 255, 255);
}

void LedstripRGBW::turnOff(){
	setValue(0, 0, 0, 0);
	transition = false;
	setAnimType(0);
}

void LedstripRGBW::setValue(int valR, int valG, int valB, int valW){
	gpio[0].setValue(valR);
	gpio[1].setValue(valG);
	gpio[2].setValue(valB);
	gpio[3].setValue(valW);
}

void LedstripRGBW::setValueHSV(int h, int s, int v){
	if (!(0 <= h && h < 360) || !(0 <= s && s <= 100) || !(0 <= v && v <= 100))
		return;

	float s2 = s/100.0;
	float v2 = v/100.0;
	float r2 = 0;
	float g2 = 0;
	float b2 = 0;
	
	float c = v2 * s2;
	float x = c * (1 - fabs(fmod((h/60.0),2) - 1));
	float m = v2 - c;

	if (0 <= h && h < 60){
		r2 = c; g2 = x; b2 = 0;
	}
	else if (60 <= h && h < 120){
		r2 = x; g2 = c; b2 = 0;
	}
	else if (120 <= h && h < 180){
		r2 = 0; g2 = c; b2 = x;
	}
	else if (180 <= h && h < 240){
		r2 = 0; g2 = x; b2 = c;
	}
	else if (240 <= h && h < 300){
		r2 = x; g2 = 0; b2 = c;
	}
	else if (300 <= h && h < 360){
		r2 = c; g2 = 0; b2 = x;
	}

	int r = (r2+m)*255;
	int g = (g2+m)*255;
	int b = (b2+m)*255;

	gpio[0].setValue(r);
	gpio[1].setValue(g);
	gpio[2].setValue(b);
}

void LedstripRGBW::colorTransition(int valNewR, int valNewG, int valNewB, int valNewW, unsigned long transitionTime)
{
	startTime = millis();
	transition = true;
	valOldRGBW[0] = gpio[0].getValue();
	valOldRGBW[1] = gpio[1].getValue();
	valOldRGBW[2] = gpio[2].getValue();
	valOldRGBW[3] = gpio[3].getValue();
	valNewRGBW[0] = valNewR;
	valNewRGBW[1] = valNewG;
	valNewRGBW[2] = valNewB;
	valNewRGBW[3] = valNewW;
	this->transitionTime = transitionTime;
}
void LedstripRGBW::colorTransitionUpdate()
{
	if (transition){
		unsigned long timeDifference = millis() - startTime;

		gpio[0].setValue(map(timeDifference, 0, transitionTime, valOldRGBW[0], valNewRGBW[0]));
		gpio[1].setValue(map(timeDifference, 0, transitionTime, valOldRGBW[1], valNewRGBW[1]));
		gpio[2].setValue(map(timeDifference, 0, transitionTime, valOldRGBW[2], valNewRGBW[2]));
		gpio[3].setValue(map(timeDifference, 0, transitionTime, valOldRGBW[3], valNewRGBW[3]));

		if (timeDifference >= transitionTime)
		{
			transition = false;
		}
	}
}

void LedstripRGBW::setAnimType(int type){
	if (type <= 0){
		animType = 0;
	}
	else{
		animType = type;
	}
}
void LedstripRGBW::setAnimSpeed(int speed){
	if (true) // set conditions
		animSpeed = speed;
}
void LedstripRGBW::animate(){
	unsigned long animTime = millis();

	switch (animType)
	{
	case 1:
		animRainbow((animTime/animSpeed)%360);
		break;
	case 2:
		animRandom((animTime/animSpeed)%360);
		break;
	case 3:
		animRandomSmooth((animTime/animSpeed)%360);
		break;
	case 4:
		animRandomBlink((animTime/animSpeed)%360);
		break;
	
	default:
		break;
	}
}
void LedstripRGBW::animRainbow(int animTime){
	setValueHSV(animTime, 100, 100);
}
void LedstripRGBW::animRandom(int animTime){
	static bool flag = false;
	if(animTime % 180 < 90) {
		if (!flag){
			flag = true;

			int hue = random(359);
			setValueHSV(hue, 100, 100);
		}
	}else{
		flag = false;
	}
}
void LedstripRGBW::animRandomSmooth(int animTime){
	static bool flag = false;
	if(animTime % 180 < 90) {
		if (!flag){
			flag = true;

			int hue = random(359);
			setValueHSV(hue, 100, 100);
			// Use colorTransition with hsv
		}
	}else{
		flag = false;
	}
}
void LedstripRGBW::animRandomBlink(int animTime){
	static bool flag = false;
	static int hue = 0;
	if(animTime < 180) {
		if (!flag){
			flag = true;

			hue = random(359);
		}
		byte value = map(animTime, 0, 179, 0, 100);
		setValueHSV(hue, 100, value);
	}else{
		flag = false;

		byte value = map(animTime, 180, 359, 100, 0);
		setValueHSV(hue, 100, value);
	}
}

// ====== LedstripW definitions

LedstripW::LedstripW(String name, Adafruit_PWMServoDriver &pwmDriver, int pinW, bool binary): Ledstrip(name), gpio(GPIO_Out_PWMServoDriver(pwmDriver, pinW)), binary(binary)
{
}

String LedstripW::getInfo(){
	String json = "{\"name\": \"" + name + "\", \"w\": " + String(gpio.getValue()) + "}";
	return json;
}

void LedstripW::turnOn(){
  	setValue(255);
}

void LedstripW::turnOff(){
  	setValue(0);
}

void LedstripW::turnOnOff(){
	if (gpio.getValue() == 0){
		turnOn();
	}
	else {
		turnOff();
	}
}

void LedstripW::setValue(int valW){
	gpio.setValue(valW);
}

// ====== LedstripRemote definitions

// LedstripRemote::LedstripRemote(String name, String url, String urlOn, String urlOff): Ledstrip(name), url(url), urlOn(urlOn), urlOff(urlOff)
// {
// }

// String LedstripRemote::getInfo(){
// 	String json = "{\"name\": \"" + name + "\", \"url\": \"" + url + "\", \"state\": \"" + String(getValue()) + "\"}";
// 	return json;
// }

// void LedstripRemote::turnOn(){
//   	sendRequest(urlOn);
// }

// void LedstripRemote::turnOff(){
//   	sendRequest(urlOff);
// }

// void LedstripRemote::turnOnOff(){
// 	bool state = getValue() == "On" ? true : false;
//   	//if (getValue() == "On")
// 	if (state)
// 	{
// 		turnOff();
// 	}
// 	else
// 	{
// 		turnOn();
// 	}
// }

// String LedstripRemote::getValue(){
// 	return sendRequest(url);
// }

// String LedstripRemote::sendRequest(String url){
// 	WiFiClient client;
// 	HTTPClient http;
// 	String payload = "";
	
// 	http.begin(client, url); // Specify request destination
	
// 	int httpCode = http.GET(); // Send the request

// 	if (httpCode > 0) // Check the returning code
// 	{
// 		payload = http.getString(); // Get the request response payload
// 	}

// 	http.end();

// 	return payload;
// }

LedstripRemote::LedstripRemote(String name, String urlOn, String urlOff): Ledstrip(name), urlOn(urlOn), urlOff(urlOff)
{
}

String LedstripRemote::getInfo(){
	String json = "{\"name\": \"" + name + "\", \"value\": \"" + String(getValue()) + "\", \"urlOn\": \"" + urlOn + "\", \"urlOff\": \"" + urlOff + "\"}";
	return json;
}

void LedstripRemote::turnOn(){
  	sendRequest(urlOn);
	value = 255;
}

void LedstripRemote::turnOff(){
  	sendRequest(urlOff);
	value = 0;
}

void LedstripRemote::turnOnOff(){
	//if (value != -1)
	//{
		if (value == 0)
		{
			turnOn();
		}
		else
		{
			turnOff();
		}
	//}
}

int LedstripRemote::getValue(){
	return value;
}

String LedstripRemote::sendRequest(String url){
	WiFiClient client;
	HTTPClient http;
	String payload = "";
	
	http.begin(client, url); // Specify request destination
	
	int httpCode = http.GET(); // Send the request

	if (httpCode > 0) // Check the returning code
	{
		payload = http.getString(); // Get the request response payload
	}
	else {
		// No response
		value = -1;
	}

	http.end();

	return payload;
}

}