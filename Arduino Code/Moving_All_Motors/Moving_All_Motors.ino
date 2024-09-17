//Ethan's Attempt at creating a skeleton to control all 4 motors

/////////////////////////////////////////
////  ---- HOW TO SEND AN INSTRUCTION ----
//// 1) verify an instruction ID exists
//// 2) In serial monitor send in one line your instruction
//// 3) it should be 10 bytes (ASCII characters) long
//// 4) it should start with 'I' and end with 'E'
//// 5) after 'I' it alternates with Command for Motor 0, Value for Motor 0, Command Motor 1, etc.
//// 6) If you send the command It0000000E it the console should spit out "Special Instruction 116 ('t'): Test Command"
//// 7) If you see another character be read of a decimal value 10 or 0x0a it is a newline character from entering the command.
//// 8) Should you see a newline character be read, make sure to change the line ending parameter to No Line Ending in the upper-right corner of the Arduino Serial Monitor
//////////////////////////////////////////

//Initializations and Constants -----------------------------------------------------------------------------
//Constants
#define NUMBER_OF_MOTORS 4  //Total number of motors
#define INSTRUCTION_SIZE 8  //Number of bytes to store

//Motor Controller Enables
#define M0_LEFT_ENABLE   22    //Choice Pin
#define M1_LEFT_ENABLE   23    //Choice Pin
#define M2_LEFT_ENABLE   24    //Choice Pin
#define M3_LEFT_ENABLE   25    //Choice Pin
#define M0_RIGHT_ENABLE  26    //Choice Pin
#define M1_RIGHT_ENABLE  27    //Choice Pin
#define M2_RIGHT_ENABLE  28    //Choice Pin
#define M3_RIGHT_ENABLE  29    //Choice Pin

//Motor 0 Constants
#define M0_LPWM A0          //aka D54
#define M0_RPWM A1          //aka D55
#define M0_ENCODER_A 18     //Interruptable (2, 3, 18, 19, 20, 21)
#define M0_ENCODER_B 17     //Choice Pin
volatile int  M0_encoder_position = 0;
volatile bool M0_ccw = false;

//Motor 1 Constants
#define M1_LPWM A2          //aka D56
#define M1_RPWM A3          //aka D57
#define M1_ENCODER_A 19     //Interruptable (2, 3, 18, 19, 20, 21)
#define M1_ENCODER_B 16     //Choice Pin
volatile int  M1_encoder_position = 0;
volatile bool M1_ccw = false;

//Motor 2 Constants
#define M2_LPWM A4          //aka D58
#define M2_RPWM A5          //aka D59
#define M2_ENCODER_A 20     //Interruptable (2, 3, 18, 19, 20, 21)
#define M2_ENCODER_B 15     //Choice Pin
volatile int  M2_encoder_position = 0;
volatile bool M2_ccw = false;

//Motor 3 Constants
#define M3_LPWM A6          //aka D60
#define M3_RPWM A7          //aka D61
#define M3_ENCODER_A 21     //Interruptable (2, 3, 18, 19, 20, 21)
#define M3_ENCODER_B 14     //Choice Pin
volatile int  M3_encoder_position = 0;
volatile bool M3_ccw = false;

int left_enable[4] = {M0_LEFT_ENABLE, M1_LEFT_ENABLE, M2_LEFT_ENABLE, M3_LEFT_ENABLE};
int right_enable[4] = {M0_RIGHT_ENABLE, M1_RIGHT_ENABLE, M2_RIGHT_ENABLE, M3_RIGHT_ENABLE};
int left_pwm[4] = {M0_LPWM, M1_LPWM, M2_LPWM, M3_LPWM};
int right_pwm[4] = {M0_RPWM, M1_RPWM, M2_RPWM, M3_RPWM};
int encoder_a[4] = {M0_ENCODER_A, M1_ENCODER_A, M2_ENCODER_A, M3_ENCODER_A};
int encoder_b[4] = {M0_ENCODER_B, M1_ENCODER_B, M2_ENCODER_B, M3_ENCODER_B};
volatile int encoder_position[4] = {M0_encoder_position, M1_encoder_position, M2_encoder_position, M3_encoder_position};
volatile bool ccw[4] = {M0_ccw, M1_ccw, M2_ccw, M3_ccw};

