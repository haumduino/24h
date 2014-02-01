#include "Arduino.h"
#include "Carte.h"
#include "io.h"
#include "my_def.h"
#include "rule_checker.h"


IODevice left(out_left, in_left);
IODevice right(out_right, in_right);

Carte my_carte(MYNAME);

// interuption toutes les 2.5ms
void tick2500us()
{
  left.tick2500us();
  right.tick2500us();
}

void io_loop()
{
  //left.sendFrame(my_carte.name, 0x01);
  //right.sendFrame(my_carte.name, 0x01);
  //Serial.println(left.input_state());
  //Serial.println(my_carte.state & LEFT_TO_RIGHT);
  
  //Serial.print("   LEFT  ");
  //Serial.print(" Connection type ");
  //Serial.println(left.connection_type);
  
  if(left.connection_type == UNKOWN){ 
    Serial.print("   LEFT  ");
    left.sendFrame(my_carte.name, my_carte.state | (1<<1));
    } 	
  if(left.inputFrameAvailable()) {
    Serial.print("   LEFT  ");
  	left.set_encountered_frame();
  	left.receiveFrame();

    //Serial.print(left.input_name());
    ///Serial.print("     ");
    //Serial.print(left.input_state());
   
    if (left.input_state() & RIGHT_TO_LEFT){
       Serial.print(" Looping ");
       left.connection_type = LOOP;
       my_carte.Lname=0x60;
    }   
    else{
      //Serial.println(my_carte.state);
      left.sendFrame(my_carte.name, my_carte.state | (1<<1));
      left.connection_type = NORMAL; 
      //my_carte.Lname=left.input_name();
      //my_carte.Lstate=left.input_state();
    }   
  }
  
  //Serial.print("   RIGHT ");
  //Serial.println("               ");
  //Serial.println(right.connection_type);
  //Serial.print(my_carte.name); 
  //Serial.print(my_carte.name); 
  //Serial.print(my_carte.state);
  if(right.connection_type == UNKOWN){
    Serial.print("  RIGHT ");
    right.sendFrame(my_carte.name, my_carte.state & ~(1<<1));
  }  
  if(right.inputFrameAvailable()) {
    Serial.print("   RIGHT  ");
  	right.set_encountered_frame();
  	right.receiveFrame();
    //Serial.print(right.input_name());
    //Serial.print("  | ");
    //Serial.print(right.input_state());
    //Serial.println("  ");
    //Serial.println(right.input_name());
    //Serial.println(RIGHT_TO_LEFT);
    //Serial.println(!(right.input_state() & RIGHT_TO_LEFT));
       
    if (!(right.input_state() & RIGHT_TO_LEFT)){
       Serial.print(" Looping");
       right.connection_type = LOOP;
       my_carte.Rname=0x62;
    }
    else{
      //Serial.print(" to RIGHT ");
      //Serial.println(my_carte.state);
      right.sendFrame(my_carte.name, my_carte.state & ~(1<<1));
      right.connection_type = NORMAL; 
      //my_carte.Rname=right.input_name();
      //my_carte.Rstate=right.input_state();
    }
  }

  
  if((left.connection_type != UNKOWN) && (right.connection_type != UNKOWN)) {
    if(left.connection_type == NORMAL) {
      my_carte.Lname=left.input_name();
      my_carte.Lstate=left.input_state();
    }
    if(right.connection_type == NORMAL) {
      my_carte.Rname = right.input_name();
      my_carte.Rstate= right.input_state();
    }
    //digitalWrite(led_pin, (right_is_ok && left_is_ok));
  }
}

void carte_setup()
{
  //io_setup();
  if (RULE != BASIC){
     rc_setup();
  }

  Serial.println("Setting carte");
  pinMode(PIN_LED_ROUGE, OUTPUT);
  pinMode(PIN_LED_JAUNE, OUTPUT);
  digitalWrite(PIN_LED_ROUGE, LOW);
  digitalWrite(PIN_LED_JAUNE, HIGH);
  // Phase 1 : Est-on en extrémité ? 
}

void carte_update()
{ 
  my_carte.update();
  //my_carte.display_names();
  //my_carte.display_states();
}
