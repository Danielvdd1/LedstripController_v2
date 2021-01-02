#include <Arduino.h> // Required for Visual Studio Code

#include "button.hpp"

namespace button_lib {


// ====== Button definitions

Button::Button(String name, int pin): name(name), pin(pin), debounce(50), startTime(0), buttonFlg(false)
{
	pinMode(pin, INPUT);
}

String Button::getInfo(){
  	String json = "{\"name\": \"" + name + "\", \"getState\": " + String(getState()) + ", \"isPressed\": " + String(isPressed()) + "}";
	return json;
}

int Button::getState(){
  	return digitalRead(pin);
}

boolean Button::isPressed(){
	// Give '1' instant once if button is pressed
	// Button must be released 50 ms before a new '1' can be given

	if (digitalRead(pin))
	{
		startTime = millis();
		if (!buttonFlg)
		{
			buttonFlg = 1;
			return (1);
		}
	}
	else
	{
		if ((millis() - startTime) > debounce)
			buttonFlg = 0;
	}
	return (0);
}

}