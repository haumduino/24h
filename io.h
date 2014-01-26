#ifndef __IO_H__
#define __IO_H__

#include <Arduino.h>

typedef enum { IDLE, SENDING, END_FRAME } output_state;
typedef enum { UNKOWN, LOOP, NORMAL } connection_type_t; 

void io_setup(void);
void io_loop(void);

// interuption toutes les 2.5ms
void tick2500us();

void sendInit(const char*);
void sendDbgFrames(const char*);
void display_address(const char*);

class IODevice {
  public:
    IODevice(const int output_pin, const int input_pin);

    void tick2500us(); // This function should be (externally) executed each 2.5ms

    void sendFrame(const byte address, const byte data);
    bool inputFrameAvailable() { return _input_received_frame_is_available; }
    uint16_t receiveFrame();

    output_state state() const { return _output_state; }

    connection_type_t connection_type;

  private:
    // Tick
    bool _tick_toogle;
    void tick5ms(); 

    // Output
    int _output_pin;
    volatile uint16_t _output_frame;
    volatile uint8_t _output_remainingTicks;
    volatile uint8_t _output_currentBit;
    volatile output_state _output_state;

    // Input
    int _input_pin;
    volatile uint16_t _input_frame;
    volatile bool _input_received_frame_is_available;
    volatile uint16_t _input_received_frame;
    volatile uint8_t _input_current_bit;
    int16_t _input_time_at_level;

    void input_bitshift(int8_t);
    void input_level_push(int);
    void input_level_detect();
};

#endif
