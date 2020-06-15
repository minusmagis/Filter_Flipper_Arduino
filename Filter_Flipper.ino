/*This is the software fos the nanopto filter wheel or filter flipper
 * If you have any questions about it please contact minusmagis@gmail.com
 */

#include <Stepper.h>

const int stepsPerRevolution = 200;   // Change this to fit the number of steps per revolution
const int leftEndstop = 3;   // Normally open
const int rightEndstop = 2;  // Normally open
boolean leftState = 0;
boolean rightState = 0;
boolean left = 0;
boolean right = 0;
boolean finished = 0;



Stepper myStepper(stepsPerRevolution, 11, 10, 9, 8); //int4->d11 int3->d10 int2->d9 int1->d8 

void setup() {
  pinMode(leftEndstop, INPUT_PULLUP);             //Turn on the Pullup that will pull the two endstop switches up. Connect both endstops to the d2 pin and to ground
  pinMode(rightEndstop, INPUT_PULLUP);
  myStepper.setSpeed(120);             //Set the speed of the x axis stepper to 120 rpm?? (or steps per min I am not sure)
  Serial.begin(9600);
  Serial.println("Filter Flipper Arduino");
}

void loop() {
  // Positive steps turn clockwise
  delay(500);
  finished = 0;
  leftState = digitalRead(leftEndstop);
  rightState = digitalRead(rightEndstop);
  Serial.print("leftState");
  Serial.println(leftState);
  Serial.print("RightState");
  Serial.println(rightState);
  digitalWrite(A0,leftState);
  digitalWrite(A1,rightState);
  left = digitalRead(A2);
  right = digitalRead(A3);
  if (left == 1 && right == 0)
  {
    if(leftState == 0)
    {
      while(0<1){
        if (digitalRead(leftEndstop) == true) {                                // Read the value of pin 2 and if it is true (pressed) break the while loop
        break;
        }
        myStepper.step(-5);                                          // Move down the z axis motor untill the endstop (pin 2 is pressed) 
        }
        finished = 1;
    }
    else{
      Serial.println("Already left");
      finished = 1;
    }
  }
  else if (left == 0 && right == 1)
  {
    if(rightState == 0)
    {
      while(0<1){
        if (digitalRead(rightEndstop) == false) {                                // Read the value of pin 2 and if it is true (pressed) break the while loop
        break;
        }
        myStepper.step(5);                                          // Move down the z axis motor untill the endstop (pin 2 is pressed) 
        }
        finished = 1;
    }
    else{
      Serial.println("Already right");
      finished = 1;
    }
    
  }
  else if (left == 1 && right == 1)
  {
    Serial.println("Input Error");
  }
  else
  {
    
  }
  digitalWrite(A4,finished);

}
