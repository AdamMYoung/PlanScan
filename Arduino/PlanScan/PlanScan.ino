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

//Menu
const int menuLeftButton = 0;
const int menuRightButton = 1;
const int menuSelectButton = 13;

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

class Ultrasonic {
  const int triggerPin;
  const int echoPin;
  NewPing sonar;
  const int maxDistance = 500;

  public:
    Ultrasonic(int trigger, int echo): triggerPin(trigger), echoPin(echo), sonar(trigger, echo, maxDistance) {  
       pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
       pinMode(echoPin, INPUT); // Sets the echoPin as an Input    
    }

    int takeReading() {
      int distance = sonar.ping_cm();
      // Prints the distance on the Serial Monitor
      Serial.println(distance);
      return distance;
    }
};

class Display {
  const LiquidCrystal lcd;

  public :
    Display(int rs, int enable, int d4, int d5, int d6, int d7) : 
    lcd(rs, enable, d4, d5, d6, d7) {
      lcd.begin(16, 2);
    
    }
     
  void write(int distance, String pos) {
    lcd.setCursor(0, 1);
    lcd.print(pos);
    lcd.print(":");
    lcd.print(distance);
    lcd.print(" cm ");
  }

  void clear() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance:");
  }
};

class SensorController {
  const Servo horizontalServo;
  const Servo verticalServo;
  const Ultrasonic ultrasonicSensorA;
  const Ultrasonic ultrasonicSensorB;
  const Display display;
  bool isReading = false;
  
  enum servoPosition {FORWARD, BACK, LEFT, RIGHT, UP};

  public :
    SensorController(int hServo, int vServo, Ultrasonic sensorA, Ultrasonic sensorB, Display disp):
    ultrasonicSensorA(sensorA), ultrasonicSensorB(sensorB), display(disp)
    {
      horizontalServo.attach(hServo);
      verticalServo.attach(vServo);
    }

    void startDataCollection() {
      isReading = true;
      setServoPosition(FORWARD);
      display.write(ultrasonicSensorA.takeReading(), "F");
      delay(2000);
      
      setServoPosition(RIGHT);
      display.write(ultrasonicSensorA.takeReading(), "R");
      delay(2000);
    
      setServoPosition(BACK);
      display.write(ultrasonicSensorA.takeReading(), "B");
      delay(2000);
      
      setServoPosition(LEFT);
      display.write(ultrasonicSensorA.takeReading(), "L");
      delay(2000);
    
      setServoPosition(UP);
      display.write(ultrasonicSensorA.takeReading(), "U");
      delay(2000);
      isReading = false;
    }

    void isTakingReading() {
      return isReading;
    }

  private:
    void setServoPosition(servoPosition position) {
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
};

class RotaryEncoder {
  int pinA;
  int pinB;
  long encoderVal = -999;
  Encoder encoder;

  public:
    RotaryEncoder(int pinA, int pinB): pinA(pinA), pinB(pinB), encoder(pinA, pinB) {
    }

    int read() {
      long newVal = encoder.read() / 4;
      if(encoderVal != newVal){
      Serial.print("Encoder Val = ");
      Serial.print(newVal);
      Serial.println();
      encoderVal = newVal;
    }
  }
};

void setup() {
  pinMode(buttonPin, INPUT); // Sets the buttonPin as an Input.
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  readEncoder();
  if((digitalRead(buttonPin) == HIGH) && !triggered) {
     clearDisplay();
     takeReading();
     
  }

  if(digitalRead(buttonPin) == LOW)
    triggered = false;
    triggerServo(FORWARD);
}
