#include "Arduino.h"
#include "Carte.h"

Carte::Carte(int val, int state){
         this->myval=val;
         this->mystate=state;
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
     //D_val=get_value(D_PIN);
     //G_val=get_value(G_PIN);
     this->DG[0]=valid(Dval, this->myval);
     this->DG[1]=valid(this->myval, Gval);
   }

void Carte::update(){
     boolean DG[2]; 
     boolean DAllG, GAllG;
     this->check();
     this->LocG=DG[0] && DG[1];  
     this->AllG=(DAllG && GAllG && LocG);
}

