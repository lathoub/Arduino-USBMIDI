#define DEBUG 4
#include <USB-MIDI.h>

byte sysex14[] = { 0xF0, 0x43, 0x20, 0x7E, 0x4C, 0x4D, 0x20, 0x20, 0x38, 0x39, 0x37, 0x33, 0x50, 0xF7 };
byte sysex15[] = { 0xF0, 0x43, 0x20, 0x7E, 0x4C, 0x4D, 0x20, 0x20, 0x38, 0x39, 0x37, 0x33, 0x50, 0x4D, 0xF7 };
byte sysex16[] = { 0xF0, 0x43, 0x20, 0x7E, 0x4C, 0x4D, 0x20, 0x20, 0x38, 0x39, 0x37, 0x33, 0x32, 0x50, 0x4D, 0xF7 };
byte sysexBig[] = { 0xF0, 0x41,
                    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
                    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
                    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
                    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
                    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
                    0x7a,

                    0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
                    0xF7
                  };

USBMIDI_CREATE_INSTANCE(0);

unsigned long t0 = millis();

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  DEBUG_BEGIN(115200);

  N_DEBUG_PRINTLN(F("Booting"));

  // Listen for MIDI messages on channel 1
  MIDI.begin(1);

  MIDI.setHandleSystemExclusive(OnMidiSysEx);
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

    MIDI.sendSysEx(sizeof(sysex14), sysex14, true);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OnMidiSysEx(byte* data, unsigned length) {
  N_DEBUG_PRINT(F("SYSEX: ("));
  N_DEBUG_PRINT(length);
  N_DEBUG_PRINT(F(" bytes) "));
  for (uint16_t i = 0; i < length; i++)
  {
    N_DEBUG_PRINT(data[i], HEX);
    N_DEBUG_PRINT(" ");
  }
  N_DEBUG_PRINTLN();
}
