#include "conversion.h";

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//pin of the sensor
const int externalSensorPin = A0;
const int input = 3;
const int internalSensorPin = A2;

//readen value from the captor
int externalSensorValue;
int internalSensorValue;
//calibration du zéro mesures initales fiables en kPa
float internalReference;
float externalReference;
//valeur calculée à partir des mesures
float curentInternalValue;
float curentExternalValue;


//precision de l'initialisation souhaitée
const float precision = 0.02;
// ratio du filtre 
const float alpha = 0.2;
//temps t
int t=0;
const int delayT = 10;


// Valeur de la commande en Bar relatif pression desirée
const float desiredValue = 0.15;
float inputValue;
// Gain de la boucle de controle
const float gain =1.1;


void setup(){
  setHigherMeasureRate();
  initPin();
  Serial.begin(9600);
  analogWrite(input,3);
  delay(100);
  initValues();  
}

void loop(){
  setInputValue();
  getMeasures();
  valuesTokPa();
  displayValue();
  t+=1;
  delay(delayT);
}

void setInputValue(){
    inputValue= desiredValue-kPaToBar(curentExternalValue);
    analogWrite(input, inputToPWM(inputValue*gain));
}


float getInitialValue(int pin, float erreur){
  float tmp1=-42;
  float tmp2; 
  tmp2=getAverageValue(pin,10,100);
  while (erreur<abs(tmp1-tmp2)){
    tmp1=tmp2;
    tmp2=getAverageValue(pin,10,100);
  }
  return (tmp1+tmp2)/2;
}
  
  
  
void initValues(){  
  internalSensorValue = getInitialValue(internalSensorPin,precision);
  externalSensorValue = getInitialValue(externalSensorPin,precision);
  
  internalReference = internalSensorValueTokPa(internalSensorValue);
  externalReference = valueTokPa(externalSensorValue);
}


float filteredValue(int vp,int v){
  return v*alpha + vp*(1-alpha);
}


float getAverageValue(int pin, int nbMeasures,float t){
  int sum=0;
  for (int i=0; i<nbMeasures; i++){
    sum+=analogRead(pin);
    delay(t);
  }
  return sum/nbMeasures;
}

void getMeasures(){
  externalSensorValue=analogRead(externalSensorPin);
  internalSensorValue=filteredValue(internalSensorValue,getAverageValue(internalSensorPin,10,0.02));
}


void valuesTokPa(){
  curentInternalValue= internalSensorValueTokPa(internalSensorValue)-internalReference;
  if (curentInternalValue<0) curentInternalValue=0;
  
  curentExternalValue = valueTokPa(externalSensorValue) - externalReference;
  if (curentExternalValue<0) curentExternalValue=0;
}


void displayValue(){
  Serial.print(t);
  Serial.print("  ");
  Serial.print(barTokPa(inputValue));
  Serial.print("  ");
  Serial.print(curentInternalValue);
  Serial.print("  ");
  Serial.print(curentExternalValue);
  Serial.print("\n");
}



void setHigherMeasureRate(){
  TCCR2B = (TCCR2B & 0b11111000) | 0x01;
  // set prescale to 16 to allow higher rate of measure
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
}


void initPin(){
  pinMode(externalSensorPin,INPUT);
  pinMode(internalSensorPin,INPUT);
  pinMode(input,OUTPUT);
}
