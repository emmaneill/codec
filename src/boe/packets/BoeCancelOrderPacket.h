/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_CANCEL_ORDER_PACKET_H
#define BOE_CANCEL_ORDER_PACKET_H

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

PACKED(class BoeCancelOrderPacket
{
    public:

        char                    mOrigClOrdID[20];
        uint8_t                 mNumberOfCancelOrderBitfields;


        BoeCancelOrderPacket ()
        {
        }

        string  getOrigClOrdID()
        {
            return getString(mOrigClOrdID, sizeof(mOrigClOrdID));
        }

        uint8_t  getNumberOfCancelOrderBitfields()
        {
            return mNumberOfCancelOrderBitfields;
        }

        bool setOrigClOrdID(char* val)
        {
            return setString(mOrigClOrdID, (unsigned char*)val, sizeof(mOrigClOrdID));
        }

        bool setNumberOfCancelOrderBitfields(uint8_t val)
        {
            mNumberOfCancelOrderBitfields = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_CANCEL_ORDER_PACKET_H
