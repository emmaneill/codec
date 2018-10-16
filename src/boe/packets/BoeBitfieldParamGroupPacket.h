/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_BITFIELD_PARAM_GROUP_PACKET_H
#define BOE_BITFIELD_PARAM_GROUP_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoePacketUtils.h"
#include "BoeParamGroupHeaderPacket.h"

namespace neueda
{

PACKED(class BoeBitfieldParamGroupPacket
{
    public:
        uint8_t                         mMessageType;
        uint8_t                         mNumberOfReturnBitfields;


        BoeBitfieldParamGroupPacket ()
        {
        }

        uint8_t getMessageType ()
        {
            return mMessageType;
        }

        uint8_t getNumberOfReturnBitfields()
        {
            return mNumberOfReturnBitfields;
        }

        bool setNumberOfReturnBitfields(uint8_t val)
        {
            mNumberOfReturnBitfields = val;
            return true;
        }

        bool setMessageType(uint8_t val)
        {
            mMessageType = val;
            return true;
        }

});


} // namespace neueda

#endif // BOE_BITFIELD_PARAM_GROUP_PACKET_H
