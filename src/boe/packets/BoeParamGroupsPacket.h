/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 00:20:32 03/12/18
 */
#ifndef _BOE_BOEPARAMGROUPSPACKET_PACKET_H_
#define _BOE_BOEPARAMGROUPSPACKET_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>


#include "boeConstants.h"
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"


namespace neueda
{

PACKED(class BoeParamGroupsPacket
{
public:
        uint16_t mParamGroupLength;
        uint8_t mParamGroupType;

    BoeParamGroupsPacket ()
    {
        mParamGroupLength = 0;
        mParamGroupType = 0;
    }

    
    uint16_t getParamGroupLength ()
    {
        return mParamGroupLength;
    }

    bool setParamGroupLength (uint16_t val)
    {
        mParamGroupLength = val;
        return true;
    }
    
    uint8_t getParamGroupType ()
    {
        return mParamGroupType;
    }

    bool setParamGroupType (uint8_t val)
    {
        mParamGroupType = val;
        return true;
    }

});

} // namespace neueda

#endif
