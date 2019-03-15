#include <Servo.h> 
#include <LiquidCrystal.h>

// defines pins numbers
const int triggerPin = 9;
const int echoPin = 10;
const int buttonPin = 8;

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

enum servoPosition {FORWARD, BACK, LEFT, RIGHT, UP};

Servo horizontalServo;
Servo verticalServo;
LiquidCrystal lcd(rsPin, enablePin, d4, d5, d6, d7);

// defines variables
bool triggered = false;

void setup() {
pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buttonPin, INPUT); // Sets the buttonPin as an Input.

horizontalServo.attach(horizontalServoPin);
verticalServo.attach(verticalServoPin);

lcd.begin(16, 2);

Serial.begin(9600); // Starts the serial communication
}

void loop() {
  if((digitalRead(buttonPin) == HIGH) && !triggered) {
     takeReading();
     triggered = true;
  }

  if(digitalRead(buttonPin) == LOW)
    triggered = false;
    triggerServo(FORWARD);
}

void writeToDisplay(int distance) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(distance + " cm");
}

void takeReading() {
  triggerServo(FORWARD);
  writeToDisplay(getDistance());
  delay(2000);
  
  triggerServo(RIGHT);
  writeToDisplay(getDistance());
  delay(2000);

  triggerServo(BACK);
  writeToDisplay(getDistance());
  delay(2000);
  
  triggerServo(LEFT);
  writeToDisplay(getDistance());
  delay(2000);

  triggerServo(UP);
  writeToDisplay(getDistance());
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
  int distance = 0;
  long duration = 0;
  
    // Clears the trigPin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  Serial.println(distance);
  return distance;
}
