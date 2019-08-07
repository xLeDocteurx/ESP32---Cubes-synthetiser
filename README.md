# ESP32---Cubes-synthetiser
Arduino project. The goal is to create a small audio workstation for initiation to audio synthésis where you program the synthetiser by placing cubes on a board. Powered by Weemos Lolin32.

( having not recieved part of my electronics parts, synth programming function are developped to be used via serial monitor. Cube identification and behavior will come last. )

![board view](https://github.com/xLeDocteurx/ESP32---Cubes-synthetiser/blob/master/git/Screenshot%20from%202019-07-28%2020-21-12.png)

![cubes view](https://github.com/xLeDocteurx/ESP32---Cubes-synthetiser/blob/master/git/Screenshot%20from%202019-07-31%2021-26-38.png)



## Minimum viable product :
- Programing the synthetiser is achieved only via cubes !
- Controlling the synthetiseur via a 12key (1octave) integrated keyboard.
  + Changing the keyboard octave via two buttons
- Volume control on a 3.3mm mono output
- Simple access to hardware oriented variables for open purposes ( To easily change sample rate resolution based on hardware, for exemple )

## Stretched goals :
- Jack 6.35mm input to control the synth frequency with an high impedance instrument. (Guitar, bass, condenser mic)
- Midi input ( don't know yet if it will be by standard midi input or CV )
- A filter section with envelope control.
- 12 bits output resolution.

## Monophonic Keyboard using one analoginput :
( Instead of using 12 digital inputs. Since, anyway, the wave algorythm will be very simple and wont permit polyphonic restitution. )

![keyboard schematics](https://github.com/xLeDocteurx/ESP32---Cubes-synthetiser/blob/master/git/analog%20button%20input.png)

## Serial protocol :
- "cwf:sine"
- "cwf:triangle"
- "cwf:square"
- "cwf:saw"
- "cwf:whiteNoise"

- "cpe:shortPeak"
- "cpe:shortPeakWithSustain"
- "cpe:longPeak"
- "cpe:longPeakWithSustain"
- "cpe:sustain"

- "cae:shortPeak"
- "cae:shortPeakWithSustain"
- "cae:longPeak"
- "cae:longPeakWithSustain"
- "cae:sustain"



## key electronical components :
- Weemos lolin32. ( any 32bit microcontroler with at least one DAC will do the job )
- TL082CN chip as input amplifier and signal offset. (high impedance instruments are delivering a really low voltage AC signal, ADC also needs DC)
- PAM8403 chip as output amplifier and signal offset.

- Maybe a MCP4725 to upgrade the DAC output from 8 to 12 bits.
