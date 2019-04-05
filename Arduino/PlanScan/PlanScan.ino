#include <Servo.h> 
#include <LiquidCrystal.h>
#include <NewPing.h>
#include <Encoder.h>
#include <LinkedList.h>

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

/*
 * A single reading of a direction.
 */
class DirectionalMeasurements 
{
  public:
    String direction;
    int leftReading;
    int rightReading;
    int angleHorizontal;
    int angleVertical;
  
    DirectionalMeasurements(String direction, int left, int right, int angleH, int angleV)
    { 
      direction = direction;
      leftReading = left;
      rightReading = right;
      angleHorizontal = angleH;
      angleVertical = angleV;
    }
    DirectionalMeasurements(){}
};

class PositionalEntry
{
  public:
    int encoderX;
    int encoderY;
    DirectionalMeasurements directions[5];
    
    PositionalEntry(int encX, int encY){
      encoderX = encX;
      encoderY = encY;
    }
    //LinkedList library requires an empty constructor.
    PositionalEntry(){}
};

//Menu
const int directionalButtonPin = A4;
const int selectButtonPin = A5;
int selectedMenuNode = 0;

// Ultrasonic
const int maxDistance = 500;
// A
const int triggerPinA = 8;
const int echoPinA = 9;

// B
const int triggerPinB = 10;
const int echoPinB = 11;

//Servo
const int horizontalServoPin = 13;
const int verticalServoPin = 12;

//LCD Display
const int rsPin = 2;
const int enablePin = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;

//Encoders
//A
const int encoderPinA = A0;
const int encoderPinB = A1;

//B
const int encoderPinC = A2;
const int encoderPinD = A3;

enum servoPosition {FORWARD, BACK, LEFT, RIGHT, UP};

Servo horizontalServo;
Servo verticalServo;
NewPing sonarA(triggerPinA, echoPinA, maxDistance);
NewPing sonarB(triggerPinB, echoPinB, maxDistance);
Encoder encoderA(encoderPinA, encoderPinB);
Encoder encoderB(encoderPinC, encoderPinD);
LiquidCrystal lcd(rsPin, enablePin, d4, d5, d6, d7);

// defines variables
long encoderValA = -999;
long encoderValB = -999;
LinkedList<PositionalEntry> *positions = new LinkedList<PositionalEntry>;

/*
 * Initializes pin I/O and libraries.
 */
void setup() {
  pinMode(directionalButtonPin, INPUT);
  pinMode(selectButtonPin, INPUT);

  horizontalServo.attach(horizontalServoPin);
  verticalServo.attach(verticalServoPin);
  horizontalServo.write(0);
  verticalServo.write(0);
  delay(200);
  
  lcd.begin(16, 2);

  encoderA.write(0);
  encoderB.write(0);

  showNodeOnDisplay();
  Serial.begin(9600); // Starts the serial communication
}

/*
 * Checks the menu buttons for input.
 */
void loop() {
  readEncoderA();
  readEncoderB();
  checkMenuButtons();
}

/*
 * Reads the encoder value, and updates the stored information if different.
 */
void readEncoderA(){
  long newVal = encoderA.read() / 4;
  if(encoderValA != newVal){
    Serial.println("Encoder Val A = ");
    Serial.println(newVal);
    encoderValA = newVal;
  }
}

/*
 * Reads the encoder value, and updates the stored information if different.
 */
