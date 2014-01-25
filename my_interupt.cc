//
//
// interuption toutes les 5ms

#include "my_interupt.h"
#include <Arduino.h>
#include "my_def.h"

void inter5()
{
static boolean output = HIGH;
  
  digitalWrite(led_pin, output);
  output = !output;
}
