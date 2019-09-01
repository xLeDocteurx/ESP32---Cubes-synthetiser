#include <soc/rtc.h>
#include <driver/dac.h>
//#include <iostream>
//#include "pitches.h"
#include "notes.h"



//// I2S VARIABLES
////#include "driver/i2s.h"
////#include "esp_system.h"
//#include <driver/i2s.h>
////#include "freertos/queue.h"


////const int dma_buf_len = 16000;
//const int dma_buf_len = 1024;
////static const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number
//static const i2s_port_t i2s_num = (i2s_port_t)I2S_NUM_0; // i2s port number
//
//static const i2s_config_t i2s_config = {
////     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
//     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
////     .sample_rate = 44100,
//     .sample_rate = 1000000,  //not really used
//     .bits_per_sample = (i2s_bits_per_sample_t)I2S_BITS_PER_SAMPLE_16BIT,
////     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
//     .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
//     .communication_format = I2S_COMM_FORMAT_I2S_MSB,
//     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // default interrupt priority
////     .dma_buf_count = 8,
//     .dma_buf_count = 2,
////     .dma_buf_len = 64,
//     .dma_buf_len = dma_buf_len,  //big buffers to avoid noises
//     .use_apll = false
////     ,
////     .tx_desc_auto_clear = false,
////     .fixed_mclk = 0
//};
//
////static const i2s_pin_config_t pin_config = {
////    .bck_io_num = 26,
////    .ws_io_num = 25,
////    .data_out_num = 22,
////    .data_in_num = I2S_PIN_NO_CHANGE
////};

// DAC pins setup and config :
const int dacPin = 25;
const int channel = 0;
const int resolution = 8;

// SOFTWARE VARIABLES
// Generals variables :
int octave = 4;
int note = 0;
double frequency = 440;
const int bufferSize = 512;
float eachWaveformValueRepeats = 180736 / bufferSize;
//const int bufferSize = 512;
//int waveIndex = 0;
int blankArray[bufferSize];
enum Waveform {sine, triangle, square, saw, sawTouth, whiteNoise};
Waveform form;
int selectedWaveform[bufferSize];
int sineValues[bufferSize];
int triangleValues[bufferSize];
int squareValues[bufferSize];
int sawValues[bufferSize];
int sawTouthValues[bufferSize];
int whiteNoiseValues[bufferSize];
enum Envelope {shortPeak, shortPeakWithSustain, longPeak, longPeakWithSustain, sustain};
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
  
  if(wf == Waveform::sine) {
    memcpy(selectedWaveform, sineValues, sizeof(selectedWaveform));
  } else if(wf == Waveform::triangle) {
    memcpy(selectedWaveform, triangleValues, sizeof(selectedWaveform));
  } else if(wf == Waveform::square) {
    memcpy(selectedWaveform, squareValues, sizeof(selectedWaveform));
  } else if(wf == Waveform::saw) {
    memcpy(selectedWaveform, sawValues, sizeof(selectedWaveform));
  } else if(wf == Waveform::sawTouth) {
    memcpy(selectedWaveform, sawTouthValues, sizeof(selectedWaveform));
  } else if(wf == Waveform::whiteNoise) {
    memcpy(selectedWaveform, whiteNoiseValues, sizeof(selectedWaveform));
  } else {
    memcpy(selectedWaveform, blankArray, sizeof(selectedWaveform));
    Serial.println("No waveform selected");
  }
  
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

