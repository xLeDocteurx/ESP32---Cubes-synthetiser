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
  }

  void changePitchEnvSettings(Envelope env) {
    
  }

  void changeAmpEnvSettings(Envelope env) {
    
  }

  inline int processSignal() {
    
  }
  
}

void setup() {
  // Serial.begin(115200);
  
  // ledcSetup(channel, freq, resolution);
  // ledcAttachPin(pin, channel);
  
}

void loop() {
//  tone(BUZZER_PIN, NOTE_C4, 500, BUZZER_CHANNEL);
//  noTone(BUZZER_PIN, BUZZER_CHANNEL);
//  delay(2000);
  dacWrite(Synth::pin, Synth::value);
}
