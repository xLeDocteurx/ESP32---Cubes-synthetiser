#include <soc/rtc.h>
//#include <driver/dac.h>
//#include <iostream>
//#include "pitches.h"
#include "notes.h"



// I2S VARIABLES
//#include "driver/i2s.h"
//#include "esp_system.h"
#include <driver/i2s.h>
//#include "freertos/queue.h"

static const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number
//static const i2s_port_t i2s_num = (i2s_port_t)I2S_NUM_0; // i2s port number

static const i2s_config_t i2s_config = {
     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
//     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
     .sample_rate = 44100,
//     .sample_rate = 1000000,  //not really used
     .bits_per_sample = (i2s_bits_per_sample_t)I2S_BITS_PER_SAMPLE_16BIT,
//     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
     .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
     .communication_format = I2S_COMM_FORMAT_I2S_MSB,
     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // default interrupt priority
     .dma_buf_count = 8,
//     .dma_buf_count = 2,
     .dma_buf_len = 64,
//     .dma_buf_len = 1024  //big buffers to avoid noises
     .use_apll = false
//     ,
//     .tx_desc_auto_clear = false,
//     .fixed_mclk = 0
};

//static const i2s_pin_config_t pin_config = {
//    .bck_io_num = 26,
//    .ws_io_num = 25,
//    .data_out_num = 22,
//    .data_in_num = I2S_PIN_NO_CHANGE
//};

// DAC pins setup and config :
const int dacPin = 25;
const int channel = 0;
const int resolution = 8;

// SOFTWARE VARIABLES
// Generals variables :
int octave = 4;
int note = 0;
double frequency = 220;
const int bufferSize = 512;
//int waveIndex = 0;
enum Waveform {sine, triangle, square, saw, sawTouth, whiteNoise};
int sineValues[bufferSize];
int triangleValues[bufferSize];
int squareValues[bufferSize];
int sawValues[bufferSize];
int sawTouthValues[bufferSize];
int whiteNoiseValues[bufferSize];
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
}

//inline double produceWaveform() {
//  // f(x) = e^(-x)
//  
//  double value = 0;
//  if(form == Waveform::sine) {
////    double time = millis() / 1000.0;
////    return sin(time * (2.0 * PI) * frequency) * 127.5 + 127.5;
//    return sin((millis() / 1000.0) * (2.0 * PI) * frequency) * 127.5 + 127.5;
//  } else if(form == Waveform::triangle) {
//    //dacWrite(dacPin, triangleValues[waveIndex]);
//  } else if(form == Waveform::square) {
//    ////dacWrite(dacPin, squareValues[waveIndex]);
//    return sin((millis() / 1000.0) * (2.0 * PI) * frequency) > 0 ? 255 : 0;
//  } else if(form == Waveform::saw) {
//    //dacWrite(dacPin, sawValues[waveIndex]);
//  } else if(form == Waveform::sawTouth) {
//    //dacWrite(dacPin, sawTouthValues[waveIndex]);
//  } else if(form == Waveform::whiteNoise) {
//    //dacWrite(dacPin, whiteNoiseValues[waveIndex]);
//  } else {
//    Serial.println("No waveform selected");
//  }
//  return value;
//  
//}


void setup() {
  Serial.begin(115200);
  Serial.println("Device starting");

  initiateWaveforms();

  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);              //highest cpu frequency

  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);   //install and start i2s driver
//  i2s_set_pin(i2s_num, &pin_config);
  i2s_set_pin(i2s_num, NULL);                           //use internal DAC
  i2s_set_sample_rates(i2s_num, 22050); //set sample rates
//  i2s_set_sample_rates(i2s_num, 1000000);               //dummy sample rate, since the function fails at high values

//  i2s_driver_uninstall(i2s_num); //stop & destroy i2s driver

////this is the hack that enables the highest sampling rate possible ~13MHz, have fun
//SET_PERI_REG_BITS(I2S_CLKM_CONF_REG(0), I2S_CLKM_DIV_A_V, 1, I2S_CLKM_DIV_A_S);
//SET_PERI_REG_BITS(I2S_CLKM_CONF_REG(0), I2S_CLKM_DIV_B_V, 1, I2S_CLKM_DIV_B_S);
//SET_PERI_REG_BITS(I2S_CLKM_CONF_REG(0), I2S_CLKM_DIV_NUM_V, 2, I2S_CLKM_DIV_NUM_S); 
//SET_PERI_REG_BITS(I2S_SAMPLE_RATE_CONF_REG(0), I2S_TX_BCK_DIV_NUM_V, 2, I2S_TX_BCK_DIV_NUM_S); 

  
  
//  dac_output_enable(DAC_CHANNEL_1);

  pinMode(keyboardMinusOctaveButtonPin, INPUT);
  pinMode(keyboardPlusOctaveButtonPin, INPUT);

//    std::cout << "Hello, World!\n";

  Serial.println("Device started properly");
}

void loop() {

  serialListen();

//  keyboardOctaveInputListen();
//  keyboardNotesInputListen();
  
//  dacWrite(dacPin, produceWaveform());
//    dac_output_voltage(DAC_CHANNEL_1, produceWaveform());
  
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
