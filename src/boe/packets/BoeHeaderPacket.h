/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 00:20:32 03/12/18
 */
#ifndef _BOE_BOEHEADERPACKET_PACKET_H_
#define _BOE_BOEHEADERPACKET_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>


#include "boeConstants.h"
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"


namespace neueda
{

PACKED(class BoeHeaderPacket
{
public:
        char mStartOfMessage[2];
        uint16_t mMessageLength;
        uint8_t mMessageType;
        uint8_t mMatchingUnit;
        uint32_t mSequenceNumber;

    BoeHeaderPacket ()
    {
        memset (mStartOfMessage, 0XBA, 2);
        mMessageLength = 0;
        mMessageType = 0;
        mMatchingUnit = 0;
        mSequenceNumber = 0;
    }

    
    string getStartOfMessage ()
    {
        return getString (mStartOfMessage, sizeof (mStartOfMessage));
    }

    bool setStartOfMessage (const string& val)
    {
        return setString (mStartOfMessage, val, sizeof (mStartOfMessage));
    }
    
    uint16_t getMessageLength ()
    {
        return mMessageLength;
    }

    bool setMessageLength (uint16_t val)
    {
        mMessageLength = val;
        return true;
    }
    
    uint8_t getMessageType ()
    {
        return mMessageType;
    }

    bool setMessageType (uint8_t val)
    {
        mMessageType = val;
        return true;
    }
    
    uint8_t getMatchingUnit ()
    {
        return mMatchingUnit;
    }

    bool setMatchingUnit (uint8_t val)
    {
        mMatchingUnit = val;
        return true;
    }
    
    uint32_t getSequenceNumber ()
    {
        return mSequenceNumber;
    }

    bool setSequenceNumber (uint32_t val)
    {
        mSequenceNumber = val;
        return true;
    }

});

} // namespace neueda

#endif
