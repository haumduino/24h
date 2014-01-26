#ifndef __IO_H__
#define __IO_H__

#include <Arduino.h>

typedef enum { IDLE, SENDING, END_FRAME } output_state;

void io_setup(void);
void io_loop(void);

// interuption toutes les 2.5ms
void tick2500us();

void plop(const char*);

class IODevice {
  public:
    IODevice(const int output_pin, const int input_pin);

    void tick2500us(); // This function should be (externally) executed each 2.5ms

    void sendFrame(const byte address, const byte data);
    bool inputFrameAvailable() { return _input_received_frame_is_available; }
    uint16_t receiveFrame();

    output_state state() const { return _output_state; }

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

    void input_bitshift(int8_t);
    void input_level_push(int);
    void input_level_detect();
};

#endif
