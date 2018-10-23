/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_TRADE_CAPTURE_REPORT_PACKET_H
#define BOE_TRADE_CAPTURE_REPORT_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeTradeCaptureReportPacket
{
    public:
        char                    mTradeReportID[20];
        uint32_t	            mLastShares;
        uint64_t                mLastPx;
        uint8_t                 mNumberOfBitfields;
        uint8_t                 mBitfield1;
        uint8_t                 mBitfield2;
        uint8_t                 mBitfield3;
        uint8_t                 mBitfield4;
        uint8_t                 mBitfield5;
        uint8_t                 mNoSides;


        BoeTradeCaptureReportPacket ()
        {
            mBitfield1 = 0;
            mBitfield2 = 0;
            mBitfield3 = 0;
            mBitfield4 = 0;
            mBitfield5 = 0;
        }

        string getTradeReportID()
        {
            return getString(mTradeReportID, sizeof(mTradeReportID));
        }

        uint32_t getLastShares ()
        {
            return mLastShares;
        }

        uint64_t getLastPx ()
        {
            return mLastPx;
        }

        uint16_t getNumberOfBitfields ()
        {
            return mNumberOfBitfields;
        }

        uint8_t getNoSides ()
        {
            return mNoSides;
        }

        bool setTradeReportID(std::string val)
        {
            return setString(mTradeReportID, val, 20);
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

        bool setNumberOfBitfields(uint8_t val)
        {
            mNumberOfBitfields = val;
            return true;
        }

        bool setNoSides(uint8_t val)
        {
            mNoSides = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_TRADE_CAPTURE_REPORT_PACKET_H
