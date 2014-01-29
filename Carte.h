#ifndef Carte_h
#define Carte_h

#include "Arduino.h"
#include "rule_checker.h"

class Carte{
   public:
      Carte(int);
      int name; //Nom ou adresse de la carte
      int state; //donnees libres
      int Rname;
      int Lname;
      int Rstate;
      int Lstate;
      boolean globSol; //All good : no error detected
      boolean locSol; //All good : no error detected
      boolean hasLCD; //All good : no error detected
      boolean DG[2]; //validite des voisins Droit et Gauche
      boolean update_globSol(); 
      boolean update_locSol(); 
      void update(); //update All Good and Locally Good
      void display_names();
      void display_states();    
};

#endif
