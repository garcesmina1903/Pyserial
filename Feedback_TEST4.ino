#include <elapsedMillis.h>
elapsedMillis timeElapsed;

int RPWM = 10;
int LPWM = 11;
int sensorVal;
int sensorPin = A3;
int maxAnalogReading;
int minAnalogReading;
int Speed = 255;
float desiredPosfloat;
int desiredPos;
int Buffer = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  Serial.setTimeout(10);
  maxAnalogReading = moveToLimit(1);            //Calibration Extension
  minAnalogReading = moveToLimit(-1);           //Calibration Retraction
}

void loop() {
  // put your main code here, to run repeatedly:
while (Serial.available()==0){}         //Wait for userInput
String userInput = Serial.readString();     //Reading userInput
//Serial.println(userInput);

desiredPosfloat = mapfloat(userInput.toFloat(),0.0,6.0,float(minAnalogReading),float(maxAnalogReading));       //converting userInput to float analog number(feedback analog signal)
desiredPos = int(desiredPosfloat);      //converting analog userInput to integer(since analog signals are integers)
//Serial.println(desiredPosfloat);              
//Serial.println(desiredPos);
sensorVal = analogRead(sensorPin);      //Read feedback analog signal to check current position

while (desiredPos != sensorVal){        //keep moving until the current position matches with desired position
  sensorVal = analogRead(sensorPin);
  //Serial.println(sensorVal);
  if(desiredPos > (sensorVal+Buffer)){      //if desired position is greater than current position, extend        
    driveActuator(1, Speed);                
    }
  else if(desiredPos < (sensorVal-Buffer)){     //if not, retract        
    driveActuator(-1, Speed);
    }
  else{
    driveActuator(0, Speed);      //if they match, break the loop and wait for another userInput
    break;
    }
  delay(10);
  }
}

int moveToLimit(int Direction){       //moving to completely extended/retracted position to read max/min analog value
  int prevReading=0;
  int currReading=0;
  do{
    prevReading = currReading;
    driveActuator(Direction, Speed);
    timeElapsed = 0;
    while(timeElapsed < 200){ delay(1);}           //keep moving until analog reading remains the same for 200ms
    currReading = analogRead(sensorPin);
  }while(prevReading != currReading);
  return currReading;
}

void driveActuator(int Direction, int Speed){
  switch(Direction){
    case 1:       //extension
      analogWrite(RPWM, Speed);
      analogWrite(LPWM, 0);
      break;
   
    case 0:       //stop/hold position
      analogWrite(RPWM, 0);
      analogWrite(LPWM, 0);
      break;

    case -1:      //retraction
      analogWrite(RPWM, 0);
      analogWrite(LPWM, Speed);
      break;
  }
}

float mapfloat(float x, float inputMin, float inputMax, float outputMin, float outputMax){
 return (x-inputMin)*(outputMax - outputMin)/(inputMax - inputMin)+outputMin;
}
