#include <driver/dac.h>
//#include <iostream>
//#include "pitches.h"
#include "notes.h"



// I2S VARIABLES
#include "driver/i2s.h"
#include <soc/rtc.h>
#include "freertos/queue.h"

static const int i2s_num = 0; // i2s port number

//static const i2s_port_t i2s_num = (i2s_port_t)I2S_NUM_0; // i2s port number

// DAC pins setup and config :
const int dacPin = 25;
const int channel = 0;
const int resolution = 8;

// SOFTWARE VARIABLES
// Generals variables :
int octave = 4;
int note = 0;
double frequency = 220;
//int waveIndex = 0;
enum Waveform {sine, triangle, square, saw, sawTouth, whiteNoise};
enum Envelope {shortPeak, shortPeakWithSustain, longPeak, longPeakWithSustain, sustain};
Waveform form = Waveform::sine;
// Waveform form;
Envelope pitchEnv;
Envelope ampEnv;

// HARDWARE VARIABLES
// Keyboard pins setup and config :
const int keyboardNotesPin = 14;
int keyboardNotesValue = 0;
bool keyboardNotesPressed = false;
int keyboardNotesStartedSince = 0;

const int keyboardMinusOctaveButtonPin = 12;
int keyboardMinusOctaveButton = 0;
bool keyboardMinusOctaveButtonPressed = false;
const int keyboardPlusOctaveButtonPin = 13;
int keyboardPlusOctaveButton = 0;
bool keyboardPlusOctaveButtonPressed = false;



// CODE
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

inline double produceWaveform() {
  // f(x) = e^(-x)
  
  double value = 0;
  if(form == Waveform::sine) {
//    double time = millis() / 1000.0;
//    return sin(time * (2.0 * PI) * frequency) * 127.5 + 127.5;
    return sin((millis() / 1000.0) * (2.0 * PI) * frequency) * 127.5 + 127.5;
        
  } else if(form == Waveform::triangle) {
    //dacWrite(dacPin, triangleValues[waveIndex]);
  } else if(form == Waveform::square) {
    ////dacWrite(dacPin, squareValues[waveIndex]);
    return sin((millis() / 1000.0) * (2.0 * PI) * frequency) > 0 ? 255 : 0;
  } else if(form == Waveform::saw) {
    //dacWrite(dacPin, sawValues[waveIndex]);
  } else if(form == Waveform::sawTouth) {
    //dacWrite(dacPin, sawTouthValues[waveIndex]);
  } else if(form == Waveform::whiteNoise) {
    //dacWrite(dacPin, whiteNoiseValues[waveIndex]);
  } else {
    Serial.println("No waveform selected");
  }
  return value;
  
}


void setup() {
  Serial.begin(115200);
  Serial.println("Device starting");
  
//  dac_output_enable(DAC_CHANNEL_1);

  pinMode(keyboardMinusOctaveButtonPin, INPUT);
  pinMode(keyboardPlusOctaveButtonPin, INPUT);

//    std::cout << "Hello, World!\n";

  Serial.println("Device started properly");
}

void loop() {

  serialListen();

  keyboardOctaveInputListen();
  keyboardNotesInputListen();

//  int waveformValue = produceWaveform();
//  dacWrite(dacPin, waveformValue);
  
  dacWrite(dacPin, produceWaveform());
//    dac_output_voltage(DAC_CHANNEL_1, waveformValue);


//  Serial.println("------------------------");
//  Serial.println(millis());
//  double blbl = millis() / 1000.0;
//  Serial.println(blbl, 3);
//  Serial.println("------------------------");
//  Serial.println(waveformValue, 3);
//  Serial.println(produceWaveform());
//  Serial.println(produceWaveform(), 3);
//  Serial.println("------------------------");
  
}

void keyboardNotesInputListen() {
  int steps = 256;
  keyboardNotesValue = analogRead(keyboardNotesPin);

  if (keyboardNotesValue > 234 && keyboardNotesPressed == false) {
    Serial.println("C");
    note = 1;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 214.5 && keyboardNotesPressed == false) {
    Serial.println("C#");
    note = 2;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 195 && keyboardNotesPressed == false) {
    Serial.println("D");
    note = 3;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 175.5 && keyboardNotesPressed == false) {
    Serial.println("D#");
    note = 4;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 156 && keyboardNotesPressed == false) {
    Serial.println("E");
    note = 5;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 136.5 && keyboardNotesPressed == false) {
    Serial.println("F");
    note = 6;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 117 && keyboardNotesPressed == false) {
    Serial.println("F#");
    note = 7;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 97.5 && keyboardNotesPressed == false) {
    Serial.println("G");
    note = 8;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 78 && keyboardNotesPressed == false) {
    Serial.println("G#");
    note = 9;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 58.5 && keyboardNotesPressed == false) {
    Serial.println("A");
    note = 10;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 39 && keyboardNotesPressed == false) {
    Serial.println("A#");
    note = 11;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue > 19.5 && keyboardNotesPressed == false) {
    Serial.println("B");
    note = 12;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  }
  else if (keyboardNotesValue < 19.5 && keyboardNotesPressed == true) {
    Serial.println("Release");
    note = 0;
    frequency = 0;
    keyboardNotesStartedSince = 0;
    keyboardNotesPressed = false;
  }
}

