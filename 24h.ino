#include <FlexiTimer2.h> // http://playground.arduino.cc/Main/FlexiTimer2
#include "Carte.h"

// Shell
#include "config.h"

// Interupt
#include "io.h"
#include "my_def.h"				// definition des pins et de leur usage

// Rule Checker
#include "rule_checker.h"
#include "utils.h"

void setup(void)
{
  Serial.begin(9600);
  pinMode(dbg0_pin, OUTPUT);
  pinMode(dbg1_pin, OUTPUT); 
  carte_setup();
  FlexiTimer2::set(25, 1.0/10000, tick2500us); // call every 500 1ms "ticks"
  // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();
  
  //carte_update();
  //io_setup();
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  //carte_update();
  //io_loop();
}

void loop(void)
{
  digitalWrite(10,HIGH); 
  digitalWrite(11,HIGH); 
  carte_update();
  io_loop();
}
