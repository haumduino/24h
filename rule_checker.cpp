#include "rule_checker.h"



int nbEltarrConformityrule;




Conformityrule *arrConformityrule; 


/******************************
//REM, il va falloir faire du paramétrage conditionnel en fonction de l'architecture cible 
//       ex atmega328P ou autre....
************************/
byte myVal = 'a'; // Valeur attribuée à la carte

// Règle de chronologie imposée
byte rule[] =  { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'};

/* 
  initialise  un dictionaire stocké dans un array indiquant les
  voisins de droite et de gauche valides
*/


void rc_setup(){
  //Serial.begin(9600);
  int nbEltarrConformityrule = 0;
  arrConformityrule = initRuleChecker();
  //displayRulechecker();
}

void rc_loop(){
  //Serial.println("test");
}



Conformityrule * initRuleChecker()
{
  Conformityrule larrConformityrule[NB_REGLES_MAX];
  // parcours le tableau rule[] pour définir chaque état de gauche et de droite valides
  int taille = sizeof(rule) / sizeof(byte);
  //itération sur rule[]
  for (int pos=0 ; pos<taille ; pos++) // generation de tous les triplets de voisins possibles
  {
      larrConformityrule[pos].milieu = rule[pos];
      if (pos == 0)
      {
           //si début de tableau
             //case gaughe = CARACTERE_EXTREMITE
        larrConformityrule[pos].gauche = CARACTERE_EXTREMITE;
        larrConformityrule[pos].droite = rule[pos+1];
        
      }
      else if (pos == (taille - 1))
      {     // si fin de tableau
            //case droite = CARACTERE_EXTREMITE
        larrConformityrule[pos].gauche = rule[pos-1];
        larrConformityrule[pos].droite = CARACTERE_EXTREMITE;        
      }
      else
      {// sinon
        larrConformityrule[pos].gauche = rule[pos-1];
        larrConformityrule[pos].droite = rule[pos+1];
      }

    nbEltarrConformityrule++;
  }
  // retourne le tableau nouvellement généré.
  return larrConformityrule;
}

void displayRulechecker(const char *)
{
  Serial.println("Etat du tableau Rulechecker : ");
  Serial.println("GAUCHE | MILIEU | DROITE ");

  /*int taille = sizeof(arrConformityrule) / sizeof(struct Conformityrule);
  Serial.println(sizeof(struct Conformityrule));
  */
  //itération sur rule[]
  for (int pos=0 ; pos<nbEltarrConformityrule ; pos++) // generation de tous les triplets de voisins possibles
  {
    Serial.print(arrConformityrule[pos].gauche);
    Serial.print(" | ");
    Serial.print(arrConformityrule[pos].milieu);
    Serial.print(" | ");
    Serial.println(arrConformityrule[pos].droite);
    ///Serial.println("--------------");
  }
  Serial.println("--------------");

}
// vérifie si l'equipement placé du cté COTE de notre arduino possède une valeur compatible avec le RuleChecker
boolean check(byte myVal, byte valGauche, byte valDroite)
{ 
  for (int pos=0 ; pos<nbEltarrConformityrule ; pos++) // itération sur tous les triplets de voisins possibles
  {
    if (   (arrConformityrule[pos].milieu == myVal)
        && (arrConformityrule[pos].gauche == valGauche)
        && (arrConformityrule[pos].droite == valDroite))
    {
      return 1; /// JEN SUIS LA !!!!!
    }
      
  }
}

/*boolean checkRight(byte voisin,)
{
  
}
*/

