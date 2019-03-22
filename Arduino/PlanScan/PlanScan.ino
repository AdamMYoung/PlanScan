#include <Servo.h> 
#include <LiquidCrystal.h>
#include <NewPing.h>
#include <Encoder.h>

/**
 * Class to manage an ultrasonic sensor data.
 */
class Ultrasonic {
  const int triggerPin;
  const int echoPin;
  NewPing sonar;
  const int maxDistance = 500;

  public:
    /**
     * Instantiates a new Ultrasonic object, using the trigger and echo pins to instantiate
     * a NewPing object.
     */
    Ultrasonic(int trigger, int echo): triggerPin(trigger), echoPin(echo), sonar(trigger, echo, maxDistance) {  
       pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
       pinMode(echoPin, INPUT); // Sets the echoPin as an Input    
    }

    /**
     * Returns the current distance from the ultrasonic sensor in centimeters.
     */
    int takeReading() {
      int distance = sonar.ping_cm();
      // Prints the distance on the Serial Monitor
      Serial.println(distance);
      return distance;
    }
};

/**
 * LiquidCrystal library wrapper to clear and format an LCD display.
 */
class Display {
  const LiquidCrystal lcd;

  public:
    /**
     * Instantiates a new Display object, using the provided pins 
     * to instantiate a LiquidCrystal object.
     */
    Display(int rs, int enable, int d4, int d5, int d6, int d7) : 
    lcd(rs, enable, d4, d5, d6, d7) {
      lcd.begin(16, 2);
    }

   /*
    * Writes the specified distance to the LCD at the start of the 2nd line.
    */
    void write(int distance, String pos) {
      lcd.setCursor(0, 1);
      lcd.print(pos);
      lcd.print(":");
      lcd.print(distance);
      lcd.print(" cm ");
    }

    /**
     * Resets the LCD display, then writes the "Distance" value to the first line.
     */
    void clear() {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Distance:");
    }
};

/**
 * Class to wrap the Encoder library.
 */
class RotaryEncoder {
  int pinA;
  int pinB;
  long encoderVal = -999;
  Encoder encoder;

  public:
    /**
     * Instantiates a new RotaryEncoder object, using the provied pins to
     * instantiate a new Encoder object.
     */
    RotaryEncoder(int pinA, int pinB): pinA(pinA), pinB(pinB), encoder(pinA, pinB) {
    }
    
    /**
     *  Reads the current encoder value, writing to the serial output
     *  if it has changed since the last reading.
     */
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

/**
 * Contoller class for handling sensor control.
 */
class SensorController {
  const Servo horizontalServo;
  const Servo verticalServo;
  const Ultrasonic ultrasonicSensorA;
  const Ultrasonic ultrasonicSensorB;
  const Display display;
  bool isReading = false;
  
  enum ServoPosition {FORWARD, BACK, LEFT, RIGHT, UP};

  public :
    /*
     * Instantiates a new SensorController object, using the provided pins to 
     * instantiate Servo objects, Ultrasonic objects for data acquisition and a Display
     * object for data output.
     */
    SensorController(int hServo, int vServo, Ultrasonic sensorA, Ultrasonic sensorB, Display disp):
    ultrasonicSensorA(sensorA), ultrasonicSensorB(sensorB), display(disp)
    {
      horizontalServo.attach(hServo);
      verticalServo.attach(vServo);
    }

    /**
     * Starts the data collection, iterating through each possible direction and
     * outputting the results to the display.
     */
    void startDataCollection() {
      isReading = true;
      display.clear();
      
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

    /**
     * Returns true if the controller is currently taking readings.
     */
    bool isTakingReading() {
      return isReading;
    }

  private:

    /**
     * Sets the direction of the ultrasonic sensor based on the position
     * enumerated type passed.
     */
    void setServoPosition(ServoPosition position) {
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

//Menu
const int menuLeftButton = 0;
const int menuRightButton = 1;
const int menuSelectButton = 13;
const int triggerButtonPin = 12;

//Ultrasonic Sensors.
Ultrasonic ultrasonicA(9, 10);
Ultrasonic ultrasonicB(9, 10); //This will need changing once pins have been sorted out.

//Display
Display displayA(2,3,4,5,6,7);

//Encoder
RotaryEncoder encoderA(A0, A1);

//Controllers
SensorController sensorController(12, 11, ultrasonicA, ultrasonicB, displayA);

void setup() {
  pinMode(triggerButtonPin, INPUT); // Sets the buttonPin as an Input.
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  if((digitalRead(triggerButtonPin) == HIGH) && !sensorController.isTakingReading()) {
    sensorController.startDataCollection();
  }
}
