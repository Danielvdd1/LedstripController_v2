/*
  Author: Daniël van der Drift

  Project: Ledstrip controller
*/

// Libraries
#include <Arduino.h> // Required for Visual Studio Code
/*
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
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const byte buttonPin[3] = {D0, D6, D7};
const byte tempPin = A0;
const byte PSUPin = D8;

// Variables
bool transition[2] = {false, false};
byte valueRGBW[2][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
byte newValueRGBW[2][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
byte valueW[3] = {0, 0, 0};
bool onoff = false;
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

void HandleRGBW();
void HandleW();
void HandleAnim();
void HandleOnOff();
void HandleSunrise();

void TurnOn();
void TurnOff();
void ColorTransition(byte i, bool firstTime);
void ColorAnimation(byte anim);
void Sunrise();
void SunriseTransition(bool firstTime);
void SetColors(byte i);
void ResetColors();

boolean button(byte i);
void HandleButton();
*/

#include "..\lib\Ledstrips\ledstrip_light.hpp"
namespace ll = ledstripLights;

ll::LedstripRGB ledstripsRGB[1] = {ll::LedstripRGB("Bureau", 1, 2, 3)};
ll::LedstripRGBW ledstripsRGBW[0];
ll::LedstripW ledstripsW[0];
ll::LedstripRemote ledstripsRemote[0];
ll::Ledstrip* ledstrips[1];

void setup()
{
	Serial.begin(115200);



	ledstrips[0] = &ledstripsRGB[0];


	Serial.print("sizeof(ll::LedstripRGB):");
	Serial.println(sizeof(ll::LedstripRGB));
	Serial.print("sizeof(ledstripsRGB[0]):");
	Serial.println(sizeof(ledstripsRGB[0]));
	Serial.print("sizeof(ledstrips[0]):");
	Serial.println(sizeof(ledstrips[0]));
	//Serial.println(sizeof(ledstripsRGB));
	//Serial.println(sizeof(ledstrips));
	Serial.println();

	for (int i = 0; i < 1; i++){
		auto ledstrip = ledstrips[i];
		String test = ledstrip->getInfo();
		Serial.println(test);
	}
	Serial.println();

	ledstripsRGB[0].setValue(10,10,20);

	for (int i = 0; i < 1; i++){
		auto ledstrip = ledstrips[i];
		String test = ledstrip->getInfo();
		Serial.println(test);
	}
	Serial.println();



  /*
  //Serial.begin(115200);

  pwm.begin();
  pwm.setPWMFreq(1600);
  Wire.setClock(400000);
  ResetColors();

  for (byte i = 0; i < sizeof(buttonPin) / sizeof(buttonPin[0]); i++)
  {
    pinMode(buttonPin[i], INPUT);
  }
  pinMode(tempPin, INPUT);
  pinMode(PSUPin, OUTPUT);

  
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

  server.on("/", HandlePageMainHTML);
  server.on("/PageMain.css", HandlePageMainCSS);
  server.on("/PageMain.js", HandlePageMainJS);
  server.on("/colorpicker", HandlePageColorPickerHTML);
  server.on("/PageColorPicker.css", HandlePageColorPickerCSS);
  server.on("/PageColorPicker.js", HandlePageColorPickerJS);
  server.on("/sunrise", HandlePageSunriseHTML);
  server.on("/PageSunrise.css", HandlePageSunriseCSS);
  server.on("/PageSunrise.js", HandlePageSunriseJS);
  server.on("/sendrgbw", HandleRGBW);
  server.on("/sendw", HandleW);
  server.on("/sendanim", HandleAnim);
  server.on("/onoff", HandleOnOff);
  server.on("/sendsunrise", HandleSunrise);
  server.begin();

  //Serial.println("Server started");
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

  timeClient.begin();
  */
}

