#include "io.h"
#include "my_def.h"

#include <Arduino.h>

/*
 * Pour chaque bit dans la frame (1 + address + data):
 *  - s'il est a 1, on met la sortie à 1 et temps = 8 (8*5 = 40 ms)
 *  - s'il est a 0, on met la sortie à 1 et temps = 4 (4*5=20ms)

 * Si temps de bit = 0 reset sortie et temps = 1 (5ms)
 * 
 * si fin de trame temps = 1 (5ms)
 */

IODevice::IODevice(const int output_pin, int input_pin) :
  _encountered_frame(false),
  _unconnected(false),
  _tick_toogle(false),
  _output_frame(0x0000),
  _output_remainingTicks(0),
  _output_currentBit(0),
  _output_state(IDLE),
  _input_frame(0x0000),
  _input_received_frame_is_available(false),
  _input_received_frame(0x0000),
  _input_current_bit(9),
  _input_time_at_level(0), // 0 == IDLE
  _input_name(0)
{
  if (input_pin==in_left) _input_state=RIGHT_TO_LEFT;
  if (input_pin==in_right) _input_state=LEFT_TO_RIGHT;
  _output_pin = output_pin;
  pinMode(_output_pin, OUTPUT);
  _input_pin = input_pin;
}

void IODevice::sendFrame(const byte address, const byte data)
{
  while(_output_state!=IDLE) {}
  _output_frame = (1 << 9) | ((uint16_t)address << 2) | ((uint16_t)data & 0x03);
  Serial.print("SENT frame: ");
  printFrame(_output_frame); 
  _output_currentBit = 9;
  _output_state = SENDING;
}

uint16_t IODevice::receiveFrame()
{
  _input_received_frame_is_available = false;
  _input_name=(_input_received_frame >> 2) & 0x07f;
  _input_state=_input_received_frame & 0x03;
  Serial.print("RECV frame: ");
  printFrame(_input_received_frame);
}

void IODevice::tick2500us()
{
  input_level_detect();
  // Prescale /2
  _tick_toogle = ! _tick_toogle;
  if(_tick_toogle) tick5ms();
  _tick_big+=1;
  if (_tick_big%20==0){
     tick50ms();
   }
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

void IODevice::tick50ms()
{
  if (_encountered_frame) _unconnected=false;
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

void IODevice::input_level_push(int _input_time_at_level)
{
  bool level = (_input_time_at_level>0);

  if(!level) _input_time_at_level = -_input_time_at_level;

  if(level) {
    if(_input_time_at_level < 7) {
      // Too short frame
      input_bitshift(-1);
    } else if(_input_time_at_level < 9) {
      input_bitshift(0);
    } else if(_input_time_at_level < 17) {
      input_bitshift(1);
    } else {
      // Too long frame
      input_bitshift(-1);
    }
  }
}

void IODevice::input_level_detect()
{
  bool in = digitalRead(_input_pin);

  if(_input_time_at_level > 0) {
    if(!in) {
      // lvl changed
      input_level_push(_input_time_at_level);
      _input_time_at_level = 0;
    } else {
      _input_time_at_level++;
    }
  } else if(_input_time_at_level < 0) {
    if(in) {
      // lvl changed
      input_level_push(_input_time_at_level);
      _input_time_at_level = 0;
    } else {
      _input_time_at_level--;
    }
  } else {
    // _input_time_at_level == 0
    if(in) {
      _input_time_at_level++;
    } else {
      _input_time_at_level--;
    }
  }
}

void IODevice::set_encountered_frame()
{
  _encountered_frame=true;
}

void IODevice::printFrame(const uint16_t frame)
{
    for(int8_t i=9; i>=0; i--) {
      char c = (frame & (1<<i))?'1':'0';
      Serial.print(c);
    }
    //Serial.print("\r\n");
}



/*
boolean isConnected(IODevice* left, IODevice* right){
  return (left._unconnected || right._unconnected)
}
*/
void io_setup(void)
{
  pinMode(loc_pin, OUTPUT);
  pinMode(glob_pin, OUTPUT);
  digitalWrite(loc_pin,HIGH); 
  digitalWrite(glob_pin,HIGH); 
}