void initiateWaveforms() {
  // for sineValues
  float conversionFactor=(2*PI)/bufferSize;
  for(int myAngle=0;myAngle < bufferSize;myAngle++) {
    sineValues[myAngle] = sin(myAngle*conversionFactor) * 127.5 + 127.5;
  }
  // for triangleValues
  for(int myAngle=0;myAngle <  bufferSize / 2;myAngle++){
    triangleValues[myAngle] = myAngle * 255 / (bufferSize / 2);
  }
  for(int myAngle=bufferSize / 2;myAngle > 0;myAngle--){
    triangleValues[myAngle] = myAngle * 255 / (bufferSize / 2);
  }
  // for squareValues
  for(int myAngle=0;myAngle < bufferSize;myAngle++){
    squareValues[myAngle] = myAngle < bufferSize / 2 ? 0 : 255;
  }
  // for sawValues
  for(int myAngle=bufferSize - 1;myAngle >= 0;myAngle--){
    sawValues[myAngle] = myAngle * 255 / bufferSize;
  }
  // for sawTouth
  for(int myAngle=bufferSize;myAngle < bufferSize;myAngle++){
    sawTouthValues[myAngle] = myAngle * 255 / bufferSize;
  }
  // for whiteNoiseValues
  for(int myAngle=0;myAngle < bufferSize;myAngle++) {
    whiteNoiseValues[myAngle] = random(0, 255);
  }
  // f(x) = e^(-x)
}

float envelopeValue() {
  // for shortPeak
  //  shortPeakValues[];
  // for shortPeakWithSustain
  //  shortPeakWithSustainValues[];
  // for longPeak
  //  longPeakValues[];
  // for longPeakWithSustain
  //  longPeakWithSustainValues[];
  // for sustain
  //  sustainValues[];
}



void setup() {
  Serial.begin(115200);
  Serial.println("Device starting");

  initiateWaveforms();
  changeFormSettings(Waveform::sine);

  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);              //highest cpu frequency

//  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);   //install and start i2s driver
////  i2s_set_pin(i2s_num, &pin_config);
//  i2s_set_pin(i2s_num, NULL);                           //use internal DAC
////  i2s_set_sample_rates(i2s_num, 22050); //set sample rates
//  i2s_set_sample_rates(i2s_num, 1000000);               //dummy sample rate, since the function fails at high values
//
////  i2s_driver_uninstall(i2s_num); //stop & destroy i2s driver
//
//  //this is the hack that enables the highest sampling rate possible ~13MHz, have fun
//  SET_PERI_REG_BITS(I2S_CLKM_CONF_REG(0), I2S_CLKM_DIV_A_V, 1, I2S_CLKM_DIV_A_S);
//  SET_PERI_REG_BITS(I2S_CLKM_CONF_REG(0), I2S_CLKM_DIV_B_V, 1, I2S_CLKM_DIV_B_S);
//  SET_PERI_REG_BITS(I2S_CLKM_CONF_REG(0), I2S_CLKM_DIV_NUM_V, 2, I2S_CLKM_DIV_NUM_S); 
//  SET_PERI_REG_BITS(I2S_SAMPLE_RATE_CONF_REG(0), I2S_TX_BCK_DIV_NUM_V, 2, I2S_TX_BCK_DIV_NUM_S); 

  
  
//  dac_output_enable(DAC_CHANNEL_1);

  pinMode(keyboardMinusOctaveButtonPin, INPUT);
  pinMode(keyboardPlusOctaveButtonPin, INPUT);

//    std::cout << "Hello, World!\n";

  Serial.println("Device started properly");
}


////buffer to store modulated samples, I2S samples of the esp32 are always 16Bit
//short buff[dma_buf_len];
//long int bufferSamplesPerCycle = 13328000;
//int eachWaveValueRepeats;
//int bufferPartsCounter = 0;
//int waveIndexCounter = 0;
////sine represented in 16 values. at 13MHz sampling rate the resulting carrier is at around 835KHz
//int sintab[] = {0, 48, 90, 117, 127, 117, 90, 48, 0, -48, -90, -117, -127, -117, -90, -48};



