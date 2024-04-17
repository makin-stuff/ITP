#include <Adafruit_NeoPixel.h>

#define DIR_PIN 3
#define STEP_PIN 4
#define LED_PIN 6
#define LED_COUNT 24

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int speedDelay;
int speedSlow = 3;
int speedFast = 1;
long count = 0;
long numSteps = 0;
int turnState = 4;
int numRotations;

int brightness = 10;
int fadeAmount = 1;

void setup() {
  Serial.begin(9600);
  if (!Serial);
  
  // put your setup code here, to run once:
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

  // Default values
  digitalWrite(STEP_PIN, HIGH);
  digitalWrite(DIR_PIN, HIGH);
  speedDelay = speedSlow;

  strip.begin(); // Initialize neopixel strip
//  strip.setBrightness(50); // Set brightness to about 1/5

  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 255, 255, 255);
  }
//  strip.setBrightness(brightness);
  strip.show();  // Turn off all pixels

//  strip.setBrightness(brightness);
//  strip.show();
}

void loop() {

  if (count == numSteps) {  // We've advanced all the steps
    // Advance to the next state
    if (turnState == 1) {
      turnState = 2;
    } else if (turnState == 2) {
      turnState = 3;
    } else if (turnState == 3) {
      turnState = 4;
    } else if (turnState == 4) {
      turnState = 1;
    }
    numRotations = random(1, 5);
    numSteps = numRotations * 1700;
    count = 0;  // reset count
  }
  
  switch(turnState) {
    case 1:
      digitalWrite(DIR_PIN, LOW);
      break;
    case 2:
      speedDelay = speedFast;
      break;
    case 3:
      digitalWrite(DIR_PIN, HIGH);
      break;
    case 4:
      speedDelay = speedSlow;
  }
  
  digitalWrite(STEP_PIN, HIGH);
  delay(speedDelay);
  digitalWrite(STEP_PIN, LOW);
  delay(speedDelay);

  count++;
//  Serial.print("numSteps: ");
//  Serial.println(numSteps);
//  Serial.print("count: ");
//  Serial.println(count);
//  Serial.print("state: ");
//  Serial.println(turnState);

  brightness = brightness + fadeAmount;

  if (brightness <= 0 || brightness >= 150) {
    fadeAmount = -fadeAmount;
  }
  Serial.println(brightness);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, brightness, brightness, brightness);
  }
  strip.show();
}
