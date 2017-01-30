#include "conversion.h"


float maxOffset = 0.403;

float valueTokPa (int value){
    float res=(value/2.421 +3.478)*maxOffset - 37;
    if (res<0){
      return 0;
    }
    else{
      return res;
    }
}

float valueToBar(int value){
  return value*0.01;
}


float internalSensorValueTokPa (int value){
  return value/2.0475 ;
}
