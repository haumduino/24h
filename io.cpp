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

static byte address = 0x55;

void display_address (const char*args)
{
  Serial.print("address: ");
  Serial.print(address, HEX);
  Serial.print("\r\n");
}

IODevice::IODevice(const int output_pin, const int input_pin) :
  _tick_toogle(false),
  _output_frame(0x0000),
  _output_remainingTicks(0),
  _output_currentBit(0),
  _output_state(IDLE),
  _input_frame(0x0000),
  _input_received_frame_is_available(false),
  _input_received_frame(0x0000),
  _input_current_bit(9),
  _input_time_at_level(0) // 0 == IDLE
{
  _output_pin = output_pin;
  pinMode(_output_pin, OUTPUT);

  _input_pin = input_pin;
}

void IODevice::sendFrame(const byte address, const byte data)
{
  while(_output_state!=IDLE) {}
  _output_frame = (1 << 9) | ((uint16_t)address << 2) | ((uint16_t)data & 0x03);
  _output_currentBit = 9;
  _output_state = SENDING;
}

void IODevice::display_status()
{
  Serial.print("status: ");
  if(connection_type == LOOP) Serial.print("LOOP");
  if(connection_type == UNKOWN) Serial.print("UNKNOWN");
  if(connection_type == NORMAL) {
    Serial.print("connected to "); 
    uint16_t address = (_input_received_frame >> 2) & 0x7f;
    Serial.print((uint8_t)address, HEX);
  }
  Serial.print("\r\n");
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
  } else {
    if(_input_time_at_level > 2) {
      // Too long low
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

void sendDbgFrames(const char* args)
{
  Serial.println("left.sendFrame(0x55, 0x02);");
  left.sendFrame(0x55, 0x02);
  while(left.state() != IDLE) {};

  Serial.println("right.sendFrame(0x2E, 0x03)");
  right.sendFrame(0x2E, 0x03);

  while(right.state() != IDLE) {};
  Serial.println("done.");
}

#define LEFT_TO_RIGHT 0x01
#define RIGHT_TO_LEFT 0x00

void sendInit(const char* args)
{
  left.sendFrame(address, LEFT_TO_RIGHT);
  right.sendFrame(address, RIGHT_TO_LEFT);
}

void io_setup(void)
{
  pinMode(led_pin, OUTPUT); 
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
  uint16_t leftFrame;
  if(left.connection_type == UNKOWN) left.sendFrame(address, LEFT_TO_RIGHT);
  if(left.inputFrameAvailable()) {
    leftFrame = left.receiveFrame();
    if(leftFrame & LEFT_TO_RIGHT) {
      // We received a frame from left expected to be on the right -> we have a loop
      Serial.print("Loop detected on the left\r\n");
      left.connection_type = LOOP;
    } else {
      left.sendFrame(address, LEFT_TO_RIGHT);
      left.connection_type = NORMAL;
    }
//    printFrame(leftFrame);
  }

  uint16_t rightFrame;
  if(right.connection_type == UNKOWN) right.sendFrame(address, RIGHT_TO_LEFT);
  if(right.inputFrameAvailable()) {
    rightFrame = right.receiveFrame();
    if(!(rightFrame & LEFT_TO_RIGHT)) {
      // We received a frame from right expected to be on the left -> we have a loop
      Serial.print("Loop detected on the right\r\n");
      right.connection_type = LOOP;
    } else {
      right.sendFrame(address, RIGHT_TO_LEFT);
      right.connection_type = NORMAL;
    }
//    printFrame(rightFrame);
  }

  bool left_is_ok = (left.connection_type == LOOP);
  bool right_is_ok = (right.connection_type == LOOP);

  if((left.connection_type != UNKOWN) && (right.connection_type != UNKOWN)) {
    if(left.connection_type == NORMAL) {
      uint16_t leftAddress = leftFrame >> 2; // Drop data bits
      leftAddress &= 0x007F; // Drop anything except address
      if (leftAddress <= address)
        left_is_ok = true;
    }
    if(right.connection_type == NORMAL) {
      uint16_t rightAddress = rightFrame >> 2; // Drop data bits
      rightAddress &= 0x007F; // Drop anything except address
      if (rightAddress >= address)
        right_is_ok = true;
    }
  }

  digitalWrite(led_pin, (right_is_ok && left_is_ok));
}

void display_status(const char* args)
{
  Serial.print("Left ");
  left.display_status();
  Serial.print("Right ");
  right.display_status();
}
