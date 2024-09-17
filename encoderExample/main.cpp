// edited from https://forum.arduino.cc/t/arduino-encoder-reading/44928/3
#include <Arduino.h>
#include <avr/interrupt.h>

#define WHEEL_DIAMETER 260  // in mm
#define PROCESSING_DELAY_SECONDS 0.1

// motor controller
#define RPWM 6
#define LPWM 7

// encoder
#define ENCODER_PIN_A 3 //Quadrature Track A
#define ENCODER_PIN_B 7 //Quadrature Track B
#define ENCODER_RESOLUTION 500  // ticks per rotation
// #define encoder0PinC 2 //Index Track

// https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
// "To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile."
volatile int encoder0Pos = 0; //the encoder position variable.
int prevEncoder0Pos = 0;
volatile bool ccw = false; //which direction the encoder is spinning.
float distanceTraversed = 0;

void doEncoder() { //every time a change happens on encoder pin A doEncoder will run.
  if (digitalRead(ENCODER_PIN_A) == HIGH) { // found a low-to-high on channel A
    if (digitalRead(ENCODER_PIN_B) == LOW) { // check channel B to see which way encoder is spinning
      ccw = true; // CCW
    } else {
      ccw = false; // CW
    }
  } else { //found a high-to-low on channel A
    if (digitalRead(ENCODER_PIN_B) == LOW) { // check channel B to see which way encoder is spinning
      ccw = false; // CW
    } else {
      ccw = true; // CCW
    }
  }

  if (ccw){
    encoder0Pos--; //if encoder is spinning CCW subtract a count from encoder position
  } else {
    encoder0Pos++; //if encoder is spinning CW add a count to encoder position
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);

  //encoder pinModes
  pinMode(ENCODER_PIN_A, INPUT);
  digitalWrite(ENCODER_PIN_A, HIGH); // enable internal pullup resistor
  pinMode(ENCODER_PIN_B, INPUT);
  digitalWrite(ENCODER_PIN_B, HIGH); // enable internal pullup resistor

  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), doEncoder, CHANGE); // encoder track A on interrupt 1 - pin 3
  // attachInterrupt(0, doIndex, RISING); // encoder Index track on interupt 0 - pin 2
}

void loop()
{
  // example motor control output
  // // backwards
  //   analogWrite(LPWM, 0);
  //   delay(100); // delay to prevent short circuit HBRIDGE if both inputs high (probably don't need switches pretty fast)
  //   analogWrite(RPWM, 255); // fully on PWM

  //   delay(10000);

  //   // forward rotation
  //   analogWrite(RPWM, 0);
  //   delay(100);
  //   analogWrite(LPWM, 255);
  prevEncoder0Pos = encoder0Pos;

  delay(PROCESSING_DELAY_SECONDS * 1000);

  // TODO: do linear interpolation between prev measured and current rotational velocity
  float rotationalVelocity = ((encoder0Pos - prevEncoder0Pos) / PROCESSING_DELAY_SECONDS) / ENCODER_RESOLUTION; // rotations per second

  // Serial.print(rotationalVelocity);
  // Serial.print(" rotations per second\n");

  distanceTraversed += rotationalVelocity * PROCESSING_DELAY_SECONDS * 3.14 * WHEEL_DIAMETER;

  Serial.println(int(distanceTraversed));
  // Serial.print(" distance traversed mm\n");
}