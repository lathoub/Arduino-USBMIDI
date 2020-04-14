#include <USB-MIDI.h>

USBMIDI_CREATE_DEFAULT_INSTANCE();

void handleNoteOn(byte inChannel, byte inNumber, byte inVelocity)
{
  Serial.print("NoteOn  ");
  Serial.print(inNumber);
  Serial.print("\tvelocity: ");
  Serial.println(inVelocity);
}

void handleNoteOff(byte inChannel, byte inNumber, byte inVelocity)
{
  Serial.print("NoteOff ");
  Serial.print(inNumber);
  Serial.print("\tvelocity: ");
  Serial.println(inVelocity);
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  MIDI.begin();
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  Serial.println("Arduino ready.");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  MIDI.read();
}