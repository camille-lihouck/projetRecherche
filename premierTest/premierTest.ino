#include "conversion.h";

//pin of the sensor
const int sensorPin = A0;
//last readen value from the captor
int sensorValue=0;
float pressureThresholdkPa = valueTokPa(1);
float pressureThresholdBar = valueToBar(1);
 
void setup(){
  pinMode(sensorPin,INPUT);
  Serial.begin(9600);
}

void loop(){
  sensorValue=analogRead(sensorPin);
  // Detected value
  Serial.print("\nsensorValue = ");
  Serial.print(sensorValue);
  // Conversion into kPa
  Serial.print("  value(kPa) = ");
  Serial.print(valueTokPa(sensorValue));
  delay(1000);
}

void displayThreshold(){
  Serial.print("minimal pressure threshold in kPa : ");
  Serial.print(pressureThresholdkPa);
  Serial.print("\nminimal pressure threshold in Bar : ");
  Serial.print(pressureThresholdBar);
  Serial.print("\n");
}
