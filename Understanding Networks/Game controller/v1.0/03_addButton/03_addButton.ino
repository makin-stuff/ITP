#include <Encoder.h>

// Up and down movement
// Yellow knob
Encoder udEnc(2, 3);
long udPrev = -999;
long udNew;

// Left and right movement
// Blue knob
Encoder lrEnc(8, 9);
long lrPrev = -999;
long lrNew;

// Button to login
// For now, I'm going to use the button built into the blue encoder
// Because I can't fit another button onto the small bread board
int buttonPin = 21;
int buttonStatePrev = HIGH;
int buttonState;
// Add some button debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  Serial.println("Basic encoder test");

  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Check if the button is pressed
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
  
  // First, check up/down encoder
  udNew = udEnc.read();
  // If the position has changed
  if (udNew != udPrev) {
    // Check which direction we moved
    if (udNew > udPrev && udNew % 4 == 0) {
      Serial.println("up");
    } else if (udNew < udPrev && udNew % 4 == 0){
      Serial.println("down");
    }
    udPrev = udNew;
  }

  // Next, check left/right encoder
  lrNew = lrEnc.read();
  // If the position has changed
  if (lrNew != lrPrev) {
    // Check which direction we moved
    if (lrNew > lrPrev && lrNew % 4 == 0) {
      Serial.println("right");
    } else if (lrNew < lrPrev && lrNew % 4 == 0){
      Serial.println("left");
    }
    lrPrev = lrNew;
  }
}
