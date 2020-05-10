#include <USB-MIDI.h>

///

struct MyMIDISettings : public MIDI_NAMESPACE::DefaultSettings
{
  // When setting UseSenderActiveSensing to true, MIDI.read() *must* be called
  // as often as possible (1000 / SenderActiveSensingPeriodicity per second).
  static const bool UseSenderActiveSensing = false;
};

USBMIDI_NAMESPACE::usbMidiTransport usbMIDI(0);
MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport, MyMIDISettings> MIDI((USBMIDI_NAMESPACE::usbMidiTransport&)usbMIDI);

unsigned long t1 = millis();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.begin(1);
}

void loop()
{
  MIDI.read();

  if ((millis() - t1) > 1000)
  {
    t1 = millis();
    MIDI.sendNoteOn(42, 55, 1);
  }
}
