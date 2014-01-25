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
Carte my_carte(1, 1);

void setup(void)
{
  SHELL_COMMAND_DECL(0, "help", "this help", false, shell_command_help);
  SHELL_COMMAND_DECL(1, "send", "send a test frame", false, plop);
  SHELL_COMMAND_DECL(2, "rebrec", "Affiche l etat de l array  ruleChecker", false, displayRulechecker);
  shell_setup();

  io_setup();
  rc_setup();

  FlexiTimer2::set(5, 1.0/1000, inter5); // call every 500 1ms "ticks"
  // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();
}

void loop(void)
{
  shell_loop();
  Serial.println(my_carte.myval);
  my_carte.update();
  delay(150);  
}
