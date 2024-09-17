//Ethan's Attempt at creating a skeleton to control all 4 motors

/////////////////////////////////////////
////  ---- HOW TO SEND AN INSTRUCTION ----
//// 1) verify an instruction ID exists
//// 2) In serial monitor send in one line your instruction
//// 3) it should be 10 bytes (ASCII characters) long
//// 4) it should start with 'I' and end with 'E'
//// 5) after 'I' it alternates with Command for Motor 1, Value for Motor 1, Command Motor 2, etc.
//// 6) If you send the command It0000000E it the console should spit out "Special Instruction 116 ('t'): Test Command"
//// 7) If you see another character be read of a decimal value 10 or 0x0a it is a newline character from entering the command.
//// 8) Should you see a newline character be read, make sure to change the line ending parameter to No Line Ending in the upper-right corner of the Arduino Serial Monitor
//////////////////////////////////////////

//Initializations and Constants -----------------------------------------------------------------------------
//Motor Controller Enables
#define LEFT_ENABLE   22    //Choice 
#define RIGHT_ENABLE  23    //Choice

//Motor 1 Constants
#define M1_LPWM A0          //aka D54
#define M1_RPWM A1          //aka D55
#define M1_ENCODER_A 18     //Interruptable (2, 3, 18, 19, 20, 21)
#define M1_ENCODER_B 17     //Choice
volatile int  M1_encoder_position = 0;
volatile bool M1_ccw = false;

//Motor 2 Constants
#define M2_LPWM A2          //aka D56
#define M2_RPWM A3          //aka D57
#define M2_ENCODER_A 19     //Interruptable (2, 3, 18, 19, 20, 21)
#define M2_ENCODER_B 16     //Choice
volatile int  M2_encoder_position = 0;
volatile bool M2_ccw = false;

//Motor 3 Constants
#define M3_LPWM A4          //aka D58
#define M3_RPWM A5          //aka D59
#define M3_ENCODER_A 20     //Interruptable (2, 3, 18, 19, 20, 21)
#define M3_ENCODER_B 15     //Choice
volatile int  M3_encoder_position = 0;
volatile bool M3_ccw = false;

//Motor 4 Constants
#define M4_LPWM A6          //aka D60
#define M4_RPWM A7          //aka D61
#define M4_ENCODER_A 21     //Interruptable (2, 3, 18, 19, 20, 21)
#define M4_ENCODER_B 14     //Choice
volatile int  M4_encoder_position = 0;
volatile bool M4_ccw = false;

bool valid_instruction = false;  //Set in validateAndParseNextInstruction() and in loop() to prevent bad instructions from being executed.
char current_byte;               //Used in validateAndParseNextInstruction()
char instruction[8];             //Contains the recieved instruction
/* From my understanding, the arduino will recieve a series of bytes (an instruction)
 * of 10 bytes long it should parse and control the motors with. This is my method of 
 * doing that. I'm not great with Serial communication so if this is "bad"
 * Let me know how I can improve it, because I'm literally just guessing here! XOXO <3
 * Index--Purpose -> Explaination
 * N/A - Initialize -> If this byte does not match the <valid command input> (temporarily 'I') byte ignore everything else
 * 0 - Command 1 --> Commands for Motor 1 or All Motors (Ex. 0 = stop motor 1; 1 = drive motor 1 forward; 2 = backwards; 3 = slowdown to stop; ... 255 = stop all motors)
 * 1 - Value 1 ----> Absolute Value for Command 1/Motor 1 (Expects DEC 0 to 255) goes to M1_LPWM or M1_RPWM depending on instruction
 * 2 - Command 2 --> Commands for Motor 2 or All Motors (Ex. 0 = stop motor 2; 1 = drive motor 2 forward; 2 = backwards; 3 = slowdown to stop; ... 255 = stop all motors)
 * 3 - Value 2 ----> Absolute Value for Command 2/Motor 2 (Expects DEC 0 to 255) goes to M2_LPWM or M2_RPWM depending on instruction
 * 4 - Command 3 --> Commands for Motor 3 or All Motors (Ex. 0 = stop motor 3; 1 = drive motor 3 forward; 2 = backwards; 3 = slowdown to stop; ... 255 = stop all motors)
 * 5 - Value 3 ----> Absolute Value for Command 3/Motor 3 (Expects DEC 0 to 255) goes to M3_LPWM or M3_RPWM depending on instruction
 * 6 - Command 4 --> Commands for Motor 4 or All Motors (Ex. 0 = stop motor 4; 1 = drive motor 4 forward; 2 = backwards; 3 = slowdown to stop; ... 255 = stop all motors)
 * 7 - Value 4 ----> Absolute Value for Command 4/Motor 4 (Expects DEC 0 to 255) goes to M4_LPWM or M4_RPWM depending on instruction
 * I don't know if this is the structure
*/

