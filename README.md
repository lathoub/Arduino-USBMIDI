# Arduino USB-MIDI Transport <Experimental>
This library implements the USB-MIDI transport layer for the [FortySevenEffects Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) 

## Installation
This library depends on the [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) and [Arduino's MIDIUSB](https://github.com/arduino-libraries/MIDIUSB).

When installing this library from the Arduino IDE, both will be downloaded and installed in the same directory as this library. (Thanks to the `depends` clause in `library.properties`)

When manually installing this library, you have to manually download [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) and [MIDIUSB](https://github.com/arduino-libraries/MIDIUSB) from github and install it in the same directory as this library - without these additional installs, this library will not be able to compile. 

## Usage
### Basic / Default
```cpp
#include <USB-MIDI.h>
...
USBMIDI_CREATE_DEFAULT_INSTANCE();
...
void setup()
{
   MIDI.begin(1);
...
void loop()
{
   MIDI.read();
```
will create a instance named `usbMIDI` and is by default connected to cable number 0 - and listens to incoming MIDI on channel 1.

### Modified
```cpp
#include <USB-MIDI.h>
...
USBMIDI_CREATE_INSTANCE(4);
```
will create a instance named `usbMIDI` and is connected to cable number 4.

### Advanced
```cpp
#include <USB-MIDI.h>
...
typedef USBMIDI_NAMESPACE::usbMidiTransport __umt;
__umt usbMIDI2(5);
MIDI_NAMESPACE::MidiInterface<__umt> MIDI2((__umt&)usbMIDI);
```
will create a instance named `usbMIDI2` (and underlaying MIDI object `MIDI2`) and is by default connected to cable number 5.

## Tested boards/modules
- Arduino Leonardo

## Memory usage
The library does not add additional buffers and is extremely efficiant and has a small memory footprint.

## Other Transport protocols:
The libraries below  the same calling mechanism (API), making it easy to interchange the transport layer.
- [Arduino AppleMIDI Transport](https://github.com/lathoub/Arduino-AppleMIDI-Library)
- [Arduino ipMIDI  Transport](https://github.com/lathoub/Arduino-ipMIDI)
- [Arduino BLE-MIDI  Transport](https://github.com/lathoub/Arduino-BLE-MIDI)
