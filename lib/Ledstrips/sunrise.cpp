#include <Arduino.h> // Required for Visual Studio Code
#include <WiFiUdp.h>
#include <NTPClient.h>


#include "ledstrip_light.hpp"

namespace ledstripLights {

Sunrise::Sunrise(LedstripRGBW &ledstrip, NTPClient &timeClient, int sunriseTime, int sunriseDuration): ledstrip(ledstrip), timeClient(timeClient), sunriseTime(sunriseTime), sunriseDuration(sunriseDuration)
{
}

String Sunrise::getInfo(){
	String json = "{\"sunriseEnabled\": \"" + String(sunriseEnabled) + "\", \"sunriseTime\": " + String(sunriseTime) + ", \"sunriseDuration\": " + String(sunriseDuration) + ", \"sunriseState\": " + String(sunriseState) + "}";
	return json;
}

void Sunrise::setEnabled(bool sunriseEnabled){
	this->sunriseEnabled = sunriseEnabled;
}
bool Sunrise::getEnabled(){
	return sunriseEnabled;
}

void Sunrise::setTime(int sunriseTime){
	sunriseTime = min(max(sunriseTime, 0), 24 * 60 - 1); // Minutes
	this->sunriseTime = sunriseTime;
}

void Sunrise::setDuration(int sunriseDuration){
	sunriseDuration = min(max(sunriseDuration, 0), 60 - 1); // Minutes
	this->sunriseDuration = sunriseDuration;
}

void Sunrise::reset(){
	sunriseState = 0;
	//interval = 0;
}

void Sunrise::sunriseUpdate()
{
	if (!sunriseEnabled){
		return;
	}

	unsigned long currentMillis = millis();
	static unsigned long previousMillis = 0;

	if (sunriseState == 0){ // Calculate hours to wait
		timeClient.update();

		int time = timeClient.getHours() * 60 + timeClient.getMinutes();

		if (time < sunriseTime-60){
			interval = ((sunriseTime-60) - time) * 60 * 1000;
		}
		else{
			interval = ((sunriseTime-60) + (24*60 - time)) * 60 * 1000;
		}
		
		sunriseState = 1;
	}
	if (sunriseState == 1){ // Wait hours
		if (currentMillis - previousMillis >= interval) {
			previousMillis = currentMillis;

			sunriseState = 2;
		}
	}

	if (sunriseState == 2){ // Calculate minutes to wait
		timeClient.update();

		int time = timeClient.getHours() * 60 + timeClient.getMinutes();

		if (time < sunriseTime-sunriseDuration){
			interval = ((sunriseTime-sunriseDuration) - time) * 60 * 1000;
		}
		else{
			//Error?
			sunriseState = 0;
		}

		sunriseState = 3;
	}
	if (sunriseState == 3){ // Wait minutes
		if (currentMillis - previousMillis >= interval) {
			previousMillis = currentMillis;

			sunriseState = 4;
		}
	}

	unsigned long delayTime1 = roundf(sunriseDuration * 60 * 1000 * 0.8);
	unsigned long delayTime2 = roundf(sunriseDuration * 60 * 1000 * 0.2);

	if (sunriseState == 4){ // Set sunrise transition
		ledstrip.setAnimType(0);
		ledstrip.setValue(0, 0, 0, 0);
		ledstrip.colorTransition(255, 70, 0, 0, delayTime1);
		sunriseState = 5;
	}
	if (sunriseState == 5){ // Wait for sunrise transition
		if (currentMillis - previousMillis >= delayTime1) {
			previousMillis = currentMillis;

			sunriseState = 6;
		}
	}

	if (sunriseState == 6){ // Set sunrise transition
		ledstrip.colorTransition(200, 70, 0, 200, delayTime2);
		sunriseState = 7;
	}
	if (sunriseState == 7){ // Wait for sunrise transition
		if (currentMillis - previousMillis >= delayTime2) {
			previousMillis = currentMillis;

			sunriseState = 8;
		}
	}
	
	if (sunriseState == 8){ // Calculate safety delay to avoid double sunrise
		interval = 60 * 60 * 1000; // Wait one hour

		sunriseState = 9;
	}
	if (sunriseState == 9){ // Wait safety delay
		if (currentMillis - previousMillis >= interval) {
			previousMillis = currentMillis;

			sunriseState = 0;
		}
	}
}

}