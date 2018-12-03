/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 00:20:32 03/12/18
 */
#ifndef _BOE_BOEUNITSEQUENCEPACKET_PACKET_H_
#define _BOE_BOEUNITSEQUENCEPACKET_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>


#include "boeConstants.h"
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"


namespace neueda
{

PACKED(class BoeUnitSequencePacket
{
public:
        uint8_t mNoUnspecifiedUnitReplay;
        uint8_t mNumOfUnits;

    BoeUnitSequencePacket ()
    {
        mNoUnspecifiedUnitReplay = 0;
        mNumOfUnits = 0;
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
    
    uint8_t getNumOfUnits ()
    {
        return mNumOfUnits;
    }

    bool setNumOfUnits (uint8_t val)
    {
        mNumOfUnits = val;
        return true;
    }

});

} // namespace neueda

#endif