//Setup -----------------------------------------------------------------------------
void setup() {
  
  Serial.begin(9600);

  //Motor Controller Enables
  pinMode(LEFT_ENABLE, OUTPUT);
  pinMode(RIGHT_ENABLE, OUTPUT);
  digitalWrite(LEFT_ENABLE, HIGH);  //Permanently HIGH
  digitalWrite(RIGHT_ENABLE, LOW);  //Toggle HIGH/LOW to toggle motor movement Enable/Disable

  //Motor 1 Pin Setup
  pinMode(M1_LPWM, OUTPUT);
  pinMode(M1_RPWM, OUTPUT);
  pinMode(M1_ENCODER_A, INPUT);
  pinMode(M1_ENCODER_B, INPUT);
  digitalWrite(M1_ENCODER_A, HIGH);   //Pullup Resistor
  digitalWrite(M1_ENCODER_B, HIGH);   //Pullup Resistor

  //Motor 2 Pin Setup
  pinMode(M2_LPWM, OUTPUT);
  pinMode(M2_RPWM, OUTPUT);
  pinMode(M2_ENCODER_A, INPUT);  
  pinMode(M2_ENCODER_B, INPUT);
  digitalWrite(M2_ENCODER_A, HIGH);   //Pullup Resistor
  digitalWrite(M2_ENCODER_B, HIGH);   //Pullup Resistor

  //Motor 3 Pin Setup
  pinMode(M3_LPWM, OUTPUT);
  pinMode(M3_RPWM, OUTPUT);
  pinMode(M3_ENCODER_A, INPUT);   
  pinMode(M3_ENCODER_B, INPUT);
  digitalWrite(M3_ENCODER_A, HIGH);   //Pullup Resistor
  digitalWrite(M3_ENCODER_B, HIGH);   //Pullup Resistor

  //Motor 4 Pin Setup
  pinMode(M4_LPWM, OUTPUT);
  pinMode(M4_RPWM, OUTPUT);
  pinMode(M4_ENCODER_A, INPUT);   
  pinMode(M4_ENCODER_B, INPUT);
  digitalWrite(M4_ENCODER_A, HIGH);   //Pullup Resistor
  digitalWrite(M4_ENCODER_B, HIGH);   //Pullup Resistor

  /*Need 4 Interrupt functions because the middle term calls a special type of function which cannot have parameters
   *This means we cannot pass in a parameter to specify which motor has ticked if we want to cound each seperately
   *If using tank controls where we only need to keep track of each pair of wheels (left front/back and right front/back)
   *we'd only need 2 because we'd only be keeping track of M1 and M2.
  */
  attachInterrupt(digitalPinToInterrupt(M1_ENCODER_A), doEncoder1, CHANGE); // encoder track A on interrupt 1 - pin 3
  attachInterrupt(digitalPinToInterrupt(M2_ENCODER_A), doEncoder2, CHANGE); // encoder track A on interrupt 1 - pin 3
  attachInterrupt(digitalPinToInterrupt(M3_ENCODER_A), doEncoder3, CHANGE); // encoder track A on interrupt 1 - pin 3
  attachInterrupt(digitalPinToInterrupt(M4_ENCODER_A), doEncoder4, CHANGE); // encoder track A on interrupt 1 - pin 3
}

