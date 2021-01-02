/*
	Author: Daniël van der Drift

	Project: Ledstrip controller
*/

// Libraries
#include <Arduino.h> // Required for Visual Studio Code

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h> // Did not work
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h> // Http request

#include <DNSServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

// Wifi settings
// Wificredentials handled by WifiManager library
ESP8266WebServer server(80);

// Webpages html
#include "Webserver\PageMenu.h"
#include "Webserver\PageColorPicker.h"
#include "Webserver\PageSunrise.h"

// IO
Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver();
const byte buttonPin[3] = {D0, D6, D7};
const byte tempPin = A0;
const byte PSUPin = D8;


// Variables
bool onoff = false;
/*
byte animActive;
byte animSpeed = 30; // 4=1sec, 8=2sec, 20=5sec, 40=10sec

// Sunrise
const long utcOffsetInSeconds = 3600; // UTC +1.00
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
bool sunriseEnabled = true;
int sunriseState = 0;
int sunriseTime = 8 * 60 + 0; // Minutes
const int sunriseDuration = 15; // Minutes // Not more than 59 minutes
unsigned long previousMillis = 0;
unsigned long  interval = 1000;
*/

// Functions
void HandlePageMainHTML();
void HandlePageMainCSS();
void HandlePageMainJS();
void HandlePageColorPickerHTML();
void HandlePageColorPickerCSS();
void HandlePageColorPickerJS();
void HandlePageSunriseHTML();
void HandlePageSunriseCSS();
void HandlePageSunriseJS();

void HandleDebug();
void HandleTest();

void HandleOnOff();
void HandleRGBW();
void HandleW();
void HandleAnim();
void HandleSunrise();

void TurnOnOff();
void TurnOn();
void TurnOff();
void ColorTransition(byte i, bool firstTime);
void ColorAnimation(byte anim);
void Sunrise();
void SunriseTransition(bool firstTime);
void SetColors(byte i);
void ResetColors();

void HandleOTA();
void OTA();

void Buttons();
boolean button(byte i);

#include "..\lib\Ledstrips\ledstrip_light.hpp"
namespace ll = ledstripLights;

const byte amountRGB = 0;
ll::LedstripRGB ledstripsRGB[amountRGB];
const byte amountRGBW = 2;
ll::LedstripRGBW ledstripsRGBW[amountRGBW] = {ll::LedstripRGBW("Bureau", pwmDriver, 7, 6, 8, 12), ll::LedstripRGBW("Bed", pwmDriver, 4, 3, 5, 11)};
const byte amountW = 1;
ll::LedstripW ledstripsW[amountW] = {ll::LedstripW("Schuineluik", pwmDriver, 9)};
const byte amountRemote = 0;
ll::LedstripRemote ledstripsRemote[amountRemote]; // Put in LedstripW ???
const byte amount = amountRGB + amountRGBW + amountW + amountRemote;
ll::Ledstrip *ledstrips[3];



void SetupIO()
{ // Replace this by object instantiation

	pwmDriver.begin();
	pwmDriver.setPWMFreq(1600);
	Wire.setClock(400000);

	ledstrips[0] = &ledstripsRGBW[0];
	ledstrips[1] = &ledstripsRGBW[1];
	ledstrips[2] = &ledstripsW[0];

	ResetColors();

	for (byte i = 0; i < sizeof(buttonPin) / sizeof(buttonPin[0]); i++)
	{
		pinMode(buttonPin[i], INPUT);
	}
	//pinMode(tempPin, INPUT);
	pinMode(PSUPin, OUTPUT); // Test
	digitalWrite(PSUPin, HIGH); // Test
}
void SetupWiFiManager()
{
	// WiFiManager
	WiFiManager wifiManager;
	//wifiManager.resetSettings(); // Reset saved settings

	//wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // Set custom ip for portal

	// Fetches ssid and pass from eeprom and tries to connect
	// if it does not connect it starts an access point with the specified name
	// here  "AutoConnectAP"
	// and goes into a blocking loop awaiting configuration
	wifiManager.autoConnect("AutoConnectAP");
	// Or use this for auto generated name ESP + ChipID
	//wifiManager.autoConnect();
}
void SetupOTA()
{
	//ArduinoOTA.setPort(8266); // Port defaults to 8266

	ArduinoOTA.setHostname("myesp8266"); // Hostname defaults to esp8266-[ChipID]

	ArduinoOTA.setPassword("Password4H7o1xAEd6"); // No authentication by default

	// Password can be set with it's md5 value as well
	// MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
	//ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

	ArduinoOTA.onStart([]() {
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH)
		{
			type = "sketch";
		}
		else
		{ // U_SPIFFS
			type = "filesystem";
		}

		// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
		//Serial.println("Start updating " + type);
	});
	ArduinoOTA.begin();
}
void SetupServer()
{
	/*
	server.on("/", HandlePageMainHTML);
	server.on("/PageMain.css", HandlePageMainCSS);
	server.on("/PageMain.js", HandlePageMainJS);

	server.on("/colorpicker", HandlePageColorPickerHTML);
	server.on("/PageColorPicker.css", HandlePageColorPickerCSS);
	server.on("/PageColorPicker.js", HandlePageColorPickerJS);

	server.on("/sunrise", HandlePageSunriseHTML);
	server.on("/PageSunrise.css", HandlePageSunriseCSS);
	server.on("/PageSunrise.js", HandlePageSunriseJS);
	*/

	server.on("/debug", HandleDebug);
	server.on("/test", HandleTest);

	server.on("/onoff", HandleOnOff);
	server.on("/sendrgbw", HandleRGBW);
	server.on("/sendw", HandleW);
	//server.on("/sendanim", HandleAnim);
	//server.on("/sendsunrise", HandleSunrise);

	server.on("/ota", HandleOTA);

	server.begin();
}

