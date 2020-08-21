# ArduinoShrink
This library replaces Arduino AVR Core functions with smaller and faster versions, with the same functionality. 
For example, the Blink example sketch built for the Uno takes 924 bytes of flash, but only needs 286 bytes with ArduinoShrink.

## Usage
Download [a release](https://github.com/nerdralph/ArduinoShrink/releases), and extract the files into your Arduino/libraries folder.  Select ArduinoShrink from the Sketch, Include Library menu.  This will replace several Arduino functions with smaller and faster versions.  The Arduino pin number must be known at compile time if you are using this library.

## Limitations
ArduinoShrink supports only ATmega8 and  ATmegax8 MCUs.  Accurate timing for millis() and delay() requires a freqquency that is a power of two such as 1Mz, 8Mhz, or 16Mhz.

