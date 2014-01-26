 #include "rule_checker.h"

int nbEltarrConformityrule;

Conformityrule arrConformityrule[NB_REGLES_MAX];
  
//Conformityrule *arrConformityrule; 


/******************************
//REM, il va falloir faire du paramétrage conditionnel en fonction de l'architecture cible 
//       ex atmega328P ou autre....
************************/
byte myVal = 'd'; // Valeur attribuée à la carte

// Règle de chronologie imposée
byte rule[] =  { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};

/* 
  initialise  un dictionaire stocké dans un array indiquant les
  voisins de droite et de gauche valides
*/


void rc_setup(){
  int nbEltarrConformityrule = 0;
  initRuleChecker();
  //displayRulechecker();
}

void rc_loop(){
  static boolean res;
  
/*  res = check('d', 'c', 'e');
  Serial.println(res);
  delay(800);
*///check (myVal, GAUCHE, DROITE)
/*delay(800);
    Serial.println(true);
  Serial.println(false);
  */  


  //Serial.println("test");
}



void initRuleChecker()
{
  // parcours le tableau rule[] pour définir chaque état de gauche et de droite valides
  int taille = sizeof(rule) / sizeof(byte);
  //Serial.println(taille);
  //itération sur rule[]
  for (int pos=0 ; pos<taille ; pos++) // generation de tous les triplets de voisins possibles
  {
      arrConformityrule[pos].milieu = rule[pos];
      if (pos == 0)
      {
           //si début de tableau
             //case gaughe = CARACTERE_EXTREMITE
        arrConformityrule[pos].gauche = CARACTERE_EXTREMITE;
        arrConformityrule[pos].droite = rule[pos+1];
        
      }
      else if (pos == (taille - 1))
      {     // si fin de tableau
            //case droite = CARACTERE_EXTREMITE
        arrConformityrule[pos].gauche = rule[pos-1];
        arrConformityrule[pos].droite = CARACTERE_EXTREMITE;        
      }
      else
      {// sinon
        arrConformityrule[pos].gauche = rule[pos-1];
        arrConformityrule[pos].droite = rule[pos+1];
      }
    nbEltarrConformityrule++;
  }
  // retourne le tableau nouvellement généré.
}

void displayRulechecker(const char *)
{
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

    if  (  (arrConformityrule[pos].milieu == myVal)
        && (arrConformityrule[pos].gauche == valGauche)
        && (arrConformityrule[pos].droite == valDroite))
    {
      //Serial.println("content");
      return 1; /// JEN SUIS LA !!!!!
    }
      
  }
  //Serial.println("pacontent");
      
  return 0;
}

/*boolean checkRight(byte voisin,)
{
  
}
*/

