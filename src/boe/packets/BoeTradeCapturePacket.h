/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_TRADE_CAPTURE_PACKET_H
#define BOE_TRADE_CAPTURE_PACKET_H

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

PACKED(class BoeTradeCapturePacket
{
    public:

        char                    mTradeReportID[20];
        uint32_t                mLastShares;
        uint64_t                mLastPx;
        uint8_t                 mNumberOfTradeCaptureBitfields;
        uint8_t                 mNoSides;


        BoeTradeCapturePacket ()
        {
        }

        string  getTradeReportID()
        {
            return getString(mTradeReportID, sizeof(mTradeReportID));
        }

        uint32_t getLastShares()
        {
            return mLastShares;
        }
        
        uint64_t getLastPx()
        {
            return mLastPx;
        }

        uint8_t  getNumberOfTradeCaptureBitfields()
        {
            return mNumberOfTradeCaptureBitfields;
        }
        
        uint8_t  getNoSides()
        {
            return mNoSides;
        }

        bool setTradeReportID(char* val)
        {
            return setString(mTradeReportID, (unsigned char*)val, sizeof(mTradeReportID));
        }

        bool setLastShares(uint32_t val)
        {
            mLastShares = val;
            return true;
        }
        
        bool setLastPx(uint64_t val)
        {
            mLastPx = val;
            return true;
        }

        bool setNumberOfTradeCaptureBitfields(uint8_t val)
        {
            mNumberOfTradeCaptureBitfields = val;
            return true;
        }
        
        bool setNoSides(uint8_t val)
        {
            mNoSides = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_TRADE_CAPTURE_PACKET_H
