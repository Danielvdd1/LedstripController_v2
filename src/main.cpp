/*
	Author: Daniël van der Drift

	Project: Ledstrip controller
*/

// ----- Libraries -----
#include <Arduino.h> // Required for Visual Studio Code

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h> // Did not work
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h> // Http request

#include <DNSServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager



#include <NTPClient.h>
#include <WiFiUdp.h>


// ----- Wifi settings -----
// Wificredentials are handled by WifiManager library
ESP8266WebServer server(80);

// Webpages html
#include "Webserver\PageMenu.hpp"
#include "Webserver\PageColorPicker.hpp"
#include "Webserver\PageSunrise.hpp"

// IO
const byte tempPin = A0;

// ----- Ledstrips -----
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver();

#include "ledstrip_light.hpp"
namespace ll = ledstripLights;

const byte amountRGB = 0;
ll::LedstripRGB ledstripsRGB[amountRGB];
const byte amountRGBW = 2;
ll::LedstripRGBW ledstripsRGBW[amountRGBW] = {ll::LedstripRGBW("Bureau", pwmDriver, 7, 6, 8, 12), ll::LedstripRGBW("Bed", pwmDriver, 4, 3, 5, 11)};
const byte amountW = 1;
ll::LedstripW ledstripsW[amountW] = {ll::LedstripW("Schuineluik", pwmDriver, 9)};
const byte amountRemote = 1; // Put in LedstripW ???
ll::LedstripRemote ledstripsRemote[amountRemote] = {ll::LedstripRemote("Light", "http://192.168.178.61/on", "http://192.168.178.61/off")};
const byte amount = amountRGB + amountRGBW + amountW + amountRemote;
ll::Ledstrip *ledstrips[amount];


// ----- Buttons -----
#include "button.hpp"
namespace bl = button_lib;

const byte amountButton = 3;
bl::Button buttons[amountButton] = {bl::Button("Top button", D0), bl::Button("Middle button", D6), bl::Button("Bottom button", D7)};


// ----- PSU -----
ll::PSU psu = ll::PSU(D8);


// ----- Sunrise -----
const long utcOffsetInSeconds = 3600; // UTC +1.00 // Todo: Summer/winter time change
WiFiUDP ntpUDP;
NTPClient timeClient = NTPClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
ll::Sunrise ledstripSunrise = ll::Sunrise(ledstripsRGBW[1], timeClient, psu, false, 8 * 60 + 0, 15);



// ----- Functions -----
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
void TurnOnOff();
void TurnOn();
void TurnOff();

void HandleRGBW();
void HandleHSV();
void HandleW();
void ResetColors();

void HandleSunrise();

void HandleOTA();
void OTA();

void Buttons();


void SetupIO()
{
	pwmDriver.begin();
	pwmDriver.setPWMFreq(1600);
	Wire.setClock(400000);

	ledstrips[0] = &ledstripsRGBW[0];
	ledstrips[1] = &ledstripsRGBW[1];
	ledstrips[2] = &ledstripsW[0];
	ledstrips[3] = &ledstripsRemote[0];

	ResetColors();

	//pinMode(tempPin, INPUT);
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
	server.on("/", HandlePageMainHTML);
	server.on("/PageMain.css", HandlePageMainCSS);
	server.on("/PageMain.js", HandlePageMainJS);

	server.on("/colorpicker", HandlePageColorPickerHTML);
	server.on("/PageColorPicker.css", HandlePageColorPickerCSS);
	server.on("/PageColorPicker.js", HandlePageColorPickerJS);
	
	server.on("/sunrise", HandlePageSunriseHTML);
	server.on("/PageSunrise.css", HandlePageSunriseCSS);
	server.on("/PageSunrise.js", HandlePageSunriseJS);
	

	server.on("/debug", HandleDebug);
	server.on("/test", HandleTest);

	server.on("/onoff", HandleOnOff);
	server.on("/sendrgbw", HandleRGBW);
	server.on("/sendhsv", HandleHSV);
	server.on("/sendw", HandleW);
	server.on("/sendsunrise", HandleSunrise);

	server.on("/ota", HandleOTA);

	server.begin();
}
void setup()
{
	SetupIO();
	SetupWiFiManager();
	SetupOTA();
	SetupServer();
	
	// Time for sunrise
	timeClient.begin();
}

void loop()
{
	Buttons();

	server.handleClient();

	for (auto &ledstrip : ledstripsRGBW)
	{
		ledstrip.colorTransitionUpdate();
		ledstrip.animate();
	}

	ledstripSunrise.sunriseUpdate();
}


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


void HandleDebug()
{
	// /debug

	String text = "Debug text:<br><br>";
	text += "Ledstrips:<br>";
	for (auto &ledstrip : ledstrips)
	{
		text += ledstrip->getInfo();
		text += "<br>";
	}
	text += "<br>";
	text += "Buttons:<br>";
	for (auto &button : buttons)
	{
		text += button.getInfo();
		text += "<br>";
	}
	text += "<br>";
	text += "Sunrise:<br>";
	text += ledstripSunrise.getInfo();
	text += "<br>";
	text += "<br>";
	text += "PSU:<br>";
	text += psu.getInfo();
	text += "<br>";

	server.send(200, "text/html", text);
}
void HandleTest()
{
	// /test?x=___&y=___&z=___
	int valX = server.arg("x").toInt();
	int valY = server.arg("y").toInt();
	int valZ = server.arg("z").toInt();
	String debugText = "x: " + String(valX) + ", y: " + String(valY) + ", z: " + String(valZ);
	debugText += "<br><br>";

	//debugText += ;
	
	server.send(200, "text/html", debugText);
}


