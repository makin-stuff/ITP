#include <EncoderStepCounter.h>

// encoder pins:
// UP/DOWN
const int pin1 = 2;
const int pin2 = 3;
// LEFT/RIGHT
const int pin3 = 8;
const int pin4 = 9;
// Create encoder instance:
// UP/DOWN
EncoderStepCounter encoderUD(pin1, pin2);
// LEFT/RIGHT
EncoderStepCounter encoderLR(pin3, pin4);

// encoder previous position:
int oldPosition1 = 0;
int oldPosition2 = 0;

void setup() {
  Serial.begin(9600);
  // Initialize encoder
  encoderUD.begin();
  encoderLR.begin();
}

void loop() {
  // if you're not using interrupts, you need this in the loop:
  encoderUD.tick();
  // read encoder position:
  int position1 = encoderUD.getPosition();

  // if there's been a change, print it:
  if (position1 != oldPosition1) {
    Serial.println(position1);
    oldPosition1 = position1;
  }

  // if you're not using interrupts, you need this in the loop:
  encoderLR.tick();
  // read encoder position:
  int position2 = encoderLR.getPosition();

  // if there's been a change, print it:
  if (position2 != oldPosition2) {
    Serial.println(position2);
    oldPosition2 = position2;
  }
}
