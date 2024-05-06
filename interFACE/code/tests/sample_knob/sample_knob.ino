#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

#include "AudioSampleBreath.h"
#include "AudioSampleClear.h"
#include "AudioSampleCough.h"
#include "AudioSampleScratch.h"
#include "AudioSampleSlurp.h"
#include "AudioSampleSnort.h"
#include "AudioSampleSnot.h"
#include "AudioSampleSwallow.h"

// GUItool: begin automatically generated code
AudioPlayMemory          sample1;        //xy=134,256
AudioPlayMemory          sample0;        //xy=136,212
AudioPlayMemory          sample2;        //xy=136,299
AudioPlayMemory          sample3;        //xy=136,341
AudioPlayMemory          sample4;       //xy=138,489
AudioPlayMemory          sample5;       //xy=140,530
AudioPlayMemory          sample6;       //xy=141,570
AudioPlayMemory          sample7;       //xy=142,613
AudioMixer4              mixer;          //xy=310,292
AudioMixer4              mixer1;         //xy=319,543
AudioMixer4              mixer2;         //xy=508,386
AudioOutputI2S           i2s1;           //xy=655,387
AudioConnection          patchCord1(sample1, 0, mixer, 1);
AudioConnection          patchCord2(sample0, 0, mixer, 0);
AudioConnection          patchCord3(sample2, 0, mixer, 2);
AudioConnection          patchCord4(sample3, 0, mixer, 3);
AudioConnection          patchCord5(sample4, 0, mixer1, 0);
AudioConnection          patchCord6(sample5, 0, mixer1, 1);
AudioConnection          patchCord7(sample6, 0, mixer1, 2);
AudioConnection          patchCord8(sample7, 0, mixer1, 3);
AudioConnection          patchCord9(mixer, 0, mixer2, 0);
AudioConnection          patchCord10(mixer1, 0, mixer2, 1);
AudioConnection          patchCord11(mixer2, 0, i2s1, 0);
AudioConnection          patchCord12(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=745,485
// GUItool: end automatically generated code

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AudioMemory(100);

  // Begin audio adapter board
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

//  mixer.gain(1, 0.5);
//  mixer.gain(2, 0.2);
//  mixer.gain(3, 0.5);
//  mixer.gain(4, 5.0);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sampleNum = map(analogRead(A14), 0, 1023, 0, 7);

  Serial.print("Sample number: ");
  Serial.println(sampleNum);
  Serial.print("Audio usage: ");
  Serial.println(AudioMemoryUsage());

  switch (sampleNum) {
    case 0:
      sample0.play(AudioSampleBreath);
      break;
    case 1:
      sample1.play(AudioSampleClear);
      break;
    case 2:
      sample2.play(AudioSampleCough);
      break;
    case 3:
      sample3.play(AudioSampleScratch);
      break;
    case 4:
      sample4.play(AudioSampleSlurp);
      break;
    case 5:
      sample5.play(AudioSampleSnort);
      break;
    case 6: 
      sample6.play(AudioSampleSnot);
      break;
    case 7:
      sample7.play(AudioSampleSwallow);
      break;
    default:
      break;
  }
  delay(2000);
}
