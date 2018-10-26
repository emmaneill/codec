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
        char         mBitfield[1];


        BoeBitfieldPacket ()
        {
        }

        string getBitfield()
        {
            return getString(mBitfield, sizeof(mBitfield));
        }

        bool setBitfield(std::string val)
        {
            return setString(mBitfield, val, 1);
        }

});


} // namespace neueda

#endif // BOE_BITFIELD_PACKET_H