void setup()
{
	SetupIO();
	SetupWiFiManager();
	SetupOTA();
	SetupServer();
	
	//timeClient.begin();
}

void loop()
{
	Buttons();

	server.handleClient();

	/*
	if (transition[0])
	{
		ColorTransition(0, false);
	}

	if (transition[1])
	{
		ColorTransition(1, false);
	}

	if (animActive > 0) {
		ColorAnimation(animActive);
	}

	Sunrise();
	*/
}


/* Webpages
void HandlePageMainHTML()
{
	// /

	String html = String(pageMain_html);
	server.send(200, "text/html", html);
}
void HandlePageMainCSS()
{
	String css = String(pageMain_css);
	server.send(200, "text/css", css);
}
void HandlePageMainJS()
{
	String js = String(pageMain_js);
	server.send(200, "text/js", js);
}

void HandlePageColorPickerHTML()
{
	// /colorpicker?id=___
	byte valId = server.arg("id").toInt();

	if (valId == 1 || valId == 2)
	{
		String html = String(pageColorPicker_html);
		server.send(200, "text/html", html);
	}
	else
	{
		server.send(200, "text/html", "<!DOCTYPE html><html><body><center><h1>ID does not exist</h1></center></body></html>");
	}
}
void HandlePageColorPickerCSS()
{
	String css = String(pageColorPicker_css);
	server.send(200, "text/css", css);
}
void HandlePageColorPickerJS()
{
	String js = String(pageColorPicker_js);
	server.send(200, "text/js", js);
}

void HandlePageSunriseHTML()
{
	// /sunrise

	String html = String(pageSunrise_html);
	server.send(200, "text/html", html);
}
void HandlePageSunriseCSS()
{
	String css = String(pageSunrise_css);
	server.send(200, "text/css", css);
}
void HandlePageSunriseJS()
{
	String js = String(pageSunrise_js);
	server.send(200, "text/js", js);
}
*/

void HandleDebug()
{
	// /debug

	String text = "Debug text:\n";
	for (auto &ledstrip : ledstrips)
	{
		text += ledstrip->getInfo();
		text += "<br>";
	}

	server.send(200, "text/html", text);
}
void HandleTest()
{
	// /test

	ledstripsRGBW[0].setValue(0, 0, 0, 255);

	server.send(200, "text/html", "Ok");
}


void HandleOnOff()
{
	// /onoff?s=___
	byte val = server.arg("s").toInt();

	if (val)
	{
		TurnOnOff();
	}

	server.send(200, "text/plain", onoff ? "On" : "Off");
}

void TurnOnOff() {
	onoff = !onoff;

	if (onoff)
	{
		TurnOn();
	}
	else
	{
		TurnOff();
	}
}
void TurnOn() {
	onoff = true;
	digitalWrite(PSUPin, HIGH);
	ledstripsRGBW[0].setValue(0,0,0,255);
}
void TurnOff() {
	onoff = false;
	digitalWrite(PSUPin, LOW);
	ResetColors();
}


