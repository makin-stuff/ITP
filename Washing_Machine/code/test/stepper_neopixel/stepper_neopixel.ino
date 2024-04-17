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

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
   pinMode(DIR_PIN, OUTPUT);
   pinMode(STEP_PIN, OUTPUT);

   digitalWrite(STEP_PIN, HIGH);
   digitalWrite(DIR_PIN, HIGH);

   strip.begin(); // Initialize neopixel strip
   strip.show();  // Turn off all pixels
   strip.setBrightness(50); // Set brightness to about 1/5

   for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 255, 255, 255);
   }
   strip.show();

   speedDelay = speedSlow;
}

void loop() {
  if (count > 0 && count <= 1700) {
    Serial.println("stage 1");
    digitalWrite(DIR_PIN, LOW);
//    speedDelay = speedSlow;
  } else if (count > 1700 && count <= 3400) {
    Serial.println("stage 2");
    speedDelay = speedFast; // speed up
  } else if (count > 3400 && count <= 5100) {
    Serial.println("stage 3");
    digitalWrite(DIR_PIN, HIGH);  // switch directions
  } else if (count > 5100 && count <= 6800) {
    Serial.println("stage 4");
    speedDelay = speedSlow; // slow down
  } else {
    Serial.println("reset");
    count = 0;  // reset count
  }
  
  digitalWrite(STEP_PIN, HIGH);
  delay(speedDelay);
  digitalWrite(STEP_PIN, LOW);
  delay(speedDelay);

  count++;
  Serial.println(count);
}
