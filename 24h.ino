#include <FlexiTimer2.h> // http://playground.arduino.cc/Main/FlexiTimer2

// Shell
#include "shell.h"
#include "config.h"

// Interupt
#include "my_interupt.h"
#include "my_def.h"				// definition des pins et de leur usage

// Shell commands
shell_command_t shell_commands[SHELL_COMMAND_COUNT];

void setup(void)
{
  SHELL_COMMAND_DECL(0, "help", "this help", false, shell_command_help);
  shell_setup();

  pinMode(led_pin, OUTPUT); // setup for test
  FlexiTimer2::set(5, 1.0/1000, inter5); // call every 500 1ms "ticks"
  // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();
}

void loop(void)
{
  shell_loop();
}
