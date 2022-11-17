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

void setup() {
  Serial.begin(115200);
  Serial.println("Basic encoder test");
}

void loop() {
  // First, check up/down encoder
  udNew = udEnc.read();
  // If the position has changed
  if (udNew != udPrev) {
    // Check which direction we moved
    if (udNew > udPrev && udNew % 4 == 0) {
      Serial.println("down");
    } else if (udNew < udPrev && udNew % 4 == 0){
      Serial.println("up");
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
