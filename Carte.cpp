#include "my_def.h"
#include "Arduino.h"
#include "Carte.h"
#include "rule_checker.h"

Carte::Carte(int myname):
  name(myname),
  state(0x03),
  Rname(0),
  Lname(0),  
  Rstate(0),
  Lstate(0),
  globSol(true),
  locSol(false),
  hasLCD(false){}

boolean Carte::update_globSol(){
     boolean globSol=this->state & (1 << 0);//macro avrlib _BV
     return globSol;
}

boolean Carte::update_locSol(){
     boolean locSol=this->state & (1 << 1);//macro avrlib _BV
     return locSol;
}

//Verififier que c est les bon bits
//boolean Carte::get_globSol(){
//     return this->state & (1 << 0);//macro avrlib _BV
//}

//boolean Carte::get_globSol(){
//     return this->state & (1 << 1);//macro avrlib _BV 
//}


void Carte::update(){
     switch (RULE){
        case (BASIC):
           this->locSol=basic_check(this->name, this->Lname, this->Rname);
        case (ALPHABET || SEMAINE):
            this->locSol=rule_check(this->name, this->Lname, this->Rname);
    }
    this->globSol=(this->Rstate%2 && this->Lstate%2 && this->globSol);

}

void Carte::display_names(){
  Serial.println(" L  |   *   |  R"); 
  Serial.println("----------------");   
  Serial.print(this->Lname); 
  Serial.print("  |  ");
  Serial.print(this->name);
  Serial.print("  |  ");
  Serial.println(this->Rname);
  Serial.println("________________"); 
}

void Carte::display_states(){
  Serial.print("                                Loc/Glob | ");
  bool isLoc=(this->Lname<=this->name && this->name<=this->Rname)? 'Good!':'Bad';
  Serial.print(isLoc);
  bool isGlob=(this->Lname<=this->name && this->name<=this->Rname)? 'Good!':'Bad';
  Serial.print(" ");
  Serial.println(isGlob);
  Serial.println("____________"); 
}