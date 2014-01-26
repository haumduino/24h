// my_def.h

// definition des pins et de leur usage
#define PIN_LED_ROUGE 8
#define PIN_LED_JAUNE 9
#define LED_JAUNE_ON digitalWrite(PIN_LED_JAUNE,HIGH)
#define LED_JAUNE_OFF digitalWrite(PIN_LED_JAUNE,LOW)
#define LED_ROUGE_ON digitalWrite(PIN_LED_ROUGE,HIGH)
#define LED_ROUGE_OFF digitalWrite(PIN_LED_ROUGE,LOW)

const int led_pin = 13;

const int out_left = 4;
const int out_right = 5;

const int in_left = 2;
const int in_right = 3;