void readEncoderB(){
  long newVal = encoderB.read() / 4;
  if(encoderValB != newVal){
    Serial.println("Encoder Val B = ");
    Serial.println(newVal);
    encoderValB = newVal;
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
void setDistance(int distanceA, int distanceB, String pos) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(pos);
  lcd.print(":");
  lcd.print(distanceA);
  lcd.print(",");
  lcd.print(distanceB);
  lcd.print(" cm");
}

/*
 * Iterates through each position, outputting the distance to the LCD.
 */
void takeReading() {
  PositionalEntry entry =  PositionalEntry(encoderValA, encoderValB);

  //Forward
  triggerServo(FORWARD);
  delay(1000);
  DirectionalMeasurements measurementForward =  DirectionalMeasurements("Forward", getDistance(sonarA), getDistance(sonarB),
                                                                  horizontalServo.read(), verticalServo.read()); 
  setDistance(measurementForward.leftReading, measurementForward.rightReading, "F");  
  delay(500);

  //Right
  triggerServo(RIGHT);
  delay(1000);
  DirectionalMeasurements measurementRight =  DirectionalMeasurements("Right", getDistance(sonarA), getDistance(sonarB),
                                                                  horizontalServo.read(), verticalServo.read()); 
  setDistance(measurementRight.leftReading, measurementRight.rightReading, "R");
  delay(500);

  //Back
  triggerServo(BACK);
  delay(1000);
  DirectionalMeasurements measurementBack =  DirectionalMeasurements("Back", getDistance(sonarA), getDistance(sonarB),
                                                                  horizontalServo.read(), verticalServo.read()); 
  setDistance(measurementBack.leftReading, measurementBack.rightReading, "B");
  delay(500);

  //Left
  triggerServo(LEFT);
  delay(1000);
  DirectionalMeasurements measurementLeft =  DirectionalMeasurements("Left", getDistance(sonarA), getDistance(sonarB),
                                                                  horizontalServo.read(), verticalServo.read()); 
  setDistance(measurementLeft.leftReading, measurementLeft.rightReading, "L");
  delay(500);

  //Up
  triggerServo(UP);
  delay(1000);
  DirectionalMeasurements measurementUp =  DirectionalMeasurements("Up", getDistance(sonarA), getDistance(sonarB),
                                                                  horizontalServo.read(), verticalServo.read()); 
  setDistance(measurementUp.leftReading, measurementUp.rightReading, "U");
  delay(500);

  entry.directions[0] = measurementForward;
  entry.directions[1] = measurementLeft;
  entry.directions[2] = measurementBack;
  entry.directions[3] = measurementRight;
  entry.directions[4] = measurementUp;

  positions->add(entry);
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
int getDistance(NewPing sonar) {
  delay(50);

  int distance = sonar.ping_cm();
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
  MenuNode("PlanScan (3/3)", "Export Data", exportReadings)
};

/*
 * Starts reading distance measurements.
 */
void startScan(){
  Serial.print("Starting scan");
  Serial.println();
  takeReading();
  delay(200);
  showNodeOnDisplay();
}

void func2(){
  
}
void exportReadings(){
  exportData();
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

unsigned long lastMenuButtonPress = millis();

/*
 * Checks each menu button for interaction, and calls associated methods if selected.
 */
void checkMenuButtons()
{
  if (millis() >= lastMenuButtonPress + 300)
  {
    int directionalInput = analogRead(directionalButtonPin);
    if (directionalInput > 500 && directionalInput < 600)
    {
      lastMenuButtonPress = millis();
      selectLeft();
    }
    else if (directionalInput > 1000 && directionalInput < 1100)
    {
      lastMenuButtonPress = millis();
      selectRight();
    }
    else if (digitalRead(selectButtonPin) == HIGH)
    {
      lastMenuButtonPress = millis();
      menuList[selectedMenuNode].action();
    }  
  }
}

//-------- Export --------

/*
 * Exports the stored data to the serial monitor.
 */
void exportData() {
  Serial.println("TRANSFERSTART");

  for(int i = 0; i < positions->size(); i++){
    PositionalEntry entry = positions->get(i);    
    Serial.print(entry.encoderX); Serial.print("|");
    Serial.println(entry.encoderY);
    
    for(int j = 0; j < 5; j++){
      exportDirections(entry, j);
    }   
  }

  Serial.println("TRANSFEREND");
}

/*
 * Gets the required measurement from the entry, and exports it to a single line through the serial port.
 */
void exportDirections(PositionalEntry entry, int index){
    DirectionalMeasurements measurement = entry.directions[index];
    Serial.print(measurement.direction); Serial.print("|");
    Serial.print(measurement.leftReading); Serial.print("|"); 
    Serial.print(measurement.rightReading); Serial.print("|"); 
    Serial.print(measurement.angleHorizontal); Serial.print("|");
    Serial.println(measurement.angleVertical);
}
