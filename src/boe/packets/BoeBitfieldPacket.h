/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_BOEBITFIELDPACKET_PACKET_H_
#define _BOE_BOEBITFIELDPACKET_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>


#include "boeConstants.h"
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"


namespace neueda
{

PACKED(class BoeBitfieldPacket
{
public:
        uint8_t mBitfield;

    BoeBitfieldPacket ()
    {
        mBitfield = 0;
    }

    
    uint8_t getBitfield ()
    {
        return mBitfield;
    }

    bool setBitfield (uint8_t val)
    {
        mBitfield = val;
        return true;
    }

});

} // namespace neueda

#endif
