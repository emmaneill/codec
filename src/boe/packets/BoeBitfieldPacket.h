/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_BITFIELD_PACKET_H
#define BOE_BITFIELD_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"

namespace neueda
{

PACKED(class BoeBitfieldPacket
{
    public:
        char         mBitfield;


        BoeBitfieldPacket ()
        {
        }

        char getBitfield()
        {
            return mBitfield;
        }

        bool setBitfield(char val)
        {
            mBitfield = val;
            return true;
        }

});


} // namespace neueda

#endif // BOE_BITFIELD_PACKET_H
