#include <MultiStepper.h>
#include <AccelStepper.h>

// EG X-Y position bed driven by 2 steppers
// Alas its not possible to build an array of these with different pins for each :-(

AccelStepper stepperX(AccelStepper::DRIVER, 12, 7);
AccelStepper stepperY(AccelStepper::DRIVER, 11, 6);
AccelStepper stepperZ(AccelStepper::DRIVER, 10, 13);
AccelStepper stepperR(AccelStepper::DRIVER, 9, 5);
// Up to 10 steppers can be handled as a group by MultiStepper

#define switch_hX 2 // Pin 2 connected to Home Switch (MicroSwitch)
#define switch_hY 3 // Pin 3 connected to Home Switch (MicroSwitch)
#define switch_hZ 19 // Pin 19 connected to Home Switch (MicroSwitch)
#define switch_hR 20 // Pin 20 connected to Home Switch (MicroSwitch)
#define switch_I 21 // Pin 21 connected to Home Switch (MicroSwitch)
#define Relais_pin 40 // Pin 40 connected to the relais that turs the drivers on

// Communication variables
char StartChar = 'a';
char EndChar = 'a';

// movement variables and constants

long Row;
long column;
long stepsY;
long stepsX;
long columndistance = 351;     // Amount of y steps between the containers (350)
long Rowdistance = 465;      // Amount of x steps between the containers
long ZconstantP = 20000;       // Amount of steps to reach the containers
long ZconstantG = 20500;       // Amount of steps to reach the containers
long Yconstant = 100;    // amount of steps for grabbing the container
long Rconstant = 2590; // amount of steps to rotate the container 180 degrees
long straight= 555;


volatile bool Homing_status = false;
long initial_homingX = -1;
long initial_homingY = -1;
long initial_homingZ = -1;
long initial_homingR = -1;
long positions[2]; // 0=X 1=Y
volatile int state = 1;

void setup() {
  Serial.begin(9600);
  // Configure each stepper
  stepperX.setMaxSpeed(500);   //400
  stepperX.setAcceleration(150); //50
  stepperY.setMaxSpeed(500); //1000
  stepperY.setAcceleration(150); //50
  stepperZ.setMaxSpeed(3000);  //3000
  stepperZ.setAcceleration(600); //3000
  stepperR.setMaxSpeed(400); //3000
  stepperR.setAcceleration(100); //3000
  digitalWrite(Relais_pin, LOW);
  /*
   attachInterrupt(0, homeSwitch, RISING); //X
   attachInterrupt(1, homeSwitch, RISING); //Y
   attachInterrupt(4, homeSwitch, RISING); //Z
   //attachInterrupt(3, homeSwitch, RISING); //R
   
   */
  attachInterrupt(2, limitSwitch, RISING); //I

  // Then give them to MultiStepper to manage

  Home();
}
void loop() {
  switch (state) {
  case 1:
    CommandMessage();

    break;
  case 2:                 // "Get" state (Get Box)
    //Home();                //Home axis
    ConvertToSteps();     // Calculate the position of the container
    MoveToGoalXY();       // Move to the container
    containerAction(StartChar);      // Grab the container
    stepsX= 3188;
    stepsY= 104;
    MoveToGoalXY();         // Go to the placing position
    handingAction(); // placing on the agv
    Serial.println("Complete");
    state = 1;
    break;

  case 3:               // "Set" state (Place box)
    //Home();  
    stepsX= 3188;
    stepsY= 104;
    MoveToGoalXY();       // Move to the container
    grabbingAction();

    ConvertToSteps();     // Calculate the position of the container
    MoveToGoalXY();       // Move to the container
    containerAction(StartChar);      // Grab the container
    MoveToGoalXY();         // Go to the placing position
    //PlaceContainer();     // Place the container
    Serial.println("Complete");
    state = 1;
    break;

  case 4:               // "Error" state (Place box)
    Serial.println("Error line");
    break;

  default:
    Home();
    break;
  }
}





