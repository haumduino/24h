#include "io.h"
#include "my_def.h"

#include <Arduino.h>

Output left(out_left);
Output right(out_right);

/*
 * Pour chaque bit dans la frame (1 + address + data):
 *  - s'il est a 1, on met la sortie à 1 et temps = 8 (8*5 = 40 ms)
 *  - s'il est a 0, on met la sortie à 1 et temps = 4 (4*5=20ms)

 * Si temps de bit = 0 reset sortie et temps = 1 (5ms)
 * 
 * si fin de trame temps = 1 (5ms)
 */

Output::Output(const int pin) :
  _frame(0),
  _remainingTicks(0),
  _currentBit(0),
  _state(IDLE)
{
  setPin(pin);
}

Output::Output() :
  _frame(0),
  _remainingTicks(0),
  _currentBit(0),
  _state(IDLE)
{
}

void Output::sendFrame(const byte address, const byte data)
{
  _frame = (1 << 9) | ((uint16_t)address << 2) | ((uint16_t)data & 0x03);
  _currentBit = 9;
  _state = SENDING;
}

void Output::setPin(const int pin)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void Output::tick5ms()
{
  switch(_state) {
    case SENDING:
      if(_remainingTicks==0) {
        bool output = (_frame & (1 << _currentBit));
        digitalWrite(_pin, HIGH);
        if (output) { 
          _remainingTicks = 8 + 1;
        } else {
          _remainingTicks = 4 + 1;
        }
      } else {
        if(_remainingTicks==1) {
        }
        if(_remainingTicks==1) {
          digitalWrite(_pin, LOW);
          if(_currentBit==0) {
            _state = END_FRAME;
          } else {
            _currentBit--;
          }
        }
      }
      _remainingTicks--;
    break;
    case END_FRAME:
      _state = IDLE;
    break;
    case IDLE:
      // nothing to do
    break;
  }
}


static volatile bool done = false;

// interuption toutes les 5ms
void inter5()
{
  static bool output = HIGH;
  digitalWrite(dbg0_pin, output);
  output = !output;

  left.tick5ms();
  right.tick5ms();
}

void plop(const char* args)
{
  Serial.println("left.sendFrame(0x55, 0x02);");
  left.sendFrame(0x55, 0x02);
  while(left.state() != IDLE) {};

  Serial.println("right.sendFrame(0x2E, 0x03)");
  right.sendFrame(0x2E, 0x03);

  while(right.state() != IDLE) {};
  Serial.println("done.");
}


void io_setup(void)
{
  pinMode(dbg0_pin, OUTPUT);
  pinMode(dbg1_pin, OUTPUT); 
}

