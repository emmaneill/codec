/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_PARAM_GROUP_HEADER_PACKET_H
#define BOE_PARAM_GROUP_HEADER_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeParamGroupHeaderPacket
{
    public:
        uint16_t	                    mParamGroupLength;
        char	                        mParamGroupType[1];


        BoeParamGroupHeaderPacket ()
        {
            mParamGroupLength       =  INT16_MIN;
        }

        uint16_t getParamGroupLength()
        {
            return mParamGroupLength;
        }

        string getParamGroupType()
        {
            return hexStr((unsigned char*)mParamGroupType, sizeof(mParamGroupType));
        }

        bool setParamGroupLength(uint16_t val)
        {
            mParamGroupLength = val;
            return true;
        }

        bool setParamGroupType(char* buf)
        {
            return setString(mParamGroupType, (unsigned char*)buf, 1);
        }
});


} // namespace neueda

#endif // BOE_PARAM_GROUP_HEADER_PACKET_H
