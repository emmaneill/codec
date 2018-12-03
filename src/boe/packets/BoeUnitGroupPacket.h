/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 00:20:32 03/12/18
 */
#ifndef _BOE_BOEUNITGROUPPACKET_PACKET_H_
#define _BOE_BOEUNITGROUPPACKET_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>


#include "boeConstants.h"
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"


namespace neueda
{

PACKED(class BoeUnitGroupPacket
{
public:
        uint8_t mUnitNumber;
        uint32_t mUnitSequence;

    BoeUnitGroupPacket ()
    {
        mUnitNumber = 0;
        mUnitSequence = 0;
    }

    
    uint8_t getUnitNumber ()
    {
        return mUnitNumber;
    }

    bool setUnitNumber (uint8_t val)
    {
        mUnitNumber = val;
        return true;
    }
    
    uint32_t getUnitSequence ()
    {
        return mUnitSequence;
    }

    bool setUnitSequence (uint32_t val)
    {
        mUnitSequence = val;
        return true;
    }

});

} // namespace neueda

#endif
