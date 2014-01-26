#include "io.h"
#include "my_def.h"

#include <Arduino.h>

IODevice left(out_left, in_left);
IODevice right(out_right, in_right);

/*
 * Pour chaque bit dans la frame (1 + address + data):
 *  - s'il est a 1, on met la sortie à 1 et temps = 8 (8*5 = 40 ms)
 *  - s'il est a 0, on met la sortie à 1 et temps = 4 (4*5=20ms)

 * Si temps de bit = 0 reset sortie et temps = 1 (5ms)
 * 
 * si fin de trame temps = 1 (5ms)
 */

IODevice::IODevice(const int output_pin, const int input_pin) :
  _tick_toogle(false),
  _output_frame(0x0000),
  _output_remainingTicks(0),
  _output_currentBit(0),
  _output_state(IDLE),
  _input_frame(0x0000),
  _input_received_frame_is_available(false),
  _input_received_frame(0x0000),
  _input_current_bit(9)

{
  _output_pin = output_pin;
  pinMode(_output_pin, OUTPUT);

  _input_pin = input_pin;
}

void IODevice::sendFrame(const byte address, const byte data)
{
  _output_frame = (1 << 9) | ((uint16_t)address << 2) | ((uint16_t)data & 0x03);
  _output_currentBit = 9;
  _output_state = SENDING;
}

void IODevice::tick2500us()
{
  input_level_detect();

  // Prescale /2
  _tick_toogle = ! _tick_toogle;
  if(_tick_toogle) tick5ms();
}

void IODevice::tick5ms()
{
  switch(_output_state) {
    case SENDING:
      if(_output_remainingTicks==0) {
        bool output = (_output_frame & (1 << _output_currentBit));
        digitalWrite(_output_pin, HIGH);
        if (output) { 
          _output_remainingTicks = 8 + 1;
        } else {
          _output_remainingTicks = 4 + 1;
        }
      } else {
        if(_output_remainingTicks==1) {
        }
        if(_output_remainingTicks==1) {
          digitalWrite(_output_pin, LOW);
          if(_output_currentBit==0) {
            _output_state = END_FRAME;
          } else {
            _output_currentBit--;
          }
        }
      }
      _output_remainingTicks--;
    break;
    case END_FRAME:
      _output_state = IDLE;
    break;
    case IDLE:
      // nothing to do
    break;
  }
}


void IODevice::input_bitshift(const int8_t bit)
{
  if(bit == -1) {
    // Invalid bit == invalid frame
    _input_frame = 0x0000;
    _input_current_bit = 9;
  } else {
    if(bit == 1) _input_frame |= (1 << _input_current_bit);
    if(_input_current_bit != 0) {
      _input_current_bit--;
    } else {
      _input_received_frame = _input_frame;
      _input_received_frame_is_available = true;
      _input_frame = 0x0000;
      _input_current_bit = 9;
    }
  }
}

void IODevice::input_level_push(int time_at_lvl)
{
  bool level = (time_at_lvl>0);

  if(!level) time_at_lvl = -time_at_lvl;

  if(level) {
    if(time_at_lvl < 7) {
      // Too short frame
      input_bitshift(-1);
    } else if(time_at_lvl < 9) {
      input_bitshift(0);
    } else if(time_at_lvl < 17) {
      input_bitshift(1);
    } else {
      // Too long frame
      input_bitshift(-1);
    }
  }
}

void IODevice::input_level_detect()
{
  static signed int time_at_lvl = 0; // 0 == IDLE

  bool in = digitalRead(in_left);

  if(time_at_lvl > 0) {
    if(!in) {
      // lvl changed
      input_level_push(time_at_lvl);
      time_at_lvl = 0;
    } else {
      time_at_lvl++;
    }
  } else if(time_at_lvl < 0) {
    if(in) {
      // lvl changed
      input_level_push(time_at_lvl);
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

uint16_t IODevice::receiveFrame()
{
  _input_received_frame_is_available = false;
  return _input_received_frame;
}

// interuption toutes les 2.5ms
void tick2500us()
{
  left.tick2500us();
  right.tick2500us();
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

void printFrame(const uint16_t frame)
{
    Serial.print("RECV frame: ");
    for(int8_t i=9; i>=0; i--) {
      char c = (frame & (1<<i))?'1':'0';
      Serial.print(c);
    }
    Serial.print("\r\n");
}

void io_loop(void)
{
  if(left.inputFrameAvailable()) {
    printFrame(left.receiveFrame());
  }
  if(right.inputFrameAvailable()) {
    printFrame(right.receiveFrame());
  }
}
