//Pin closest to capacitor is ground, connect power supply (-) to silver wire
//Pin next to ground is Main power (I believe 24-26V)
//Next 2 pins are motor output. Take voltmeter reading across for testing.

#define RPWM 2
#define LPWM 4
#define left_enable 6
#define right_enable 8

void setup() {

  //set motor connections as outputs
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(left_enable, OUTPUT);
  pinMode(right_enable, OUTPUT);

  //Enable motors
  digitalWrite(left_enable, HIGH);
  digitalWrite(right_enable, HIGH);

  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);
}

void loop(){

safety();

//accelerate forward
for(int i = 0; i <= 255; i++){  //increases speed for 5.1 seconds
  analogWrite(LPWM, i);         //puts value of i into LPWM, the higher the value, the faster the speed.
  delay(20);
}

delay(1000); // keeps speed for 500ms

//Decelerate forward
for(int i = 255; i >= 0; i--){  //decreases forward speed until 0
  analogWrite(LPWM, i);         //puts value of i into LPWM, the higher the value, the faster the speed.
  delay (20);
}

delay(500);
safety();


//accelerate backwards
for(int i = 0; i <= 255; i++){  //increases speed for 5.1 seconds
  analogWrite(RPWM, i);         //puts value of i into LPWM, the higher the value, the faster the speed.
  delay(20);
}

delay(1000);

//Decelerate backwards 
for(int i = 255; i >= 0; i--){  //decreases forward speed until 0
  analogWrite(RPWM, i);         //puts value of i into LPWM, the higher the value, the faster the speed.
  delay (20);
}
}

void safety(){
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);
}
