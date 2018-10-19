/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_MODIFY_ORDER_PACKET_H
#define BOE_MODIFY_ORDER_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeModifyOrderPacket
{
    public:

        char                    mClOrdID[20];
        char                    mOrigClOrdID[20];
        uint8_t                 mNumberOfModifyOrderBitfields;


        BoeModifyOrderPacket ()
        {
        }
        
        string  getClOrdID()
        {
            return getString(mClOrdID, sizeof(mClOrdID));
        }

        string  getOrigClOrdID()
        {
            return getString(mOrigClOrdID, sizeof(mOrigClOrdID));
        }

        uint8_t  getNumberOfModifyOrderBitfields()
        {
            return mNumberOfModifyOrderBitfields;
        }
        
        bool setClOrdID(char* val)
        {
            return setString(mClOrdID, (unsigned char*)val, sizeof(mClOrdID));
        }

        bool setOrigClOrdID(char* val)
        {
            return setString(mOrigClOrdID, (unsigned char*)val, sizeof(mOrigClOrdID));
        }

        bool setNumberOfModifyOrderBitfields(uint8_t val)
        {
            mNumberOfModifyOrderBitfields = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_MODIFY_ORDER_PACKET_H
