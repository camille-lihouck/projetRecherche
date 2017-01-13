#include "conversion.h"

float valueTokPa (int value){
  return value/2.421+3.478;
}

float valueToBar(int value){
  return valueTokPa(value*0.01);
}

