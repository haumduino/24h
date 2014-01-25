#ifndef __IO_H__
#define __IO_H__

#include <Arduino.h>

typedef enum { IDLE, SENDING, END_FRAME } output_state;

void io_setup(void);
void io_loop(void);

// interuption toutes les 2.5ms
void tick2500us();

void plop(const char*);

class Output {
  public:
    Output();
    Output(const int pin);

    void tick5ms(); // This function will be executed each 5ms

    void setPin(const int pin);

    void sendFrame(const byte address, const byte data);

    output_state state() const { return _state; }

  private:
    volatile int _pin;
    volatile uint16_t _frame;
    volatile uint8_t _remainingTicks;
    volatile uint8_t _currentBit;
    volatile output_state _state;
};

#endif
