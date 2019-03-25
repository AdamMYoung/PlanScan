#include <Servo.h> 
#include <LiquidCrystal.h>
#include <NewPing.h>
#include <Encoder.h>

/**
 * Class to manage an ultrasonic sensor data.
 */
class Ultrasonic {
   private:
      NewPing* _sonar;
      const int maxDistance = 500;
      
   public:
    /**
     * Instantiates a new Ultrasonic object, using the trigger and echo pins to instantiate
     * a NewPing object.
     */
     
    Ultrasonic(int trigger, int echo) {  
       pinMode(trigger, OUTPUT); // Sets the trigPin as an Output
       pinMode(echo, INPUT); // Sets the echoPin as an Input 
       _sonar = new NewPing(trigger, echo, maxDistance);
    }

    /**
     * Returns the current distance from the ultrasonic sensor in centimeters.
     */
    int takeReading() {
      int distance = _sonar->ping_cm();
      // Prints the distance on the Serial Monitor
      Serial.println(distance);
      return distance;
    }
};

class RotaryEncoder {
  private:
    long encoderVal = -999;
    Encoder* encoder;

  public:
    RotaryEncoder(int pinA, int pinB) {
      encoder = new Encoder(pinA, pinB);
    }

    int readEncoder() {
      long newVal = encoder->read() / 4;
      if(encoderVal != newVal){
        Serial.print("Encoder Val = ");
        Serial.print(newVal);
        Serial.println();
        encoderVal = newVal;
      }
    }
};

class Display {
  private:
    LiquidCrystal* lcd;

  public:
    Display() {
      lcd = new LiquidCrystal(2,3,4,5,6,7);
      lcd->begin(16,2);
    }
    
    /*
    * Writes the two lines specified to the LCD
    */
    void write(String line1, String line2)
    {
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print(line1);
      lcd->setCursor(0, 1);
      lcd->print(line2);
    }
    
    /*
    * Writes the specified distance to the LCD at the start of the 2nd line.
    */
    void writeDistance(int distance, String pos) {
      lcd->setCursor(0, 1);
      lcd->print(pos);
      lcd->print(":");
      lcd->print(distance);
      lcd->print(" cm ");
    }

    /**
     * Resets the LCD display, then writes the "Distance" value to the first line.
     */
    void clear() {
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("Distance:");
    }
};

 /*
 * Instantiates a new SensorController object, using the provided pins to 
 * instantiate Servo objects, Ultrasonic objects for data acquisition and a Display
 * object for data output.
 */
class SensorController {
  private:
    const Servo* horizontalServo;
    const Servo* verticalServo;
    Ultrasonic* ultrasonicA;
    Display* displayA;
    bool isReading = false;
    enum ServoPosition {FORWARD, BACK, LEFT, RIGHT, UP};

    /**
     * Sets the direction of the ultrasonic sensor based on the position
     * enumerated type passed.
     */
    void setServoPosition(ServoPosition position) {
      switch(position) {
         case FORWARD: 
            horizontalServo->write(90);
            verticalServo->write(0);
            break;
    
          case RIGHT: 
            horizontalServo->write(180);
            verticalServo->write(0);
            break;
    
          case BACK: 
            horizontalServo->write(90);
            verticalServo->write(180);
            break;
    
          case LEFT: 
            horizontalServo->write(0);
            verticalServo->write(0);
            break;
         
          case UP: 
            horizontalServo->write(90);
            verticalServo->write(90);
            break;
      }
    }

  public:
    SensorController() {
        ultrasonicA = new Ultrasonic(9, 10);
        displayA = new Display();

        horizontalServo = new Servo();
        verticalServo = new Servo();
        
        horizontalServo->attach(12);
        verticalServo->attach(11);
    }

    /**
     * Starts the data collection, iterating through each possible direction and
     * outputting the results to the display.
     */
    void startDataCollection() {
      isReading = true;
      displayA->clear();
      
      setServoPosition(FORWARD);
      displayA->write(ultrasonicA->takeReading(), "F");
      delay(2000);
      
      setServoPosition(RIGHT);
      displayA->write(ultrasonicA->takeReading(), "R");
      delay(2000);
    
      setServoPosition(BACK);
      displayA->write(ultrasonicA->takeReading(), "B");
      delay(2000);
      
      setServoPosition(LEFT);
      displayA->write(ultrasonicA->takeReading(), "L");
      delay(2000);
    
      setServoPosition(UP);
      displayA->write(ultrasonicA->takeReading(), "U");
      delay(2000);
      isReading = false;
    }

    /**
     * Returns true if the controller is currently taking readings.
     */
    bool isTakingReading() {
      return isReading;
    }  
};

//__________________________________________________
// Menu

class MenuNode
{
  public:
    String line1;
    String line2;
    MenuNode(String l1, String l2)
    {
      line1 = l1;
      line2 = l2;
    }
}

MenuNode menuList[3] =
{
  MenuNode("PlanScan (1/3)", "Start Scan"),
  MenuNode("PlanScan (2/3)", "Measurements"),
  MenuNode("PlanScan (3/3)", "Calibration")
};
int menuLast = sizeof(menuList) - 1;
int selectedMenuNode = 1;




const int buttonPin = 8;
SensorController controller;

void setup() {
  pinMode(buttonPin, INPUT); // Sets the buttonPin as an Input.
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  if((digitalRead(buttonPin) == HIGH) && !controller.isTakingReading()) {
     controller.startDataCollection();
  }
}
