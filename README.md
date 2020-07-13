# ArduinoShrink
This library replaces Arduino AVR Core functions with smaller and faster versions.  
For example, the Blink example sketch built for the Uno takes 924 bytes of flash, but only needs 196 bytes with ArduinoShrink.

## Usage
Download [https://github.com/nerdralph/ArduinoShrink/releases](a release), and extract the files into your Arduino/libraries folder.  Select ArduinoShrink from the Sketch, Include Library menu.  This will replace pinMode, digitalWrite, and digitalRead with much smaller and faster versions.  The Arduino pin number must be known at compile time if you are using this library.

## Limitations
ArduinoShrink implements a simple millis timer, but does not implement micros.

## Development plans
Future versions will include support for 8Mhz clock rate.