bool valid_instruction = false;  //Set in validateAndParseNextInstruction() and in loop() to prevent bad instructions from being executed.
char current_byte;               //Used in validateAndParseNextInstruction()
char instruction[8];             //Contains the recieved instruction
/* From my understanding, the arduino will recieve a series of bytes (an instruction)
 * of 10 bytes long it should parse and control the motors with. This is my method of 
 * doing that. I'm not great with Serial communication so if this is "bad"
 * Let me know how I can improve it, because I'm literally just guessing here! XOXO <3
 * Index--Purpose -> Explaination
 * N/A - Initialize -> If this byte does not match the <valid command input> (temporarily 'I') byte ignore everything else
 * 0 - Command 1 --> Commands for Motor 0 or All Motors (Ex. 0 = stop Motor 0; 1 = drive Motor 0 forward; 2 = backwards; 3 = slowdown to stop; ... 255 = stop all motors)
 * 1 - Value 1 ----> Absolute Value for Command 1/Motor 0 (Expects DEC 0 to 255) goes to M0_LPWM or M0_RPWM depending on instruction
 * 2 - Command 2 --> Commands for Motor 1 or All Motors (Ex. 0 = stop Motor 1; 1 = drive Motor 1 forward; 2 = backwards; 3 = slowdown to stop; ... 255 = stop all motors)
 * 3 - Value 2 ----> Absolute Value for Command 2/Motor 1 (Expects DEC 0 to 255) goes to M1_LPWM or M1_RPWM depending on instruction
 * 4 - Command 3 --> Commands for Motor 2 or All Motors (Ex. 0 = stop Motor 2; 1 = drive Motor 2 forward; 2 = backwards; 3 = slowdown to stop; ... 255 = stop all motors)
 * 5 - Value 3 ----> Absolute Value for Command 3/Motor 2 (Expects DEC 0 to 255) goes to M2_LPWM or M2_RPWM depending on instruction
 * 6 - Command 4 --> Commands for Motor 3 or All Motors (Ex. 0 = stop Motor 3; 1 = drive Motor 3 forward; 2 = backwards; 3 = slowdown to stop; ... 255 = stop all motors)
 * 7 - Value 4 ----> Absolute Value for Command 4/Motor 3 (Expects DEC 0 to 255) goes to M3_LPWM or M3_RPWM depending on instruction
 * I don't know if this is the structure
*/

//Setup -----------------------------------------------------------------------------
void setup() {
  
  Serial.begin(9600);

  //Motor Controller and Encoder arrays setup
  for(int i = 0; i < NUMBER_OF_MOTORS; i++){
    pinMode(left_enable[i], OUTPUT);  //permanently HIGH
    pinMode(right_enable[i], OUTPUT); //motor enable toggles
    pinMode(left_pwm[i], OUTPUT);     //
    pinMode(right_pwm[i], OUTPUT);   //
    pinMode(encoder_a[i], INPUT);     //
    pinMode(encoder_b[i], INPUT);     //
    digitalWrite(encoder_a[i], HIGH); //pullup resistors
    digitalWrite(encoder_b[i], HIGH); //pullup resistors
    
  }

  /*Need 4 Interrupt functions because the middle term calls a special type of function which cannot have parameters
   *This means we cannot pass in a parameter to specify which motor has ticked if we want to cound each seperately
   *If using tank controls where we only need to keep track of each pair of wheels (left front/back and right front/back)
   *we'd only need 2 because we'd only be keeping track of M0 and M1.
  */
  attachInterrupt(digitalPinToInterrupt(M0_ENCODER_A), doEncoder1, CHANGE); // encoder track A on interrupt 1 - pin 3
  attachInterrupt(digitalPinToInterrupt(M1_ENCODER_A), doEncoder2, CHANGE); // encoder track A on interrupt 1 - pin 3
  attachInterrupt(digitalPinToInterrupt(M2_ENCODER_A), doEncoder3, CHANGE); // encoder track A on interrupt 1 - pin 3
  attachInterrupt(digitalPinToInterrupt(M3_ENCODER_A), doEncoder4, CHANGE); // encoder track A on interrupt 1 - pin 3
}