void loop()
{
  /*
  HandleButton();

  server.handleClient();

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

/*
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

void HandleRGBW()
{
  // /sendrgbw?id=___&r=___&g=___&b=___&w=___
  byte valId = server.arg("id").toInt();
  byte valR = server.arg("r").toInt();
  byte valG = server.arg("g").toInt();
  byte valB = server.arg("b").toInt();
  byte valW = server.arg("w").toInt();

  String json;

  if (valId > 0)
  {
    animActive = 0; // Turn off animation
    if (!onoff) TurnOn();

    newValueRGBW[valId - 1][0] = valR;
    newValueRGBW[valId - 1][1] = valG;
    newValueRGBW[valId - 1][2] = valB;
    newValueRGBW[valId - 1][3] = valW;

    ColorTransition(valId - 1, true);

    switch (valId - 1)
    {
    case 0:
      json = "[{\"r\": " + String(valR) + ", \"g\": " + String(valG) + ", \"b\": " + String(valB) + ", \"w\": " + String(valW) + "}, {\"r\": " + String(valueRGBW[1][0]) + ", \"g\": " + String(valueRGBW[1][1]) + ", \"b\": " + String(valueRGBW[1][2]) + ", \"w\": " + String(valueRGBW[1][3]) + "}]";
      break;
    case 1:
      json = "[{\"r\": " + String(valueRGBW[0][0]) + ", \"g\": " + String(valueRGBW[0][1]) + ", \"b\": " + String(valueRGBW[0][2]) + ", \"w\": " + String(valueRGBW[0][3]) + "}, {\"r\": " + String(valR) + ", \"g\": " + String(valG) + ", \"b\": " + String(valB) + ", \"w\": " + String(valW) + "}]";
      break;
    }
  }
  else
  {
    json = "[{\"r\": " + String(valueRGBW[0][0]) + ", \"g\": " + String(valueRGBW[0][1]) + ", \"b\": " + String(valueRGBW[0][2]) + ", \"w\": " + String(valueRGBW[0][3]) + "}, {\"r\": " + String(valueRGBW[1][0]) + ", \"g\": " + String(valueRGBW[1][1]) + ", \"b\": " + String(valueRGBW[1][2]) + ", \"w\": " + String(valueRGBW[1][3]) + "}]";
  }
  server.send(200, "text/json", json);
}

void HandleW()
{
  // /sendw?id=___&w=___
  byte valId = server.arg("id").toInt();
  byte valW = server.arg("w").toInt();

  if (valId > 0)
  {
    //animActive = 0; // Turn off animation

    valueW[valId - 1] = valW;

    switch (valId)
    {
    case 1:
      if (!onoff) TurnOn();
      pwm.setPWM(9, 0, (valW == 1 ? 4095 : 0));
      #ifdef ESP8266
        yield(); // take a breather, required for ESP8266
      #endif
      break;
    case 2:
      // Lamp2
      // Wall lamp
      break;
    case 3:
      // Lamp3
      // Sealing lamp
      break;
    }
  }

  String json = "{\"a\": " + String(valueW[0]) + ", \"b\": " + String(valueW[1]) + ", \"c\": " + String(valueW[2]) + "}";
  server.send(200, "text/json", json);
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

void HandleOnOff()
{
  // /onoff?s=___
  byte val = server.arg("s").toInt();

  if (val)
  {
    onoff = !onoff; // Also in TurnOn() & TurnOff()

    if (onoff)
    {
      TurnOn();

      newValueRGBW[0][0] = 0;
      newValueRGBW[0][1] = 0;
      newValueRGBW[0][2] = 0;
      newValueRGBW[0][3] = 255;
      ColorTransition(0, true);
    }
    else
    {
      TurnOff();
    }
  }

  server.send(200, "text/plain", onoff ? "On" : "Off");
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


void TurnOn() {
  //Serial.println("ON");
  digitalWrite(PSUPin, HIGH);
  onoff = true;
}
void TurnOff() {
  //Serial.println("OFF");
  digitalWrite(PSUPin, LOW);
  onoff = false;
  animActive = 0; // Turn off animation
  ResetColors();
}

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
/ * // Backup
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

  if (timeDifference < delayTime1)
  {
	valueRGBW[1][0] = map(timeDifference, 0, delayTime1, oldValueRGBW[0], newValueRGBW[1][0]);
	valueRGBW[1][1] = map(timeDifference, 0, delayTime1, oldValueRGBW[1], newValueRGBW[1][1]);
	valueRGBW[1][2] = map(timeDifference, 0, delayTime1, oldValueRGBW[2], newValueRGBW[1][2]);
	valueRGBW[1][3] = map(timeDifference, 0, delayTime1, oldValueRGBW[3], newValueRGBW[1][3]);
  }
  else
  {
	newValueRGBW[1][0] = 200;
    newValueRGBW[1][1] = 70;
    newValueRGBW[1][2] = 0;
    newValueRGBW[1][3] = 255;

	valueRGBW[1][0] = map(timeDifference, delayTime2, sunriseDuration, oldValueRGBW[0], newValueRGBW[1][0]);
	valueRGBW[1][1] = map(timeDifference, delayTime2, sunriseDuration, oldValueRGBW[1], newValueRGBW[1][1]);
	valueRGBW[1][2] = map(timeDifference, delayTime2, sunriseDuration, oldValueRGBW[2], newValueRGBW[1][2]);
	valueRGBW[1][3] = map(timeDifference, delayTime2, sunriseDuration, oldValueRGBW[3], newValueRGBW[1][3]);
  }

  SetColors(1);

  if (timeDifference >= sunriseDuration)
  {
	  sunriseState = 6;
  }
}* /

void SetColors(byte i)
{
  switch (i)
  {
  case 0:
    pwm.setPWM(7, 0, valueRGBW[0][0] * 16);
    pwm.setPWM(6, 0, valueRGBW[0][1] * 16);
    pwm.setPWM(8, 0, valueRGBW[0][2] * 16);
    pwm.setPWM(12, 0, valueRGBW[0][3] * 16);
    #ifdef ESP8266
      yield(); // take a breather, required for ESP8266
    #endif
    break;

  case 1:
    pwm.setPWM(4, 0, valueRGBW[1][0] * 16);
    pwm.setPWM(3, 0, valueRGBW[1][1] * 16);
    pwm.setPWM(5, 0, valueRGBW[1][2] * 16);
    pwm.setPWM(11, 0, valueRGBW[1][3] * 16);
    #ifdef ESP8266
      yield(); // take a breather, required for ESP8266
    #endif
    break;
  
  default:
    break;
  }
}

void ResetColors()
{
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      valueRGBW[i][j] = 0;
      newValueRGBW[i][j] = 0;
    }
  }
  for (int i = 0; i < 3; i++)
  {
    valueW[i] = 0;
  }
  //ColorTransition(0, true);
  //ColorTransition(1, true);

  //SetColors(0);
  //SetColors(1);
  for (int i = 0; i < 16; i++)
  {
    pwm.setPWM(i, 0, 0);
  }
  #ifdef ESP8266
    yield(); // take a breather, required for ESP8266
  #endif
}

void HandleButton()
{
  // OTA
  if (digitalRead(buttonPin[0]) && digitalRead(buttonPin[1]) && digitalRead(buttonPin[2]))
  {
    unsigned long OTAtime = millis();
    unsigned long debounce = 25000; // Time before OTA Upload
    while (millis() - OTAtime < debounce)
    {
      ArduinoOTA.handle();
      delay(10);
    }
  }

  if (button(0))
  { // On/off
    onoff = !onoff;

    if (onoff)
    {
      //Serial.println("ON");
      digitalWrite(PSUPin, HIGH);
      newValueRGBW[0][0] = 0;
      newValueRGBW[0][1] = 0;
      newValueRGBW[0][2] = 0;
      newValueRGBW[0][3] = 255;

      ColorTransition(0, true);
    }
    else
    {
      //Serial.println("OFF");
      digitalWrite(PSUPin, LOW);
      ResetColors();
    }
  }
  if (button(1))
  { // Lamp1
    WiFiClient client;
    HTTPClient http;
    //http.begin("http://192.168.178.61/on");     //Specify request destination
    if (valueW[1] == 0)
    {
      valueW[1] = 255;
      http.begin(client, "http://192.168.178.61/on"); //Specify request destination
    }
    else
    {
      valueW[1] = 0;
      http.begin(client, "http://192.168.178.61/off"); //Specify request destination
    }

    int httpCode = http.GET(); //Send the request

    if (httpCode > 0)
    {                                    //Check the returning code
      String payload = http.getString(); //Get the request response payload
      //Serial.println(payload);                  //Print the response payload
    }

    http.end();
  }
  if (button(2))
  { // Schuineluik
    if (valueW[0] == 0)
    {
      valueW[0] = 255;
      pwm.setPWM(9, 0, 4095);
    }
    else
    {
      valueW[0] = 0;
      pwm.setPWM(9, 0, 0);
    }
    #ifdef ESP8266
      yield(); // take a breather, required for ESP8266
    #endif
  }
}

boolean button(byte i) // geeft DIRECT EENMALIG een '1' als knop i ingedrukt wordt
{                      // knop i moet 50 ms los zijn voordat een nieuwe '1' gegeven kan worden
                       // in dit voorbeeld is bereik i: [0..3]

  const unsigned long debounce = 50;
  static unsigned long startTime[sizeof(buttonPin)]; // static array!!
  static boolean buttonFlg[sizeof(buttonPin)];        // static array!!

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
*/