#include "conversion.h";

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


// ALPHA à tester entre 0.1 et 0.5
const float alpha = 0.2
;

//pin of the sensor
const int externalSensorPin = A0;
const int input = 3;
const int internalSensorPin = A2;

//last readen value from the captor
int externalSensorValue=0;
int initialExternalSensorValue=0;

int internalSensorValue=0;
float previousInternalSensorValue=0;
int initialInternalSensorValue=0;


int inputValue = 10;


//temps t
int t=0;
const int delayT = 10;

//minimal threshold
float pressureThresholdkPa = valueTokPa(1);
float pressureThresholdBar = valueToBar(1);
 
void setup(){
  TCCR2B = (TCCR2B & 0b11111000) | 0x01;
  // set prescale to 16 to allow higher rate of measure
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
  
  pinMode(externalSensorPin,INPUT);
  pinMode(internalSensorPin,INPUT);
  pinMode(input,OUTPUT);
  //analogReference(EXTERNAL);
  Serial.begin(9600);
  analogWrite(input, inputValue);
  initialExternalSensorValue = analogRead(externalSensorPin);
  initialInternalSensorValue = analogRead(internalSensorPin);
  previousInternalSensorValue = initialInternalSensorValue;
}

void loop(){
  externalSensorValue=analogRead(externalSensorPin);
  internalSensorValue=getAverageValue(internalSensorPin,10);
  // Detected value
  previousInternalSensorValue = filteredValue(previousInternalSensorValue, internalSensorValue);
  displayValue();
  t+=1;
  delay(delayT);
}

float getAverageValue(int pin, int nbMeasures){
  int sum=0;
  for (int i=0; i<nbMeasures; i++){
    sum+=analogRead(pin);
    delay(0.02);
  }
  return sum/nbMeasures;
}

float filteredValue(int vp,int v){
  return v*alpha + vp*(1-alpha);
}

void displayThreshold(){
  Serial.print("minimal pressure threshold in kPa : ");
  Serial.print(pressureThresholdkPa);
  Serial.print("\nminimal pressure threshold in Bar : ");
  Serial.print(pressureThresholdBar);
  Serial.print("\n");
}

void displayValue(){
  Serial.print(t);
  Serial.print("  ");
  Serial.print(internalSensorValueTokPa(previousInternalSensorValue));
  Serial.print("  ");
  Serial.print(valueTokPa(externalSensorValue));
  Serial.print("\n");
}
