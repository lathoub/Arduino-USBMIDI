# Arduino USB-MIDI Transport
This library implements the USB-MIDI transport layer for the [FortySevenEffects Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) 

## Installation
This library depends on the [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library).

When installing this library from the Arduino IDE, the [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) will be downloaded and installed in the same directory as this library.

When manually installing this library, you have to manually download [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) from github and install it in the same directory as this library - without this additional install, the library will not be able to compile. 

## Tested boards/modules
- Arduino Leonardo

## Memory usage
The library does not add additional buffers and is extremely efficiant and has a small memory footprint.

## Other Transport protocols:
- [Arduino AppleMIDI Transport](https://github.com/lathoub/Arduino-AppleMIDI-Library)
- [Arduino ipMIDI  Transport](https://github.com/lathoub/Arduino-ipMIDI)
- [Arduino BLE-MIDI  Transport](https://github.com/lathoub/Arduino-BLE-MIDI)
