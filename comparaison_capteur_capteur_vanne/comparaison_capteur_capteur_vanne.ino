#include "conversion.h";

// ALPHA à tester entre 0.1 et 0.5
const float alpha = 0.1;

//pin of the sensor
const int externalSensorPin = A0;
const int input = 3;
const int internalSensorPin = A2;

//last readen value from the captor
int externalSensorValue=0;
int initialExternalSensorValue=0;

int internalSensorValue=0;
int previousInternalSensorValue=0;
int initialInternalSensorValue=0;


int inputValue = 10;


//temps t
int t=0;
const int delayT = 10;

//minimal threshold
float pressureThresholdkPa = valueTokPa(1);
float pressureThresholdBar = valueToBar(1);
 
void setup(){
  pinMode(externalSensorPin,INPUT);
  pinMode(internalSensorPin,INPUT);
  pinMode(input,OUTPUT);
  //analogReference(INTERNAL);
  Serial.begin(9600);
  analogWrite(input, inputValue);
  initialExternalSensorValue = analogRead(externalSensorPin);
  initialInternalSensorValue = analogRead(internalSensorPin);
  internalSensorValue = initialInternalSensorValue;
}

void loop(){
  previousInternalSensorValue = externalSensorValue;
  externalSensorValue=analogRead(externalSensorPin);
  internalSensorValue=analogRead(internalSensorPin);
  // Detected value
  displayValue();
  t+=1;
  delay(delayT);
}

// à faire
void getNextValue(){
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
  Serial.print(internalSensorValue);
  Serial.print("  ");
  Serial.print(valueTokPa(externalSensorValue));
  Serial.print("\n");
}
