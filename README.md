# ArduinoShrink
This library replaces Arduino AVR Core functions with smaller and faster versions, with the same functionality. The ATmega168 and 328 MCUs are supported at both 8MHz and 16Mhz.
The Arduino Blink example sketch built for the Uno takes 924 bytes of flash, but only needs 286 bytes with ArduinoShrink.

## Usage
Download [a release](https://github.com/nerdralph/ArduinoShrink/releases), and extract the files into your Arduino/libraries folder.  Select ArduinoShrink from the Sketch, Include Library menu.  This will replace several Arduino functions with smaller and faster versions.  The Arduino pin number must be known at compile time if you are using this library.

