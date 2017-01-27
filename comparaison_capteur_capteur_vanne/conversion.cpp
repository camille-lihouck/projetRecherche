#include "conversion.h"

float initialOffset = 64.06;
float maxOffset = 0.403;

float valueTokPa (int value){
    return (value/2.421 +3.478)*maxOffset + initialOffset;
}

float valueToBar(int value){
  return valueTokPa(value*0.01);
}


float internalSensorValueTokPa (int value){
  return value/2.0475 + 101.325;
}
