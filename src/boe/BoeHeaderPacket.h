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
        char                    mMessageType[1];
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

        string getMessageType ()
        {
            return getString(mMessageType, sizeof(mMessageType));
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

        bool setMessageType(char* buf)
        {
            return setString(mMessageType, (unsigned char*)buf, 1);
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
