/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_CANCELORDER_PACKET_H_
#define _BOE_CANCELORDER_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boeCancelOrderPacket
{
public:
        char mOrigClOrdID[20];
        uint8_t mNumberOfBitfields;
        char mClearingFirm[4];

    boeCancelOrderPacket ()
    {
        memset (mOrigClOrdID, 0, 20);
        mNumberOfBitfields = 0;
        memset (mClearingFirm, 0, 4);
    }

    
    string getOrigClOrdID ()
    {
        return getString (mOrigClOrdID, sizeof (mOrigClOrdID));
    }

    bool setOrigClOrdID (const string& val)
    {
        return setString (mOrigClOrdID, val, sizeof (mOrigClOrdID));
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
});

} // namespace neueda

#endif
