#include <FastLED.h>

#define NUM_LEDS 160
#define LED_PIN 6
#define LED_DIM 40
#define LED_BRIGHT 100

CRGB leds[NUM_LEDS];

double speed = 0.25;

unsigned long startTime;
unsigned long endTime;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, LED_PIN>(leds, NUM_LEDS);

  Serial.begin(115200);
}

void loop() {
  // Comet for 10 seconds
  startTime = millis();
  endTime = startTime;
  while ((endTime-startTime) <= 10000) {
    drawComet();
    endTime = millis();
  }

  // Brain freeze for 3 seconds
  startTime = millis();
  endTime = startTime;
  while ((endTime-startTime) <= 3000) {
    drawFuzz();
    endTime = millis();
  }
}

void drawComet() {
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
    delay(5); // Slow down flashing a bit
  }
  FastLED.clear();
}