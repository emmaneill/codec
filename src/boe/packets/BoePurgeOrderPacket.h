/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 00:20:32 03/12/18
 */
#ifndef _BOE_PURGEORDER_PACKET_H_
#define _BOE_PURGEORDER_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boePurgeOrderPacket
{
public:
        char mMassCancel[1];
        uint8_t mNumberOfBitfields;
        char mClearingFirm[4];
        uint8_t mNumberOfCustomGroupIDs;

    boePurgeOrderPacket ()
    {
        memset (mMassCancel, 0, 1);
        mNumberOfBitfields = 0;
        memset (mClearingFirm, 0, 4);
        mNumberOfCustomGroupIDs = 0;
    }

    
    string getMassCancel ()
    {
        return getString (mMassCancel, sizeof (mMassCancel));
    }

    bool setMassCancel (const string& val)
    {
        return setString (mMassCancel, val, sizeof (mMassCancel));
    }
    
    uint8_t getNumberOfBitfields ()
    {
        return mNumberOfBitfields;
    }

    bool setNumberOfBitfields (uint8_t val)
    {
        mNumberOfBitfields = val;
        return true;
    }
    
    string getClearingFirm ()
    {
        return getString (mClearingFirm, sizeof (mClearingFirm));
    }

    bool setClearingFirm (const string& val)
    {
        return setString (mClearingFirm, val, sizeof (mClearingFirm));
    }
    
    uint8_t getNumberOfCustomGroupIDs ()
    {
        return mNumberOfCustomGroupIDs;
    }

    bool setNumberOfCustomGroupIDs (uint8_t val)
    {
        mNumberOfCustomGroupIDs = val;
        return true;
    }
});

} // namespace neueda

#endif
