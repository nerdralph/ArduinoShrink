# ArduinoShrink
This library replaces Arduino AVR Core functions with smaller and faster versions, with the same or better functionality. It is designed for the ATmega168 and 328 MCUs at 4 to 16 MHZ, though only 8MHz and 16Mhz are regularly tested.
The Arduino Blink example sketch built for the Uno takes 924 bytes of flash, but only needs 192 bytes with ArduinoShrink.

## Usage
Download [a release](https://github.com/nerdralph/ArduinoShrink/releases), and extract the files into your Arduino/libraries folder.  Select ArduinoShrink from the Sketch, Include Library menu.  This will replace several Arduino functions with smaller and faster versions.  The Arduino pin number must be known at compile time if you are using this library.

## Planned Improvements
Update digitalWrite() to work when the pin is not known at compile time.

