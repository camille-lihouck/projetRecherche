#include "conversion.h"

float valueTokPa (int value){
    return value/2.421+3.478+2.421;
}

float valueToBar(int value){
  return valueTokPa(value*0.01);
}


float internalSensorValueTokPa (int value){
  return value/2.0475 + 101.325;
}
