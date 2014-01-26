#include "Arduino.h"
#include "Carte.h"

#define BASIC 0
#define ALPHABET 1
#define SEMAINE 2

#define RULE BASIC

Carte my_carte(1, 1);

Carte::Carte(int val, int state){
         this->myval=val;
         this->mystate=state; 
         this->Dval=2;
         this->Gval=4;
         this->AllG=true; 
         this->LocG=false; 
         this->DG[2];
}

//Verififier que c est les bon bits
boolean Carte::get_AllGood(int PIN,byte mystate){
     boolean AllG=this->mystate & (1 << 0);//macro avrlib _BV
     return AllG; 
}

boolean Carte::get_LocGood(int PIN, byte mystate){
     boolean AllL=this->mystate & (1 << 1);//macro avrlib _BV
     return LocG; 
}

//tester si le comp fonctionne en bytes
boolean Carte::valid(int D_val,int G_val){
     return D_val-'0'<=G_val-'0';
}

void Carte::check(){
     byte Dval=2;
     byte Gval=4;
     this->DG[0]=valid(this->Dval, this->myval);
     this->DG[1]=valid(this->myval, this->Gval);
  }

void Carte::update(){
     boolean DG[2]; 
     boolean DAllG, GAllG;
     /*
     switch (RULE){
        case (0){
           this->check();
           this->LocG=DG[0] && DG[1];
        }

        case (1){
            LocG=rule_check(this->myval, this->Gval, this->Dval);
     }
     */
    this->check();
    this->LocG=DG[0] && DG[1];
    this->AllG=(DAllG && GAllG && LocG);
}

void carte_setup()
{
  if (RULE>0){
     rc_setup();
  }
}

void carte_loop()
{
  //Serial.println(my_carte.myval);
  my_carte.update();
  //display_carte_props();
}

void display_carte_props()
{
  /*
  Serial.println("Droite| Moi |Gauche");
  Serial.print(my_carte.Dval);
  Serial.print("   |   ");
  Serial.print(my_carte.myval);
  Serial.print("   |   ");
  Serial.println(my_carte.Gval);
  */
  Serial.print(my_carte.DG[0]);
  Serial.print("   |  ");
  Serial.print("D G");
  Serial.print("  |   ");
  Serial.print(my_carte.DG[1]);
  
}
