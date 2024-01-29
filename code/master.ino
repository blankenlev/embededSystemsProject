//Master
#include <Wire.h>

//Set variables/pins
const int pingPin = 7;
const int echoPin = 6;
const int startButton = 9;
const int resetButton = 3;
const int gLEDpin = 12, yLEDpin = 11, rLEDpin = 8, buzzPin = 5, delayPin = 0;
long startDistance = 0, distance = 0, maxDistance = 132, currentDist = 132; //Used to set defualt distances
int resetState, startState;
long startTime = 0, elapsedTime = 0, duration;
int delaySet;
int reset = 1; //Used as a sort of toggle
int delayMultiplier = 30; //In ms
int roomForError = 5; //In inches
int threshold = maxDistance-roomForError; //Adds room for error (bumping sensor)

//Initial setup
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(startButton, INPUT);
  pinMode(resetButton, INPUT);
  pinMode(gLEDpin, OUTPUT);
  pinMode(yLEDpin, OUTPUT);
  pinMode(rLEDpin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
}

void loop()
{
  //Set current distance and add thresholds
  delaySet = analogRead(delayPin)*delayMultiplier;
  startState = digitalRead(startButton);
  resetState = digitalRead(resetButton);
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  distance = microsecondsToInches(duration);
  currentDist = distance-startDistance+maxDistance;
  
  //When start button is pressed clear screen and call start sequence
  if(startState == LOW){
    reset = 0;
    Wire.beginTransmission(8);
    Wire.write('c');
    Wire.endTransmission();
    threshold = maxDistance-roomForError;
    startDistance = distance;
    startSequence();
    startTime = millis();
  }
  
  //Used to toggle reset button (after start pressed reset will no longer work)
  if(resetState == LOW){
   reset = 1; 
  }
  
  //Used to set delay
  if(reset == 1){
    digitalWrite(rLEDpin, LOW);
    Serial.print("Delay: ");
    Serial.println(delaySet);
    Wire.beginTransmission(8);
    Wire.write('e');
    Wire.write(highByte(delaySet));
    Wire.write(lowByte(delaySet));
    Wire.endTransmission();
  }
  
  //Used to display time after passing US sensor
  if(currentDist < threshold){
    digitalWrite(gLEDpin, LOW);
    digitalWrite(rLEDpin, HIGH);
    tone(buzzPin, 650, 100);
    elapsedTime = millis() - startTime;
    Wire.beginTransmission(8);
    Wire.write('d');
    Wire.write(highByte(elapsedTime));
    Wire.write(lowByte(elapsedTime));
    Wire.endTransmission();
    threshold = 1;
  }
}

//Used to convert seconds to inches for US sensor
long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

//Used for LEDs and buzzer for start sequence
void startSequence(){
  delay(delaySet);
  digitalWrite(rLEDpin, LOW);
  delay(1000);
  digitalWrite(rLEDpin, HIGH);
  tone(buzzPin, 350, 50);
  delay(1000);
  digitalWrite(rLEDpin, LOW);
  delay(1000);
  digitalWrite(yLEDpin, HIGH);
  tone(buzzPin, 350, 50);
  delay(1000);
  digitalWrite(yLEDpin, LOW);
  delay(1000);
  digitalWrite(gLEDpin, HIGH);
  tone(buzzPin, 500, 300);
}