void HandleRGBW()
{
	// /sendrgbw?id=___&r=___&g=___&b=___&w=___
	int valId = server.arg("id").toInt(); if (valId > 0) valId = valId-1;
	int valR = server.arg("r").toInt(); if (valR > 0) valR = min(max(valR-1, 255), 0);
	int valG = server.arg("g").toInt(); if (valG > 0) valG = min(max(valG-1, 255), 0);
	int valB = server.arg("b").toInt(); if (valB > 0) valB = min(max(valB-1, 255), 0);
	int valW = server.arg("w").toInt(); if (valW > 0) valW = min(max(valW-1, 255), 0);

	if (valId >= 0 && valId < amountRGBW)
	{
		if (!onoff) TurnOn();

		ledstripsRGBW[valId].setValue(valR, valG, valB, valW);
	}

	String json = "[";
	for (int i = 0; i < amountRGBW; i++)
	{
		json += ledstripsRGBW[i].getInfo();
		if (i < amountRGBW - 1) json += ",";
	}
	json += "]";
	server.send(200, "text/json", json);
}

void HandleW()
{
	// /sendw?id=___&w=___
	int valId = server.arg("id").toInt(); if (valId > 0) valId = valId-1;
	int valW = server.arg("w").toInt(); if (valW > 0) valW = min(max(valW-1, 255), 0);

	if (valId >= 0 && valId < amountW)
	{
		if (!onoff) TurnOn();

		ledstripsW[valId].setValue(valW);
	}

	String json = "[";
	for (int i = 0; i < amountW; i++)
	{
		json += ledstripsW[i].getInfo();
		if (i < amountW - 1) json += ",";
	}
	json += "]";
	server.send(200, "text/json", json);
}

void ResetColors()
{
	for (auto &ledstrip : ledstrips)
	{
		ledstrip->turnOff();
	}
}

