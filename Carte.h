#ifndef Carte_h
#define Carte_h

#include "Arduino.h"

class Carte{
   public:
      Carte(int val, int state);
      int myval; //Nom ou adresse de la carte
      int mystate; //donnees libres
      int Dval;
      int Gval;
      boolean AllG; //All good : no error detected
      boolean LocG; //Local good: mes voisins sont bons 
      boolean DG[2]; //validite des voisins Droit et Gauche
      boolean get_AllGood(int, byte); 
      boolean get_LocGood(int, byte); 
      boolean valid(int, int); //rule checker
      void check(); //update DG
      void update(); //update All Good and Locally Good
      void display();
      //private:
      //  int _myval;
      //  int _mystate;
};

void carte_setup();
void carte_loop();
void display_carte_props();
#endif
