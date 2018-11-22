/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_BOESIDEGROUPPACKET_PACKET_H_
#define _BOE_BOESIDEGROUPPACKET_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>


#include "boeConstants.h"
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"


namespace neueda
{

PACKED(class BoeSideGroupPacket
{
public:
        uint8_t mSide;
        uint8_t mCapacity;
        char mPartyID[4];
        char mAccount[16];
        uint8_t mPartyRole;

    BoeSideGroupPacket ()
    {
        mSide = 0;
        mCapacity = 0;
        memset (mPartyID, 0, 4);
        memset (mAccount, 0, 16);
        mPartyRole = 0;
    }

    
    uint8_t getSide ()
    {
        return mSide;
    }

    bool setSide (uint8_t val)
    {
        mSide = val;
        return true;
    }
    
    uint8_t getCapacity ()
    {
        return mCapacity;
    }

    bool setCapacity (uint8_t val)
    {
        mCapacity = val;
        return true;
    }
    
    string getPartyID ()
    {
        return getString (mPartyID, sizeof (mPartyID));
    }

    bool setPartyID (const string& val)
    {
        return setString (mPartyID, val, sizeof (mPartyID));
    }
    
    string getAccount ()
    {
        return getString (mAccount, sizeof (mAccount));
    }

    bool setAccount (const string& val)
    {
        return setString (mAccount, val, sizeof (mAccount));
    }
    
    uint8_t getPartyRole ()
    {
        return mPartyRole;
    }

    bool setPartyRole (uint8_t val)
    {
        mPartyRole = val;
        return true;
    }

});

} // namespace neueda

#endif