/*
void ColorTransition(byte i, bool firstTime)
{
	const unsigned long delayTime = 1000;
	static unsigned long startTime[2];

	static byte oldValueRGBW[2][4];

	if (firstTime)
	{
		startTime[i] = millis();
		transition[i] = true;
		oldValueRGBW[i][0] = valueRGBW[i][0];
		oldValueRGBW[i][1] = valueRGBW[i][1];
		oldValueRGBW[i][2] = valueRGBW[i][2];
		oldValueRGBW[i][3] = valueRGBW[i][3];
	}

	unsigned long timeDifference = millis() - startTime[i];

	valueRGBW[i][0] = map(timeDifference, 0, delayTime, oldValueRGBW[i][0], newValueRGBW[i][0]);
	valueRGBW[i][1] = map(timeDifference, 0, delayTime, oldValueRGBW[i][1], newValueRGBW[i][1]);
	valueRGBW[i][2] = map(timeDifference, 0, delayTime, oldValueRGBW[i][2], newValueRGBW[i][2]);
	valueRGBW[i][3] = map(timeDifference, 0, delayTime, oldValueRGBW[i][3], newValueRGBW[i][3]);

	SetColors(i);

	if (timeDifference >= delayTime)
	{
		transition[i] = false;
	}
}



void HandleAnim()
{
	// /sendanim?anim=___&speed=___
	byte valAnim = server.arg("anim").toInt();
	byte valSpeed = server.arg("speed").toInt();

	if (valAnim > 0) {
		animActive = --valAnim;
		if (!onoff) TurnOn();
	}

	if (valSpeed > 0) {
		animSpeed = valSpeed;
	}

	String json = "{\"anim\": " + String(animActive) + ", \"speed\": " + String(animSpeed) + "}";
	server.send(200, "text/json", json);
}

void Wheel(int i, byte wheelPos) { // https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest_wheel/strandtest_wheel.ino
	wheelPos = 255 - wheelPos;
	if(wheelPos < 85) {
		// {255 - wheelPos * 3, 0, wheelPos * 3};
		newValueRGBW[i][0] = 255 - wheelPos * 3;
		newValueRGBW[i][1] = 0;
		newValueRGBW[i][2] = wheelPos * 3;
		newValueRGBW[i][3] = 0;
	}
	else if(wheelPos < 170) {
		wheelPos -= 85;
		//{0, wheelPos * 3, 255 - wheelPos * 3};
		newValueRGBW[i][0] = 0;
		newValueRGBW[i][1] = wheelPos * 3;
		newValueRGBW[i][2] = 255 - wheelPos * 3;
		newValueRGBW[i][3] = 0;
	}
	else {
		wheelPos -= 170;
		//{wheelPos * 3, 255 - wheelPos * 3, 0};
		newValueRGBW[i][0] = wheelPos * 3;
		newValueRGBW[i][1] = 255 - wheelPos * 3;
		newValueRGBW[i][2] = 0;
		newValueRGBW[i][3] = 0;
	}
}
void animRainbow(int i, byte wheelPos) {
	Wheel(i, wheelPos);
	for (byte j = 0; j < 4; j++)
	{
		valueRGBW[i][j] = newValueRGBW[i][j];
	}
}
void animRandom(int i, byte wheelPos) {
	wheelPos = 255 - wheelPos;
	static byte lastWheelPos[2] = {0,0};
	if(wheelPos % 85 < 43) {
		if (lastWheelPos[i] % 85 >= 43){
			lastWheelPos[i] = wheelPos;

			byte wheelPos2 = random(255);
			Wheel(i, wheelPos2);

			for (byte j = 0; j < 4; j++)
			{
				valueRGBW[i][j] = newValueRGBW[i][j];
			}
		}
	}else{
		if(lastWheelPos[i] % 85 < 43) {
			lastWheelPos[i] = wheelPos;

			// byte wheelPos2 = random(255);
			// Wheel(i, wheelPos2);

			// for (byte j = 0; j < 4; j++)
			// {
			//   valueRGBW[i][j] = newValueRGBW[i][j];
			// }
		}
	}
}
void animRandomSmooth(int i, byte wheelPos) {
	wheelPos = 255 - wheelPos;
	static byte lastWheelPos[2] = {0,0};
	if(wheelPos % 85 < 43) {
		if (lastWheelPos[i] % 85 >= 43){
			lastWheelPos[i] = wheelPos;

			byte wheelPos2 = random(255);
			Wheel(i, wheelPos2);

			ColorTransition(i, true);
		}
	}else{
		if(lastWheelPos[i] % 85 < 43) {
			lastWheelPos[i] = wheelPos;

			//byte wheelPos2 = random(255);
			//wheel(i, wheelPos2);
		}
	}
}
void animRandomBlink(int i, byte wheelPos) {
	wheelPos = 255 - wheelPos;
	static byte lastWheelPos[2] = {0,0};
	if(wheelPos % 85 < 43) {
		if (lastWheelPos[i] % 85 >= 43){
			lastWheelPos[i] = wheelPos;

			//byte wheelPos2 = random(255);
			//Wheel(i, wheelPos2);
		}
		for (byte j = 0; j < 4; j++)
		{
			valueRGBW[i][j] = map(wheelPos % 85, 0, 43, 0, newValueRGBW[i][j]);
		}
	}else{
		if(lastWheelPos[i] % 85 < 43) {
			lastWheelPos[i] = wheelPos;

			byte wheelPos2 = random(256);
			Wheel(i, wheelPos2);
		}
		for (byte j = 0; j < 4; j++)
		{
			valueRGBW[i][j] = map(wheelPos % 85, 44, 85, newValueRGBW[i][j], 0);
		}
	}
}

void ColorAnimation(byte anim)
{
	unsigned long animTime = millis();
	byte animTime2 = (animTime/animSpeed) % 255;


	switch (anim)
	{
	case 1: // Rainbow
		animRainbow(0, animTime2);
		animRainbow(1, animTime2);
		SetColors(0);
		SetColors(1);
		break;
	case 2: // Random
		animRandom(0, animTime2);
		animRandom(1, animTime2);
		SetColors(0);
		SetColors(1);
		break;
	case 3: // Random smooth
		animRandomSmooth(0, animTime2);
		animRandomSmooth(1, animTime2);
		//SetColors(0);
		//SetColors(1);
		break;
	case 4: // Random blink
		animRandomBlink(0, animTime2);
		animRandomBlink(1, animTime2);
		SetColors(0);
		SetColors(1);
		break;
	default:
		break;
	}
}

void HandleSunrise()
{
	// /sendsunrise?e=___&t=___
	byte valE = server.arg("e").toInt();
	//byte valT = server.arg("t").toInt();

	if (valE == 1){
		sunriseEnabled = true;
	}
	else if (valE == 2) {
		sunriseEnabled = false;
		sunriseState = 0;
	}

	server.send(200, "text/json", sunriseEnabled ? "1" : "0");
}
void Sunrise()
{
	if (!sunriseEnabled){
		return;
	}

	unsigned long currentMillis = millis();

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

	if (sunriseState == 4){ // Set sunrise transition
		SunriseTransition(true);
		sunriseState = 5;
	}

	if (sunriseState == 5 || sunriseState == 6){ // Do sunrise transition
		SunriseTransition(false);
	}
	// 5 -> 6 in SunriseTransition function
	
	if (sunriseState == 7){ // Calculate safety delay to avoid double sunrise
		interval = 3600 * 1000;

		sunriseState = 8;
	}

	if (sunriseState == 8){ // Wait safety delay
		if (currentMillis - previousMillis >= interval) {
			previousMillis = currentMillis;

			sunriseState = 0;
		}
	}
}
void SunriseTransition(bool firstTime)
{
	const unsigned long delayTime1 = roundf(sunriseDuration * 60 * 1000 * 0.8);
	const unsigned long delayTime2 = roundf(sunriseDuration * 60 * 1000 * 0.2);
	static unsigned long startTime;

	static byte oldValueRGBW[4];

	if (firstTime)
	{
		animActive = 0; // Turn off animation
		if (!onoff) TurnOn();

		newValueRGBW[1][0] = 255;
		newValueRGBW[1][1] = 70;
		newValueRGBW[1][2] = 0;
		newValueRGBW[1][3] = 0;

		startTime = millis();
		oldValueRGBW[0] = valueRGBW[1][0];
		oldValueRGBW[1] = valueRGBW[1][1];
		oldValueRGBW[2] = valueRGBW[1][2];
		oldValueRGBW[3] = valueRGBW[1][3];
	}

	unsigned long timeDifference = millis() - startTime;

	if (sunriseState == 5){
		if (timeDifference < delayTime1)
		{
			valueRGBW[1][0] = map(timeDifference, 0, delayTime1, oldValueRGBW[0], newValueRGBW[1][0]);
			valueRGBW[1][1] = map(timeDifference, 0, delayTime1, oldValueRGBW[1], newValueRGBW[1][1]);
			valueRGBW[1][2] = map(timeDifference, 0, delayTime1, oldValueRGBW[2], newValueRGBW[1][2]);
			valueRGBW[1][3] = map(timeDifference, 0, delayTime1, oldValueRGBW[3], newValueRGBW[1][3]);
		}

		if (timeDifference >= delayTime1)
		{
			sunriseState = 6;
			timeDifference = 0;

			newValueRGBW[1][0] = 200;
			newValueRGBW[1][1] = 70;
			newValueRGBW[1][2] = 0;
			newValueRGBW[1][3] = 255;

			startTime = millis();
			oldValueRGBW[0] = valueRGBW[1][0];
			oldValueRGBW[1] = valueRGBW[1][1];
			oldValueRGBW[2] = valueRGBW[1][2];
			oldValueRGBW[3] = valueRGBW[1][3];
		}
	}
	
	if (sunriseState == 6){
		if (timeDifference < delayTime2)
		{
			valueRGBW[1][0] = map(timeDifference, 0, delayTime2, oldValueRGBW[0], newValueRGBW[1][0]);
			valueRGBW[1][1] = map(timeDifference, 0, delayTime2, oldValueRGBW[1], newValueRGBW[1][1]);
			valueRGBW[1][2] = map(timeDifference, 0, delayTime2, oldValueRGBW[2], newValueRGBW[1][2]);
			valueRGBW[1][3] = map(timeDifference, 0, delayTime2, oldValueRGBW[3], newValueRGBW[1][3]);
		}
		if (timeDifference >= delayTime2)
		{
			sunriseState = 7;
		}
	}

	SetColors(1);
}
*/

