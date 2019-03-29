#include <Servo.h> 
#include <LiquidCrystal.h>
#include <NewPing.h>
#include <Encoder.h>

//________________________________________________________________________________
// Menu


const int leftButtonPin = A3;
const int rightButtonPin = A4;
const int selectButtonPin = A5;
int selectedMenuNode = 0;

class MenuNode
{
  public:
    String line1;
    String line2;
    void (* action)();
    MenuNode(String l1, String l2, void(* a)())
    {
      line1 = l1;
      line2 = l2;
      action = a;
    }
};

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
  pinMode(buttonPin, INPUT); // Sets the buttonPin as an Input.
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  pinMode(selectButtonPin, INPUT);

  horizontalServo.attach(horizontalServoPin);
  verticalServo.attach(verticalServoPin);
  
  lcd.begin(16, 2);

  encoder.write(0);

  showNodeOnDisplay();
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  checkMenuButtons();
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

void setMenu(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void setDistance(int distance, String pos) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(pos);
  lcd.print(":");
  lcd.print(distance);
  lcd.print(" cm ");
}

void takeReading() {
  triggerServo(FORWARD);
  setDistance(getDistance(), "F");
  delay(2000);
  
  triggerServo(RIGHT);
  setDistance(getDistance(), "R");
  delay(2000);

  triggerServo(BACK);
  setDistance(getDistance(), "B");
  delay(2000);
  
  triggerServo(LEFT);
  setDistance(getDistance(), "L");
  delay(2000);

  triggerServo(UP);
  setDistance(getDistance(), "U");
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

MenuNode menuList[3] =
{
  MenuNode("PlanScan (1/3)", "Start Scan", startScan),
  MenuNode("PlanScan (2/3)", "Measurements", func2),
  MenuNode("PlanScan (3/3)", "Calibration", func3)
};

//Menu Functions
void startScan(){
  takeReading();
  showNodeOnDisplay();
}

void func2(){
  
}
void func3(){
  
}

//Menu Controls

void showNodeOnDisplay()
{
  setMenu(menuList[selectedMenuNode].line1, menuList[selectedMenuNode].line2);
}

void selectLeft()
{
  if (selectedMenuNode != 0)
  {
    selectedMenuNode = selectedMenuNode - 1;
    showNodeOnDisplay();
  }
}
void selectRight()
{
  if (selectedMenuNode != 2)
  {
    selectedMenuNode = selectedMenuNode + 1;
    showNodeOnDisplay();
  }
}
void selectAction()
{
  menuList[selectedMenuNode].action();
  showNodeOnDisplay();
}

void checkMenuButtons()
{
  if (digitalRead(leftButtonPin) == HIGH)
  {
    selectLeft();
    delay(300);
  }
  else if (digitalRead(rightButtonPin) == HIGH)
  {
    selectRight();
    delay(300);
  }
  else if (digitalRead(selectButtonPin) == HIGH)
  {
    menuList[selectedMenuNode].action();
  }
}
