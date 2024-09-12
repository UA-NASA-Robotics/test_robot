//Modified/Simplified from --> https://github.com/UA-NASA-Robotics/test_bot/blob/main/encoderExample/main.cpp

// encoder pins
#define ENCODER_PIN_A 3   //Must be an interruptable pin. (e.g. 2, 3, 18, 19, 20, 21 on the Mega 2560) See --> https://www.arduino.cc/reference/tr/language/functions/external-interrupts/attachinterrupt/
#define ENCODER_PIN_B 7 

volatile int encoder0Pos = 0; //current encoder count
volatile bool ccw = false; //which direction the encoder is spinning.

void doEncoder() { //every time a change happens on encoder pin A doEncoder will run.
  if (digitalRead(ENCODER_PIN_A) == HIGH) { // found a low-to-high on channel A
    if (digitalRead(ENCODER_PIN_B) == LOW) { // check channel B to see which way encoder is spinning
      ccw = true; // CCW
    } 
    else {
      ccw = false; // CW
    }
  } 
  else { //found a high-to-low on channel A
    if (digitalRead(ENCODER_PIN_B) == LOW) { // check channel B to see which way encoder is spinning
      ccw = false; // CW
    } 
    else {
      ccw = true; // CCW
    }
  }

  if (ccw){
    encoder0Pos--; //if encoder is spinning CCW subtract a count from encoder position
  } 
  else {
    encoder0Pos++; //if encoder is spinning CW add a count to encoder position
  }
}

void setup() {
  Serial.begin(9600);

  //encoder pinModes
  pinMode(ENCODER_PIN_A, INPUT);
  digitalWrite(ENCODER_PIN_A, HIGH); // enable internal pullup resistor
  pinMode(ENCODER_PIN_B, INPUT);
  digitalWrite(ENCODER_PIN_B, HIGH); // enable internal pullup resistor

  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), doEncoder, CHANGE); // encoder track A on interrupt 1 - pin 3
}

void loop() {
  Serial.println(encoder0Pos);
}
