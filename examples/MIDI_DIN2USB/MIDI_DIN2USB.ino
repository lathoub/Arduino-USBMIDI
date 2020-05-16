#include <USB-MIDI.h>
USING_NAMESPACE_MIDI

USBMIDI_CREATE_INSTANCE(0, MIDICoreUSB);
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
void onUsbMessage(const MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport>::MidiMessage& message)
{
  MIDICoreSerial.send(message);
}

void onSerialMessage(const MidiInterface<SerialMIDI<HardwareSerial>>::MidiMessage& message)
{
  MIDICoreUSB.send(message);
}