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
int externalSensorValue=0;
int initialExternalSensorValue=0;

int internalSensorValue=0;
float previousInternalSensorValue=0;
int initialInternalSensorValue=0;

//calibration du zéro mesures initales fiables en kPa
float internalReference;
float externalReference;

//valeur brute mesurée avant la comparaison avec la référence
float curentInternalValue;
float curentExternalValue;

// Valeur de la commande en Bar relatif
float inputValue = 0.1;

//precision de l'initialisation souhaitée
const float precision = 0.02;

// ratio du filtre 
const float alpha = 0.1;


//temps t
int t=0;
const int delayT = 10;

//minimal threshold
float pressureThresholdkPa = valueTokPa(1);
 
void setup(){
  TCCR2B = (TCCR2B & 0b11111000) | 0x01;
  // set prescale to 16 to allow higher rate of measure
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
  
  pinMode(externalSensorPin,INPUT);
  pinMode(internalSensorPin,INPUT);
  pinMode(input,OUTPUT);
  
  Serial.begin(9600);
  analogWrite(input,3);
  delay(100);
  
  initialInternalSensorValue = getInitialValue(internalSensorPin,precision);
  initialExternalSensorValue = getInitialValue(externalSensorPin,precision);
 
  //analogWrite(input,inputValue);
  initValues();  
  //displayValue();
  
}

void loop(){
  /*Serial.print(inputValue);
  Serial.print("  ");
  Serial.print(analogRead(externalSensorPin));
  Serial.print("\n");
  */getValues();
  valuesTokPa();
  displayValue();
  t+=1;
  delay(delayT);
  if (t==20) analogWrite(input, inputToPWM(inputValue));
  if (t==250){
    inputValue=0.2; 
    analogWrite(input, inputToPWM(inputValue));
  }
  if (t==500){ 
    inputValue=0.3;
    analogWrite(input, inputToPWM(inputValue));
  }
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
  previousInternalSensorValue = initialInternalSensorValue;
  internalReference = internalSensorValueTokPa(initialInternalSensorValue);
  externalReference = valueTokPa(initialExternalSensorValue);
  curentInternalValue = internalReference;
  curentExternalValue = externalReference;
}

void getValues(){
  externalSensorValue=analogRead(externalSensorPin);
  internalSensorValue=getAverageValue(internalSensorPin,10,0.02);
  previousInternalSensorValue = filteredValue(previousInternalSensorValue, internalSensorValue);
}

float getAverageValue(int pin, int nbMeasures,float t){
  int sum=0;
  for (int i=0; i<nbMeasures; i++){
    sum+=analogRead(pin);
    delay(t);
  }
  return sum/nbMeasures;
}

float filteredValue(int vp,int v){
  return v*alpha + vp*(1-alpha);
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

void valuesTokPa(){
  curentInternalValue= internalSensorValueTokPa(previousInternalSensorValue)-internalReference;
  curentExternalValue = valueTokPa(externalSensorValue) - externalReference;
  if (curentInternalValue<0) curentInternalValue=0;
  if (curentExternalValue<0) curentExternalValue=0;
}
