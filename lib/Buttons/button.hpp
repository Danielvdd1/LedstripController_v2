#include <Arduino.h> // Required for Visual Studio Code

namespace button_lib {

class Button;


class Button{

  public:
    Button(String name, int pin);
	String getInfo();
	int getState();
	boolean isPressed();
  
  protected:
    String name;
    int pin;
	const unsigned long debounce;
    unsigned long startTime;
	boolean buttonFlg;
};

}