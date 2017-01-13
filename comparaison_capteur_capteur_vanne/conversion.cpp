#include "conversion.h"

float valueTokPa (int value){
  return value*20/4.095+3.478;
}

float valueToBar(int value){
  return valueTokPa(value*0.01);
}

