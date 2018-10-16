/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_UNIT_GROUP_PACKET_H
#define BOE_UNIT_GROUP_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeUnitGroupPacket
{
    public:
        uint8_t	                mUnitNumber;
        uint32_t                mUnitSequence;


        BoeUnitGroupPacket ()
        {
        }

	    uint8_t getUnitNumber()
	    {
	        return mUnitNumber;
    	}

        int16_t getUnitSequence()
        {
            return mUnitSequence;
        }

        bool setUnitNumber(uint8_t val)
        {
            mUnitNumber = val;
            return true;
        }

        bool setUnitSequence(uint32_t val)
        {
            mUnitSequence = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_UNIT_GROUP_PACKET_H
