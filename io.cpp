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

static bool received_frame_is_valid = false;
static int16_t received_frame = 0x0000;

void bitshift(const int8_t bit)
{
  static int16_t frame = 0x0000;
  static uint8_t currentBit = 9;

  if(bit == -1) {
    // Invalid bit == invalid frame
    frame = 0x0000;
    currentBit = 9;
  } else {
    if(bit == 1) frame |= (1 << currentBit);
    if(currentBit != 0) {
      currentBit--;
    } else {
      received_frame = frame;
      received_frame_is_valid = true;
      frame = 0x0000;
      currentBit = 9;
    }
  }
}

void level_push(int time_at_lvl)
{
  bool level = (time_at_lvl>0);

  if(!level) time_at_lvl = -time_at_lvl;

  if(level) {
    if(time_at_lvl < 7) {
      // Too short frame
      bitshift(-1);
    } else if(time_at_lvl < 9) {
      bitshift(0);
    } else if(time_at_lvl < 17) {
      bitshift(1);
    } else {
      // Too long frame
      bitshift(-1);
    }
  }
}

void level_detect()
{
  static signed int time_at_lvl = 0; // 0 == IDLE

  bool in = digitalRead(in_left);

  if(time_at_lvl > 0) {
    if(!in) {
      // lvl changed
      level_push(time_at_lvl);
      time_at_lvl = 0;
    } else {
      time_at_lvl++;
    }
  } else if(time_at_lvl < 0) {
    if(in) {
      // lvl changed
      level_push(time_at_lvl);
      time_at_lvl = 0;
    } else {
      time_at_lvl--;
    }
  } else {
    // time_at_lvl == 0
    if(in) {
      time_at_lvl++;
    } else {
      time_at_lvl--;
    }
  }
}

// interuption toutes les 2.5ms
static volatile unsigned int counter = 0;
void tick2500us()
{
  level_detect();

  if(counter%2) {
    static bool output = HIGH;
    digitalWrite(dbg0_pin, output);
    output = !output;

    left.tick5ms();
    right.tick5ms();
  }
  counter++;
}

unsigned int delta_2500us(unsigned int i)
{
  if(i > counter) {
    return (i-counter);
  }
  return (counter-i);
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

void io_loop(void)
{
  if(received_frame_is_valid) {
    Serial.print("RECV frame: ");
    for(int8_t i=9; i>=0; i--) {
      char c = (received_frame & (1<<i))?'1':'0';
      Serial.print(c);
    }
    Serial.print("\r\n");
    received_frame_is_valid=false;
  }
}
