// #include <iostream>
// using namespace std;

using namespace std;

#include "Tone32.h"

namespace Synth {

  const int resolution = 8;
  const int pin = 5;
  const int channel = 0;
  
  int freq = 0;
  int value = 0;

  enum Waveform {sine, triangle, square, saw, sawTouth, whiteNoise};
  enum Envelope {shortPeak, shortPeakWithSustain, longPeak, longPeakWithSustain, sustain};

  Waveform form;
  Envelope pitchEnv;
  Envelope ampEnv;

  void changeFormSettings(Waveform wf) {
    form = wf;
    Serial.println("changed waveform settings : " + wf);
  }
  void changePitchEnvSettings(Envelope env) {
    pitchEnv = env;
    Serial.println("changed pitch envelope settings : " + env);
  }
  void changeAmpEnvSettings(Envelope env) {
    ampEnv = env;
    Serial.println("changed amplitude envelope settings : " + env);
  }

  inline int processSignal() {
    
  }
  
}

void setup() {
  Serial.begin(115200);
  
}

void loop() {
  
  // dacWrite(Synth::pin, Synth::value);
}
