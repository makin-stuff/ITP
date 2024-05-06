#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

// Include all of our audio files
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

int noteCount = 0;

// LEDs
int step0 = 33;
int step1 = 34;
int step2 = 35;
int step3 = 36;
int step4 = 37;
int step5 = 32;
int step6 = 31;
int step7 = 30;

int brainTrig = 26;
int brainState = 0;
int lastBrainState = 0;

void setup() {
  // Step indicator LEDs
  pinMode(step0, OUTPUT);
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(step4, OUTPUT);
  pinMode(step5, OUTPUT);
  pinMode(step6, OUTPUT);
  pinMode(step7, OUTPUT);

  // Brain trigger
  pinMode(brainTrig, INPUT);

  Serial.begin(115200);
  AudioMemory(200);
  sgtl5000_1.enable();
  sgtl5000_1.volume(.8);

  mixer.gain(1, 1);
  mixer.gain(2, 1);
  mixer.gain(3, 1);
  mixer.gain(4, 1);

  mixer1.gain(1, 1);
  mixer1.gain(2, 1);
  mixer1.gain(3, 1);
  mixer1.gain(4, 1);
}

void loop() {

  // check if the tempo has changed
//  tempo = map(analogRead(A13), 0, 1023, 200, 1500);

  brainState = digitalRead(brainTrig);

  if (brainState != lastBrainState) {
    if (brainState == HIGH) {
//      Serial.println("step");
      playSequence(noteCount);

      if (noteCount < 7) {
        noteCount++;
      } else {
        noteCount = 0;  // Reset count
      }

//      Serial.println(noteCount);
    }
  }

  // Update last brain state
  lastBrainState = brainState;

//  Serial.println(noteCount);
}


//This function plays a random sequence as set by a hardcoded scale
//and calculation by a random value. The knobs determine note length,
//filter attack time, envelope release time, and tempo via a simple delay.
void playSequence(int noteCount) {
//  Serial.println("step");
  Serial.print("noteCount: ");
  Serial.println(noteCount);
  
  int sampleNum;

  // For every note (step), read a different potentiometer
  switch (noteCount) {
    case 0:
      sampleNum = map(analogRead(A14), 0, 1023, 0, 7);
      digitalWrite(step0, HIGH);
      break;
    case 1:
      sampleNum = map(analogRead(A15), 0, 1023, 0, 7);
      digitalWrite(step1, HIGH);
      break;
    case 2:
      sampleNum = map(analogRead(A16), 0, 1023, 0, 7);
      digitalWrite(step2, HIGH);
      break;
    case 3:
      sampleNum = map(analogRead(A17), 0, 1023, 0, 7);
      digitalWrite(step3, HIGH);
      break;
    case 4:
      sampleNum = map(analogRead(A0), 0, 1023, 0, 7);
      digitalWrite(step4, HIGH);
      break;
    case 5:
      sampleNum = map(analogRead(A1), 0, 1023, 0, 7);
      digitalWrite(step5, HIGH);
      break;
    case 6:
      sampleNum = map(analogRead(A2), 0, 1023, 0, 7);
      digitalWrite(step6, HIGH);
      break;
    case 7:
      sampleNum = map(analogRead(A3), 0, 1023, 0, 7);
      digitalWrite(step7, HIGH);
      break;
    default:
      sampleNum = 7;
      break;
  }

  Serial.print("Sample number: ");
  Serial.println(sampleNum);
  Serial.print("Audio usage: ");
  Serial.println(AudioMemoryUsage());

  // Now, pick the correct sample to play
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

  // Turn off indicator LEDs
  delay(10);
  turnOffLEDs();
}

void turnOffLEDs() {
  digitalWrite(step0, LOW);
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4, LOW);
  digitalWrite(step5, LOW);
  digitalWrite(step6, LOW);
  digitalWrite(step7, LOW);
}
