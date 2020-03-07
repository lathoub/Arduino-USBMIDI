#include <USB-MIDI.h>

USBMIDI_CREATE_INSTANCE(0);

unsigned long t0 = millis();

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  // Listen for MIDI messages on channel 1
  MIDI.begin(1);

  MIDI.setHandleNoteOn(OnNoteOn);
  MIDI.setHandleNoteOff(OnNoteOff);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  // Listen to incoming notes
  MIDI.read();

  // send a note every second
  // (dont cáll delay(1000) as it will stall the pipeline)
  if ((millis() - t0) > 1000)
  {
    t0 = millis();
    //   Serial.print(F(".");

    byte note = random(1, 127);
    byte velocity = 55;
    byte channel = 1;

    MIDI.sendNoteOn(note, velocity, channel);
    MIDI.sendNoteOff(note, velocity, channel);
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnNoteOn(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnNoteOff(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}
