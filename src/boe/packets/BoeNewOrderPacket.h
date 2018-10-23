/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_NEW_ORDER_PACKET_H
#define BOE_NEW_ORDER_PACKET_H

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

PACKED(class BoeNewOrderPacket
{
    public:

        char                    mClOrdID[20];
        char                    mSide[1];
        uint32_t                mOrderQty;
        uint8_t                 mNumberOfBitfields;


        BoeNewOrderPacket ()
        {
        }

        string  getClOrdID()
        {
            return getString(mClOrdID, sizeof(mClOrdID));
        }

        string  getSide()
        {
            return getString(mSide, sizeof(mSide)) ;
        }

        uint32_t getOrderQty()
        {
            return mOrderQty;
        }

        uint8_t  getNumberOfBitfields()
        {
            return mNumberOfBitfields;
        }

        bool setClOrdID(char* val)
        {
            return setString(mClOrdID, (unsigned char*)val, sizeof(mClOrdID));
        }

        bool setSide(char* val)
        {
            return setString(mSide, (unsigned char*)val, sizeof(mSide));
        }

        bool setOrderQty(uint32_t val)
        {
            mOrderQty = val;
            return true;
        }

        bool setNumberOfBitfields(uint8_t val)
        {
            mNumberOfBitfields = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_NEW_ORDER_PACKET_H