void loop() {

  serialListen();

//  keyboardOctaveInputListen();
//  keyboardNotesInputListen();
//  int h = 0;
//  while(keyboardNotesPressed == true && ){

//float eachWaveformValueRepeats = 180736 / bufferSize;
  for(float i = 0; i < bufferSize; i += 1 / (eachWaveformValueRepeats / frequency)){
    dacWrite(dacPin, selectedWaveform[static_cast<int>(i+0.5f)]);
//    dac_output_voltage(DAC_CHANNEL_1, selectedWaveform[static_cast<int>(i+0.5f)]);
  }
  



//  
//  //fill the sound buffer
//  for(int i = 0; i < 1024; i+=16){
//    
//    //modulating that sample on the 16 values of the carrier wave (respect to I2S byte order, 16Bit/Sample)
//    for(int j = 0; j < 16; j += 4){          
//      buff[i + j + 1] = (sintab[j + 0] + 0x8000);
//      buff[i + j + 0] = (sintab[j + 1] + 0x8000);
//      buff[i + j + 3] = (sintab[j + 2] + 0x8000);
//      buff[i + j + 2] = (sintab[j + 3] + 0x8000);
//    }
//  }




//  eachWaveValueRepeats = (13328000 / frequency) / bufferSize;
//  //fill the sound buffer
////  for(int i = 0; i < dma_buf_len; i += 4) {
//  for(int i = 0; i < dma_buf_len; i++) {
//    
//    buff[i] = (sineValues[waveIndexCounter] + 0x8000);
//           
////    buff[i + 1] = (selectedWaveform[waveIndexCounter + 0] + 0x8000);
////    buff[i + 0] = (selectedWaveform[waveIndexCounter + 1] + 0x8000);
////    buff[i + 3] = (selectedWaveform[waveIndexCounter + 2] + 0x8000);
////    buff[i + 2] = (selectedWaveform[waveIndexCounter + 3] + 0x8000);
//  
//  }

  
  
//  //write the buffer (waits until a buffer is ready to be filled, that's timing for free)
//  i2s_write_bytes(i2s_num, (char*)buff, sizeof(buff), portMAX_DELAY);
//  
//  if(bufferPartsCounter < eachWaveValueRepeats) {
//    bufferPartsCounter += 1;
////  } else if(bufferPartsCounter >= eachWaveValueRepeats) {
//  } else {
//    bufferPartsCounter = 0;
//    waveIndexCounter = waveIndexCounter < bufferSize ? waveIndexCounter + 1 : 0;
//  }
  
}

void keyboardNotesInputListen() {
  int steps = 256;
  keyboardNotesValue = analogRead(keyboardNotesPin);

  if(keyboardNotesValue > 234 && keyboardNotesPressed == false) {
    Serial.println("C" + octave);
    note = 1;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 214.5 && keyboardNotesPressed == false) {
    Serial.println("C#" + octave);
    note = 2;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 195 && keyboardNotesPressed == false) {
    Serial.println("D" + octave);
    note = 3;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 175.5 && keyboardNotesPressed == false) {
    Serial.println("D#" + octave);
    note = 4;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 156 && keyboardNotesPressed == false) {
    Serial.println("E" + octave);
    note = 5;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 136.5 && keyboardNotesPressed == false) {
    Serial.println("F" + octave);
    note = 6;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 117 && keyboardNotesPressed == false) {
    Serial.println("F#" + octave);
    note = 7;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 97.5 && keyboardNotesPressed == false) {
    Serial.println("G" + octave);
    note = 8;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 78 && keyboardNotesPressed == false) {
    Serial.println("G#" + octave);
    note = 9;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 58.5 && keyboardNotesPressed == false) {
    Serial.println("A" + octave);
    note = 10;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 39 && keyboardNotesPressed == false) {
    Serial.println("A#" + octave);
    note = 11;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue > 19.5 && keyboardNotesPressed == false) {
    Serial.println("B" + octave);
    note = 12;
    frequency = notes[octave - 1][note - 1];
    keyboardNotesStartedSince = millis();
    keyboardNotesPressed = true;
  } else if (keyboardNotesValue < 19.5 && keyboardNotesPressed == true) {
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
