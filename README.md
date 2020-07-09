# ArduinoShrink
This library replaces Arduino AVR Core functions with smaller and faster versions.  
For example, the Blink example sketch build for the Uno is 284 bytes smaller with ArduinoShrink.

## Usage
Select ArduinoShrink from the Sketch, Include Library menu.  This will replace pinMode, digitalWrite, and digitalRead with much smaller and faster versions.  The Arduino pin number must be known at compile time if you are using this library.

## Development plans
Future versions will include replacements for millis() and delay().
