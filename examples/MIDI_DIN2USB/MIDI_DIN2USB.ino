#include <USB-MIDI.h>
USING_NAMESPACE_MIDI;

typedef USBMIDI_NAMESPACE::usbMidiTransport __umt;
typedef MIDI_NAMESPACE::MidiInterface<__umt> __ss;
__umt usbMIDI(0); // cableNr
__ss MIDICoreUSB((__umt&)usbMIDI);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDICoreSerial);

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  MIDICoreUSB.setHandleMessage(onUsbMessage);
  MIDICoreSerial.setHandleMessage(onSerialMessage);

  MIDICoreUSB.begin(MIDI_CHANNEL_OMNI);
  MIDICoreSerial.begin(MIDI_CHANNEL_OMNI);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  MIDICoreUSB.read();
  MIDICoreSerial.read();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void onUsbMessage(const TypedMidiInterface::MidiMessage& message)
{
  MIDICoreSerial.send(message);
}

void onSerialMessage(const TypedMidiInterface::MidiMessage& message)
{
  MIDICoreUSB.send(message);
}
