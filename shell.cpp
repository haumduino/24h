#include "shell.h"

// Serial
#include <Arduino.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#include "config.h"

#define SHELL_MAX_COMMAND_LINE_LENGTH 128
#ifndef SHELL_COMMAND_COUNT
#error Please set SHELL_COMMAND_COUNT and check your shell_commands[] array
#endif

extern shell_command_t shell_commands[];

static bool _debug_mode = false;

void shell_setup(void)
{
  Serial.begin(9600);
  Serial.setTimeout(1);
}

void shell_execute(const char* buffer)
{
  char command[128];

  if (sscanf_P(buffer, PSTR("%s "), command) > 0) {
    // Look for an exact match
    size_t n;
    for (n = 0; n < SHELL_COMMAND_COUNT; n++) {
      if (0 == strcmp_P(command, shell_commands[n].text)) {
        (shell_commands[n].function)(buffer);
        return;
      }
    }
    Serial.print(command);
    Serial.print(F(": unknown command\r\n"));
  } else {
    Serial.print(F("Huhu?!?\r\n"));
  }
}

void shell_loop(void)
{
  static char buf[128];
  static byte i = 0;
  
  while(Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case '\r':
      case '\n':
        buf[i] = '\0';
        // Serial.print("buf(");Serial.print(i);Serial.print("): \t");Serial.print(buf);Serial.print("\r\n");
        Serial.print("\r\n");
        if(i) shell_execute(buf);

        i = 0;
        Serial.print("\r\n$ ");
        break;
      default:
        buf[i++] = c;
        Serial.print(c);
        if(i == sizeof(buf)) {
          Serial.println("OVERRUN !");
          i = 0;
          Serial.flush();
        }
    }
  }
}

// Help command
void
shell_command_help(const char *args)
{
  (void)args;
  char buffer[128];
  printf_P(PSTR("supported commands:\n"));
  for (size_t n = 0; n < SHELL_COMMAND_COUNT; n++) {
    if ((_debug_mode) || (shell_commands[n].debug == false)) {
      Serial.print("\t");
      strcpy_P(buffer, shell_commands[n].text); // Necessary casts and dereferencing, just copy. 
      Serial.print(buffer);
      Serial.print(" - ");
      strcpy_P(buffer, shell_commands[n].description); // Necessary casts and dereferencing, just copy. 
      Serial.print(buffer);
      Serial.print("\r\n");
    }
  }
}
