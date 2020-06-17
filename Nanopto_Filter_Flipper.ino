/*This is the software for the nanopto filter wheel or filter flipper
   If you have any questions about it please contact minusmagis@gmail.com
*/

#include <Stepper.h>

const int stepsPerRevolution = 200;   // Change this to fit the number of steps per revolution
const int leftEndstop = 3;   // Normally open (normally high)
const int rightEndstop = 2;  // Normally open
boolean leftState = 0;
boolean rightState = 0;
boolean left = 0;
boolean right = 0;
String Command = "";
int counter = 0;



Stepper myStepper(stepsPerRevolution, 11, 10, 9, 8); //int4->d11 int3->d10 int2->d9 int1->d8

void setup() {
  pinMode(leftEndstop, INPUT_PULLUP);             //Turn on the Pullup that will pull the two endstop switches up. Connect both endstops to the d2 pin and to ground
  pinMode(rightEndstop, INPUT_PULLUP);
  myStepper.setSpeed(120);             //Set the speed of the x axis stepper to 120 rpm?? (or steps per min I am not sure)
  Serial.begin(9600);
  Serial.println("Filter fipper arduino");
}

void loop() {
//    Serial.println("it Works");
//    delay(500);
  Command = "";                                           //Restart the Command string
  Command = readCommand();                                //Read if any command is available
  if (Command == "70636478"  ||  Command == "38636478"  || Command == "38313246")                           //If The command is Left or left turn the left filter on
  {
    Serial.print("Turning right (Left Filter On)...   ");
    while (0 < 1) {
      if (digitalRead(rightEndstop) == false) {                                // Read the value of pin 2 and if it is true (pressed) break the while loop
        Serial.print("Left Filter On");
        break;
      }
      myStepper.step(-5);                                          // Move down the z axis motor untill the endstop (pin 2 is pressed)
    }
    for (int i = 8; i < 12; i++)
    {
      digitalWrite(i, LOW);                                     // Turn off all the pins to lower power consumption, the torque is high enough to prevent the axis from involuntarily moving.
    }
  }
  else if (Command == "7667656678"  ||  Command == "4467656678"  ||  Command == "4435333446")                          // If the master asks for the "homed" status send the homed status
  {
    Serial.print("Turning left (Right Filter On)...   ");
    while (0 < 1) {
      if (digitalRead(leftEndstop) == false) {                                // Read the value of pin 2 and if it is true (pressed) break the while loop
        Serial.print("Right Filter On");
        break;
      }
      myStepper.step(5);                                          // Move down the z axis motor untill the endstop (pin 2 is pressed)
    }
    for (int i = 8; i < 12; i++)
    {
      digitalWrite(i, LOW);                                     // Turn off all the pins to lower power consumption, the torque is high enough to prevent the axis from involuntarily moving.
    }
  }
}

String readCommand() {
  if (Serial.available() > 0)
  {
    String Command = "";                                          // Reset Coommand to a blank space before reading
    delay(200);                                                    // Small delay to allow for the serial to buffer all the data
    while(Serial.available() > 0)                  // Take the amount of digits in the serial and read them; i.e if the Serial.available() is 3 it will read the serial 3 times
    {
      delay(10);
      int v = Serial.read() - 38;
      Serial.println(v);
      Command += (v);                       // Append the read value (-48 which is the 0 value) to the previous one to form a complete string with all the values from the serial with their real value
    }
    Serial.println(Command); //For debugging purposes
    return Command;
  }
  else{
    counter ++;
    delay(50);
    if (counter >= 100){
      counter = 0;
    Serial.println("...");
    }
  }
}
