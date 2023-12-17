#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[];

void DrawComet()
{
  // FastLED.clear(false);

  const byte fadeAmt = 64; // Fraction of 256 to fade a pixel by if it is chosen to be faded this pass
  const int cometSize = 3;  // Size of the comet in pixels
  const double cometSpeed = 0.25;  // How far to advance the comet every frame

  static int iDirection = 1;  // Current direction (-1 or +1)
  static double iPos = 0.0;  // Current comet position on strip

  iPos += iDirection * cometSpeed; // Update the comet position 

  // Flip the comet direction when it hits either end
  if (iPos == (NUM_LEDS - cometSize) || iPos == 0) {
    iDirection *= -1;
  }

  // Draw the comet at its current position
  for (int i = 0; i < cometSize; i++) {
    g_LEDs[(int)iPos + i].setRGB(255, 255, 255);
  }

  // Fade the LEDs one step
  for (int j = 0; j < NUM_LEDS; j++) {
    if (random(2) == 1) {
      g_LEDs[j] = g_LEDs[j].fadeToBlackBy(fadeAmt);
    }
  }

  delay(20);
}

void DrawCometSegment(int start, int end)
{
  const byte fadeAmt = 64; // Fraction of 256 to fade a pixel by if it is chosen to be faded this pass
  const int cometSize = 3;  // Size of the comet in pixels
  const double cometSpeed = 0.25;  // How far to advance the comet every frame

  static int iDirection = 1;  // Current direction (-1 or +1)
  static double iPos = start;  // Current comet position on strip

  iPos += iDirection * cometSpeed; // Update the comet position 

  // Flip the comet direction when it hits either end
  if (iPos == (end - cometSize) || iPos == start) {
    iDirection *= -1;
  }

  // Draw the comet at its current position
  for (int i = start; i < end; i++) {
    g_LEDs[(int)iPos + i].setRGB(255, 255, 255);
  }

  // Fade the LEDs one step
  for (int j = start; j < end; j++) {
    if (random(2) == 1) {
      g_LEDs[j] = g_LEDs[j].fadeToBlackBy(fadeAmt);
    }
  }

  delay(20);
}