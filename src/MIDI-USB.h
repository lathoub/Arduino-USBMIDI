/*
 Copyright (c) 2020 lathoub

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#pragma once

#include <MIDI.h>
#include <usbmidi.h>

#include "USB-MIDI_Namespace.h"

BEGIN_USBMIDI_NAMESPACE

class midiUsbTransport
{
private:
	uint8_t cableNumber;

public:
	midiUsbTransport(uint8_t cableNumber = 0)
	{
		this->cableNumber = cableNumber;
	};

public:
	static const bool thruActivated = false;

	void begin(){};

	void end(){};

	bool beginTransmission(MIDI_NAMESPACE::MidiType status)
	{
		return true;
	};

	void write(byte byte)
	{
		USBMIDI.write(byte);
	};

	void endTransmission()
	{
		USBMIDI.flush();
	};

	byte read()
	{
		return USBMIDI.read();
	};

	unsigned available()
	{
		USBMIDI.poll();

		return USBMIDI.available();
	};
};

END_USBMIDI_NAMESPACE

/*! \brief
 */
#define USBMIDI_CREATE_INSTANCE(CableNr, Name)                \
	USBMIDI_NAMESPACE::midiUsbTransport __usb##Name(CableNr); \
	MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::midiUsbTransport> Name((USBMIDI_NAMESPACE::midiUsbTransport &)__usb##Name);

#define USBMIDI_CREATE_CUSTOM_INSTANCE(CableNr, Name, Settings) \
	USBMIDI_NAMESPACE::midiUsbTransport __usb##Name(CableNr);   \
	MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::midiUsbTransport, Settings> Name((USBMIDI_NAMESPACE::midiUsbTransport &)__usb##Name);

#define USBMIDI_CREATE_DEFAULT_INSTANCE() \
	USBMIDI_CREATE_INSTANCE(0, MIDI)
