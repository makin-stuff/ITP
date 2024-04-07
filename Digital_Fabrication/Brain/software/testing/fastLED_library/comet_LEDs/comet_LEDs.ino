// This code comes from this tutorial: https://www.youtube.com/watch?v=yM5dY7K2KHM

#include <Arduino.h>            // Arduino Framework
#define FASTLED_INTERNAL        // Suppress build banner
#include <FastLED.h>

#define NUM_LEDS    160          // FastLED definitions
#define LED_PIN     6

CRGB g_LEDs[NUM_LEDS] = {0};    // Frame buffer for FastLED

int g_Brightness = 70;           // 0-255 LED brightness scale

void set_max_power_indicator_LED(uint8_t);

#include "twinkle.h"
#include "comet.h"

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);               // Add our LED strip to the FastLED library
  FastLED.setBrightness(g_Brightness);
  FastLED.setMaxPowerInMilliWatts(900);
  set_max_power_indicator_LED(LED_BUILTIN);

  FastLED.clear(true);
}

void loop() 
{

  while (true)
  {
    double dStart = millis() / 1000.0;                 // Display a frame and calc how long it takes

    // Handle LEDs

    // DrawCometSegment(0, 39);
    // DrawTwinkle();
    DrawComet();

    FastLED.show(g_Brightness);
  }
}
