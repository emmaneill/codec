/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 00:20:32 03/12/18
 */
#ifndef _BOE_BOERETURNBITFIELDPACKET_PACKET_H_
#define _BOE_BOERETURNBITFIELDPACKET_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>


#include "boeConstants.h"
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"


namespace neueda
{

PACKED(class BoeReturnBitfieldPacket
{
public:
        uint8_t mMessageType;
        uint8_t mNumOfReturnBitfield;
        uint8_t mReturnBitfield;

    BoeReturnBitfieldPacket ()
    {
        mMessageType = 0;
        mNumOfReturnBitfield = 0;
        mReturnBitfield = 0;
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
    
    uint8_t getNumOfReturnBitfield ()
    {
        return mNumOfReturnBitfield;
    }

    bool setNumOfReturnBitfield (uint8_t val)
    {
        mNumOfReturnBitfield = val;
        return true;
    }
    
    uint8_t getReturnBitfield ()
    {
        return mReturnBitfield;
    }

    bool setReturnBitfield (uint8_t val)
    {
        mReturnBitfield = val;
        return true;
    }

});

} // namespace neueda

#endif
