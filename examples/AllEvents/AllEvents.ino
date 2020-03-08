#define DEBUG 7
#include <USB-MIDI.h>

USBMIDI_CREATE_DEFAULT_INSTANCE();

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

  MIDI.setHandleNoteOn(OnNoteOn);
  MIDI.setHandleNoteOff(OnNoteOff);
  MIDI.setHandleAfterTouchPoly(OnAfterTouchPoly);
  MIDI.setHandleControlChange(OnControlChange);
  MIDI.setHandleProgramChange(OnProgramChange);
  MIDI.setHandleAfterTouchChannel(OnAfterTouchChannel);
  MIDI.setHandlePitchBend(OnPitchBend);
  MIDI.setHandleSystemExclusive(OnSystemExclusive);
  MIDI.setHandleTimeCodeQuarterFrame(OnTimeCodeQuarterFrame);
  MIDI.setHandleSongPosition(OnSongPosition);
  MIDI.setHandleSongSelect(OnSongSelect);
  MIDI.setHandleTuneRequest(OnTuneRequest);
  MIDI.setHandleClock(OnClock);
  MIDI.setHandleStart(OnStart);
  MIDI.setHandleContinue(OnContinue);
  MIDI.setHandleStop(OnStop);
  MIDI.setHandleActiveSensing(OnActiveSensing);
  MIDI.setHandleSystemReset(OnSystemReset);
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
  N_DEBUG_PRINT(F("NoteOn  from channel: "));
  N_DEBUG_PRINT(channel);
  N_DEBUG_PRINT(F(", note: "));
  N_DEBUG_PRINT(note);
  N_DEBUG_PRINT(F(", velocity: "));
  N_DEBUG_PRINTLN(velocity);
}

static void OnNoteOff(byte channel, byte note, byte velocity) {
  N_DEBUG_PRINT(F("NoteOff from channel: "));
  N_DEBUG_PRINT(channel);
  N_DEBUG_PRINT(F(", note: "));
  N_DEBUG_PRINT(note);
  N_DEBUG_PRINT(F(", velocity: "));
  N_DEBUG_PRINTLN(velocity);
}

static void OnAfterTouchPoly(byte channel, byte note, byte pressure) {
  N_DEBUG_PRINT(F("AfterTouchPoly from channel: "));
  N_DEBUG_PRINT(channel);
  N_DEBUG_PRINT(F(", note: "));
  N_DEBUG_PRINT(note);
  N_DEBUG_PRINT(F(", pressure: "));
  N_DEBUG_PRINTLN(pressure);
 }

static void OnControlChange(byte channel, byte number, byte value) {
  N_DEBUG_PRINT(F("ControlChange from channel: "));
  N_DEBUG_PRINT(channel);
  N_DEBUG_PRINT(F(", number: "));
  N_DEBUG_PRINT(number);
  N_DEBUG_PRINT(F(", value: "));
  N_DEBUG_PRINTLN(value);
}

static void OnProgramChange(byte channel, byte number) {
  N_DEBUG_PRINT(F("ProgramChange from channel: "));
  N_DEBUG_PRINT(channel);
  N_DEBUG_PRINT(F(", number: "));
  N_DEBUG_PRINTLN(number);
}

static void OnAfterTouchChannel(byte channel, byte pressure) {
  N_DEBUG_PRINT(F("AfterTouchChannel from channel: "));
  N_DEBUG_PRINT(channel);
  N_DEBUG_PRINT(F(", pressure: "));
  N_DEBUG_PRINTLN(pressure);
}

static void OnPitchBend(byte channel, int bend) {
  N_DEBUG_PRINT(F("PitchBend from channel: "));
  N_DEBUG_PRINT(channel);
  N_DEBUG_PRINT(F(", bend: "));
  N_DEBUG_PRINTLN(bend);
}

static void OnSystemExclusive(byte * array, unsigned size) {
  N_DEBUG_PRINTLN(F("SystemExclusive"));
}

static void OnTimeCodeQuarterFrame(byte data) {
  N_DEBUG_PRINTLN(F("TimeCodeQuarterFrame: "));
  N_DEBUG_PRINTLN(data, HEX);
}

static void OnSongPosition(unsigned beats) {
  N_DEBUG_PRINTLN(F("SongPosition: "));
  N_DEBUG_PRINTLN(beats);
}

static void OnSongSelect(byte songnumber) {
  N_DEBUG_PRINTLN(F("SongSelect: "));
  N_DEBUG_PRINTLN(songnumber);
}

static void OnTuneRequest() {
  N_DEBUG_PRINTLN(F("TuneRequest"));
}

static void OnClock() {
  N_DEBUG_PRINTLN(F("Clock"));
}

static void OnStart() {
  N_DEBUG_PRINTLN(F("Start"));
}

static void OnContinue() {
  N_DEBUG_PRINTLN(F("Continue"));
}

static void OnStop() {
  N_DEBUG_PRINTLN(F("Stop"));
}

static void OnActiveSensing() {
  N_DEBUG_PRINTLN(F("ActiveSensing"));
}

static void OnSystemReset() {
  N_DEBUG_PRINTLN(F("SystemReset"));
}
