#include <Arduino.h> // Required for Visual Studio Code


#include "ledstrip_light.hpp"

namespace ledstripLights {

PSU::PSU(int pin): pin(pin)
{
	pinMode(pin, OUTPUT);
}

String PSU::getInfo(){
	String json = "{\"state\": " + String(state) + "}";
	return json;
}

void PSU::setState(bool state){
	this->state = state;
	digitalWrite(pin, state);
}
bool PSU::getState(){
	return state;
}

}