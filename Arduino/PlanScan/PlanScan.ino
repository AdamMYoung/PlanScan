#include <Servo.h> 
#include <LiquidCrystal.h>
#include <NewPing.h>
#include <Encoder.h>

//________________________________________________________________________________
// Menu

/*
 * MenuNode class representing an entry of the LCD menu.
 */
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

//Menu
const int leftButtonPin = A3;
const int rightButtonPin = A4;
const int selectButtonPin = A5;
int selectedMenuNode = 0;

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

/*
 * Initializes pin I/O and libraries.
 */
void setup() {
  pinMode(buttonPin, INPUT); // Sets the buttonPin as an Input.
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  pinMode(selectButtonPin, INPUT);

  horizontalServo.attach(horizontalServoPin);
  verticalServo.attach(verticalServoPin);
  horizontalServo.write(0);
  verticalServo.write(0);
  delay(200);
  
  lcd.begin(16, 2);

  encoder.write(0);

  showNodeOnDisplay();
  Serial.begin(9600); // Starts the serial communication
}

/*
 * Checks the menu buttons for input.
 */
void loop() {
  checkMenuButtons();
}

/*
 * Reads the encoder value, and updates the stored information if different.
 */
void readEncoder(){
  long newVal = encoder.read() / 4;
  if(encoderVal != newVal){
    Serial.print("Encoder Val = ");
    Serial.print(newVal);
    Serial.println();
    encoderVal = newVal;
  }
}

/*
 * Sets the menu text. 
 */
void setMenu(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

/*
 * Set the distance text on the LCD.
 */
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

/*
 * Iterates through each position, outputting the distance to the LCD.
 */
void takeReading() {
  triggerServo(FORWARD);
  delay(1000);
  setDistance(getDistance(), "F");  
  delay(500);
  
  triggerServo(RIGHT);
  delay(1000);
  setDistance(getDistance(), "R");
  delay(500);

  triggerServo(BACK);
  delay(1000);
  setDistance(getDistance(), "B");
  delay(500);
  
  triggerServo(LEFT);
  delay(1000);
  setDistance(getDistance(), "L");
  delay(500);
 
  triggerServo(UP);
  delay(1000);
  setDistance(getDistance(), "U");
  delay(500);
}

/*
 * Moves the servos to the specified position.
 */
void triggerServo(servoPosition position) {
  switch(position) {
     case FORWARD: 
        horizontalServo.write(90);
        verticalServo.write(0);
        break;

      case RIGHT: 
        horizontalServo.write(180);
        //verticalServo.write(0);
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

/*
 * Gets the current distance from the ultrasonic sensor.
 */
int getDistance() {
  delay(50);
  
  int distance = sonar.ping_cm();
  // Prints the distance on the Serial Monitor
  Serial.println(distance);
  return distance;
}

//-------- Menu ---------

/*
 * Menu items for selection.
 */
MenuNode menuList[3] =
{
  MenuNode("PlanScan (1/3)", "Start Scan", startScan),
  MenuNode("PlanScan (2/3)", "Measurements", func2),
  MenuNode("PlanScan (3/3)", "Calibration", func3)
};

/*
 * Starts reading distance measurements.
 */
void startScan(){
  takeReading();
  delay(200);
  showNodeOnDisplay();
}

void func2(){
  
}
void func3(){
  
}

/*
 * Displays the currently selected node onto the LCD.
 */
void showNodeOnDisplay()
{
  setMenu(menuList[selectedMenuNode].line1, menuList[selectedMenuNode].line2);
}

/*
 * Called when the left button is selected.
 */
void selectLeft()
{
  if (selectedMenuNode != 0)
  {
    selectedMenuNode = selectedMenuNode - 1;
    showNodeOnDisplay();
  }
}

/*
 * Called when the right button is selected.
 */
void selectRight()
{
  if (selectedMenuNode != 2)
  {
    selectedMenuNode = selectedMenuNode + 1;
    showNodeOnDisplay();
  }
}

/*
 * Called when the center button is selected.
 */
void selectAction()
{
  menuList[selectedMenuNode].action();
  showNodeOnDisplay();
}

/*
 * Checks each menu button for interaction, and calls associated methods if selected.
 */
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
