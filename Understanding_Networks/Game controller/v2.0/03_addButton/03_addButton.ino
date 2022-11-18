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

// Button
int buttonPin = 21;
int buttonStatePrev = HIGH;
int buttonState;
// Add some button debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  // Initialize encoder
  encoderUD.begin();
  encoderLR.begin();

  // Initialize button
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {

  // Check if button is pressed
  int reading = digitalRead(buttonPin);
  if (reading != buttonStatePrev) {
    lastDebounceTime = millis();
  }

  // Software debouncing
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        Serial.println("Button pressed");
      }
    }
  }
  buttonStatePrev = reading;
  
  // if you're not using interrupts, you need this in the loop:
  encoderUD.tick();
  // read encoder position:
  int position1 = encoderUD.getPosition();

  // if there's been a change, print it:
  if (position1 > oldPosition1) {
    Serial.println("up");
    oldPosition1 = position1;
  } else if (position1 < oldPosition1) {
    Serial.println("down");
    oldPosition1 = position1;
  }

  // if you're not using interrupts, you need this in the loop:
  encoderLR.tick();
  // read encoder position:
  int position2 = encoderLR.getPosition();

  // if there's been a change, print it:
  if (position2 > oldPosition2) {
    Serial.println("right");
    oldPosition2 = position2;
  } else if (position2 < oldPosition2) {
    Serial.println("left");
    oldPosition2 = position2;
  }
}
