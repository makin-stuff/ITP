#include <Adafruit_CircuitPlayground.h>

void setup() {
  //start serial connection
  Serial.begin(9600);

  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(255);

  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
}

void loop() {
  //read the pushbutton value into a variable
  int greenButton = digitalRead(2);
  int purpleButton = digitalRead(3);
  int blueButton = digitalRead(10);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (greenButton == HIGH) {
    digitalWrite(13, LOW);
    CircuitPlayground.setPixelColor(2, 0, 0, 0);
  } else {
    digitalWrite(13, HIGH);
    CircuitPlayground.setPixelColor(2, 0, 255, 0);
  }

  if (purpleButton == HIGH) {
    digitalWrite(13, LOW);
    CircuitPlayground.setPixelColor(9, 0, 0, 0);
  } else {
    digitalWrite(13, HIGH);
    CircuitPlayground.setPixelColor(9, 255, 0, 255);
  }

  if (blueButton == HIGH) {
    digitalWrite(13, LOW);
    CircuitPlayground.setPixelColor(7, 0, 0, 0);
  } else {
    digitalWrite(13, HIGH);
    CircuitPlayground.setPixelColor(7, 0, 0, 255);
  }

  // Wait a bit
  delay(1);
}
