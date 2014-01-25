//
//
// interuption toutes les 5ms

#include "my_interupt.h"
#include <Arduino.h>
#include "my_def.h"

static byte 	Address;		// Board adress as XEEEAAAA
							// X undef
							// EEE tean adress
							// AAAA board adress
								
static byte 	Data;		// data as XXXXXXdd
							//
/*
 * pour chaque bit dans address + data
 * 	si il est a 1 set soritie et temps = 8 (8*5 = 40 ms)
 *  si il est a 0 set sortie et temps = 4 (4*5=20ms)
 * 	si temp de bit = 0 reset sortie et temps = 1 (5ms)
 * 
 * si fin de trame temps = 1 (5ms)
 * 
 * 							


void inter5()
{
static boolean output = HIGH;

  digitalWrite(led_pin, output);
  output = !output;
}
