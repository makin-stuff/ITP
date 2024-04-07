// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define RGB_LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 160
#define LED_SEG 40  // 40 LEDs in each brain segment

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

int trigCount = 0;
unsigned long loopCount = 0;
int LEDspeed = 80;

int debugLED = LED_BUILTIN; // Use the on board LED to see when the PIR is triggered
int pirPin = 2; // Input pin for the PIR sensor
int pirState = LOW; // start assuming no motion is detected
int val = 0;  // Variable for reading the pin status

void setup() {
  Serial.begin(115200);

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)

  pinMode(debugLED, OUTPUT);
  pinMode(pirPin, INPUT);
}

void loop() {

  val = digitalRead(pirPin);  // Read input value
  if (val == HIGH) {
    digitalWrite(debugLED, HIGH); // turn LED ON
    if (pirState == LOW) {
      // We have just turned on
      Serial.println("Motion detected!");
      trigCount++;
      // We only want to print on the output change, not state
      pirState = HIGH;
    } 
  } else {
    digitalWrite(debugLED, LOW);  // turn LED OFF
    if (pirState == HIGH) {
      // We have just turned off
      Serial.println("Motion ended!"); 
      // We only want ot print on the output change, not state
      pirState = LOW;
    }
  }

  Serial.print("Count = ");
  Serial.println(trigCount);

  if (trigCount == 0) {
    LEDspeed = 80;
  } else if (trigCount == 1) {
    LEDspeed = 50;
  } else if (trigCount == 2) {
    LEDspeed = 30;
  } else if (trigCount == 3) {
    trigCount = 0;  // Reset count
    LEDspeed = 80;  // Reset the speed
    Serial.println("Resetting count!");
    fuzz();
  }

  if (loopCount % 2 == 0) { // 
    spiral1(LEDspeed);
  } else {
    spiral2(LEDspeed);
  }

  loopCount++;
}

void spiral1(int wait) {
  // For PCB 1 --> SPIRAL
  for(int i=0; i < LED_SEG-3; i++) { // For each pixel in strip segment...
    strip.clear();
    strip.setPixelColor(i, 255, 255, 255);         //  Set pixel's color (in RAM)
    strip.setPixelColor(i+1, 127, 127, 127);         //  Set pixel's color (in RAM)
    strip.setPixelColor(i+2, 63, 63, 63);         //  Set pixel's color (in RAM)
    strip.setPixelColor(i+3, 31, 31, 31);         //  Set pixel's color (in RAM)
    strip.setPixelColor(79-i, 255, 255, 255);
    strip.setPixelColor(78-i, 127, 127, 127);
    strip.setPixelColor(77-i, 63, 63, 63);
    strip.setPixelColor(76-i, 31, 31, 31);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void spiral2(int wait) {
  // For PCB 2 --> NEURONS
  for (int i = 0; i < LED_SEG-3; i++) {
    strip.clear();
    strip.setPixelColor(i+80, 255, 255, 255);
    strip.setPixelColor(i+81, 127, 127, 127);
    strip.setPixelColor(i+82, 63, 63, 63);
    strip.setPixelColor(i+83, 31, 31, 31);
    strip.setPixelColor(i+120, 255, 255, 255);
    strip.setPixelColor(i+121, 127, 127, 127);
    strip.setPixelColor(i+122, 63, 63, 63);
    strip.setPixelColor(i+123, 31, 31, 31);
    strip.show();
    delay(wait);
  }
}

void fuzz() {
  for (int i = 0; i < 250; i++) {
    strip.clear();
    int pix1 = random(0, 160);
    int pix2 = random(0, 160);
    int pix3 = random(0, 160);
    int pix4 = random(0, 160);
    int pix5 = random(0, 160);
    strip.setPixelColor(pix1, 255, 255, 255);
    strip.setPixelColor(pix2, 255, 255, 255);
    strip.setPixelColor(pix3, 255, 255, 255);
    strip.setPixelColor(pix4, 255, 255, 255);
    strip.setPixelColor(pix5, 255, 255, 255);
    strip.show();
    delay(30);
  }
  strip.clear();
  strip.show();
}
