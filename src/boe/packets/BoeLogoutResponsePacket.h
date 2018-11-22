/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_LOGOUTRESPONSE_PACKET_H_
#define _BOE_LOGOUTRESPONSE_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boeLogoutResponsePacket
{
public:
        uint8_t mLogoutReason;
        char mLogoutReasonText[60];
        uint32_t mLastReceivedSequenceNumber;
        uint8_t mNumberOfUnits;

    boeLogoutResponsePacket ()
    {
        mLogoutReason = 0;
        memset (mLogoutReasonText, 0, 60);
        mLastReceivedSequenceNumber = 0;
        mNumberOfUnits = 0;
    }

    
    uint8_t getLogoutReason ()
    {
        return mLogoutReason;
    }

    bool setLogoutReason (uint8_t val)
    {
        mLogoutReason = val;
        return true;
    }
    
    string getLogoutReasonText ()
    {
        return getString (mLogoutReasonText, sizeof (mLogoutReasonText));
    }

    bool setLogoutReasonText (const string& val)
    {
        return setString (mLogoutReasonText, val, sizeof (mLogoutReasonText));
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
