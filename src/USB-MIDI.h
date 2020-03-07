#pragma once

#include <MIDI.h>
#include <MIDIUSB.h>

#include "utility/Deque.h"

#include "USB-MIDI_Namespace.h"

BEGIN_USBMIDI_NAMESPACE

class usbMidiTransport
{
private:
    byte mTxBuffer[4];
    size_t mTxIndex;
    
    Deque<byte, 44> mRxBuffer; // 44 is an arbitary number
    
    midiEventPacket_t packet;

    uint8_t cableNumber;
    
    static bool midi_is_real_time(uint8_t byte)
    {
        return byte == 0xf8 || (byte >= 0xfa && byte != 0xfd);
    }

    static bool midi_is_sysex_start(uint8_t byte)
    {
        return byte == 0xf0;
    }

    static bool midi_is_sysex_end(uint8_t byte)
    {
        return byte == 0xf7;
    }

    static bool midi_is_single_byte_system_common(uint8_t byte)
    {
        return byte >= 0xf4 && byte <= 0xf6;
    }

public:
	usbMidiTransport(uint8_t cableNumber = 0) // cable number?
	{
        this->cableNumber = cableNumber;
	};

public:
	void begin(MIDI_NAMESPACE::Channel inChannel = 1)
	{
        mRxBuffer.clear();
        mTxIndex = 0;
    };

	bool beginTransmission(MIDI_NAMESPACE::MidiType)
	{
        mTxIndex = 0;
        
        return true;
	};

	void write(byte byte)
	{
        mTxBuffer[mTxIndex++] = byte;
	};

	void endTransmission()
	{
        midiEventPacket_t packet;
        packet.header = 0;
        packet.byte1  = 0;
        packet.byte2  = 0;
        packet.byte3  = 0;

        auto status = mTxBuffer[0] & 0x0f0;
        switch (status)
        {
            case MIDI_NAMESPACE::MidiType::NoteOn:
            case MIDI_NAMESPACE::MidiType::NoteOff:
                packet.header = cableNumber | (status >> 4);
                packet.byte1  = mTxBuffer[0];
                packet.byte2  = mTxBuffer[1];
                packet.byte3  = mTxBuffer[2];
                break;
        }
        
        MidiUSB.sendMIDI(packet);
        MidiUSB.flush();
	};

	byte read()
	{
        auto byte = mRxBuffer.pop_back();
        
		return byte;
	};

	unsigned available()
	{
        // from https://www.usb.org/sites/default/files/midi10.pdf
        // 4 USB-MIDI Event Packets
        // Table 4-1: Code Index Number Classifications
        static byte cinToLen[16][2] = { {0,0}, {1,0}, {2,2}, {3,3}, {4,3}, {5,1}, {6,2}, {7,3}, {8,3}, {9,3}, {10,3}, {11,3}, {12,2}, {13,2}, {14,3}, {15,1} };

        midiEventPacket_t packet = MidiUSB.read();
        if (packet.header != 0)
        {
            auto cn  = packet.header & 0xf0 >> 4;
 //           if (cn != cableNumber)
 //               break;
                
            auto cin = packet.header & 0x0f;
            auto len = cinToLen[cin][1];
            switch (len)
            {
                case 1:
                    mRxBuffer.push_back(packet.byte1);
                    break;
                case 2:
                    mRxBuffer.push_back(packet.byte1);
                    mRxBuffer.push_back(packet.byte2);
                    break;
                case 3:
                    mRxBuffer.push_back(packet.byte1);
                    mRxBuffer.push_back(packet.byte2);
                    mRxBuffer.push_back(packet.byte3);
                    break;
                default:
                    break;
            }
        }

        return mRxBuffer.size();
	};
};

/*! \brief
 */
#define USBMIDI_CREATE_DEFAULT_INSTANCE()  \
    typedef USBMIDI_NAMESPACE::usbMidiTransport __amt;\
    __amt USBMIDI;\
    MIDI_NAMESPACE::MidiInterface<__amt> MIDI((__amt&)USBMIDI);

END_USBMIDI_NAMESPACE
