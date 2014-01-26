#include <FlexiTimer2.h> // http://playground.arduino.cc/Main/FlexiTimer2
#include "Carte.h"

// Shell
#include "shell.h"
#include "config.h"

// Interupt
#include "io.h"
#include "my_def.h"				// definition des pins et de leur usage

// Rule Checker
#include "rule_checker.h"

// Shell commands
shell_command_t shell_commands[SHELL_COMMAND_COUNT];

void setup(void)
{
  SHELL_COMMAND_DECL(0, "help", "this help", false, shell_command_help);
  SHELL_COMMAND_DECL(1, "send", "send a debug static frame", true, sendDbgFrames);
  SHELL_COMMAND_DECL(2, "init", "send a debug static frame", false, sendInit);
  SHELL_COMMAND_DECL(3, "address", "display address", false, display_address);
  //SHELL_COMMAND_DECL(2, "rebrec", "Affiche l etat de l array  ruleChecker", false, displayRulechecker);
  shell_setup();

  io_setup();
  //rc_setup();
  carte_setup();
  FlexiTimer2::set(25, 1.0/10000, tick2500us); // call every 500 1ms "ticks"
  // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();
}

void loop(void)
{
  shell_loop();
  carte_loop();
  io_loop();
  // Please dont add delay here :(
  // delay(1500);
}
