#pragma once

#include "utility/Logging.h"

#include <MIDI.h>
#include <MIDIUSB.h>

#include <USB-MIDI_defs.h>

#include "USB-MIDI_Namespace.h"

BEGIN_USBMIDI_NAMESPACE

class usbMidiTransport
{
private:
    byte mTxBuffer[4];
    size_t mTxIndex;
    MidiType mTxStatus;

    byte mRxBuffer[4];
    size_t mRxLength;
    size_t mRxIndex;

    midiEventPacket_t mPacket;
    uint8_t cableNumber;
    
public:
	usbMidiTransport(uint8_t cableNumber = 0)
	{
        this->cableNumber = cableNumber;
	};

public:
	void begin(MIDI_NAMESPACE::Channel inChannel = 1)
	{
        mTxIndex = 0;
        mRxIndex = 0;
        mRxLength = 0;
    };

	bool beginTransmission(MidiType status)
	{
        mTxStatus = status;
        
        byte cin = 0;
        if (status < SystemExclusive)
        {
            // Non System messages
            cin = type2cin[((status & 0xF0) >> 4) - 7][1];
            mPacket.header = (((cableNumber & 0x0f) << 4) | cin);
        }
        else
        {
            // Only System messages
            cin = system2cin[status & 0x0F][1];
            mPacket.header = (((cableNumber & 0x0f) << 4) | 0x04);
        }
        
        mPacket.byte1  = 0;
        mPacket.byte2  = 0;
        mPacket.byte3  = 0;

        mTxIndex = 0;

        return true;
	};

	void write(byte byte)
	{
        if (mTxStatus != MidiType::SystemExclusive)
        {
            if (mTxIndex == 0)      mPacket.byte1 = byte;
            else if (mTxIndex == 1) mPacket.byte2 = byte;
            else if (mTxIndex == 2) mPacket.byte3 = byte;
        }
        else if (byte == MidiType::SystemExclusiveStart)
        {
            mPacket.header = (((cableNumber & 0x0f) << 4) | 0x04);
            mPacket.byte1 = byte;
        }
        else // SystemExclusiveEnd or SysEx data
        {
            auto i = mTxIndex % 3;
            if (byte == MidiType::SystemExclusiveEnd)
                mPacket.header = (((cableNumber & 0x0f) << 4) | (0x05 + i));
            
            if (i == 0)
            {
                mPacket.byte1 = byte;
                mPacket.byte2 = 0x00;
                mPacket.byte3 = 0x00;
            }
            else if (i == 1)
            {
                mPacket.byte2 = byte;
                mPacket.byte3 = 0x00;
            }
            else if (i == 2)
            {
                mPacket.byte3 = byte;
                
                if (byte != MidiType::SystemExclusiveEnd)
                {
                    V_DEBUG_PRINT (mPacket.header, HEX);
                    V_DEBUG_PRINT (" ");
                    V_DEBUG_PRINT (mPacket.byte1, HEX);
                    V_DEBUG_PRINT (" ");
                    V_DEBUG_PRINT (mPacket.byte2, HEX);
                    V_DEBUG_PRINT (" ");
                    V_DEBUG_PRINTLN (mPacket.byte3, HEX);

                    MidiUSB.sendMIDI(mPacket);
                    MidiUSB.flush();
                }
            }
        }
        mTxIndex++;
    };

	void endTransmission()
	{
        V_DEBUG_PRINT (mPacket.header, HEX);
        V_DEBUG_PRINT (" ");
        V_DEBUG_PRINT (mPacket.byte1, HEX);
        V_DEBUG_PRINT (" ");
        V_DEBUG_PRINT (mPacket.byte2, HEX);
        V_DEBUG_PRINT (" ");
        V_DEBUG_PRINTLN (mPacket.byte3, HEX);

        MidiUSB.sendMIDI(mPacket);
        MidiUSB.flush();
        
        mTxIndex = 0;
	};

	byte read()
	{
        auto byte = mRxBuffer[mRxIndex++];
        mRxLength--;

        V_DEBUG_PRINT ("read() ");
        V_DEBUG_PRINT (byte, HEX);
        V_DEBUG_PRINT (" mRxBuffer size is ");
        V_DEBUG_PRINTLN(mRxLength);

		return byte;
	};

	unsigned available()
	{
        // consume mRxBuffer first, before getting a new packet
        if (mRxLength > 0)
            return mRxLength;

        mRxIndex = 0;
        
        mPacket = MidiUSB.read();
        if (mPacket.header != 0)
        {
            auto cn  = (mPacket.header >> 4);
            V_DEBUG_PRINT ("cableNr: ");
            V_DEBUG_PRINTLN(cn);
            if (cn != cableNumber)
                return 0;

            V_DEBUG_PRINT ("available() ");
            V_DEBUG_PRINT (mPacket.header, HEX);
            V_DEBUG_PRINT (" ");
            V_DEBUG_PRINT (mPacket.byte1, HEX);
            V_DEBUG_PRINT (" ");
            V_DEBUG_PRINT (mPacket.byte2, HEX);
            V_DEBUG_PRINT (" ");
            V_DEBUG_PRINTLN (mPacket.byte3, HEX);
 
            auto cin = mPacket.header & 0x0f;
            auto len = cin2Len[cin][1];
            switch (len)
            {
                case 0:
                    if (cin == 0x4 || cin == 0x7)
                        RXBUFFER3
                    else if (cin == 0x5)
                        RXBUFFER1
                    else if (cin == 0x6)
                        RXBUFFER2
                    break;
                case 1:
                    RXBUFFER1
                    break;
                case 2:
                    RXBUFFER2
                    break;
                case 3:
                    RXBUFFER3
                    break;
                default:
                    break;
            }
        }

        return mRxLength;
	};
};

/*! \brief
 */
#define USBMIDI_CREATE_INSTANCE(CABLENR)  \
    typedef USBMIDI_NAMESPACE::usbMidiTransport __amt;\
    __amt usbMIDI(CABLENR);\
    MIDI_NAMESPACE::MidiInterface<__amt> MIDI((__amt&)usbMIDI);

#define USBMIDI_CREATE_DEFAULT_INSTANCE()  \
    USBMIDI_CREATE_INSTANCE(0)

END_USBMIDI_NAMESPACE
