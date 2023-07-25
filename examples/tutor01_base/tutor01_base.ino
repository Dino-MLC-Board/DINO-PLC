#include "DINO_PLC.h"

long lastTime;

void setup(){
  initPLC();
}

void loop(){
  if(millis() - lastTime > 1500){    
    lastTime = millis();
    //- Random value and assing to D10
    setU32D(10,random(190000,3999999));
    //- Set and Reset M2
    if(getM(2)){
      resetM(2);
    }else{
      setM(2);
    }
  }
}
