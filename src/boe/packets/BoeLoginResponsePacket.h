/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_LOGINRESPONSE_PACKET_H_
#define _BOE_LOGINRESPONSE_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boeLoginResponsePacket
{
public:
        uint8_t mLoginResponseStatus;
        char mLoginResponseText[60];
        uint8_t mNoUnspecifiedUnitReplay;
        uint32_t mLastReceivedSequenceNumber;
        uint8_t mNumberOfUnits;

    boeLoginResponsePacket ()
    {
        mLoginResponseStatus = 0;
        memset (mLoginResponseText, 0, 60);
        mNoUnspecifiedUnitReplay = 0;
        mLastReceivedSequenceNumber = 0;
        mNumberOfUnits = 0;
    }

    
    uint8_t getLoginResponseStatus ()
    {
        return mLoginResponseStatus;
    }

    bool setLoginResponseStatus (uint8_t val)
    {
        mLoginResponseStatus = val;
        return true;
    }
    
    string getLoginResponseText ()
    {
        return getString (mLoginResponseText, sizeof (mLoginResponseText));
    }

    bool setLoginResponseText (const string& val)
    {
        return setString (mLoginResponseText, val, sizeof (mLoginResponseText));
    }
    
    uint8_t getNoUnspecifiedUnitReplay ()
    {
        return mNoUnspecifiedUnitReplay;
    }

    bool setNoUnspecifiedUnitReplay (uint8_t val)
    {
        mNoUnspecifiedUnitReplay = val;
        return true;
    }
    
    uint32_t getLastReceivedSequenceNumber ()
    {
        return mLastReceivedSequenceNumber;
    }

    bool setLastReceivedSequenceNumber (uint32_t val)
    {
        mLastReceivedSequenceNumber = val;
        return true;
    }
    
    uint8_t getNumberOfUnits ()
    {
        return mNumberOfUnits;
    }

    bool setNumberOfUnits (uint8_t val)
    {
        mNumberOfUnits = val;
        return true;
    }
});

} // namespace neueda

#endif
