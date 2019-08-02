#include <XT_DAC_Audio.h>
#include <MusicDefinitions.h>

// #include <iostream>
// using namespace std;

using namespace std;

#include "Tone32.h"

namespace Synth {

  const int resolution = 8;
  const int pin = 25;
  const int channel = 0;

  enum Waveform {sine, triangle, square, saw, sawTouth, whiteNoise};
  enum Envelope {shortPeak, shortPeakWithSustain, longPeak, longPeakWithSustain, sustain};

  Waveform form = Waveform::square;
  int sineValues[32 * resolution];
  int triangleValues[32 * resolution];
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
    for(int myAngle=0;myAngle<32 * resolution;myAngle++) {
      radAngle = myAngle*conversionFactor;
      sineValues[myAngle] = (sin(radAngle)*127)+128;
    }
    // for tringleValues
    // for squareValues
    for(int myAngle=0;myAngle<32 * resolution;myAngle++){
      squareValues[myAngle] = myAngle < 128 ? 0 : 255;
    }
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
  
   // for(int i = 0; i < sizeof(Synth::squareValues) / sizeof(Synth::squareValues[0]); i++) {
   //   Serial.println(Synth::squareValues[i]);
   // }
  
  Serial.println("Device started properly");
}

int waveIndex = 0;

void loop() {

  serialListen();
  
  if(Synth::form == Synth::Waveform::sine) {
    dacWrite(Synth::pin, Synth::sineValues[waveIndex]);
  } else if(Synth::form == Synth::Waveform::triangle) {
    dacWrite(Synth::pin, Synth::triangleValues[waveIndex]);
  } else if(Synth::form == Synth::Waveform::square) {
    dacWrite(Synth::pin, Synth::squareValues[waveIndex]);
  } else if(Synth::form == Synth::Waveform::saw) {
    dacWrite(Synth::pin, Synth::sawValues[waveIndex]);
  } else if(Synth::form == Synth::Waveform::whiteNoise) {
    dacWrite(Synth::pin, Synth::whiteNoiseValues[waveIndex]);
  }

  waveIndex = waveIndex >= 255 ? 0 : waveIndex + 1;
  
//  if (analogRead(Synth::pin)) {
//      Serial.println("Output is in use");
//      int sensorVal = analogRead(Synth::pin);
//      Serial.println(sensorVal);
//  }
//  Serial.println("Analog Read : ");
//  Serial.println(Synth::squareValues[waveIndex]);
  
}

String inputString = "";
bool stringComplete = false;

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void serialListen() {
  
  if (stringComplete) {
    Serial.println(inputString);

    if(inputString == "cwf:sine") {
      Synth::changeFormSettings(Synth::Waveform::sine);
    } else if(inputString == "cwf:triangle") {
      Synth::changeFormSettings(Synth::Waveform::triangle);
    } else if(inputString == "cwf:square") {
      Synth::changeFormSettings(Synth::Waveform::square);
    } else if(inputString == "cwf:saw") {
      Synth::changeFormSettings(Synth::Waveform::saw);
    } else if(inputString == "cwf:whiteNoise") {
      Synth::changeFormSettings(Synth::Waveform::whiteNoise);
    }

    inputString = "";
    stringComplete = false;
  }
  
}
