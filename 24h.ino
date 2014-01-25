#include <FlexiTimer2.h> // http://playground.arduino.cc/Main/FlexiTimer2


#include "my_interupt.h"

#include "my_def.h"				// definition des pins et de leur usage

// const int led_pin = 13;			// default to pin 13

void setup(void)
{
pinMode(led_pin, OUTPUT); // setup for test

  FlexiTimer2::set(5, 1.0/1000, inter5); // call every 500 1ms "ticks"
  // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();
	
}

void loop(void)
{
}
