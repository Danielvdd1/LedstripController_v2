#include <Arduino.h> // Required for Visual Studio Code
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESP8266HTTPClient.h> // Http request
#include <WiFiUdp.h>
#include <NTPClient.h>

namespace ledstripLights {

class GPIO_Out_PWMServoDriver;
class Ledstrip;
class LedstripRGB;
class LedstripRGBW;
class LedstripW;
class LedstripRemote;


class PSU{

  public:
    PSU(int pin);
	String getInfo();
	void setState(bool state);
	bool getState();
  
  protected:
    int pin;
    bool state;
};

// class GPIO_Out{

//   public:
//     GPIO_Out(int pin);
// 	virtual void setValue(int value) = 0;
// 	virtual int getValue() = 0;
// };

class GPIO_Out_PWMServoDriver{

  public:
    GPIO_Out_PWMServoDriver(Adafruit_PWMServoDriver &pwmDriver, int pin);
	void setValue(int value);
	int getValue();
  
  protected:
	Adafruit_PWMServoDriver &pwmDriver;
    int pin;
    int value;
};

// class GPIO_Out_Pin{

//   public:
//     GPIO_Out_Pin(int pin);
// 	void setValue(int value);
// 	int getValue();
  
//   protected:
//     int pin;
//     int value;
// };

// class GPIO_Out_Remote{

//   public:
//     GPIO_Out_Remote(String urlOn, String urlOff);
// 	void turnOn();
// 	void turnOff();
// 	String getValue();
  
//   protected:
//     String urlOn;
// 	String urlOff;
// 	String sendRequest(String url);
// };

class Ledstrip{

  public:
    Ledstrip(String name);
	virtual String getInfo() = 0;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;

  protected:
    String name;
    //int brightness;
};


class LedstripRGB: public Ledstrip{

  public:
    LedstripRGB(String name, Adafruit_PWMServoDriver &pwmDriver, int pinR, int pinG, int pinB);
	String getInfo();
    void turnOn();
    void turnOff();
    void setValue(int valR, int valG, int valB);

	//void setAnimType(int);
	//void setAnimSpeed(int);
	//void animate();
	

  protected:
	GPIO_Out_PWMServoDriver gpio[3];

	//int animType;
	//int animSpeed;
};

class LedstripRGBW: public Ledstrip{

  public:
    LedstripRGBW(String name, Adafruit_PWMServoDriver &pwmDriver, int pinR, int pinG, int pinB, int pinW);
	String getInfo();
    void turnOn();
    void turnOff();
    void setValue(int valR, int valG, int valB, int valW);
    void setValueHSV(int h, int s, int l); // 360°, 100%, 100%

	void colorTransition(int valNewR, int valNewG, int valNewB, int valNewW, unsigned long transitionTime = 1000);
	void colorTransitionUpdate();

	void setAnimType(int type); // Use enum
	void setAnimSpeed(int speed);
	void animate();

	
  protected:
  	GPIO_Out_PWMServoDriver gpio[4];

	bool transition = false;
	unsigned long transitionTime = 0;
	unsigned long startTime = 0;
	int valOldRGBW[4] = {0,0,0,0};
	int valNewRGBW[4] = {0,0,0,0};

	int animType = 0;
	int animSpeed = 30;
	void animRainbow(int animTime);
	void animRandom(int animTime);
	void animRandomSmooth(int animTime);
	void animRandomBlink(int animTime);
};

class LedstripW: public Ledstrip{

  public:
    LedstripW(String name, Adafruit_PWMServoDriver &pwmDriver, int pinW, bool binary = false);
	String getInfo();
    void turnOn();
    void turnOff();
	void turnOnOff();
	void setValue(int valW);

  
  protected:
	GPIO_Out_PWMServoDriver gpio;
    bool binary;
};

// class LedstripRemote: public Ledstrip{

//   public:
//     LedstripRemote(String name, String url, String urlOn, String urlOff);
// 	String getInfo();
// 	void turnOn();
// 	void turnOff();
// 	void turnOnOff();
// 	String getValue();
  
//   protected:
//     String url;
//     String urlOn;
// 	String urlOff;
// 	String sendRequest(String url);
// };

class LedstripRemote: public Ledstrip{

  public:
    LedstripRemote(String name, String urlOn, String urlOff);
	String getInfo();
	void turnOn();
	void turnOff();
	void turnOnOff();
	int getValue();
  
  protected:
    int value;
    String urlOn;
	String urlOff;
	String sendRequest(String url);
};



class Sunrise {

  public:
    Sunrise(LedstripRGBW &ledstrip, NTPClient &timeClient, PSU &psu, int sunriseTime, int sunriseDuration);
	String getInfo();
	void setEnabled(bool sunriseEnabled);
	bool getEnabled();
	void setTime(int sunriseTime);
	void setDuration(int sunriseDuration);
	void reset();

	void sunriseUpdate();
  
  protected:
	LedstripRGBW &ledstrip;
    NTPClient &timeClient;
    PSU &psu;
    bool sunriseEnabled = true;
    int sunriseTime; // Minutes // Not more than 1439 minutes
    int sunriseDuration; // Minutes // Not more than 59 minutes
    int sunriseState = 0;
	unsigned long interval;
};

}