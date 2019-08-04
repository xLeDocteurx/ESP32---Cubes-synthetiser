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
  int sineValues[32 * resolution * 2];
  int triangleValues[32 * resolution * 2];
  int squareValues[32 * resolution * 2];
  int sawValues[32 * resolution * 2];
  int sawTouthValues[32 * resolution * 2];
  int whiteNoiseValues[32 * resolution * 2];
  
  Envelope pitchEnv;
  Envelope ampEnv;
  int shortPeakValues[32 * resolution * 2];
  int shortPeakWithSustainValues[32 * resolution * 2];
  int longPeakValues[32 * resolution * 2];
  int longPeakWithSustainValues[32 * resolution * 2];
  int sustainValues[32 * resolution * 2];
  
  void initiateWaveforms() {
    // for sineValues
    float conversionFactor=(2*PI)/(32 * resolution * 2);
    float radAngle;
    for(int myAngle=0;myAngle<32 * resolution * 2;myAngle++) {
      radAngle = myAngle*conversionFactor;
      sineValues[myAngle] = (sin(radAngle)*((32 * resolution / 2) - 1))+(32 * resolution / 2);
    }
    // for triangleValues
    for(int myAngle=0;myAngle<32 * resolution;myAngle++){
      triangleValues[myAngle] = myAngle;
    }
    for(int myAngle=32 * resolution;myAngle<32 * resolution * 2;myAngle++){
      triangleValues[myAngle] = (32 * resolution * 2) - myAngle;
    }
    // for squareValues
    for(int myAngle=0;myAngle<32 * resolution * 2;myAngle++){
      squareValues[myAngle] = myAngle < 32 * resolution ? 0 : 32 * resolution - 1;
    }
    // for sawValues
    int index = 0;
    for(int myAngle=(32 * resolution) - 1;myAngle>=0;myAngle--){
      sawValues[index] = myAngle;
      index += 1;
    }
    for(int myAngle=(32 * resolution) - 1;myAngle>=0;myAngle--){
      sawValues[index] = myAngle;
      index += 1;
    }
    // for sawTouth
    for(int myAngle=0;myAngle<32 * resolution;myAngle++){
      sawTouthValues[myAngle] = myAngle;
    }
    for(int myAngle=32 * resolution;myAngle<32 * resolution * 2;myAngle++){
      sawTouthValues[myAngle] = myAngle - (32 * resolution);
    }
    // for whiteNoiseValues
    for(int myAngle=0;myAngle<32 * resolution * 2;myAngle++) {
      whiteNoiseValues[myAngle] = random(0, 32 * resolution);
    }
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
    } else if(form == Waveform::sawTouth) {
      dacWrite(pin, sawTouthValues[waveIndex]);
    } else if(form == Waveform::whiteNoise) {
      dacWrite(pin, whiteNoiseValues[waveIndex]);
    } else {
      Serial.println("No waveform selected");
    }
  
    waveIndex = waveIndex >= (32 * resolution * 2) - 1 ? 0 : waveIndex + 1;
    
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
    } else if(inputString == "cwf:sawTouth") {
      Synth::changeFormSettings(Synth::Waveform::sawTouth);
    } else if(inputString == "cwf:whiteNoise") {
      Synth::changeFormSettings(Synth::Waveform::whiteNoise);
      //
    } else if(inputString == "cpe:shortPeak") {
      Synth::changePitchEnvSettings(Synth::Envelope::shortPeak);
    } else if(inputString == "cpe:shortPeakWithSustain") {
      Synth::changePitchEnvSettings(Synth::Envelope::shortPeakWithSustain);
    } else if(inputString == "cpe:longPeak") {
      Synth::changePitchEnvSettings(Synth::Envelope::longPeak);
    } else if(inputString == "cpe:longPeakWithSustain") {
      Synth::changePitchEnvSettings(Synth::Envelope::longPeakWithSustain);
    } else if(inputString == "cpe:sustain") {
      Synth::changePitchEnvSettings(Synth::Envelope::sustain);
      //
    } else if(inputString == "cae:shortPeak") {
      Synth::changeAmpEnvSettings(Synth::Envelope::shortPeak);
    } else if(inputString == "cae:shortPeakWithSustain") {
      Synth::changeAmpEnvSettings(Synth::Envelope::shortPeakWithSustain);
    } else if(inputString == "cae:longPeak") {
      Synth::changeAmpEnvSettings(Synth::Envelope::longPeak);
    } else if(inputString == "cae:longPeakWithSustain") {
      Synth::changeAmpEnvSettings(Synth::Envelope::longPeakWithSustain);
    } else if(inputString == "cae:sustain") {
      Synth::changeAmpEnvSettings(Synth::Envelope::sustain);
      //
    } else {
      Serial.println("ELSE :");
      Serial.println(inputString);
    }

  }
  
}
