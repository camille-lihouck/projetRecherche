#include "conversion.h";

//pin of the sensor
const int externalSensorPin = A0;
const int internalSensorPin = A2;
//last readen value from the captor
int externalSensorValue=0;
int internalSensorValue=0;

int t=0;

//minimal threshold
float pressureThresholdkPa = valueTokPa(1);
float pressureThresholdBar = valueToBar(1);
 
void setup(){
  pinMode(externalSensorPin,INPUT);
  pinMode(internalSensorPin,OUTPUT);
  analogReference(INTERNAL);
  Serial.begin(9600);
}

void loop(){
  externalSensorValue=analogRead(externalSensorPin);
  internalSensorValue=analogRead(internalSensorPin);
  // Detected value
  Serial.print(t);
  Serial.print("  ");
  Serial.print(internalSensorValue);
  Serial.print("  ");
  Serial.print(externalSensorValue);
  Serial.print("\n");
  t+=1;
  delay(1000);
}

void displayThreshold(){
  Serial.print("minimal pressure threshold in kPa : ");
  Serial.print(pressureThresholdkPa);
  Serial.print("\nminimal pressure threshold in Bar : ");
  Serial.print(pressureThresholdBar);
  Serial.print("\n");
}