//Program Loop  ----------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  while(Serial.available() >= 9){   //If a full new instruction is ready

    validateAndParseNextInstruction();             //parse the instruction into instruction[0..7]
    
    if(valid_instruction){
      
      if(instruction[0] >= 97){       //Check to see if the first command is a "special command" (notated with a lowercase ASCII letter, a = 97, b = 98, ...)
        doSpecialInstruction();       //If so, use the "special instrcution" Look-Up-Table (LUT)
      }
      else{                           //otherwise
        doM1Instruction();            //Go through and do each motor's instruction (ASCII characters up to but excluding 97)
        doM2Instruction();            //This can probably be condensed in some form by using the same LUT for each motor
        doM3Instruction();            //and looping through an array where each index corresponds to a different "motor ID" 1..4
        doM4Instruction();            //I did it this way to break things up for testing purposes as I haven't thought of which way is faster/more efficient.
      }
      
      Serial.println("Debug Text");
      
      while(Serial.available() >= 0){
        Serial.println(Serial.read());
      }

    }
  }
}

//Functions  ----------------------------------------------------------------------------------------------------------------------------------------------------------
void doEncoder1() { //every time a change happens on encoder pin A doEncoder will run.
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

void doEncoder2() { //every time a change happens on encoder pin A doEncoder will run.
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

void doEncoder3() { //every time a change happens on encoder pin A doEncoder will run.
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

void doEncoder4() { //every time a change happens on encoder pin A doEncoder will run.
  if (digitalRead(M4_ENCODER_A) == HIGH) { // found a low-to-high on channel A
    if (digitalRead(M4_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M4_ccw = true; // CCW
    } 
    else {
      M4_ccw = false; // CW
    }
  } 
  else { //found a high-to-low on channel A
    if (digitalRead(M4_ENCODER_B) == LOW) { // check channel B to see which way encoder is spinning
      M4_ccw = false; // CW
    } 
    else {
      M4_ccw = true; // CCW
    }
  }

  if (M4_ccw){
    M4_encoder_position--; //if encoder is spinning CCW subtract a count from encoder position
  } 
  else {
    M4_encoder_position++; //if encoder is spinning CW add a count to encoder position
  }
}

void validateAndParseNextInstruction(){
  current_byte = Serial.read();    //Get first byte
  if(current_byte != 'I'){         //Only continue on valid initialization
    Serial.println("Not a valid instruction header");
    valid_instruction = false;
    return;
  }
  
  //Temp Debug Test
  Serial.println("Valid instruction header, filling out instruction array:");
  
  //get the rest of the instruction
  for(int i = 0; i <= 8; i++){
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
    return;
  }
  valid_instruction = true;
}

//TODO:
//INSTRUCTION FUNCTIONS -------------------------------------------------------------------------------------------------------------------------------------------------
/* There is probably a much faster way to do this but for each Special/M1/M2/M3/M4
 * instruction table there is a switch...case statement to put in the code
 * that matches each instruction ID.
 * There is also a default case which disables all motors (can be changed so each motor has seperate turn off switch) 
*/

//Special Instruction Lookup Table
void doSpecialInstruction(){
  switch (instruction[0]){
  //'d'...
  case 100:
    Serial.print("Special Instruction 99 ('c'): Disable Motors");
    digitalWrite(RIGHT_ENABLE, LOW);
    break;
  //'e'
  case 101: 
    Serial.print("Special Instruction 101 ('e'): Enable Motors");
    digitalWrite(RIGHT_ENABLE, HIGH);      
    break;
  //'f'
  case 102:
    Serial.print("Special Instruction 98 ('b'): Set All Motors to Max ('Forwards')");
    for(int i = 255; i > 0; i >> 1){  //while any motor spins backwards, to avoid destroying a motor, decelerate all motors to zero.
      analogWrite(M1_LPWM, analogRead(M1_LPWM));  //by dividing by 2
      analogWrite(M2_LPWM, analogRead(M2_LPWM));
      analogWrite(M3_LPWM, analogRead(M3_LPWM));
      analogWrite(M4_LPWM, analogRead(M4_LPWM));
    }
    for(int i = 1; i <= 255;){  //Then increase motor speed....
      analogWrite(M1_RPWM, i);
      analogWrite(M2_RPWM, i);
      analogWrite(M3_RPWM, i);
      analogWrite(M4_RPWM, i);  
      i << 1;                   //by doubling the speed every iteration
      i++;                      //and adding one to make shift work
      }                         //chose to shift instead of i *= 2 to get to exactly 255 without making a special case
      break;
  //'r'
  case 114:
    Serial.print("Special Instruction 114 ('r'): Set All Motors to Max ('Reverse')");
     for(int i = 255; i > 0; i >> 1){  //while any motor spins backwards, to avoid destroying a motor, decelerate all motors to zero.
      analogWrite(M1_RPWM, analogRead(M1_RPWM));  //by dividing by 2
      analogWrite(M2_RPWM, analogRead(M2_RPWM));
      analogWrite(M3_RPWM, analogRead(M3_RPWM));
      analogWrite(M4_RPWM, analogRead(M4_RPWM));
    }
    for(int i = 1; i <= 255;){  //Then increase motor speed....
      analogWrite(M1_LPWM, i);
      analogWrite(M2_LPWM, i);
      analogWrite(M3_LPWM, i);
      analogWrite(M4_LPWM, i);  
      i << 1;                   //by doubling the speed every iteration
      i++;                      //and adding one to make shift work
      }                         //chose to shift instead of i *= 2 to get to exactly 255 without making a special case
      break;
  //'t'...
  case 116:
    Serial.print("Special Instruction 116 ('t'): Test Command");
    break;
  //'etc'...
  //case ASCII ID [97..127]:
  //  Serial.print("Special Instruction [97..127] ('ASCII char'): Command Name");
  //  break;
  default:  
    //if a command was sent, but no valid option was found, diable motors
    digitalWrite(RIGHT_ENABLE, LOW);
    break;
  //End switch..case
  Serial.print("Special Instruction ");
  Serial.print(instruction[0]);
  Serial.print(" was recieved and interpereted");
  }
}

//Motor 1 Instruction Lookup Table
void doM1Instruction(){
  switch (instruction[0]){
  /*case a:
    //Code for M1 instruction with ID 'a'
    break;
  case b:
    //Code for M1 instruction with ID 'b'
    break;
  case c:
    //Code for M1 instruction with ID 'c'
    break; */
  default:
    digitalWrite(RIGHT_ENABLE, LOW);
    Serial.print("No M1 instruction found by the instruction ID: ");
    Serial.println(instruction[0]);
    break;
  Serial.print("M2 instruction ");
  Serial.print(instruction[0]);
  Serial.print(" was recieved and interpereted"); 
  }
}

//Motor 2 Instruction Lookup Table
void doM2Instruction(){
  switch (instruction[2]){
  /*case a:
    //Code for M2 instruction with ID 'a'
    break;
  case b:
    //Code for M2 instruction with ID 'b'
    break;
  case c:
    //Code for M2 instruction with ID 'c'
    break; */
  default:
    digitalWrite(RIGHT_ENABLE, LOW);
    Serial.print("No M2 instruction found by the instruction ID: ");
    Serial.println(instruction[2]);
    break;
  Serial.print("M2 instruction ");
  Serial.print(instruction[2]);
  Serial.print(" was recieved and interpereted"); 
  }
}

//Motor 3 Instruction Lookup Table
void doM3Instruction(){
  switch (instruction[4]){
  /*case a:
    //Code for M3 instruction with ID 'a'
    break;
  case b:
    //Code for M3 instruction with ID 'b'
    break;
  case c:
    //Code for M3 instruction with ID 'c'
    break; */
  default:
    digitalWrite(RIGHT_ENABLE, LOW);
    Serial.print("No M3 instruction found by the instruction ID: ");
    Serial.println(instruction[4]);
    break;
  Serial.print("M3 instruction ");
  Serial.print(instruction[4]);
  Serial.print(" was recieved and interpereted"); 
  }
}

//Motor 4 Instruction Lookup Table
void doM4Instruction(){
  switch (instruction[6]){
  /*case a:
    //Code for M4 instruction with ID 'a'
    break;
  case b:
    //Code for M4 instruction with ID 'b'
    break;
  case c:
    //Code for M4 instruction with ID 'c'
    break; */
  default:
    digitalWrite(RIGHT_ENABLE, LOW);
    Serial.print("No M4 instruction found by the instruction ID: ");
    Serial.println(instruction[6]);
    break;
  Serial.print("M4 instruction ");
  Serial.print(instruction[6]);
  Serial.print(" was recieved and interpereted"); 
  }
}