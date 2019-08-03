#include "pitches.h"

namespace Synth {

  const int resolution = 8;
  const int pin = 25;
  const int channel = 0;

  int waveIndex = 0;

  enum Waveform {sine, triangle, square, saw, sawTouth, whiteNoise};
  enum Envelope {shortPeak, shortPeakWithSustain, longPeak, longPeakWithSustain, sustain};

  // Waveform form = Waveform::square;
  Waveform form;
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

  void initiateEnvelopes(){
    // for shortPeakValues
    // for shortPeakWithSustainValues
    // for longPeakValues
    // for longPeakWithSustainValues
    // for sustainValues
  }
  
  void changeFormSettings(Waveform wf) {
    form = wf;
    Serial.println("Changed waveform settings : " + wf);
  }
  
  void changePitchEnvSettings(Envelope env) {
    pitchEnv = env;
    Serial.println("Changed pitch envelope settings : " + env);
  }
  
  void changeAmpEnvSettings(Envelope env) {
    ampEnv = env;
    Serial.println("Changed amplitude envelope settings : " + env);
  }

  inline int processSignalAndWriteToPin() {
    
    if(form == Waveform::sine) {
      dacWrite(pin, sineValues[waveIndex]);
    } else if(form == Waveform::triangle) {
      dacWrite(pin, triangleValues[waveIndex]);
    } else if(form == Waveform::square) {
      dacWrite(pin, squareValues[waveIndex]);
    } else if(form == Waveform::saw) {
      dacWrite(pin, sawValues[waveIndex]);
    } else if(form == Waveform::whiteNoise) {
      dacWrite(pin, whiteNoiseValues[waveIndex]);
    } else {
      // Serial.println("No waveform selected");
    }
  
    waveIndex = waveIndex >= 255 ? 0 : waveIndex + 1;
    
  }
  
}

void setup() {
  Serial.begin(115200);
  Serial.println("Device starting");

  Synth::initiateWaveforms();
  
  Serial.println("Device started properly");
}

void loop() {

  serialListen();

  Synth::processSignalAndWriteToPin();
  
}

//String inputString = "";
//bool stringComplete = false;

//void serialEvent() {
//  while (Serial.available()) {
//    char inChar = (char)Serial.read();
//    inputString += inChar;
//    if (inChar == '\n') {
//      stringComplete = true;
//    }
//  }
//}

void serialListen() {
  
  if (Serial.available()) {
    String inputString = Serial.readString();
    inputString.trim();
    
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
    } else {
      Serial.println("ELSE :");
      Serial.println(inputString);
    }

  }
  
}
