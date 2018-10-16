/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_UNIT_SEQUENCES_PARAM_GROUP_PACKET_H
#define BOE_UNIT_SEQUENCES_PARAM_GROUP_PACKET_H

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

PACKED(class BoeUnitSequencesParamGroupPacket
{
    public:
        uint8_t                         mNoUnspecifiedReplay;
        uint8_t                         mNumberOfUnits;


        BoeUnitSequencesParamGroupPacket ()
        {
            mNoUnspecifiedReplay    =  INT8_MIN;
        }

        uint8_t getNoUnspecifiedReplay()
        {
            return mNoUnspecifiedReplay;
        }

        uint8_t getNumberOfUnits()
        {
            return mNumberOfUnits;
        }
   
        bool setNoUnspecifiedReplay(uint8_t val)
        {
            mNoUnspecifiedReplay = val;
            return true;
        }

        bool setNumberOfUnits(uint8_t val)
        {
            mNumberOfUnits = val;
            return true;
        }


});


} // namespace neueda

#endif // BOE_UNIT_SEQUENCES_PARAM_GROUP_PACKET_H