void HandleOnOff()
{
	// /onoff?s=___
	byte val = server.arg("s").toInt();

	if (val)
	{
		TurnOnOff();
	}

	server.send(200, "text/plain", psu.getState() ? "On" : "Off");
}

void TurnOnOff() {
	if (!psu.getState())
	{
		TurnOn();
	}
	else
	{
		TurnOff();
	}
}
void TurnOn() {
	psu.setState(true);
	//ledstripsRGBW[0].setValue(0,0,0,255);
	ledstripsRGBW[0].colorTransition(0,0,0,255);
}
void TurnOff() {
	ResetColors();
	psu.setState(false);
}


void HandleRGBW()
{
	// /sendrgbw?id=___&r=___&g=___&b=___&w=___&at=___&as=___
	int valId = server.arg("id").toInt(); valId = valId-1;
	int valR = server.arg("r").toInt(); if (valR > 0) {valR = min(max(valR-1, 0), 255);}
	int valG = server.arg("g").toInt(); if (valG > 0) {valG = min(max(valG-1, 0), 255);}
	int valB = server.arg("b").toInt(); if (valB > 0) {valB = min(max(valB-1, 0), 255);}
	int valW = server.arg("w").toInt(); if (valW > 0) {valW = min(max(valW-1, 0), 255);}
	int valAT = server.arg("at").toInt();
	int valAS = server.arg("as").toInt();

	if (valId >= 0 && valId < amountRGBW)
	{
		if (!psu.getState()) psu.setState(true);

		if (valAT > 0 || valAS > 0){ // Something with animation
			if (valAT > 0){
				valAT = max(valAT-1, 0);
				ledstripsRGBW[valId].setAnimType(valAT);
			}
			if (valAS > 0){
				valAS = max(valAS-1, 1);
				ledstripsRGBW[valId].setAnimSpeed(valAS);
			}
		}
		else{
			ledstripsRGBW[valId].setAnimType(0); // Turn of animation

			//ledstripsRGBW[valId].setValue(valR, valG, valB, valW);
			ledstripsRGBW[valId].colorTransition(valR, valG, valB, valW);
		}
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
void HandleHSV()
{
	// /sendhsv?id=___&h=___&s=___&v=___ //&w=___
	int valId = server.arg("id").toInt(); valId = valId-1;
	int valH = server.arg("h").toInt(); if (valH > 0) valH = min(max(valH-1, 0), 359);
	int valS = server.arg("s").toInt(); if (valS > 0) valS = min(max(valS-1, 0), 100);
	int valV = server.arg("v").toInt(); if (valV > 0) valV = min(max(valV-1, 0), 100);
	//int valW = server.arg("w").toInt(); if (valW > 0) valW = min(max(valW-1, 0), 255);

	if (valId >= 0 && valId < amountRGBW)
	{
		if (!psu.getState()) psu.setState(true);

		ledstripsRGBW[valId].setValueHSV(valH, valS, valV);
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
	int valId = server.arg("id").toInt(); valId = valId-1;
	int valW = server.arg("w").toInt(); if (valW > 0) valW = min(max(valW-1, 0), 255);

	if (valId >= 0 && valId < amountW)
	{
		if (!psu.getState()) psu.setState(true);

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


void HandleSunrise()
{
	// /sendsunrise?e=___&t=___
	int valE = server.arg("e").toInt();
	int valT = server.arg("t").toInt();
	int valD = server.arg("d").toInt();

	if (valE > 0){
		valE = max(valE-1, 0);
		if (valE > 0){
			ledstripSunrise.setEnabled(true);
		} else {
			ledstripSunrise.setEnabled(false);
			ledstripSunrise.reset();
		}
	}

	if (valT > 0){
		valT = min(max(valT-1, 0), 24 * 60 - 1); // Minutes
		
		// Set time variable
		ledstripSunrise.setTime(valT);
	}

	if (valD > 0){
		valD = min(max(valD-1, 0), 60 - 1); // Minutes
		
		// Set duration variable
		ledstripSunrise.setDuration(valD);
	}

	server.send(200, "text/json", ledstripSunrise.getInfo());
}


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
	if (buttons[0].getState() && buttons[1].getState() && buttons[2].getState())
	{
		OTA();
	}

	if (buttons[0].isPressed())
	{ // On/off
		TurnOnOff();
	}
	if (buttons[1].isPressed())
	{ // Lamp1
		ledstripsRemote[0].turnOnOff();
	}
	if (buttons[2].isPressed())
	{ // Schuineluik
		psu.setState(true);
		ledstripsW[0].turnOnOff();
	}
}