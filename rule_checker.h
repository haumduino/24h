#ifndef __RULE_CHECKER_H__
#define __RULE_CHECKER_H__

#include "Arduino.h"

#define COTE_GAUCHE 0
#define COTE_DROITE 1
#define PIN_LEFT 2
#define PIN_RIGHT 3
#define NB_REGLES_MAX 250 // purement arbitraire, représente le nombre de lignes dans le tableau permettant de vérifier son emplacement dans la chaine à valider
//int NB_REGLES_MAX = 250; // je sais pas pourquoi le define ne fonctionne pas, en attendant on met un int
#define CARACTERE_EXTREMITE 0
// structure définissant une ligne définissant une combinaison valide de valeurs 

struct Conformityrule
{ 
    byte milieu;
    byte gauche;
    byte droite;
}; 

void initRuleChecker();

void displayRulechecker(const char *);

// vérifie si l'equipement placé du cté COTE de notre arduino possède une valeur compatible avec le RuleChecker
boolean rule_check(byte myVal, byte valGauche, byte valDroite);

void rc_setup();

void rc_loop();



#endif
