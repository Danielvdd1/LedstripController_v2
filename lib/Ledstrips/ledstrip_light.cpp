#include <Arduino.h> // Required for Visual Studio Code
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "ledstrip_light.hpp"

namespace ledstripLights {

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


//st::cout << "Debug" << '\n'; // Remove

// ====== GPIO definitions

GPIO_Out_PWMServoDriver::GPIO_Out_PWMServoDriver(int pin): pin(pin), value(0)
{
	
}

void GPIO_Out_PWMServoDriver::setValue(int value){
  	this->value = value;
	pwm.setPWM(pin, 0, value * 16);
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

LedstripRGB::LedstripRGB(String name, Adafruit_PWMServoDriver &pwmDriver, int pinR, int pinG, int pinB): Ledstrip(name), pwmDriver(pwmDriver), gpio{GPIO_Out_PWMServoDriver(pinR), GPIO_Out_PWMServoDriver(pinG), GPIO_Out_PWMServoDriver(pinB)}
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

LedstripRGBW::LedstripRGBW(String name, Adafruit_PWMServoDriver &pwmDriver, int pinR, int pinG, int pinB, int pinW): Ledstrip(name), pwmDriver(pwmDriver), gpio{GPIO_Out_PWMServoDriver(pinR), GPIO_Out_PWMServoDriver(pinG), GPIO_Out_PWMServoDriver(pinB), GPIO_Out_PWMServoDriver(pinW)}
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

LedstripW::LedstripW(String name, Adafruit_PWMServoDriver &pwmDriver, int pinW, bool binary): Ledstrip(name), pwmDriver(pwmDriver), gpio(GPIO_Out_PWMServoDriver(pinW)), binary(binary)
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
	gpio = valW;
}

// ====== LedstripRemote definitions

LedstripRemote::LedstripRemote(String name, String urlOn, String urlOff): Ledstrip(name), urlOn(urlOn), urlOff(urlOff)
{
}

String LedstripRemote::getInfo(){
	//String json = "{\"name\": " + name + ", \"w\": " + String(gpio.getValue()) + "}"; // Put url requests in gpio object.
	//return json;
	String json = "{\"name\": \"" + name + "\"}";
	return json;
}

void LedstripRemote::turnOn(){
  	//TODO:
  	// HTTP request to urlOn
}

void LedstripRemote::turnOff(){
  	//TODO:
  	// HTTP request to urlOff
}

void LedstripRemote::setValue(int valW){
	//?
}


}