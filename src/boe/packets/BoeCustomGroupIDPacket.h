/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 00:20:32 03/12/18
 */
#ifndef _BOE_BOECUSTOMGROUPIDPACKET_PACKET_H_
#define _BOE_BOECUSTOMGROUPIDPACKET_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>


#include "boeConstants.h"
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"


namespace neueda
{

PACKED(class BoeCustomGroupIDPacket
{
public:
        uint16_t mCustomGroupID;

    BoeCustomGroupIDPacket ()
    {
        mCustomGroupID = 0;
    }

    
    uint16_t getCustomGroupID ()
    {
        return mCustomGroupID;
    }

    bool setCustomGroupID (uint16_t val)
    {
        mCustomGroupID = val;
        return true;
    }

});

} // namespace neueda

#endif
