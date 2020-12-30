#include <Arduino.h> // Required for Visual Studio Code

#include "ledstrip_light.hpp"

namespace ledstripLights {


//st::cout << "Debug" << '\n'; // Remove

// ====== LedstripRGB definitions

GPIOOut::GPIOOut(int pin): pin(pin), value(0)
{
	//pinMode(pin, OUTPUT);
}

void GPIOOut::setValue(int value){
  	this->value = value;
}

int GPIOOut::getValue(){
	//value = digitalRead(pin);
	return value;
}

// ====== Ledstrip definitions

Ledstrip::Ledstrip(String name): name(name), brightness(255)
{
}

// ====== LedstripRGB definitions

LedstripRGB::LedstripRGB(String name, int pinR, int pinG, int pinB): Ledstrip(name), gpio{GPIOOut(pinR), GPIOOut(pinG), GPIOOut(pinB)}
{
}

String LedstripRGB::getInfo(){
	String result = "";

	result += "LedstripRGB: ";
	result += name;
	result += '\n';

	result += "PinR: ";
	result += String(gpio[0].getValue());
	result += "\n";
	result += "PinG: ";
	result += String(gpio[1].getValue());
	result += "\n";
	result += "PinB: ";
	result += String(gpio[2].getValue());

	/*
	for (int i = 3 - 1; i >= 0; i--)
	{
		result += "Pin";
		result += i;
		result += ": ";
		result += gpio[i].getValue();
		result += "\n";
	}
	*/

	return result;
}

void LedstripRGB::turnOn(){
  //TODO:
}

void LedstripRGB::turnOff(){
  //TODO:
}

void LedstripRGB::setValue(int valR, int valG, int valB){
	gpio[0].setValue(valR);
	gpio[1].setValue(valG);
	gpio[2].setValue(valB);
}

// ====== LedstripRGBW definitions

LedstripRGBW::LedstripRGBW(String name, int pinR, int pinG, int pinB, int pinW): Ledstrip(name), gpio{GPIOOut(pinR), GPIOOut(pinG), GPIOOut(pinB), GPIOOut(pinW)}
{
}

String LedstripRGBW::getInfo(){
	String result = "";

	result += "LedstripRGBW: ";
	result += name;
	result += '\n';

	result += "PinR: ";
	result += String(gpio[0].getValue());
	result += "\n";
	result += "PinG: ";
	result += String(gpio[1].getValue());
	result += "\n";
	result += "PinB: ";
	result += String(gpio[2].getValue());
	result += "\n";
	result += "PinW: ";
	result += String(gpio[3].getValue());

	return result;
}

void LedstripRGBW::turnOn(){
  //TODO:
}

void LedstripRGBW::turnOff(){
  //TODO:
}

void LedstripRGBW::setValue(int valR, int valG, int valB, int valW){
	gpio[0].setValue(valR);
	gpio[1].setValue(valG);
	gpio[2].setValue(valB);
	gpio[3].setValue(valW);
}

// ====== LedstripW definitions

LedstripW::LedstripW(String name, int pinW, bool binary): Ledstrip(name), gpio(GPIOOut(pinW)), binary(binary)
{
}

String LedstripW::getInfo(){
	String result = "";

	result += "LedstripW: ";
	result += name;
	result += '\n';

	result += "PinW: ";
	result += String(gpio.getValue());

	return result;
}

void LedstripW::turnOn(){
  //TODO:
}

void LedstripW::turnOff(){
  //TODO:
}

void LedstripW::setValue(int valW){
	gpio = valW;
}

// ====== LedstripRemote definitions

LedstripRemote::LedstripRemote(String name, String urlOn, String urlOff): Ledstrip(name), urlOn(urlOn), urlOff(urlOff)
{
}

String LedstripRemote::getInfo(){
	String result = "";

	result += "LedstripRemote: ";
	result += name;

	//result += "PinW: ";
	//result += st::to_string(gpio.getValue());

	return result;
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