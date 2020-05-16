#include <USB-MIDI.h>
USING_NAMESPACE_MIDI

struct ModifiedSettings : public MIDI_NAMESPACE::DefaultSettings
{
  static const unsigned SysExMaxSize = 8196;
};

// Both Instances *must* use the same SysExMaxSize
USBMIDI_CREATE_CUSTOM_INSTANCE(0, MIDICoreUSB, ModifiedSettings);
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial1, MIDICoreSerial, ModifiedSettings);

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
void onUsbMessage(const MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport, ModifiedSettings>::MidiMessage& message)
{
  MIDICoreSerial.send(message);
}

void onSerialMessage(const MidiInterface<SerialMIDI<HardwareSerial>, ModifiedSettings>::MidiMessage& message)
{
  MIDICoreUSB.send(message);
}
