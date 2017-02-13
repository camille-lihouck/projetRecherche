#include "conversion.h"


float valueTokPa (int value){
    float res=(value/2.421 +3.478)*0.96 ;//- 37;
    //float res = value/2.421 + 3.478;
    return res;
}

float kPaToBar(float value){
  return value*0.01;
}

float barTokPa(float value){
  return value*100;
}


float internalSensorValueTokPa (int value){
  return value*0.733;
  //return (value/2.0475)*2.5 ;
}

// input in bar
int inputToPWM (float input){
  return (int)(input*255/3);
}
