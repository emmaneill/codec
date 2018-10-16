/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_HEADER_PACKET_H
#define BOE_HEADER_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeHeaderPacket
{
    public:
        char     	            mStartOfMessage[2];
        uint16_t                mMessageLength;
        uint8_t                 mMessageType;
        uint8_t	                mMatchingUnit;
        uint32_t                mSeqNum;


        BoeHeaderPacket ()
        {
        }

        string getStartOfMessage()
        {
            return getString(mStartOfMessage, sizeof(mStartOfMessage));
        }

        uint16_t getMessageLength ()
        {
            return mMessageLength;
        }

        uint8_t getMessageType ()
        {
            return mMessageType;
        }

	    int getMatchingUnit()
	    {
	        return mMatchingUnit;
    	}

        int16_t getSequenceNumber ()
        {
            return mSeqNum;
        }

        bool setStartOfMessage(char* buf)
        {
            return setString(mStartOfMessage, (unsigned char*)buf, 2);
        }

        bool setMessageLength(uint16_t val)
        {
            mMessageLength = val;
            return true;
        }

        bool setMessageType(uint8_t val)
        {
            mMessageType = val;
            return true;
        }

        bool setMatchingUnit(uint8_t val)
        {
            mMatchingUnit = val;
            return true;
        }

        bool setSequenceNumber(uint32_t val)
        {
            mSeqNum = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_HEADER_PACKET_H
