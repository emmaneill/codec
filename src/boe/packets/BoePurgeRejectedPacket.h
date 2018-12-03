/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 00:20:32 03/12/18
 */
#ifndef _BOE_PURGEREJECTED_PACKET_H_
#define _BOE_PURGEREJECTED_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boePurgeRejectedPacket
{
public:
        uint64_t mTransactTime;
        uint8_t mPurgeRejectReason;
        char mPurgeRejectText[60];
        char mReservedInternal[1];

    boePurgeRejectedPacket ()
    {
        mTransactTime = 0;
        mPurgeRejectReason = 0;
        memset (mPurgeRejectText, 0, 60);
        memset (mReservedInternal, 0, 1);
    }

    
    uint64_t getTransactTime ()
    {
        return mTransactTime;
    }

    bool setTransactTime (uint64_t val)
    {
        mTransactTime = val;
        return true;
    }
    
    uint8_t getPurgeRejectReason ()
    {
        return mPurgeRejectReason;
    }

    bool setPurgeRejectReason (uint8_t val)
    {
        mPurgeRejectReason = val;
        return true;
    }
    
    string getPurgeRejectText ()
    {
        return getString (mPurgeRejectText, sizeof (mPurgeRejectText));
    }

    bool setPurgeRejectText (const string& val)
    {
        return setString (mPurgeRejectText, val, sizeof (mPurgeRejectText));
    }
    
    string getReservedInternal ()
    {
        return getString (mReservedInternal, sizeof (mReservedInternal));
    }

    bool setReservedInternal (const string& val)
    {
        return setString (mReservedInternal, val, sizeof (mReservedInternal));
    }
});

} // namespace neueda

#endif