//Program Loop  ----------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  while(Serial.available() >= 10){   //If a full new instruction is ready

    validateAndParseNextInstruction();             //parse the instruction into instruction[0..7]
    
    if(valid_instruction){
      
      if(instruction[0] >= 97){       //Check to see if the first command is a "special command" (notated with a lowercase ASCII letter, a = 97, b = 98, ...)
        doSpecialInstruction();       //If so, use the "special instrcution" Look-Up-Table (LUT)
      }
      else{                           //otherwise
        for(int i = 0; i < NUMBER_OF_MOTORS; i++){
          doIndividualMotorInstruction(i);
        }
      }   
      Serial.println("Awaiting next instruction");
    }
  }
}

//Functions  ----------------------------------------------------------------------------------------------------------------------------------------------------------
void doEncoder1() { //every time a change happens on encoder pin A doEncoder will run.
  if (digitalRead(M0_ENCODER_A) == HIGH) { // found a low-to-high on channel A
    if (digitalRead(M0_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M0_ccw = true; // CCW
    } 
    else {
      M0_ccw = false; // CW
    }
  } 
  else { //found a high-to-low on channel A
    if (digitalRead(M0_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M0_ccw = false; // CW
    } 
    else {
      M0_ccw = true; // CCW
    }
  }

  if (M0_ccw){
    M0_encoder_position--; //if encoder is spinning CCW subtract a count from encoder position
  } 
  else {
    M0_encoder_position++; //if encoder is spinning CW add a count to encoder position
  }
}

void doEncoder2() { //every time a change happens on encoder pin A doEncoder will run.
  if (digitalRead(M1_ENCODER_A) == HIGH) { // found a low-to-high on channel A
    if (digitalRead(M1_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M1_ccw = true; // CCW
    } 
    else {
      M1_ccw = false; // CW
    }
  } 
  else { //found a high-to-low on channel A
    if (digitalRead(M1_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M1_ccw = false; // CW
    } 
    else {
      M1_ccw = true; // CCW
    }
  }

  if (M1_ccw){
    M1_encoder_position--; //if encoder is spinning CCW subtract a count from encoder position
  } 
  else {
    M1_encoder_position++; //if encoder is spinning CW add a count to encoder position
  }
}

void doEncoder3() { //every time a change happens on encoder pin A doEncoder will run.
  if (digitalRead(M2_ENCODER_A) == HIGH) { // found a low-to-high on channel A
    if (digitalRead(M2_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M2_ccw = true; // CCW
    } 
    else {
      M2_ccw = false; // CW
    }
  } 
  else { //found a high-to-low on channel A
    if (digitalRead(M2_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M2_ccw = false; // CW
    } 
    else {
      M2_ccw = true; // CCW
    }
  }

  if (M2_ccw){
    M2_encoder_position--; //if encoder is spinning CCW subtract a count from encoder position
  } 
  else {
    M2_encoder_position++; //if encoder is spinning CW add a count to encoder position
  }
}

void doEncoder4() { //every time a change happens on encoder pin A doEncoder will run.
  if (digitalRead(M3_ENCODER_A) == HIGH) { // found a low-to-high on channel A
    if (digitalRead(M3_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M3_ccw = true; // CCW
    } 
    else {
      M3_ccw = false; // CW
    }
  } 
  else { //found a high-to-low on channel A
    if (digitalRead(M3_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M3_ccw = false; // CW
    } 
    else {
      M3_ccw = true; // CCW
    }
  }

  if (M3_ccw){
    M3_encoder_position--; //if encoder is spinning CCW subtract a count from encoder position
  } 
  else {
    M3_encoder_position++; //if encoder is spinning CW add a count to encoder position
  }
}

void validateAndParseNextInstruction(){
  current_byte = Serial.read();    //Get first byte
  if(current_byte != 'I'){         //Only continue on valid initialization
    Serial.println("Invalid instruction header");
    valid_instruction = false;
    return;
  }
  
  //Temp Debug Test
  Serial.println("Valid instruction header, filling out instruction array:");
  
  //get the rest of the instruction
  for(int i = 0; i <= 7; i++){
    current_byte = Serial.read();
    instruction[i] = current_byte;
    
    //Temp Debug Text
    Serial.print("Index ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(current_byte);
  }
  
  current_byte = Serial.read();  //Get "Conclusion byte"
  
  if(current_byte != 'E'){       //If it is the wrong byte
    valid_instruction = false;
    Serial.println("Invalid instruction conclusion");
    return;
  }
  Serial.println("Valid cnstruction concludion");
  valid_instruction = true;
}

//TODO:
//INSTRUCTION FUNCTIONS -------------------------------------------------------------------------------------------------------------------------------------------------
/* There is probably a much faster way to do this but for each Special/M0/M1/M2/M3
 * instruction table there is a switch...case statement to put in the code
 * that matches each instruction ID.
 * There is also a default case which disables all motors (can be changed so each motor has seperate turn off switch) 
*/

//Special Instruction Lookup Table
void doSpecialInstruction(){
  switch (instruction[0]){
  //'d'...
  case 100:
    Serial.println("Special Instruction 100 ('d'): Disable Motors");
    for(int i = 0; i < NUMBER_OF_MOTORS; i++){
      digitalWrite(right_enable[i], LOW);
    }
    break;
  //'e'
  case 101: 
    Serial.println("Special Instruction 101 ('e'): Enable Motors");
    for(int i = 0; i < NUMBER_OF_MOTORS; i++){
      digitalWrite(right_enable[i], HIGH);
    }     
    break;
  //'f'
  case 102:
    Serial.println("Special Instruction 102 ('f'): Set All Motors to Max ('Forwards')");
    for(int i = 255; i > 0; i >> 1){  //while any motor spins backwards, to avoid destroying a motor, decelerate all motors to zero.
      for(int j = 0; j < NUMBER_OF_MOTORS; j++){
        analogWrite(left_pwm[j], analogRead(left_pwm[j])/2);
      }
    }
    for(int i = 1; i <= 255;){  //Then increase motor speed....
      for(int j = 0; j < NUMBER_OF_MOTORS; j++){
        analogWrite(right_pwm[j], i);
      } 
      i << 1;                 //by doubling then...
      i++;                    //and adding one to make shift work
    }                         //chose to shift instead of i *= 2 to get to exactly 255 without making a special case
    break;
  //'r'
  case 114:
    Serial.println("Special Instruction 114 ('r'): Set All Motors to Max ('Reverse')");
    for(int i = 255; i > 0; i >> 1){  //while any motor spins backwards, to avoid destroying a motor, decelerate all motors to zero.
      for(int j = 0; j < NUMBER_OF_MOTORS; j++){
        analogWrite(right_pwm[j], analogRead(right_pwm[j])/2);
      }
    }
    for(int i = 1; i <= 255;){  //Then increase motor speed....
      for(int j = 0; j < NUMBER_OF_MOTORS; j++){
        analogWrite(left_pwm[j], i);
      } 
      i << 1;                 //by doubling then...
      i++;                    //and adding one to make shift work
    }                         //chose to shift instead of i *= 2 to get to exactly 255 without making a special case
    break;
  //'t'...
  case 116:
    Serial.println("Special Instruction 116 ('t'): Test Command");
    break;
  //'etc'...
  //case ASCII ID [97..127]:
  //  Serial.print("Special Instruction [97..127] ('ASCII char'): Command Name");
  //  break;
  default:  
    //if a command was sent, but no valid option was found, diable motors
    for(int i = 0; i < NUMBER_OF_MOTORS; i++){
      digitalWrite(right_enable[i], LOW);
    }
    break;
  //End switch..case
  Serial.print("Special Instruction ");
  Serial.print(instruction[0]);
  Serial.println(" was recieved and interpereted");
  }
}

//Individual Motor Instruction Lookup Table
void doIndividualMotorInstruction(int motorID){
  int instructionID = 2 * instruction[motorID];
  switch (instructionID){
  /*case a:
    //Code for M0 instruction with ID 'a'
    break;
  case b:
    //Code for M0 instruction with ID 'b'
    break;
  case c:
    //Code for M0 instruction with ID 'c'
    break; */
  default:
    for(int i = 0; i < NUMBER_OF_MOTORS; i++){
      digitalWrite(right_enable[i], LOW);
    }
    Serial.print("No M0 instruction found by the instruction ID: ");
    Serial.println(instruction[0]);
    break;
  Serial.print("M0 instruction ");
  Serial.print(instruction[0]);
  Serial.println(" was recieved and interpereted"); 
  }
}
