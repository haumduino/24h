#include "io.h"
#include "my_def.h"

#include <Arduino.h>

static byte 	address;	// Board adress as XEEEAAAA
				// X undef
				// EEE tean adress
				// AAAA board adress
								
static byte 	data;		// data as XXXXXXdd


/*
 * Pour chaque bit dans la frame (1 + address + data):
 *  - s'il est a 1, on met la sortie à 1 et temps = 8 (8*5 = 40 ms)
 *  - s'il est a 0, on met la sortie à 1 et temps = 4 (4*5=20ms)

 * Si temps de bit = 0 reset sortie et temps = 1 (5ms)
 * 
 * si fin de trame temps = 1 (5ms)
 */


typedef enum { IDLE, SENDING, END_FRAME } output_state;

static uint16_t leftFrame;
static uint8_t leftRemainingTick = 0;
static uint8_t leftCurrentBit = 0;
static output_state leftOutputState = IDLE;

static volatile bool done = false;

// interuption toutes les 5ms
void inter5()
{
  static bool output = HIGH;
  digitalWrite(dbg0_pin, output);
  output = !output;

  switch(leftOutputState) {
    case SENDING:
      if(leftRemainingTick==0) {
        bool output = (leftFrame & (1 << leftCurrentBit));
        digitalWrite(dbg1_pin, output);
        digitalWrite(out_left, HIGH);
        if (output) { 
          leftRemainingTick = 8 + 1;
        } else {
          leftRemainingTick = 4 + 1;
        }
      } else {
        if(leftRemainingTick==1) {
        }
        if(leftRemainingTick==1) {
          digitalWrite(out_left, LOW);
          if(leftCurrentBit==0) {
            leftOutputState = END_FRAME;
          } else {
            leftCurrentBit--;
          }
        }
      }
      leftRemainingTick--;
    break;
    case END_FRAME:
      leftOutputState = IDLE;
      done = true;
    break;
    case IDLE:
      digitalWrite(out_left, HIGH);
      // nothing to do
    break;
  }
}

void sendFrameLeft(const byte address, const byte data)
{
  leftFrame = (1 << 9) | ((uint16_t)address << 2) | ((uint16_t)data & 0x03);
  leftCurrentBit = 9;
  leftOutputState = SENDING;
}

void io_setup(void)
{
  pinMode(dbg0_pin, OUTPUT);
  pinMode(dbg1_pin, OUTPUT); 
  pinMode(out_left, OUTPUT); // setup for test
}

void plop(const char* args)
{
  Serial.println("sendFrameLeft(0x55, 0x02);");
  done = false;
  sendFrameLeft(0x55, 0x02);
  while(!done) {};
  Serial.println("done.");
}
