//This is a general code which homes the front right wheel to home position from any other location
// I do this using the state machine approach which can be used with memory to implement the steering
// The assumption here goes that the the wheel is always between -45 degree and 90 degrees and does not cross those limits
// That condition can be maintained by using interrupts to force stop the steering from turning beyond those limits
// There is a foregoing assumption that the steering's state is known before the first homing -> before triggering homing, the steering has been in a known state


// Yet to do:
// Replace the serial print statements with apt motor control commands
// Generalise code to any of the four wheels
// Extend functionality to home to -45 degrees and +90 degrees.

#define LSL 34
#define LSR 35
#define button 13

bool start = 0; // This is the variable that will be used to check if the homing is triggered
int A, B; //The two variables which indicate which of the limit switches have been triggered
int Am, Bm; //These two variable together show the current VALID STATE of the wheel
Am = 0;
Bm = 0;

// State mapping is as follows(all states are of the form AmBm - 2 bit binary number):
// 00 Wheel is to the left of home (-45 to 0)
// 01 Wheel is between home and right (0 to 45)
// 10 (45 to 90 degrees)
bool home = 0; // This variable indicates the reaching of home from a non-home position

void IRAM_ATTR wait(){//This interrupt is used to trigger the homing mechanism when the steering is not homed already
//  A = digitalRead(LSL);
//  B = digitalRead(LSR);
//  if ~(A == 0 && B == 1){ // This is the condition for the wheel to be in the home position
//    start = 0;
//  }
  start = 1; // No checking of conditions is done. A press of the button is assumed to trigger the homing.
  // Instead should I read values and check as done above in the commented part?
}

void IRAM_ATTR LSleft(){
  A = 0; //Indicates the trigger of the left limit switch
}

void IRAM_ATTR LSright(){
  B = 0; //Sets the variable to zero indicating a trigger of the right limit switches
}

void IRAM_ATTR LSLaway(){
  A = 1;
  if(Am == 0 && Bm == 0){Am = 0; Bm = 1;} // Turning towards the right away from home position
  else{Am = 0; Bm = 0;} // Turning towards the left away from the home position
}

void IRAM_ATTR LSRaway(){
  B = 1;
  if(Am == 0 && Bm == 1){Am = 1; Bm = 0;}// Turning towards the right away from +45 degrees
  else{Am = 0; Bm = 1;} // Turning towards the left away from +45 degrees
}

void setup() {
  Serial.begin(115200);
  pinMode(LSL, INPUT_PULLUP);
  pinMode(LSR, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(button, wait, FALLING);
  attachInterrupt(LSL, LSleft, FALLING); // These interrupts are designed such that they will be triggered when the corresponding limit switch is obstructed
  attachInterrupt(LSR, LSright, FALLING);
  attachInterrupt(LSL, LSLaway, RISING);
  attachInterrupt(LSR, LSRaway, RISING); // These will be used to reset the A, B readings when the steering overshoots
}

void loop() {
  // The part of the code from here to end of this segment will home the wheel back given the previous state is known.
  // ---------------------- START OF SECTION -------------------------------------------------------------
  if (~(A == 0 && B == 1) && start == 1){//Represents the condition where the steering is NOT at home and homing has been triggered
    if(Am == 0 && Bm == 0){// When the present state is to the left
      Serial.println("Turning right");
    }
    else if(Am == 0 && Bm == 1){// When the state is in between
      Serial.println("Turning left");
    }
    else if(Am == 1 && Bm == 0){// When the state is to the rightmost
      Serial.println("Turning left");
    }
  }
  else if(A == 0 && B == 1){//Home reached
    start = 0; //reset the trigger
    Serial.println("Reached Home");
  }
  // ------------------------ END OF SECTION ---------------------------------------------------------------
}
