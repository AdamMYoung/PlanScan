#include <Servo.h> 
#include <LiquidCrystal.h>
#include <NewPing.h>
#include <Encoder.h>

// Ultrasonic
const int triggerPin = 9;
const int echoPin = 10;
const int buttonPin = 8;
const int maxDistance = 500;

//Servo
const int horizontalServoPin = 12;
const int verticalServoPin = 11;

//LCD Display
const int rsPin = 2;
const int enablePin = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;

//Encoder
const int encoderPinA = A0;
const int encoderPinB = A1;

enum servoPosition {FORWARD, BACK, LEFT, RIGHT, UP};

Servo horizontalServo;
Servo verticalServo;
NewPing sonar(triggerPin, echoPin, maxDistance);
Encoder encoder(encoderPinA, encoderPinB);
LiquidCrystal lcd(rsPin, enablePin, d4, d5, d6, d7);

// defines variables
bool triggered = false;
long encoderVal = -999;

void setup() {
  pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(buttonPin, INPUT); // Sets the buttonPin as an Input.
  
  horizontalServo.attach(horizontalServoPin);
  verticalServo.attach(verticalServoPin);
  
  lcd.begin(16, 2);

  encoder.write(0);
  
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  readEncoder();
  if((digitalRead(buttonPin) == HIGH) && !triggered) {
     clearDisplay();
     takeReading();
     triggered = true;
  }

  if(digitalRead(buttonPin) == LOW)
    triggered = false;
    triggerServo(FORWARD);
}

void readEncoder(){
  long newVal = encoder.read() / 4;
  if(encoderVal != newVal){
    Serial.print("Encoder Val = ");
    Serial.print(newVal);
    Serial.println();
    encoderVal = newVal;
  }
}

void clearDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
}

void writeToDisplay(int distance, String pos) {
  lcd.setCursor(0, 1);
  lcd.print(pos);
  lcd.print(":");
  lcd.print(distance);
  lcd.print(" cm ");
}

void takeReading() {
  triggerServo(FORWARD);
  writeToDisplay(getDistance(), "F");
  delay(2000);
  
  triggerServo(RIGHT);
  writeToDisplay(getDistance(), "R");
  delay(2000);

  triggerServo(BACK);
  writeToDisplay(getDistance(), "B");
  delay(2000);
  
  triggerServo(LEFT);
  writeToDisplay(getDistance(), "L");
  delay(2000);

  triggerServo(UP);
  writeToDisplay(getDistance(), "U");
  delay(2000);
}

void triggerServo(servoPosition position) {
  switch(position) {
     case FORWARD: 
        horizontalServo.write(90);
        verticalServo.write(0);
        break;

      case RIGHT: 
        horizontalServo.write(180);
        verticalServo.write(0);
        break;

      case BACK: 
        horizontalServo.write(90);
        verticalServo.write(180);
        break;

      case LEFT: 
        horizontalServo.write(0);
        verticalServo.write(0);
        break;
     
      case UP: 
        horizontalServo.write(90);
        verticalServo.write(90);
        break;
  }
}

//Gets the measured distance from the ultrasonic sensor.
int getDistance() {
  delay(50);
  
  int distance = sonar.ping_cm();
  // Prints the distance on the Serial Monitor
  Serial.println(distance);
  return distance;
}