void HandleOTA()
{
	// /ota

	OTA();

	server.send(200, "text/html", "Ok");
}
void OTA(){
	unsigned long OTAtime = millis();
	unsigned long debounce = 25000; // Time before OTA Upload
	while (millis() - OTAtime < debounce)
	{
		ArduinoOTA.handle();
		delay(10);
	}
}

void Buttons()
{
	// OTA
	if (digitalRead(buttonPin[0]) && digitalRead(buttonPin[1]) && digitalRead(buttonPin[2]))
	{
		OTA();
	}

	if (button(0))
	{ // On/off
		TurnOnOff();
	}
	if (button(1))
	{ // Lamp1

	}
	if (button(2))
	{ // Schuineluik

	}
}
boolean button(byte i) // geeft DIRECT EENMALIG een '1' als knop i ingedrukt wordt
{					   // knop i moet 50 ms los zijn voordat een nieuwe '1' gegeven kan worden
					   // in dit voorbeeld is bereik i: [0..3]

	const unsigned long debounce = 50;
	static unsigned long startTime[sizeof(buttonPin)]; // static array!!
	static boolean buttonFlg[sizeof(buttonPin)];	   // static array!!

	if (digitalRead(buttonPin[i]))
	{
		startTime[i] = millis();
		if (!buttonFlg[i])
		{
			buttonFlg[i] = 1;
			return (1);
		}
	}
	else
	{
		if ((millis() - startTime[i]) > debounce)
			buttonFlg[i] = 0;
	}
	return (0);
}