void keyboardOctaveInputListen() {
  
  keyboardMinusOctaveButton = digitalRead(keyboardMinusOctaveButtonPin);
  if(keyboardMinusOctaveButton == HIGH && keyboardMinusOctaveButtonPressed == false) {
    // turn LED on:
    keyboardMinusOctaveButtonPressed = true;
    octave += 1;
    Serial.println("Device octave changed to : ");
    Serial.println(octave);
  } else if(keyboardMinusOctaveButton == LOW && keyboardMinusOctaveButtonPressed == true) {
    // turn LED off:
    keyboardMinusOctaveButtonPressed = false;
  }
  
  keyboardPlusOctaveButton = digitalRead(keyboardPlusOctaveButtonPin);
  if(keyboardPlusOctaveButton == HIGH && keyboardPlusOctaveButtonPressed == false) {
    // turn LED on:
    keyboardPlusOctaveButtonPressed = true;
    octave -= 1;
    Serial.println("Device octave changed to : ");
    Serial.println(octave);
  } else if(keyboardPlusOctaveButton == LOW && keyboardPlusOctaveButtonPressed == true) {
    // turn LED off:
    keyboardPlusOctaveButtonPressed = false;
  }
  
}

void serialListen() {
  
  if (Serial.available()) {
    String inputString = Serial.readString();
    inputString.trim();
    
    if (inputString == "play") {
      
    } else if (inputString == "frequency") {
      Serial.println("Device frequency : ");
      Serial.println(frequency);
    } else if (inputString == "1") {
      frequency = 110;
      Serial.println("Device frequency changed to : ");
      Serial.println(frequency);
    } else if (inputString == "2") {
      frequency = 220;
      Serial.println("Device frequency changed to : ");
      Serial.println(frequency);
    } else if (inputString == "3") {
      frequency = 440;
      Serial.println("Device frequency changed to : ");
      Serial.println(frequency);
    } else if (inputString == "4") {
      frequency = 880;
      Serial.println("Device frequency changed to : ");
      Serial.println(frequency);
    } else if (inputString == "5") {
      frequency = 1760;
      Serial.println("Device frequency changed to : ");
      Serial.println(frequency);
    } else if (inputString == "6") {
      frequency = 3520;
      Serial.println("Device frequency changed to : ");
      Serial.println(frequency);
      
    } else if(inputString == "cwf:sine") {
      changeFormSettings(Waveform::sine);
    } else if(inputString == "cwf:triangle") {
      changeFormSettings(Waveform::triangle);
    } else if(inputString == "cwf:square") {
      changeFormSettings(Waveform::square);
    } else if(inputString == "cwf:saw") {
      changeFormSettings(Waveform::saw);
    } else if(inputString == "cwf:sawTouth") {
      changeFormSettings(Waveform::sawTouth);
    } else if(inputString == "cwf:whiteNoise") {
      changeFormSettings(Waveform::whiteNoise);
      //
    } else if(inputString == "cpe:shortPeak") {
      changePitchEnvSettings(Envelope::shortPeak);
    } else if(inputString == "cpe:shortPeakWithSustain") {
      changePitchEnvSettings(Envelope::shortPeakWithSustain);
    } else if(inputString == "cpe:longPeak") {
      changePitchEnvSettings(Envelope::longPeak);
    } else if(inputString == "cpe:longPeakWithSustain") {
      changePitchEnvSettings(Envelope::longPeakWithSustain);
    } else if(inputString == "cpe:sustain") {
      changePitchEnvSettings(Envelope::sustain);
      //
    } else if(inputString == "cae:shortPeak") {
      changeAmpEnvSettings(Envelope::shortPeak);
    } else if(inputString == "cae:shortPeakWithSustain") {
      changeAmpEnvSettings(Envelope::shortPeakWithSustain);
    } else if(inputString == "cae:longPeak") {
      changeAmpEnvSettings(Envelope::longPeak);
    } else if(inputString == "cae:longPeakWithSustain") {
      changeAmpEnvSettings(Envelope::longPeakWithSustain);
    } else if(inputString == "cae:sustain") {
      changeAmpEnvSettings(Envelope::sustain);
      //
    } else {
      Serial.println("!!! ELSE INPUT !!!");
      Serial.println(inputString);
    }

  }
  
}
