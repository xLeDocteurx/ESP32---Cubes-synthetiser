# ESP32---Cubes-synthetiser
Arduino project. The goal is to create a small audio workstation for initiation to audio synthésis where you program the synthetiser by placing cubes on a board. Powered by Weemos Lolin32.

![board view](https://github.com/xLeDocteurx/ESP32---Cubes-synthetiser/blob/master/git/Screenshot%20from%202019-07-28%2020-21-12.png)

![cubes view](https://github.com/xLeDocteurx/ESP32---Cubes-synthetiser/blob/master/git/Screenshot%20from%202019-07-31%2021-26-38.png)

## Minimum viable product :
- xxx

## Stretched goals :
- Jack 6.35mm input to control the synth frequency with an high impedance instrument. (Guitar, bass, condenser mic)
- Midi input ( don't know yet if it will be by standard midi input or CV )
- A filter section with envelope control.

## Serial protocol :
- "cwf:sine"
- "cwf:triangle"
- "cwf:square"
- "cwf:saw"
- "cwf:whiteNoise"

## key electronical components :
- TL082CN chip as input amplifier and signal offset. (high impedance instruments are delivering a really low voltage AC signal, ADC also needs DC)
- PAM8403 chip as output amplifier and signal offset.

- Maybe a MCP4725 to upgrade the DAC output from 8 to 12 bits.
