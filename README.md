# ESP32---Cubes-synthetiser
Arduino project. The goal is to create a small audio workstation for initiation to audio synthésis where you program the synthetiser by placing cubes on a board. Powered by Weemos Lolin32.

![prototype view](https://github.com/xLeDocteurx/ESP32---Cubes-synthetiser/blob/master/git/IMG_20190910_125214.jpg)

![board view](https://github.com/xLeDocteurx/ESP32---Cubes-synthetiser/blob/master/git/Screenshot%20from%202019-07-28%2020-21-12.png)

![cubes view](https://github.com/xLeDocteurx/ESP32---Cubes-synthetiser/blob/master/git/Screenshot%20from%202019-07-31%2021-26-38.png)



## Minimum viable product :
- Programing the synthetiser is achieved only via cubes !
- Controlling the synthetiseur via a 12key (1octave) integrated keyboard.
  + Changing the keyboard octave via two buttons
- Volume control on a 3.3mm mono output
- Serial MIDI USB connection ( Plug the thing via usb to a computer and sends notes using your favorite DAW software like Ableton Live, Reason, Grageband, etc.. )

## Stretched goals :
- Jack 6.35mm input to control the synth frequency with an high impedance instrument. (Guitar, bass, condenser mic)
    ( Got the idea from this article wich covers frequency detection and links midi ressources https://www.instructables.com/id/Arduino-Frequency-Detection/ )
- A filter section with envelope control.
- 12 bits output resolution.

## Monophonic Keyboard using one analoginput :
( Instead of using 12 digital inputs. Since, anyway, the wave algorythm will be very simple and wont permit polyphonic restitution. )

![keyboard schematics](https://github.com/xLeDocteurx/ESP32---Cubes-synthetiser/blob/master/git/analog_button_input.png)

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
- TL082CN chip as input amplifier and signal offset via voltage divider. (high impedance instruments are delivering a really low voltage AC signal, ADC also needs DC)
- PAM8403 chip as output amplifier and signal offset.

- Maybe a MCP4725 to upgrade the DAC output from 8 to 12 bits.
