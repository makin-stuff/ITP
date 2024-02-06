#include <FastLED.h>

#define NUM_LEDS 160
#define LED_PIN 6
#define LED_DIM 40
#define LED_BRIGHT 100

CRGB leds[NUM_LEDS];

int ledPin = LED_BUILTIN;
int pirPin = 2;
int pirState = LOW;
int val = 0;

double speed = 0.25;

int moveCount = 0;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, LED_PIN>(leds, NUM_LEDS);

  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);

  Serial.begin(115200);
}

void loop() {
  val = digitalRead(pirPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      moveCount++;
      Serial.print("Move count: ");
      Serial.println(moveCount);
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }

  if (moveCount == 0){
    speed = 0.25;
  }
  else if (moveCount == 1) {
    speed = 0.5;
  } else if (moveCount == 2) {
    speed = 0.75;
  } else {  // count is 3?
    Serial.println("We made it");
    moveCount = 0;  // Reset count
    // speed = 0.25; // Reset speed
  }

  drawComet(speed);
}

void drawComet(double speed) {
  static double iPos1 = 0.0;
  static double iPos2 = 80.0;
  double cometSpeed = speed;
  const int cometSize = 3;
  const byte fadeAmt = 64;

  iPos1 += cometSpeed;
  iPos2 -= cometSpeed;

  if (iPos1 == 39) {
    iPos1 = 120.0;
  }

  if (iPos1 == 159) {
    iPos1 = 0.0;
  }

  if (iPos2 == 40) {
    iPos2 = 119.0;
  }

  if (iPos2 == 80) {
    iPos2 = 80.0;
  }

  for (int i = 0; i < cometSize; i++) {
    leds[(int)iPos1] = CRGB::White;
    leds[(int)iPos2] = CRGB::White;
  }
  
  for (int j = 0; j < 160; j++) {
    if (random(2) == 1) {
      leds[j] = leds[j].fadeLightBy(fadeAmt);
    }
  }

  delay(20);
  FastLED.show(LED_DIM);
}

void drawFuzz() {

  for (int i = 0; i < 10; i++) {
    leds[random(NUM_LEDS)] = CRGB::White;
    FastLED.show(LED_BRIGHT);
  }
  FastLED.clear();
}