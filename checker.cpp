#include "my_def.h"
/*
class CARTE{

   Carte:Carte(byte val, byte state){
      public:
         byte myval=val;
         byte mystate=state;
         boolean AllG=True;
         boolean LocG=False;
   }

   boolean get_AllGood(int PIN,byte mystate){
     bool AllG=mystate & (1 << 0);//macro avrlib _BV
     return AllG; 
   } 

   boolean get_LocGood(int PIN, byte mystate){
     bool AllL=mystate & (1 << 1);//macro avrlib _BV
     return LocG; 
   } 

   boolean valid(int D_adress,int G_adress){
     return D_adress-'0'<=G_adress-'0';
   }

   boolean* check(byte myval){
     bool[2] DG=[False,False]; 
     int Dval;
     int Gval;
     D_val=get_value(D_PIN);
     G_val=get_value(G_PIN);
     DG[0]=valid(Dval, myval);
     DG[1]=valid(myval, Gval);
     return DG; 
   }

   void update(){
     bool DG[2]; 
     bool DAllG, GAllG;
     DG=check(myval);
     LocG=DG[0] && DG[1];  
     AllG=(DAllG && GAllG && LocG);
   }
}
*/
