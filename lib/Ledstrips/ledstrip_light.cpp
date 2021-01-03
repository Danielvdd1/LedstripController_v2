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

LedstripRGBW::LedstripRGBW(String name, Adafruit_PWMServoDriver &pwmDriver, int pinR, int pinG, int pinB, int pinW): Ledstrip(name), gpio{GPIO_Out_PWMServoDriver(pwmDriver, pinR), GPIO_Out_PWMServoDriver(pwmDriver, pinG), GPIO_Out_PWMServoDriver(pwmDriver, pinB), GPIO_Out_PWMServoDriver(pwmDriver, pinW)}
{
}

String LedstripRGBW::getInfo(){
	String json = "{\"name\": \"" + name + "\", \"r\": " + String(gpio[0].getValue()) + ", \"g\": " + String(gpio[1].getValue()) + ", \"b\": " + String(gpio[2].getValue()) + ", \"w\": " + String(gpio[3].getValue()) + "}";
	return json;
}

void LedstripRGBW::turnOn(){
  	setValue(255, 255, 255, 255);
}

void LedstripRGBW::turnOff(){
  	setValue(0, 0, 0, 0);
}

void LedstripRGBW::setValue(int valR, int valG, int valB, int valW){
	gpio[0].setValue(valR);
	gpio[1].setValue(valG);
	gpio[2].setValue(valB);
	gpio[3].setValue(valW);
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
		if (value > 0)
		{
			turnOff();
		}
		else
		{
			turnOn();
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