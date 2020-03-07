#pragma once

#include "utility/Logging.h"

#include <MIDI.h>
#include <MIDIUSB.h>

#include "utility/Deque.h"

#include "USB-MIDI_Namespace.h"

BEGIN_USBMIDI_NAMESPACE

using namespace MIDI_NAMESPACE;

class usbMidiTransport
{
    // from https://www.usb.org/sites/default/files/midi10.pdf
    // 4 USB-MIDI Event Packets
    // Table 4-1: Code Index Number Classifications
    
    static uint8_t type2cin[][2] = { {InvalidType,0}, {NoteOff,8}, {NoteOn,9}, {AfterTouchPoly,0xA}, {ControlChange,0xB}, {ProgramChange,0xC}, {AfterTouchChannel,0xD}, {PitchBend,0xE} };
    
    static uint8_t system2cin[][2] = { {SystemExclusive,0}, {TimeCodeQuarterFrame,2}, {SongPosition,3}, {SongSelect,2}, {0,0}, {0,0}, {TuneRequest,5}, {SystemExclusiveEnd,0}, {Clock,0xF}, {0,0}, {Start,0xF}, {Continue,0xF}, {Stop,0xF}, {0,0}, {ActiveSensing,0xF}, {SystemReset,0xF} };
    
    static byte cin2Len[][2] = { {0,0}, {1,0}, {2,2}, {3,3}, {4,3}, {5,1}, {6,2}, {7,3}, {8,3}, {9,3}, {10,3}, {11,3}, {12,2}, {13,2}, {14,3}, {15,1} };

private:
    byte mTxBuffer[4];
    size_t mTxIndex;
    midiEventPacket_t mTxPacket;
    MidiType mTxStatus;

    Deque<byte, 44> mRxBuffer; // 44 is an arbitary number
    
    uint8_t cableNumber;
    
public:
	usbMidiTransport(uint8_t cableNumber = 0)
	{
        this->cableNumber = cableNumber;
	};

public:
	void begin(MIDI_NAMESPACE::Channel inChannel = 1)
	{
        mRxBuffer.clear();
        mTxIndex = 0;
    };

	bool beginTransmission(MidiType status)
	{

        mTxStatus = status;
        
        byte cin = 0;
        if (status < SystemExclusive)
        {
            cin = type2cin[((status & 0xF0) >> 4) - 7][1];
            mTxPacket.header = (cableNumber | cin);
        }
        else
        {
            cin = system2cin[status & 0x0F][1];
            mTxPacket.header = (cableNumber | 0x04);
        }
        
        mTxPacket.byte1  = 0;
        mTxPacket.byte2  = 0;
        mTxPacket.byte3  = 0;

        mTxIndex = 0;

        return true;
	};

	void write(byte byte)
	{
        if (mTxStatus != MidiType::SystemExclusive)
        {
            if (mTxIndex == 0)      mTxPacket.byte1 = byte;
            else if (mTxIndex == 1) mTxPacket.byte2 = byte;
            else if (mTxIndex == 2) mTxPacket.byte3 = byte;
        }
        else if (byte == MidiType::SystemExclusiveStart)
        {
            mTxPacket.header = (cableNumber | (0x04));
            mTxPacket.byte1 = byte;
        }
        else // SystemExclusiveEnd or SysEx data
        {
            auto i = mTxIndex % 3;
            if (byte == MidiType::SystemExclusiveEnd)
                mTxPacket.header = (cableNumber | (0x05 + i));
            
            if (i == 0)
            {
                mTxPacket.byte1 = byte;
                mTxPacket.byte2 = 0x00;
                mTxPacket.byte3 = 0x00;
            }
            else if (i == 1)
            {
                mTxPacket.byte2 = byte;
                mTxPacket.byte3 = 0x00;
            }
            else if (i == 2)
            {
                mTxPacket.byte3 = byte;
                
                if (byte != MidiType::SystemExclusiveEnd)
                {
                    MidiUSB.sendMIDI(mTxPacket);
                    MidiUSB.flush();
                }
            }
        }
        mTxIndex++;
    };

	void endTransmission()
	{
        MidiUSB.sendMIDI(mTxPacket);
        MidiUSB.flush();
        
        mTxIndex = 0;
	};

	byte read()
	{
        auto byte = mRxBuffer.pop_back();
        
		return byte;
	};

	unsigned available()
	{
        midiEventPacket_t packet = MidiUSB.read();
        if (packet.header != 0)
        {
            auto cn  = packet.header & 0xf0 >> 4;
 //           if (cn != cableNumber)
 //               break;
                
            auto cin = packet.header & 0x0f;
            auto len = cin2Len[cin][1];
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
