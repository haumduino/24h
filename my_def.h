// my_def.h

// definition des pins et de leur usage
#define PIN_LED_ROUGE 8
#define PIN_LED_JAUNE 9
#define LED_JAUNE_ON digitalWrite(PIN_LED_JAUNE,HIGH)
#define LED_JAUNE_OFF digitalWrite(PIN_LED_JAUNE,LOW)
#define LED_ROUGE_ON digitalWrite(PIN_LED_ROUGE,HIGH)
#define LED_ROUGE_OFF digitalWrite(PIN_LED_ROUGE,LOW)

//Def for rule checkers
#define BASIC 0
#define ALPHABET 1
#define SEMAINE 2
#define RULE ALPHABET

//initial trame
#define MYNAME 'A'

#define LEFT_TO_RIGHT 0x00
#define RIGHT_TO_LEFT 0x02

const int dbg0_pin = 1;
const int dbg1_pin = 12;

const int led_pin = 13;
const int loc_pin = 10;
const int glob_pin = 11;

const int out_left = 4;
const int out_right = 5;

const int in_left = 2;
const int in_right = 3;
