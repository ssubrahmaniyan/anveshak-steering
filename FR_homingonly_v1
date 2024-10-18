// This code is designed for the front right wheel
// Trivial changes must be made for the other wheels(both in terms of code and physical flap design)
// At the moment the code has printing messages to signify the actions performed. Needs to be modified to perform actual steering

#define LSL 34
#define LSR 35
#define button 13

bool vlp, vrp, vln, vrn; //Using 4 variables to store both the present and the previous states
// vlp ~ value on left limit switch previously, vrn ~ value on right limit switch now
int trig = 0;
int dir = 0;
int ended = 0;

void IRAM_ATTR watch(){
  vln = digitalRead(LSL);
  vrn = digitalRead(LSR);
  trig = 1;
  dir = 0; //0 means it is turning left and 1 means it turns right
}

void IRAM_ATTR turnright(){
  vln = digitalRead(LSL);
  vrn = digitalRead(LSR);
  dir = 1;
}

void IRAM_ATTR home(){
  ended = 1;
}

void setup() {
  Serial.begin(115200);
  pinMode(LSL, INPUT_PULLUP);
  pinMode(LSR, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(button, watch, FALLING); // this interrupt is used to wait for the button to be pressed commanding the homing(turn left by default)
  attachInterrupt(LSL, turnright, FALLING); // this is called when the wheel homes beyond the correct position and requires to turn back
  attachInterrupt(LSR, home, FALLING); // this is called when home is reached and stops the turning
}

void loop() {
//      vln = digitalRead(LSL);
//      vrn = digitalRead(LSR);
  if (ended == 1){
    Serial.println("Home");
    ended = 0;
  }
  if (trig == 1){
      Serial.println("Button pressed");
      if ((vln == 1 && vrn == 1)){
        returntohome();
      }
      trig = 0;
  }
  if ((vln == 0 && vrn == 1) || (vln == 1 && vrn == 0)){
    vlp = vln; //storing the value for the next checking when the rover is at home or 45 degree angled
    vrp = vrn;
  }
}

void returntohome() {
  vln = digitalRead(LSL);
  vrn = digitalRead(LSR);
  Serial.println("Return to home sequence begin: \n");
  if(vln == 1 && vrn == 0){
    Serial.println("Returned home\n");
  }
  else if (vlp == 1 && vrp == 0){ //case where the rover is within -45 to +90 degrees from the home position
    vlp = vln;
    vrp = vrn;
    turn();
  }
}

void turn(){
  if (ended == 0 && dir == 0){
  Serial.println("turning left");
    turn();
  }
  else if (ended == 0 && dir == 1){
    Serial.println("turning right");
      turn();
  }
}
