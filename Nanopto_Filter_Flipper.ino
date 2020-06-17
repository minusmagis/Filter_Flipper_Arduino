/*This is the software for the nanopto filter wheel or filter flipper
   If you have any questions about it please contact minusmagis@gmail.com
*/

#include <TinyStepper_28BYJ_48.h>

//
// pin assignments, any digital pins can be used
//
const int LED_PIN = 13;
const int MOTOR_IN1_PIN = 11;
const int MOTOR_IN2_PIN = 10;
const int MOTOR_IN3_PIN = 9;
const int MOTOR_IN4_PIN = 8;


const int STEPS_PER_REVOLUTION = 2048;



//
// create the stepper motor object
//
TinyStepper_28BYJ_48 stepper;

const int leftEndstop = 3;   // Normally open (normally high)
const int rightEndstop = 2;  // Normally open
boolean leftState = 0;
boolean rightState = 0;


const byte CommandSize = 64;                                               // Expect commands of up to 64 bytes
char Command[CommandSize];                                                 // initialize an array of characters that will be the received command


void setup() {
  pinMode(leftEndstop, INPUT_PULLUP);             //Turn on the Pullup that will pull the two endstop switches up. Connect both endstops to the d2 pin and to ground
  pinMode(rightEndstop, INPUT_PULLUP);
  stepper.connectToPins(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_IN3_PIN, MOTOR_IN4_PIN);
  stepper.setSpeedInStepsPerSecond(500);
  stepper.setAccelerationInStepsPerSecondPerSecond(1000);
  Serial.begin(9600);
  Serial.println("Filter fipper arduino");
}

void loop() {
  GcodeCommandReceive();
  if (Command[0] == 'r' || Command[0] == 'R') {
    MoveRight();
  }
  else  if (Command[0] == 'l' || Command[0] == 'L') {
    MoveLeft();
  }
  else {
    //    Serial.println(digitalRead(rightEndstop));
    //    Serial.println(digitalRead(leftEndstop));
    ResetCommand();
    DisableSteppers();
  }
}

void MoveRight() {
  stepper.setCurrentPositionInSteps(0);
  stepper.setupMoveInSteps(-2048);
  while (!stepper.motionComplete()) {
    stepper.processMovement();
    if (digitalRead(rightEndstop) == 0)
    {
      break;
    }
    //    Serial.println("Moving Right");
  }
  Serial.println("Moved");
  ResetCommand();
}

void MoveLeft() {
  stepper.setCurrentPositionInSteps(0);
  stepper.setupMoveInSteps(2048);
  while (!stepper.motionComplete()) {
    stepper.processMovement();
    if (digitalRead(leftEndstop) == 0)
    {
      break;
    }
    //    Serial.println("Moving Left");
  }
  Serial.println("Moved");
  ResetCommand();
}

void ResetCommand() {
  for (int i = 0; i < CommandSize; i++) {
    Command[i] = '\0';
  }
}
void GcodeCommandReceive() {
  byte ndx = 0;                                                      // Define the index where the array will be written
  char endMarker = '\n';                                                    // Define the endmarker that will be recognized
  char rc;                                                                  // Received character

  while (Serial.available() > 0) {                   // While there is data in the serial port and the newData is not true keep reading
    rc = Serial.read();                                                  // Read the current byte in the buffer and store it in rc
//    Serial.println(rc);
    if (rc != endMarker) {                                               // If rc is not equal to the endmarker it means it is part of the command so write it on the array (in the place indicated by indx)
      Command[ndx] = rc;
      ndx++;                                                             // Increment the ndx counter so that the next character is written in the next spot on the array
      if (ndx >= CommandSize) {                                          // If the ndx overflows (is bigger than the commandSize specified) bring it down by one and start writing over the last value
        ndx = CommandSize - 1;                                           // of the array thus only replacing the last char and not causing errors
      }
    }
    else {
      Command[ndx] = '\0';                                               // Else (if rc == endMarker) then write \0 null value to the end of the array
      ndx = 0;                                                           // Reset the index counter for the next command
    }
  }
}

void DisableSteppers() {
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
}

