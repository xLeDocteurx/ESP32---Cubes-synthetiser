#include <XT_DAC_Audio.h>
#include <MusicDefinitions.h>

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
  int sineValues[32 * resolution];
  int tringleValues[32 * resolution];
  int squareValues[32 * resolution];
  int sawValues[32 * resolution];
  int whiteNoiseValues[32 * resolution];
  
  Envelope pitchEnv;
  Envelope ampEnv;
  int shortPeakValues[32 * resolution];
  int shortPeakWithSustainValues[32 * resolution];
  int longPeakValues[32 * resolution];
  int longPeakWithSustainValues[32 * resolution];
  int sustainValues[32 * resolution];
  
  void initiateWaveforms() {
    // for sineValues
    float conversionFactor=(2*PI)/(32 * resolution);
    float radAngle;

    for(int myAngle=0;myAngle<256;myAngle++) {
      radAngle=myAngle*conversionFactor;
      sineValues[myAngle]=(sin(radAngle)*127)+128;
    }
    // for tringleValues
    // for squareValues
    // for sawValues
    // for sawTouth
    // for whiteNoiseValues
  }
  void changeFormSettings(Waveform wf) {
    form = wf;
    Serial.println("Changed waveform settings : " + wf);
  }

  void initiateEnvelopes(){
    // for shortPeakValues
    // for shortPeakWithSustainValues
    // for longPeakValues
    // for longPeakWithSustainValues
    // for sustainValues
  }
  void changePitchEnvSettings(Envelope env) {
    pitchEnv = env;
    Serial.println("Changed pitch envelope settings : " + env);
  }
  void changeAmpEnvSettings(Envelope env) {
    ampEnv = env;
    Serial.println("Changed amplitude envelope settings : " + env);
  }

  inline int processSignal() {
    
  }
  
}

void setup() {
  Serial.begin(115200);
  Serial.println("Device starting");

  Synth::initiateWaveforms();

  Serial.println("Device started properly");
}

void loop() {
  
  // dacWrite(Synth::pin, Synth::value);
}
